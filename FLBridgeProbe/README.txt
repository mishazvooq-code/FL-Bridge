FL Bridge Probe - VST3-проба.
Грузит DLL нативного FL-плагина и проверяет, что в ней есть экспорт CreatePlugInstance.
Звук проходит насквозь, плагин ничего не меняет.

СБОРКА БЕЗ УСТАНОВКИ СОФТА (GitHub Actions)
1. Создай репозиторий на GitHub и залей туда содержимое архива (вместе с папкой .github).
2. Открой вкладку Actions и дождись зелёной галки (минут 10).
3. Внутри запуска скачай артефакт FL-Bridge-Probe-VST3. Там папка "FL Bridge Probe.vst3".

ЛОКАЛЬНАЯ СБОРКА (нужны Visual Studio с C++, CMake, Git, интернет)
  cmake -B build -A x64
  cmake --build build --config Release
Результат: build\FLBridgeProbe_artefacts\Release\VST3\FL Bridge Probe.vst3

УСТАНОВКА
Папку "FL Bridge Probe.vst3" целиком положи в C:\Program Files\Common Files\VST3
(или добавь свой путь в настройках DAW) и пересканируй плагины.

ИСПОЛЬЗОВАНИЕ
Повесь плагин на трек как эффект. Вставь полный путь к DLL плагина из папки FL
(например ...\Plugins\Fruity\Generators\Sytrus\Sytrus_x64.dll) или нажми Browse, затем Probe.
Перед пробой сохрани проект: чужая DLL может уронить DAW.
