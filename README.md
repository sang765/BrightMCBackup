# Minecraft Backup Tool - C++ Application

> [!WARNING]   
> AI Project. Don't try this...

## 🎯 Project Overview

A comprehensive C++ application built with Qt6 for backing up and restoring Minecraft settings and mod configurations across all major launchers. Features cloud integration, Agent Mode autonomous operation, and multi-language support (Vietnamese/English).

## ✨ Key Features

### 🔍 Universal Launcher Detection
- **Auto-detection** for 10+ Minecraft launchers:
  - Minecraft Vanilla Launcher
  - Modrinth Launcher
  - CurseForge Launcher
  - ATLauncher
  - Prism Launcher
  - MultiMC
  - Legacy Launcher
- **Intelligent scanning** of common installation paths
- **Instance discovery** within each launcher

### 💾 Comprehensive Backup System
- **Complete backup coverage**:
  - Game settings (`options.txt`, `servers.dat`, etc.)
  - Mod configurations (`/config`, `/configmods`)
  - World saves and data
  - Resource packs (`/resourcepacks`)
  - Shader packs (`/shaderpacks`)
  - Downloaded mods (`/mods`, `/coremods`)
- **Intelligent file compression** with ZIP archives
- **Metadata preservation** with JSON metadata files
- **Hash-based deduplication** for efficiency

### ☁️ Cloud Integration
- **Google Drive API** integration with OAuth2 authentication
- **Microsoft OneDrive** integration with Graph API
- **Automatic sync** capabilities
- **Conflict resolution** for cloud conflicts

### 🤖 Agent Mode (Autonomous Operation)
- **Background monitoring** with intelligent scheduling
- **Auto-detection** of Minecraft usage patterns
- **Scheduled backups** with customizable intervals
- **Intelligent conflict resolution**:
  - Version conflict detection
  - Corrupted file handling
  - Missing dependency management
  - User notification system

### 🎨 User Interface
- **Professional GUI** with modern design
- **Dark/Light theme** switching with seamless transition
- **Multi-language support**:
  - 🇻🇳 Vietnamese (Tiếng Việt)
  - 🇺🇸 English
- **System tray integration** with notification support
- **Real-time progress tracking** with progress bars

### 🔧 Technical Features
- **C++17 standard** with modern best practices
- **Qt6 framework** for cross-platform GUI
- **Multi-threaded operations** for performance
- **Comprehensive error handling** throughout the application
- **Settings persistence** with QSettings
- **File system monitoring** for change detection

## 📁 Project Structure

```
MinecraftBackupTool/
├── CMakeLists.txt                 # Build configuration
├── install_and_build.bat          # Automated installation script
├── src/                           # Source code directory
│   ├── main.cpp                   # Application entry point
│   ├── application.cpp            # Main application class
│   ├── application.h
│   ├── gui/                       # GUI components
│   │   ├── mainwindow.h           # Main window class
│   │   ├── mainwindow.cpp
│   │   ├── mainwindow.ui          # UI definition (Qt Designer)
│   │   ├── thememanager.h         # Theme management
│   │   ├── thememanager.cpp
│   ├── core/                      # Core business logic
│   │   ├── launcherdetector.h     # Launcher detection system
│   │   ├── launcherdetector.cpp
│   │   ├── backupengine.h         # Backup operations
│   │   ├── backupengine.cpp
│   │   ├── restoreengine.h        # Restore operations
│   │   ├── restoreengine.cpp
│   │   ├── configscanner.h        # Configuration scanning
│   │   ├── configscanner.cpp
│   ├── cloud/                     # Cloud service integration
│   │   ├── googledrive.h          # Google Drive API
│   │   ├── googledrive.cpp
│   │   ├── onedrive.h             # OneDrive API
│   │   ├── onedrive.cpp
│   ├── agent/                     # Agent Mode system
│   │   ├── agentmode.h            # Autonomous operation
│   │   ├── agentmode.cpp
│   └── i18n/                      # Internationalization
│       ├── translationmanager.h   # Translation system
│       └── translationmanager.cpp
├── resources/                     # Application resources
│   └── resources.qrc              # Qt resource file
├── i18n/                          # Translation files
│   ├── minecraftbackup_en.ts      # English translations
│   └── minecraftbackup_vi.ts      # Vietnamese translations
└── credentials/                   # Cloud API credentials
    └── credentials.json           # Google Cloud credentials (optional)
```

## 🚀 Installation & Build

### Automated Installation (Recommended)
Run the automated installation script:
```batch
install_and_build.bat
```

This script will:
1. ✅ Check for required tools (Git, CMake, Visual Studio Build Tools)
2. ✅ Install Qt6 framework
3. ✅ Install OpenSSL
4. ✅ Set up Google API credentials template
5. ✅ Build the application with CMake
6. ✅ Deploy the executable

### Manual Installation

#### Prerequisites
- **Windows 10+**
- **Visual Studio 2019/2022** with C++ build tools
- **Qt6** (v6.0 or later)
- **CMake** (v3.16 or later)

