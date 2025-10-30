#include "application.h"
#include "gui/mainwindow.h"
#include "i18n/translationmanager.h"

#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QProcess>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>
#include <QSystemTrayIcon>

namespace MinecraftBackupTool {

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    // Connect quit signal
    connect(this, &QApplication::aboutToQuit, this, &Application::onAboutToQuit);
    
    // Setup application paths
    m_applicationDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    m_backupPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/backups";
    m_tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/minecraft-backup";
    
    // Create directories
    QDir().mkpath(m_applicationDataPath);
    QDir().mkpath(m_configPath);
    QDir().mkpath(m_backupPath);
    QDir().mkpath(m_tempPath);
}

Application::~Application() = default;

bool Application::initialize()
{
    setupLogging();
    setupGlobalSettings();
    setupEnvironmentVariables();
    return true;
}

void Application::cleanup()
{
    // Cleanup temporary files
    QDir tempDir(m_tempPath);
    if (tempDir.exists()) {
        tempDir.removeRecursively();
    }
}

void Application::createSystemTray()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }
    
    m_trayIcon = std::make_unique<QSystemTrayIcon>(this);
    m_trayIcon->setIcon(QIcon(":/icons/app-icon"));
    
    setupTrayActions();
    
    m_trayIcon->show();
    
    // Show welcome message
    showTrayMessage(tr("Minecraft Backup Tool"), 
                   tr("Application started successfully. Available in system tray."));
}

void Application::setupTrayActions()
{
    m_trayMenu = std::make_unique<QMenu>();
    
    QAction *showAction = m_trayMenu->addAction(tr("Show Window"));
    connect(showAction, &QAction::triggered, this, &Application::onShowMainWindow);
    
    m_trayMenu->addSeparator();
    
    QAction *exitAction = m_trayMenu->addAction(tr("Exit"));
    connect(exitAction, &QAction::triggered, this, &Application::onExitApplication);
    
    m_trayIcon->setContextMenu(m_trayMenu.get());
    
    // Handle tray icon activation
    connect(m_trayIcon.get(), &QSystemTrayIcon::activated,
            this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) {
            onShowMainWindow();
        }
    });
}

void Application::showTrayMessage(const QString &title, const QString &message)
{
    if (m_trayIcon && m_trayIcon->isVisible()) {
        m_trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 5000);
    }
}

QString Application::getApplicationDataPath() const
{
    return m_applicationDataPath;
}

QString Application::getConfigPath() const
{
    return m_configPath;
}

QString Application::getBackupPath() const
{
    return m_backupPath;
}

QString Application::getTempPath() const
{
    return m_tempPath;
}

void Application::setupLogging()
{
    // Set up logging to file
    QString logPath = m_applicationDataPath + "/logs";
    QDir().mkpath(logPath);
    
    QLoggingCategory::installFilter([](QtMsgType type, const QLoggingCategory &category) {
        // Always show critical and warning messages
        if (type == QtCriticalMsg || type == QtWarningMsg) {
            return true;
        }
        
        // Only show debug messages for our application
        if (type == QtDebugMsg) {
            return category.categoryName().startsWith("MinecraftBackupTool");
        }
        
        return false;
    });
}

void Application::logMessage(const QString &message, QtMsgType type)
{
    QLoggingCategory category("MinecraftBackupTool.Application");
    switch (type) {
        case QtDebugMsg:
            qDebug(category) << message;
            break;
        case QtInfoMsg:
            qInfo(category) << message;
            break;
        case QtWarningMsg:
            qWarning(category) << message;
            break;
        case QtCriticalMsg:
            qCritical(category) << message;
            break;
    }
}

void Application::onAboutToQuit()
{
    cleanup();
}

void Application::onShowMainWindow()
{
    if (m_mainWindow) {
        m_mainWindow->show();
        m_mainWindow->raise();
        m_mainWindow->activateWindow();
    }
}

void Application::onExitApplication()
{
    quit();
}

void Application::setupGlobalSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, 
                      organizationName(), applicationName());
    
    // Set default values if not present
    if (!settings.contains("ui/theme")) {
        settings.setValue("ui/theme", "light");
    }
    
    if (!settings.contains("ui/language")) {
        settings.setValue("ui/language", "en");
    }
    
    if (!settings.contains("agent/enabled")) {
        settings.setValue("agent/enabled", false);
    }
    
    if (!settings.contains("cloud/auto-sync")) {
        settings.setValue("cloud/auto-sync", false);
    }
}

void Application::setupEnvironmentVariables()
{
    // Set up environment variables for Google APIs
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    
    // Google Drive API requires OAuth2 credentials
    // These will be set later when user configures cloud services
    
    QCoreApplication::setEnvironment(env);
}

} // namespace MinecraftBackupTool