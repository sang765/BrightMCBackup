cmake_minimum_required(VERSION 3.16)
project(MinecraftBackupTool VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================================
# Qt5 Configuration (Free alternative to Qt6)
# ==============================================================================

# Qt5 module configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt5 configuration hints for CMake
# Common Qt5 installation paths on Windows (can be overridden with CMAKE_PREFIX_PATH)
if(NOT CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64;C:/Qt/5.15.0/msvc2019_64;C:/Qt/5.12.12/msvc2019_64")
endif()

# Message for Qt5 configuration
message(STATUS "Qt5 Prefix Paths: ${CMAKE_PREFIX_PATH}")

# Find required packages - Qt5 is free under LGPL
find_package(Qt5 5.12 COMPONENTS Widgets Core Gui Network I18n REQUIRED)
find_package(Qt5 COMPONENTS Core5Compat REQUIRED)

# Handle optional Qt6 modules (removed Core5Compat as it's not available in Qt6)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# Google Drive API
find_package(PkgConfig REQUIRED)
pkg_check_modules(GOOGLE_API REQUIRED googleapis)

# Microsoft Graph API
find_library(MS_GRAPH_LIB msgraph)

# Source files
set(SOURCES
    src/main.cpp
    src/application.cpp
    src/mainwindow.cpp
    src/gui/mainwindow.h
    src/gui/mainwindow.cpp
    src/gui/thememanager.h
    src/gui/thememanager.cpp
    src/core/launcherdetector.h
    src/core/launcherdetector.cpp
    src/core/configscanner.h
    src/core/configscanner.cpp
    src/core/backupengine.h
    src/core/backupengine.cpp
    src/core/restoreengine.h
    src/cloud/googledrive.h
    src/cloud/googledrive.cpp
    src/cloud/onedrive.h
    src/cloud/onedrive.cpp
    src/agent/agentmode.h
    src/agent/agentmode.cpp
    src/utils/filemanager.h
    src/utils/filemanager.cpp
    src/utils/compression.h
    src/utils/compression.cpp
    src/i18n/translationmanager.h
    src/i18n/translationmanager.cpp
)

# Header files
set(HEADERS
    src/gui/mainwindow.h
    src/gui/thememanager.h
    src/core/launcherdetector.h
    src/core/configscanner.h
    src/core/backupengine.h
    src/cloud/googledrive.h
    src/cloud/onedrive.h
    src/agent/agentmode.h
    src/utils/filemanager.h
    src/utils/compression.h
    src/i18n/translationmanager.h
    src/application.h
)

# UI files
set(UI_FILES
    src/gui/mainwindow.ui
    src/gui/settingsdialog.ui
    src/gui/backupschedule.ui
)

# Translation files
set(TRANSLATIONS
    i18n/minecraftbackup_en.ts
    i18n/minecraftbackup_vi.ts
)

# Resource files
set(RESOURCES
    resources/resources.qrc
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GOOGLE_API_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(MinecraftBackupTool PRIVATE
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::I18n
    Qt5::Core5Compat
    OpenSSL::SSL
    OpenSSL::Crypto
    ZLIB::ZLIB
    ${GOOGLE_API_LIBRARIES}
    ${MS_GRAPH_LIB}
    ws2_32
    wininet
)

# Copy translation files
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    install(FILES ${TRANSLATIONS} DESTINATION "bin/i18n")
endif()

# Copy runtime dependencies
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

install(TARGETS MinecraftBackupTool
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install Qt plugins (Qt5 version)
qt5_generate_deployment_script(
    TARGET MinecraftBackupTool
    SCRIPT qt_deploy_script.cmake
    NO_RUN_DEPENDENCIES
    NO_TRANSLATIONS
    NO_PLUGIN_DIRS
)

# Custom deployment
add_custom_target(deploy
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_BINARY_DIR}/MinecraftBackupTool
    ${CMAKE_INSTALL_PREFIX}/bin
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    ${CMAKE_INSTALL_PREFIX}/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/i18n
    ${CMAKE_INSTALL_PREFIX}/i18n
    DEPENDS MinecraftBackupTool
)