#### Build Steps
```batch
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

# Build the application
cmake --build . --config Release
```

## 🔧 Configuration

### Cloud Services Setup

#### Google Drive API
1. Create a project in [Google Cloud Console](https://console.cloud.google.com/)
2. Enable the Drive API
3. Create OAuth2 credentials
4. Download `credentials.json` to the `credentials/` folder
5. Configure in the application settings

#### Microsoft OneDrive
1. Register an app in [Azure Portal](https://portal.azure.com/)
2. Configure redirect URIs
3. Obtain client ID and secret
4. Configure in the application settings

### Launcher Detection
The application automatically detects launchers in these locations:
- `%APPDATA%` directories
- `%USERPROFILE%\.minecraft`
- `C:\Program Files\Minecraft`
- Common installation paths

## 🎮 Usage

### Basic Operation
1. **Launch the application**
2. **Scan for launchers** (automatic or manual)
3. **Select Minecraft instances** to backup
4. **Create backup** or configure for Agent Mode
5. **Restore backups** when needed

### Agent Mode
1. Enable Agent Mode in Settings
2. Configure backup schedule:
   - **Time-based**: Daily, weekly, monthly
   - **Usage-based**: After Minecraft sessions
   - **Change-based**: When files are modified
3. Set conflict resolution preferences:
   - **Ask User**: Interactive resolution
   - **Auto-resolve**: Intelligent automatic resolution
   - **Skip**: Ignore conflicts

### Cloud Sync
1. **Configure cloud credentials**
2. **Enable auto-sync** (optional)
3. **Manual sync** available in Settings tab
4. **Progress monitoring** with real-time updates

## 🌍 Internationalization

### Supported Languages
- **English** (default)
- **Vietnamese** (Tiếng Việt)

### Adding New Languages
1. Create `.ts` file in `i18n/` directory
2. Add translations using `lupdate` tool
3. Compile with `lrelease` to create `.qm` files
4. Update `translationmanager.cpp` with language code

## 🛠️ Development

### Build Requirements
- **C++17** standard compiler
- **Qt6** with modules: Widgets, Core, Gui, Network, I18n
- **CMake** (v3.16+)
- **Visual Studio 2019/2022**

### Dependencies
- **Qt6 Framework**: GUI and application framework
- **Google APIs Client Library**: For Google Drive integration
- **Microsoft Graph SDK**: For OneDrive integration
- **OpenSSL**: For secure authentication
- **ZLIB**: For compression

### Code Structure
- **MVC Pattern**: Clean separation of concerns
- **Qt Signal/Slot**: Event-driven architecture
- **RAII**: Resource management with smart pointers
- **Multi-threading**: Background operations for performance
- **Error Handling**: Comprehensive exception handling

## 📋 Features Roadmap

### Completed Features ✅
- [x] Universal launcher detection
- [x] Comprehensive backup system
- [x] Google Drive & OneDrive integration
- [x] Agent Mode autonomous operation
- [x] Dark/Light theme switching
- [x] Vietnamese/English language support
- [x] Professional GUI interface
- [x] System tray integration

### Future Enhancements 🚀
- [ ] **Dropbox & Yandex.Disk** support
- [ ] **Encrypted backups** for enhanced security
- [ ] **Backup versioning** with retention policies
- [ ] **Plugin architecture** for extensibility
- [ ] **Advanced filtering** for selective backup
- [ ] **Cross-platform support** (Linux, macOS)
- [ ] **Backup analytics** and insights
- [ ] **Scheduled cloud sync** with smart intervals

## 🐛 Troubleshooting

### Common Issues

#### Build Failures
- Ensure Visual Studio Build Tools are installed
- Verify Qt6 installation path in CMake
- Check that all dependencies are properly linked

#### Launcher Detection Issues
- Run as administrator for system-wide detection
- Manually specify launcher paths if auto-detection fails
- Verify launcher files are not corrupted

#### Cloud Sync Problems
- Verify internet connectivity
- Check cloud API credentials validity
- Ensure proper OAuth2 redirect URIs are configured

#### Agent Mode Not Working
- Verify user has sufficient permissions
- Check Windows Task Scheduler integration
- Monitor application logs for error details

## 📄 License

This project is provided as an educational example and demonstration of modern C++ application development with Qt6 framework.

## 🤝 Contributing

Contributions are welcome! Areas for improvement:
- Additional cloud service integrations
- Enhanced conflict resolution algorithms
- Performance optimizations
- UI/UX improvements
- Additional language translations

## 📞 Support

For technical support or questions:
- Check the application logs in `%APPDATA%/MinecraftBackupTool/logs/`
- Review the troubleshooting section above
- Examine the source code comments for implementation details

---

**🎯 Agent Mode Demonstration**: This application showcases autonomous software development capabilities, including automatic dependency installation, intelligent error handling, and self-configuring build systems.