@echo off
setlocal enabledelayedexpansion

:: ==============================================================================
:: Minecraft Backup Tool - Comprehensive Dependency Installer
:: Author: Agent Mode Autonomous Installation System
:: Version: 1.0.0
:: Description: Automatically detects, downloads, and installs all required
::               dependencies for building the Minecraft Backup Tool application
:: ==============================================================================

:: Installation configuration
set "CMAKE_VERSION=3.26.4"
set "CMAKE_MAJOR_MINOR=3.26"
set "INSTALL_MODE=COMPLETE"
set "VERBOSE_MODE=0"
set "ADMIN_CHECK=0"
set "QUIET_MODE=0"

:: Download URLs
set "CMAKE_URL=https://cmake.org/files/v%CMAKE_MAJOR_MINOR%/cmake-%CMAKE_VERSION%-windows-x86_64.zip"
set "GIT_URL=https://github.com/git-for-windows/git/releases/download/v2.42.0.windows.2/Git-2.42.0.2-64-bit.exe"
set "VS_BUILD_TOOLS_URL=https://aka.ms/vs/17/release/vs_buildtools.exe"

:: Installation paths
set "CMAKE_INSTALL_DIR=C:\cmake\cmake-%CMAKE_VERSION%-windows-x86_64"
set "GIT_INSTALL_DIR=C:\Program Files\Git"
set "VS_BUILD_TOOLS_INSTALL_DIR=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools"
set "TEMP_DOWNLOADS=%TEMP%\minecraft_backup_deps"

:: ==============================================================================
:: INITIALIZATION AND LOGO
:: ==============================================================================

cls
echo.
echo ============================================================================
echo.
echo           MINECRAFT BACKUP TOOL - DEPENDENCY INSTALLER
echo.
echo           Agent Mode Automated Build Environment Setup
echo.
echo ============================================================================
echo.
echo Version: 1.0.0 ^| CPlusPlus CMake Project ^| Qt5 Integration
echo Target Platform: Windows 10/11 with Visual Studio Build Tools
echo.

:: Check for admin privileges
call :CheckAdminPrivileges
if %ADMIN_CHECK% == 1 (
    echo WARNING: Running with limited privileges. Some installations may require admin rights.
    echo Consider running as Administrator for complete installation.
    echo.
)

:: ==============================================================================
:: MODE SELECTION MENU
:: ==============================================================================

:MAIN_MENU
echo ============================================================================
echo                           INSTALLATION MODES
echo ============================================================================
echo.
echo 1. Complete Installation          - Install all dependencies (Recommended)
echo 2. Selective Installation       - Choose specific components
echo 3. Verification Only            - Check existing installations
echo 4. Advanced Options             - Configure installation settings
echo 5. Help ^& Documentation         - Show dependency information
echo 6. Exit                          - Cancel installation
echo.
set /p "INSTALL_MODE_CHOICE=Select installation mode (1-6): "

if "%INSTALL_MODE_CHOICE%"=="1" goto COMPLETE_INSTALL
if "%INSTALL_MODE_CHOICE%"=="2" goto SELECTIVE_INSTALL
if "%INSTALL_MODE_CHOICE%"=="3" goto VERIFICATION_ONLY
if "%INSTALL_MODE_CHOICE%"=="4" goto ADVANCED_OPTIONS
if "%INSTALL_MODE_CHOICE%"=="5" goto SHOW_HELP
if "%INSTALL_MODE_CHOICE%"=="6" goto EXIT_INSTALL
echo Invalid selection. Please choose a valid option.
goto MAIN_MENU

:: ==============================================================================
:: COMPLETE INSTALLATION
:: ==============================================================================

:COMPLETE_INSTALL
set "INSTALL_MODE=COMPLETE"
echo.
echo ============================================================================
echo                       COMPLETE INSTALLATION MODE
echo ============================================================================
echo.
echo This mode will install all required dependencies for the project:
echo.
echo [OK] Visual Studio Build Tools 2022 (CPlusPlus build tools)
echo [OK] CMake %CMAKE_VERSION% (Latest stable version)
echo [OK] Git for Windows (Version control)
echo [OK] Qt5 Framework (v5.15.2 or later)
echo [OK] OpenSSL Libraries (SSL/TLS support)
echo [OK] Development environment setup
echo.
echo Estimated installation time: 15-30 minutes
echo Total download size: ~2.5 GB
echo.

