#include "PluginEditor.h"

#if JUCE_WINDOWS
 #ifndef NOMINMAX
  #define NOMINMAX
 #endif
 #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
 #endif
 #include <windows.h>
#endif

namespace
{
    // Loads an FL Studio native plugin DLL and checks for the CreatePlugInstance export.
    // It does NOT call the export: that needs a real FL host object.
    juce::String probeFlPlugin (const juce::File& dll)
    {
       #if JUCE_WINDOWS
        if (! dll.existsAsFile())
            return "File not found:\n" + dll.getFullPathName();

        // FL folder = parent of "Plugins" (...\FL Studio\Plugins\Fruity\Generators\Name\Name_x64.dll)
        juce::File flRoot;
        for (auto dir = dll.getParentDirectory(); dir != dir.getParentDirectory(); dir = dir.getParentDirectory())
        {
            if (dir.getFileName().equalsIgnoreCase ("Plugins"))
            {
                flRoot = dir.getParentDirectory();
                break;
            }
        }

        juce::String log;
        log << "DLL: " << dll.getFullPathName() << "\n";
        log << "FL folder: "
            << (flRoot == juce::File() ? juce::String ("(not detected)") : flRoot.getFullPathName())
            << "\n\n";

        // The plugin may depend on helper DLLs that live in the FL folder.
        if (flRoot != juce::File())
            SetDllDirectoryW (flRoot.getFullPathName().toWideCharPointer());

        SetLastError (0);
        HMODULE lib = LoadLibraryExW (dll.getFullPathName().toWideCharPointer(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
        const DWORD error = GetLastError();
        SetDllDirectoryW (nullptr);

        if (lib == nullptr)
        {
            log << "LoadLibrary FAILED, error " << static_cast<int> (error) << "\n"
                << "(126 = a dependency DLL is missing, 193 = wrong bitness, 5 = access denied)";
            return log;
        }

        const bool found = GetProcAddress (lib, "CreatePlugInstance") != nullptr;
        log << "Loaded OK.\nCreatePlugInstance export: " << (found ? "FOUND" : "NOT FOUND");

        FreeLibrary (lib);
        return log;
       #else
        juce::ignoreUnused (dll);
        return "Windows only.";
       #endif
    }
}

ProbeEditor::ProbeEditor (ProbeProcessor& p)
    : juce::AudioProcessorEditor (p), proc (p)
{
    pathBox.setText (proc.dllPath, false);
    pathBox.setTextToShowWhenEmpty ("Full path to an FL plugin DLL, e.g. ...\\Plugins\\Fruity\\Generators\\Sytrus\\Sytrus_x64.dll",
                                    juce::Colours::grey);
    pathBox.onTextChange = [this] { proc.dllPath = pathBox.getText().trim().unquoted(); };
    addAndMakeVisible (pathBox);

    logBox.setMultiLine (true);
    logBox.setReadOnly (true);
    logBox.setScrollbarsShown (true);
    logBox.setText ("Pick the plugin DLL from your FL Studio folder and press Probe.", false);
    addAndMakeVisible (logBox);

    browseButton.onClick = [this] { browse(); };
    probeButton.onClick  = [this] { runProbe(); };
    addAndMakeVisible (browseButton);
    addAndMakeVisible (probeButton);

    setSize (620, 260);
}

void ProbeEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void ProbeEditor::resized()
{
    auto area = getLocalBounds().reduced (10);

    auto row = area.removeFromTop (28);
    probeButton.setBounds (row.removeFromRight (70));
    row.removeFromRight (6);
    browseButton.setBounds (row.removeFromRight (90));
    row.removeFromRight (6);
    pathBox.setBounds (row);

    area.removeFromTop (8);
    logBox.setBounds (area);
}

void ProbeEditor::browse()
{
    auto start = juce::File (proc.dllPath);
    if (! start.exists())
        start = juce::File::getSpecialLocation (juce::File::globalApplicationsDirectory).getChildFile ("Image-Line");

    chooser = std::make_unique<juce::FileChooser> ("Select an FL Studio plugin DLL", start, "*.dll");

    const auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    juce::Component::SafePointer<ProbeEditor> safeThis (this);

    chooser->launchAsync (flags, [safeThis] (const juce::FileChooser& fc)
    {
        auto* editor = safeThis.getComponent();
        if (editor == nullptr)
            return;

        const auto file = fc.getResult();
        if (file != juce::File())
        {
            editor->proc.dllPath = file.getFullPathName();
            editor->pathBox.setText (editor->proc.dllPath, false);
        }
    });
}

void ProbeEditor::runProbe()
{
    proc.dllPath = pathBox.getText().trim().unquoted();

    const auto dll = juce::File::getCurrentWorkingDirectory().getChildFile (proc.dllPath);
    logBox.setText (probeFlPlugin (dll), false);
}
