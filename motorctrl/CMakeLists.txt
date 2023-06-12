cmake_minimum_required(VERSION 3.5)

project(motorctrl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(MY_LIBRARY_DIR /path/to/library)
#set(MY_LIBRARY_NAME mylibrary)
#这里的 `/path/to/library` 是库的路径，`mylibrary` 是库的名称。

set(CARDS_LIBRARY_DIR F:/MotionControl/qt/lib)
set(CARDS_INCLUDE_DIR F:/MotionControl/qt/include)
set(CARDS_LIBRARY_NAME Dmc1000)

#include_directories(${MY_LIBRARY_DIR}/include)
#这里的 `${MY_LIBRARY_DIR}/include` 是库的头文件路径。
include_directories(${CARDS_INCLUDE_DIR})


#link_directories(${MY_LIBRARY_DIR})
#target_link_libraries(${PROJECT_NAME} ${MY_LIBRARY_NAME})
#这里的 `${MY_LIBRARY_DIR}` 是库文件的路径，`${PROJECT_NAME}` 是项目名称。

link_directories(${CARDS_LIBRARY_DIR})




find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        axis.h
        axis.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(motorctrl
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET motorctrl APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(motorctrl SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(motorctrl
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(motorctrl PRIVATE Qt${QT_VERSION_MAJOR}::Widgets  ${CARDS_LIBRARY_NAME})

set_target_properties(motorctrl PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS motorctrl
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(motorctrl)
endif()