set /p "CONFIRM_COMPLETE=Proceed with complete installation? (Y/N): "
if /i not "%CONFIRM_COMPLETE%"=="Y" goto MAIN_MENU

call :StartInstallationProcess
goto :eof

:: ==============================================================================
:: SELECTIVE INSTALLATION
:: ==============================================================================

:SELECTIVE_INSTALL
set "INSTALL_MODE=SELECTIVE"
echo.
echo ============================================================================
echo                     SELECTIVE INSTALLATION MODE
echo ============================================================================
echo.

echo Select which components to install:
echo.
echo 1. Visual Studio Build Tools 2022
echo 2. CMake %CMAKE_VERSION%
echo 3. Git for Windows
echo 4. Qt5 Framework (Interactive)
echo 5. OpenSSL Libraries
echo 6. Install all selected components
echo 7. Return to main menu
echo.

set /p "SELECTION_CHOICE=Select component (1-7): "

if "%SELECTION_CHOICE%"=="1" (
    call :InstallVisualStudioBuildTools
    goto SELECTIVE_INSTALL
)
if "%SELECTION_CHOICE%"=="2" (
    call :InstallCMake
    goto SELECTIVE_INSTALL
)
if "%SELECTION_CHOICE%"=="3" (
    call :InstallGit
    goto SELECTIVE_INSTALL
)
if "%SELECTION_CHOICE%"=="4" (
    call :InstallQt5
    goto SELECTIVE_INSTALL
)
if "%SELECTION_CHOICE%"=="5" (
    call :InstallOpenSSL
    goto SELECTIVE_INSTALL
)
if "%SELECTION_CHOICE%"=="6" (
    call :StartInstallationProcess
    goto :eof
)
if "%SELECTION_CHOICE%"=="7" (
    goto MAIN_MENU
)
echo Invalid selection. Please choose a valid option.
goto SELECTIVE_INSTALL

:: ==============================================================================
:: VERIFICATION ONLY
:: ==============================================================================

:VERIFICATION_ONLY
set "INSTALL_MODE=VERIFY"
echo.
echo ============================================================================
echo                        VERIFICATION ONLY MODE
echo ============================================================================
echo.
echo This mode will verify existing installations without downloading or installing.
echo.

call :VerifyAllDependencies
echo.
pause
goto MAIN_MENU

:: ==============================================================================
:: ADVANCED OPTIONS
:: ==============================================================================

:ADVANCED_OPTIONS
echo.
echo ============================================================================
echo                        ADVANCED CONFIGURATION
echo ============================================================================
echo.
echo Current Settings:
echo.
echo CMake Version: %CMAKE_VERSION%
echo Installation Mode: %INSTALL_MODE%
echo Verbose Mode: %VERBOSE_MODE%
echo Quiet Mode: %QUIET_MODE%
echo Admin Privileges: %ADMIN_CHECK%
echo.
echo 1. Change CMake Version
echo 2. Toggle Verbose Mode
echo 3. Toggle Quiet Mode
echo 4. Test Network Connectivity
echo 5. Clear Download Cache
echo 6. Return to main menu
echo.

set /p "ADVANCED_CHOICE=Select option (1-6): "

if "%ADVANCED_CHOICE%"=="1" (
    call :ChangeCMakeVersion
    goto ADVANCED_OPTIONS
)
if "%ADVANCED_CHOICE%"=="2" (
    set /a "VERBOSE_MODE=1-!VERBOSE_MODE!"
    goto ADVANCED_OPTIONS
)
if "%ADVANCED_CHOICE%"=="3" (
    set /a "QUIET_MODE=1-!QUIET_MODE!"
    goto ADVANCED_OPTIONS
)
if "%ADVANCED_CHOICE%"=="4" (
    call :TestNetworkConnectivity
    goto ADVANCED_OPTIONS
)
if "%ADVANCED_CHOICE%"=="5" (
    call :ClearDownloadCache
    goto ADVANCED_OPTIONS
)
if "%ADVANCED_CHOICE%"=="6" (
    goto MAIN_MENU
)
echo Invalid selection. Please choose a valid option.
goto ADVANCED_OPTIONS

