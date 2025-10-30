#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>
#include <memory>

namespace MinecraftBackupTool {

class MainWindow;
class TranslationManager;

class Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application(int &argc, char **argv);
    ~Application() override;

    // Application lifecycle
    bool initialize();
    void cleanup();
    
    // System tray
    void createSystemTray();
    void setupTrayActions();
    void showTrayMessage(const QString &title, const QString &message);
    
    // Settings and configuration
    QString getApplicationDataPath() const;
    QString getConfigPath() const;
    QString getBackupPath() const;
    QString getTempPath() const;
    
    // Application state
    bool isAgentMode() const { return m_agentMode; }
    void setAgentMode(bool enabled) { m_agentMode = enabled; }
    
    // Logging
    void setupLogging();
    void logMessage(const QString &message, QtMsgType type = QtInfoMsg);

public slots:
    void onAboutToQuit();
    void onShowMainWindow();
    void onExitApplication();

private:
    void setupGlobalSettings();
    void setupEnvironmentVariables();
    
private:
    std::unique_ptr<MainWindow> m_mainWindow;
    std::unique_ptr<QSystemTrayIcon> m_trayIcon;
    std::unique_ptr<QMenu> m_trayMenu;
    std::unique_ptr<QTimer> m_agentTimer;
    
    bool m_agentMode = false;
    QString m_applicationDataPath;
    QString m_configPath;
    QString m_backupPath;
    QString m_tempPath;
};

} // namespace MinecraftBackupTool

#endif // APPLICATION_H