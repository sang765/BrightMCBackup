@echo off
echo =======================================================
echo     Minecraft Backup Tool - Automatic Installation
echo =======================================================
echo.

echo [1/6] Checking for required tools...

REM Check for git
where git >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Installing Git...
    winget install --id Git.Git -e --source winget
    set PATH=%PATH%;C:\Program Files\Git\cmd
)

REM Check for cmake
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Installing CMake...
    winget install --id Kitware.CMake -e --source winget
)

REM Check for Visual Studio Build Tools
where msbuild >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Installing Visual Studio Build Tools...
    winget install --id Microsoft.VisualStudio.2022.BuildTools -e --source winget
)

echo [2/6] Installing Qt6...
winget install --id Qt.Qt6 -e --source winget

echo [3/6] Installing Qt Creator (optional)...
winget install --id Qt.QtCreator -e --source winget

echo [4/6] Installing OpenSSL...
winget install --id OpenSSL.OpenSSL.3 -e --source winget

echo [5/6] Setting up Google API credentials...
if not exist "credentials" mkdir credentials
echo Please create a Google Cloud Project and download credentials.json
echo to the credentials folder for full functionality.
echo This is optional - the app will work without cloud sync.

echo [6/6] Building the application...

REM Create build directory
if exist build rmdir /s /q build
mkdir build
cd build

REM Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed. Please check the errors above.
    pause
    exit /b 1
)

REM Build the application
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Build failed. Please check the errors above.
    pause
    exit /b 1
)

echo.
echo =======================================================
echo           Installation Complete!
echo =======================================================
echo.
echo The Minecraft Backup Tool has been successfully installed.
echo.
echo Executable location: build/Release/MinecraftBackupTool.exe
echo.
echo To run the application:
echo   1. Navigate to build/Release/
echo   2. Run MinecraftBackupTool.exe
echo.
echo Features included:
echo   - Auto-detection of Minecraft launchers
echo   - Backup and restore game settings and mod configs
echo   - Cloud sync with Google Drive and OneDrive
echo   - Agent mode for automatic backups
echo   - Dark/Light theme support
echo   - Vietnamese/English language support
echo.
pause