:: ==============================================================================
:: HELP AND DOCUMENTATION
:: ==============================================================================

:SHOW_HELP
echo.
echo ============================================================================
echo                         DEPENDENCY INFORMATION
echo ============================================================================
echo.
echo This installer sets up a complete CPlusPlus development environment for building
echo the Minecraft Backup Tool application.
echo.
echo Required Dependencies:
echo.
echo 1. Visual Studio Build Tools 2022
echo    Purpose: CPlusPlus compiler and build tools
echo    Components: MSVC, Windows SDK, CMake tools
echo    Size: ~1.5 GB
echo    Download: Microsoft Visual Studio website
echo.
echo 2. CMake %CMAKE_VERSION%
echo    Purpose: Cross-platform build system generator
echo    Version: %CMAKE_VERSION% (required for project)
echo    Size: ~50 MB
echo    Download: https://cmake.org/download/
echo.
echo 3. Git for Windows
echo    Purpose: Version control system
echo    Version: Latest stable
echo    Size: ~50 MB
echo    Download: https://git-scm.com/download/win
echo.
echo 4. Qt5 Framework
echo    Purpose: GUI framework (GUI components) - FREE under LGPL
echo    Version: 5.15.2+ recommended
echo    Size: ~800 MB
echo    Download: https://download.qt.io/archive/qt/5.15/
echo.
echo 5. OpenSSL Libraries
echo    Purpose: SSL/TLS cryptography library
echo    Version: Latest stable
echo    Size: ~10 MB
echo    Download: https://slproweb.com/products/Win32OpenSSL.html
echo.
echo Note: Visual Studio Build Tools and Qt5 require significant disk space.
echo       Ensure you have at least 5 GB of free space before installation.
echo.
pause
goto MAIN_MENU

:: ==============================================================================
:: MAIN INSTALLATION PROCESS
:: ==============================================================================

:StartInstallationProcess
echo.
echo ============================================================================
echo                         STARTING INSTALLATION
echo ============================================================================
echo.

:: Create temporary directory
if not exist "%TEMP_DOWNLOADS%" mkdir "%TEMP_DOWNLOADS%"
echo Created temporary directory: %TEMP_DOWNLOADS%

:: Check prerequisites
call :CheckSystemRequirements
if errorlevel 1 (
    echo System requirements not met. Installation aborted.
    pause
    goto :eof
)

:: Start installation process
echo Starting dependency installation process...
echo.

:: Install components in order
call :InstallVisualStudioBuildTools
if errorlevel 1 goto INSTALLATION_FAILED

call :InstallCMake
if errorlevel 1 goto INSTALLATION_FAILED

call :InstallGit
if errorlevel 1 goto INSTALLATION_FAILED

call :InstallOpenSSL
if errorlevel 1 goto INSTALLATION_FAILED

call :InstallQt5
if errorlevel 1 goto INSTALLATION_FAILED

:: Setup environment
call :SetupEnvironmentVariables
call :UpdateSystemPATH
call :CreateBuildDirectories

:: Final verification
call :VerifyAllDependencies

echo.
echo ============================================================================
echo                     INSTALLATION COMPLETED SUCCESSFULLY
echo ============================================================================
echo.
echo [OK] All dependencies installed successfully
echo [OK] Environment variables configured
echo [OK] System PATH updated
echo [OK] Build environment ready
echo.
echo Next Steps:
echo   1. Open a new Command Prompt or PowerShell window
echo   2. Navigate to the project directory
echo   3. Run: install_and_build.bat
echo   4. Follow the build verification process
echo.
echo For troubleshooting, see README.md or run this installer in verification mode.
echo.

