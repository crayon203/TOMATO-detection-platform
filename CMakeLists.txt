include_directories(${CMAKE_CURRENT_SOURCE_DIR})
cmake_minimum_required(VERSION 3.16)

project(tomatos VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        AnnotWidget.h
        AnnotWidget.cpp
        database.h
        database.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(tomatos
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(tomatos SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(tomatos
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(tomatos PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.tomatos)
endif()
set_target_properties(tomatos PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS tomatos
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(tomatos)
endif()

# 构建后自动复制图片格式插件和 Python 辅助脚本
add_custom_command(TARGET tomatos POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/convert_img.py"
        "$<TARGET_FILE_DIR:tomatos>/convert_img.py"
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${QT6_INSTALL_PREFIX}/plugins/imageformats"
        "$<TARGET_FILE_DIR:tomatos>/imageformats"
    COMMENT "Deploying image format plugins and Python helper scripts..."
)
