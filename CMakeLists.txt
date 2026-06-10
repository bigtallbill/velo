cmake_minimum_required(VERSION 3.20)
project(Velo VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -g")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia Svg Concurrent)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
  libavformat libavcodec libavutil libswscale libswresample)

add_executable(velo
  resources.qrc
  src/main.cpp
  src/core/Keyframes.h
  src/core/Model.h          src/core/Model.cpp
  src/core/Document.h       src/core/Document.cpp
  src/media/MediaCache.h    src/media/MediaCache.cpp
  src/effects/Effects.h     src/effects/Effects.cpp
  src/engine/Compositor.h   src/engine/Compositor.cpp
  src/engine/AudioEngine.h  src/engine/AudioEngine.cpp
  src/engine/RenderWorker.h src/engine/RenderWorker.cpp
  src/engine/Exporter.h     src/engine/Exporter.cpp
  src/ui/Theme.h            src/ui/Theme.cpp
  src/ui/MainWindow.h       src/ui/MainWindow.cpp
  src/ui/MediaBin.h         src/ui/MediaBin.cpp
  src/ui/EffectsPanel.h     src/ui/EffectsPanel.cpp
  src/ui/PreviewWidget.h    src/ui/PreviewWidget.cpp
  src/ui/TimelineWidget.h   src/ui/TimelineWidget.cpp
  src/ui/PropertiesPanel.h  src/ui/PropertiesPanel.cpp
  src/ui/Dialogs.h          src/ui/Dialogs.cpp
)

target_include_directories(velo PRIVATE src)
target_link_libraries(velo PRIVATE
  Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia Qt6::Svg Qt6::Concurrent
  PkgConfig::FFMPEG)

install(TARGETS velo RUNTIME DESTINATION bin)
install(FILES dist/velo.desktop DESTINATION share/applications)
install(FILES dist/velo.svg DESTINATION share/icons/hicolor/scalable/apps)