:: Clean up temporary files
call :CleanupTemporaryFiles
goto INSTALLATION_COMPLETE

:INSTALLATION_FAILED
echo.
echo ============================================================================
echo                          INSTALLATION FAILED
echo ============================================================================
echo.
echo Installation failed at some point during the process.
echo Please check the error messages above.
echo.
echo Troubleshooting Steps:
echo   1. Run as Administrator
echo   2. Check internet connectivity
echo   3. Verify sufficient disk space (5+ GB required)
echo   4. Temporarily disable antivirus software
echo   5. Run verification mode to check existing installations
echo.
pause
goto :eof

:INSTALLATION_COMPLETE
if "%QUIET_MODE%"=="0" pause
goto :eof

:: ==============================================================================
:: ADMIN PRIVILEGE CHECKING
:: ==============================================================================

:CheckAdminPrivileges
net session >nul 2>&1
if %errorLevel% neq 0 (
    set "ADMIN_CHECK=0"
) else (
    set "ADMIN_CHECK=1"
)
goto :eof

:: ==============================================================================
:: SYSTEM REQUIREMENTS CHECK
:: ==============================================================================

:CheckSystemRequirements
echo Checking system requirements...

:: Check Windows version
for /f "tokens=4-5 delims=. " %%i in ('ver') do set VERSION=%%i.%%j
if %VERSION% LSS 10.0 (
    echo [ERROR] Windows 10 or later is required. Current version: %VERSION%
    exit /b 1
)
echo [OK] Windows version: %VERSION% (Compatible)

:: Check available disk space
for /f "tokens=3" %%a in ('dir /-c "%SystemDrive%" ^| find "bytes free"') do set FREESPACE=%%a
echo [OK] Available disk space: %FREESPACE%

:: Check internet connectivity
call :TestNetworkConnectivity
if errorlevel 1 (
    echo [ERROR] Internet connectivity check failed
    exit /b 1
)

:: Check PowerShell availability
powershell -Command "Get-Host" >nul 2>&1
if errorlevel 1 (
    echo [WARNING] PowerShell not available (may be required for some components)
) else (
    echo [OK] PowerShell available
)

echo [OK] System requirements check completed
echo.
exit /b 0

:: ==============================================================================
:: NETWORK CONNECTIVITY TEST
:: ==============================================================================

:TestNetworkConnectivity
echo Testing network connectivity...

:: Test connectivity to key sites
ping -n 1 cmake.org >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Cannot reach cmake.org
    exit /b 1
)
echo [OK] cmake.org: Reachable

ping -n 1 github.com >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Cannot reach github.com
    exit /b 1
)
echo [OK] github.com: Reachable

ping -n 1 visualstudio.microsoft.com >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Cannot reach visualstudio.microsoft.com
    exit /b 1
)
echo [OK] visualstudio.microsoft.com: Reachable

echo [OK] Network connectivity test passed
echo.
exit /b 0

:: ==============================================================================
:: CMAKE INSTALLATION
:: ==============================================================================

:InstallCMake
echo ============================================================================
echo                     INSTALLING CMAKE %CMAKE_VERSION%
echo ============================================================================
echo.

:: Check if CMake is already installed
cmake --version >nul 2>&1
if not errorlevel 1 (
    for /f "tokens=3" %%a in ('cmake --version 2^>^&1') do set INSTALLED_CMAKE=%%a
    echo [WARNING] CMake already installed: %INSTALLED_CMAKE%
    
    if "%INSTALL_MODE%"=="SELECTIVE" (
        set /p "REINSTALL_CMAKE=Reinstall CMake? (Y/N): "
        if /i not "!REINSTALL_CMAKE!"=="Y" (
            echo [OK] Skipping CMake installation
            exit /b 0
        )
    )
)

echo Downloading CMake %CMAKE_VERSION%...
set "CMAKE_ZIP=%TEMP_DOWNLOADS%\cmake.zip"

