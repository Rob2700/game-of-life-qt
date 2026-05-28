cmake_minimum_required(VERSION 3.16)

project(GameOfLife VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(GameOfLifeLib STATIC
    guimanager.cpp
    guimanager.h
    guimanager.ui
    simulationengine.cpp
    simulationengine.h
    grid.cpp
    grid.h
    cell.cpp
    cell.h
)

set(PROJECT_SOURCES
        main.cpp
        guimanager.cpp
        guimanager.h
        guimanager.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GameOfLife
        MANUAL_FINALIZATION
	main.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GameOfLife APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(GameOfLife SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GameOfLife
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(GameOfLifeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(GameOfLifeLib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(GameOfLife PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets 
    GameOfLifeLib
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GameOfLife)
endif()
set_target_properties(GameOfLife PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GameOfLife
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GameOfLife)
endif()
