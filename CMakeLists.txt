cmake_minimum_required(VERSION 3.22)

# Static MSVC runtime: the plugin won't need the VC++ redistributable on the target PC.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(FLBridgeProbe VERSION 0.1.0)

# CMake downloads JUCE by itself (internet is needed on the first configure).
include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.14
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(FLBridgeProbe
    COMPANY_NAME "FLBridge"
    PLUGIN_MANUFACTURER_CODE Flbr
    PLUGIN_CODE Fbpr
    FORMATS VST3
    PRODUCT_NAME "FL Bridge Probe"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE)

target_sources(FLBridgeProbe PRIVATE
    PluginProcessor.cpp
    PluginProcessor.h
    PluginEditor.cpp
    PluginEditor.h)

target_compile_definitions(FLBridgeProbe PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(FLBridgeProbe
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