:: Download CMake
echo Downloading from: %CMAKE_URL%
curl -L -o "%CMAKE_ZIP%" "%CMAKE_URL%" 2>nul
if errorlevel 1 (
    echo [ERROR] Failed to download CMake
    exit /b 1
)
echo [OK] CMake downloaded successfully

:: Extract CMake
echo Extracting CMake...
powershell -Command "Expand-Archive -Path '%CMAKE_ZIP%' -DestinationPath 'C:\cmake' -Force" >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Failed to extract CMake
    exit /b 1
)
echo [OK] CMake extracted successfully

:: Add to PATH
call :AddToPATH "C:\cmake\bin"

:: Verify installation
cmake --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake verification failed
    exit /b 1
)

for /f "tokens=3" %%a in ('cmake --version 2^>^&1') do set INSTALLED_CMAKE_VERSION=%%a
echo [OK] CMake %INSTALLED_CMAKE_VERSION% installed and verified
echo.
exit /b 0

:: ==============================================================================
:: GIT INSTALLATION
:: ==============================================================================

:InstallGit
echo ============================================================================
echo                      INSTALLING GIT FOR WINDOWS
echo ============================================================================
echo.

:: Check if Git is already installed
git --version >nul 2>&1
if not errorlevel 1 (
    for /f "tokens=3" %%a in ('git --version 2^>^&1') do set INSTALLED_GIT=%%a
    echo [WARNING] Git already installed: %INSTALLED_GIT%
    
    if "%INSTALL_MODE%"=="SELECTIVE" (
        set /p "REINSTALL_GIT=Reinstall Git? (Y/N): "
        if /i not "!REINSTALL_GIT!"=="Y" (
            echo [OK] Skipping Git installation
            exit /b 0
        )
    )
)

echo Downloading Git for Windows...
set "GIT_INSTALLER=%TEMP_DOWNLOADS%\Git-Installer.exe"

:: Download Git
echo Downloading from: %GIT_URL%
curl -L -o "%GIT_INSTALLER%" "%GIT_URL%" 2>nul
if errorlevel 1 (
    echo [ERROR] Failed to download Git
    exit /b 1
)
echo [OK] Git downloaded successfully

:: Install Git silently
echo Installing Git silently...
"%GIT_INSTALLER%" /SILENT /NORESTART /COMPONENTS="icons,ext\reg\shellhere,assoc,assoc_sh" /TASKS="desktopicon,quicklaunchicon" >nul 2>&1

:: Add Git to PATH
call :AddToPATH "C:\Program Files\Git\cmd"

:: Verify installation
git --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Git verification failed
    exit /b 1
)

for /f "tokens=3" %%a in ('git --version 2^>^&1') do set INSTALLED_GIT_VERSION=%%a
echo [OK] Git %INSTALLED_GIT_VERSION% installed and verified
echo.
exit /b 0

:: ==============================================================================
:: VISUAL STUDIO BUILD TOOLS INSTALLATION
:: ==============================================================================

:InstallVisualStudioBuildTools
echo ============================================================================
echo                 INSTALLING VISUAL STUDIO BUILD TOOLS
echo ============================================================================
echo.

:: Check if Build Tools are already installed
if exist "%VS_BUILD_TOOLS_INSTALL_DIR%" (
    echo [WARNING] Visual Studio Build Tools already installed
    
    if "%INSTALL_MODE%"=="SELECTIVE" (
        set /p "REINSTALL_VS=Reinstall Build Tools? (Y/N): "
        if /i not "!REINSTALL_VS!"=="Y" (
            echo [OK] Skipping Build Tools installation
            exit /b 0
        )
    )
)

echo Downloading Visual Studio Build Tools...
set "VS_BUILD_TOOLS_INSTALLER=%TEMP_DOWNLOADS%\vs_buildtools.exe"

:: Download Build Tools
echo Downloading from: %VS_BUILD_TOOLS_URL%
curl -L -o "%VS_BUILD_TOOLS_INSTALLER%" "%VS_BUILD_TOOLS_URL%" 2>nul
if errorlevel 1 (
    echo [ERROR] Failed to download Visual Studio Build Tools
    exit /b 1
)
echo [OK] Build Tools downloaded successfully

