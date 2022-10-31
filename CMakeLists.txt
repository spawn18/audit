cmake_minimum_required(VERSION 3.0)

project(AudioEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WIN32_EXECUTABLE TRUE)
set(APP_ICON "icons/icon.rc")

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

set(PROJECT_SOURCES
        Main.cpp
        Track.cpp
        Track.hpp
        Editor.cpp
        Editor.hpp
        Selection.cpp
        Selection.hpp
        Editor.ui
        Properties.hpp
        Properties.cpp
        Properties.ui
        Time.hpp
        Time.cpp
        Wave.cpp
        Wave.hpp
        Utils.hpp
        Utils.cpp
        Audio.cpp
        TODO.txt
        Audio.hpp
        resources.qrc
)

add_executable(AudioEditor ${PROJECT_SOURCES} ${APP_ICON})
target_link_libraries(AudioEditor PRIVATE Qt5::Widgets Qt5::Multimedia)