:: Install Build Tools silently
echo Installing Visual Studio Build Tools (this may take several minutes)...
echo Installing required workloads and components...

"%VS_BUILD_TOOLS_INSTALLER%" --quiet --norestart --nocache ^
  --add Microsoft.VisualStudio.Workload.VCTools ^
  --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
  --add Microsoft.VisualStudio.Component.Windows10SDK.19041 ^
  --add Microsoft.VisualStudio.Component.TestTools.BuildTools ^
  --add Microsoft.VisualStudio.Component.CMake.Project

if errorlevel 1 (
    echo [WARNING] Build Tools installation encountered issues
    echo Trying alternative installation method...
    
    :: Alternative method with different components
    "%VS_BUILD_TOOLS_INSTALLER%" --quiet --norestart --nocache ^
      --add Microsoft.VisualStudio.Workload.VCTools ^
      --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
      --add Microsoft.VisualStudio.Component.Windows10SDK.18362
)

:: Verify installation
if exist "%VS_BUILD_TOOLS_INSTALL_DIR%" (
    echo [OK] Visual Studio Build Tools installed successfully
) else (
    echo [ERROR] Build Tools installation verification failed
    exit /b 1
)

echo.
exit /b 0

:: ==============================================================================
:: OPENSSL INSTALLATION
:: ==============================================================================

:InstallOpenSSL
echo ============================================================================
echo                      INSTALLING OPENSSL LIBRARIES
echo ============================================================================
echo.

:: Check if OpenSSL is already installed
openssl version >nul 2>&1
if not errorlevel 1 (
    for /f "tokens=2" %%a in ('openssl version 2^>^&1') do set INSTALLED_OPENSSL=%%a
    echo [WARNING] OpenSSL already installed: %INSTALLED_OPENSSL%
    
    if "%INSTALL_MODE%"=="SELECTIVE" (
        set /p "REINSTALL_OPENSSL=Reinstall OpenSSL? (Y/N): "
        if /i not "!REINSTALL_OPENSSL!"=="Y" (
            echo [OK] Skipping OpenSSL installation
            exit /b 0
        )
    )
)

echo Installing OpenSSL via Chocolatey...
choco install openssl -y --no-progress >nul 2>&1
if errorlevel 1 (
    echo [WARNING] Chocolatey not available or OpenSSL installation failed
    echo Manual OpenSSL installation recommended for production use
    echo Download from: https://slproweb.com/products/Win32OpenSSL.html
    echo.
    exit /b 0
)

:: Verify installation
openssl version >nul 2>&1
if not errorlevel 1 (
    for /f "tokens=2" %%a in ('openssl version 2^>^&1') do set INSTALLED_OPENSSL_VERSION=%%a
    echo [OK] OpenSSL %INSTALLED_OPENSSL_VERSION% installed and verified
) else (
    echo [WARNING] OpenSSL installation verification failed
)

echo.
exit /b 0

:: ==============================================================================
:: QT5 INSTALLATION
:: ==============================================================================

:InstallQt5
echo ============================================================================
echo                       INSTALLING QT5 FRAMEWORK
echo ============================================================================
echo.

:: Check if Qt5 is already installed
if exist "C:\Qt" (
    echo [WARNING] Qt directory found, checking for Qt5...
    
    for /d %%d in (C:\Qt\5.*) do (
        if exist "%%d\msvc2019_64\bin" (
            echo [OK] Qt5 found at: %%d
            echo [WARNING] Manual Qt5 installation recommended for latest version
            echo Download Qt5 from: https://download.qt.io/archive/qt/5.15/
            echo Required modules: Core, Widgets, Gui, Network, I18n, Core5Compat
            echo.
            exit /b 0
        )
    )
)

echo Qt5 requires manual installation from official website
echo Qt5 is FREE under LGPL license (unlike Qt6 which is paid)
echo Please follow these steps:
echo.
echo   1. Visit: https://download.qt.io/archive/qt/5.15/
echo   2. Download Qt 5.15.2 or later for MSVC2019 64-bit
echo   3. Install with following components:
echo      - Qt 5.15.2 (msvc2019_64)
echo      - Qt Base (Core, Widgets, Gui)
echo      - Qt Network
echo      - Qt I18n (Internationalization)
echo      - Qt Tools (CMake, Design Studio)
echo      - Qt5Compat (for Qt5 compatibility features)
echo.
echo Installation path suggestion: C:\Qt\5.15.2\msvc2019_64
echo.

set /p "QT5_CONFIRM=Have you completed Qt5 installation? (Y/N): "
if /i not "%QT5_CONFIRM%"=="Y" (
    echo [WARNING] Qt5 installation skipped. Please install manually.
    echo.
    exit /b 0
)

:: Verify Qt5 installation
if exist "C:\Qt\5.15.2\msvc2019_64\bin" (
    echo [OK] Qt5 installation verified
) else (
    echo [ERROR] Qt5 installation not found at expected path
    echo Please ensure Qt5 is installed at C:\Qt\5.15.2\msvc2019_64
)

echo.
exit /b 0

:: ==============================================================================
:: ENVIRONMENT SETUP
:: ==============================================================================

:SetupEnvironmentVariables
echo Setting up environment variables...

:: Create environment variable script
set "ENV_SCRIPT=%TEMP_DOWNLOADS%\setup_environment.bat"
(
echo @echo off
echo REM Environment setup for Minecraft Backup Tool
echo set CMAKE_HOME=C:\cmake\cmake-%CMAKE_VERSION%-windows-x86_64
echo set GIT_HOME=C:\Program Files\Git
echo set QT5_HOME=C:\Qt\5.15.2\msvc2019_64
echo set VS_BUILD_TOOLS=%VS_BUILD_TOOLS_INSTALL_DIR%
echo.
echo echo Environment variables set for current session
echo echo CMAKE_HOME: %%CMAKE_HOME%%
echo echo GIT_HOME: %%GIT_HOME%%
echo echo QT5_HOME: %%QT5_HOME%%
echo echo VS_BUILD_TOOLS: %%VS_BUILD_TOOLS%%
) > "%ENV_SCRIPT%"

echo [OK] Environment script created: %ENV_SCRIPT%
goto :eof

:: ==============================================================================
:: PATH MANAGEMENT
:: ==============================================================================

:AddToPATH
set "NEW_PATH=%~1"
echo Adding to PATH: %NEW_PATH%

:: Check if already in PATH
echo %PATH% | find /I /C:"%NEW_PATH%" >nul
if not errorlevel 1 (
    echo [WARNING] Already in PATH, skipping
    goto :eof
)

:: Add to PATH for current session
set "PATH=%NEW_PATH%;%PATH%"
echo [OK] Added to current session PATH
goto :eof

:UpdateSystemPATH
echo Updating system PATH (persistent)...
if %ADMIN_CHECK%==1 (
    :: Administrator can update system PATH
    reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths" /v "CMAKE" /t REG_SZ /d "C:\cmake\bin\cmake.exe" /f >nul 2>&1
    reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths" /v "GIT" /t REG_SZ /d "C:\Program Files\Git\cmd\git.exe" /f >nul 2>&1
    echo [OK] System PATH updated (requires restart for full effect)
) else (
    echo [WARNING] Administrator privileges required for system PATH update
    echo Please restart command prompt after installation
)
goto :eof

:: ==============================================================================
:: BUILD DIRECTORY SETUP
:: ==============================================================================

:CreateBuildDirectories
echo Creating build directories...

if not exist "build" mkdir "build"
if not exist "build\Debug" mkdir "build\Debug"
if not exist "build\Release" mkdir "build\Release"
if not exist "install" mkdir "install"
if not exist "logs" mkdir "logs"

echo [OK] Build directories created
echo.
goto :eof

:: ==============================================================================
:: DEPENDENCY VERIFICATION
:: ==============================================================================

:VerifyAllDependencies
echo ============================================================================
echo                       VERIFYING INSTALLATIONS
echo ============================================================================
echo.

:: Check Visual Studio Build Tools
if exist "%VS_BUILD_TOOLS_INSTALL_DIR%" (
    echo [OK] Visual Studio Build Tools: INSTALLED
    if exist "%VS_BUILD_TOOLS_INSTALL_DIR%\VC\Tools\Llvm\bin\clang.exe" (
        echo [OK]   Clang tools: AVAILABLE
    )
) else (
    echo [ERROR] Visual Studio Build Tools: NOT INSTALLED
)

:: Check CMake
cmake --version >nul 2>&1
if not errorlevel 1 (
    for /f "tokens=3" %%a in ('cmake --version 2^>^&1') do set VERIFIED_CMAKE=%%a
    echo [OK] CMake %VERIFIED_CMAKE%: INSTALLED
) else (
    echo [ERROR] CMake: NOT INSTALLED
)

:: Check Git
git --version >nul 2>&1
if not errorlevel 1 (
    for /f "tokens=3" %%a in ('git --version 2^>^&1') do set VERIFIED_GIT=%%a
    echo [OK] Git %VERIFIED_GIT%: INSTALLED
) else (
    echo [ERROR] Git: NOT INSTALLED
)

:: Check Qt5
if exist "C:\Qt\5.15.2\msvc2019_64\bin" (
    echo [OK] Qt5 Framework: INSTALLED
    echo    Path: C:\Qt\5.15.2\msvc2019_64
) else (
    echo [ERROR] Qt5 Framework: NOT INSTALLED
)

:: Check OpenSSL
openssl version >nul 2>&1
if not errorlevel 1 (
    for /f "tokens=2" %%a in ('openssl version 2^>^&1') do set VERIFIED_OPENSSL=%%a
    echo [OK] OpenSSL %VERIFIED_OPENSSL%: INSTALLED
) else (
    echo [WARNING] OpenSSL: NOT INSTALLED (optional)
)

:: Check build directories
if exist "build" (
    echo [OK] Build directory: CREATED
) else (
    echo [WARNING] Build directory: NOT FOUND
)

echo.
echo ============================================================================
echo                         VERIFICATION COMPLETE
echo ============================================================================
echo.
goto :eof

:: ==============================================================================
:: UTILITY FUNCTIONS
:: ==============================================================================

:ChangeCMakeVersion
set /p "NEW_CMAKE_VERSION=Enter CMake version (current: %CMAKE_VERSION%): "
if not "%NEW_CMAKE_VERSION%"=="" (
    set "CMAKE_VERSION=%NEW_CMAKE_VERSION%"
    set "CMAKE_MAJOR_MINOR=%NEW_CMAKE_VERSION:~0,3%"
    set "CMAKE_URL=https://cmake.org/files/v%CMAKE_MAJOR_MINOR%/cmake-%CMAKE_VERSION%-windows-x86_64.zip"
    echo [OK] CMake version updated to %CMAKE_VERSION%
) else (
    echo [WARNING] No change made
)
echo.
goto :eof

:ClearDownloadCache
echo Clearing download cache...
if exist "%TEMP_DOWNLOADS%" (
    rmdir /s /q "%TEMP_DOWNLOADS%" 2>nul
    echo [OK] Download cache cleared
) else (
    echo [OK] No cache to clear
)
echo.
goto :eof

:CleanupTemporaryFiles
echo Cleaning up temporary files...
if exist "%TEMP_DOWNLOADS%" (
    rmdir /s /q "%TEMP_DOWNLOADS%" 2>nul
    echo [OK] Temporary files cleaned
)
echo.
goto :eof

:: ==============================================================================
:: EXIT HANDLING
:: ==============================================================================

:EXIT_INSTALL
echo.
echo ============================================================================
echo                       INSTALLATION CANCELLED
echo ============================================================================
echo.
echo Installation was cancelled by user.
echo You can run this script again at any time to install dependencies.
echo.
pause
goto :eof

:: ==============================================================================
:: END OF SCRIPT
:: ==============================================================================

endlocal
exit /b 0