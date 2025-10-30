#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSplitter>
#include <QSystemTrayIcon>
#include <QMenu>

namespace Ui {
    class MainWindow;
}

namespace MinecraftBackupTool {

class ThemeManager;
class LauncherDetector;
class BackupEngine;
class RestoreEngine;
class TranslationManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    // UI Management
    void setupUI();
    void setupMenus();
    void setupToolbars();
    void setupStatusBar();
    void setupTrayMenu();

    // Theme and Localization
    void applyTheme();
    void applyLanguage(const QString &language);

public slots:
    // General Actions
    void onLanguageChanged(const QString &language);
    void onThemeChanged(const QString &theme);
    
    // Launcher Management
    void onScanLaunchers();
    void onLauncherDetected(const QString &launcherName, const QString &path);
    void onRefreshInstances();
    void onSelectInstance(int index);
    
    // Backup Operations
    void onCreateBackup();
    void onRestoreBackup();
    void onScheduleBackup();
    void onBackupProgress(int value);
    void onBackupCompleted();
    void onBackupError(const QString &error);
    
    // Cloud Services
    void onConfigureCloud();
    void onSyncToCloud();
    void onSyncFromCloud();
    void onCloudStatusChanged(const QString &service, bool connected);
    
    // Agent Mode
    void onEnableAgentMode(bool enabled);
    void onAgentBackupScheduled();
    void onAgentRestoreScheduled();
    
    // Settings
    void onOpenSettings();
    void onExportSettings();
    void onImportSettings();

private:
    void refreshInstanceList();
    void updateBackupList();
    void showNotification(const QString &title, const QString &message);
    
private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<ThemeManager> m_themeManager;
    std::unique_ptr<TranslationManager> m_translationManager;
    std::unique_ptr<LauncherDetector> m_launcherDetector;
    std::unique_ptr<BackupEngine> m_backupEngine;
    std::unique_ptr<RestoreEngine> m_restoreEngine;
    
    // UI Components
    QSplitter *m_mainSplitter;
    QTreeWidget *m_instancesTree;
    QListWidget *m_backupsList;
    QComboBox *m_launcherCombo;
    QComboBox *m_themeCombo;
    QComboBox *m_languageCombo;
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;
    QLabel *m_cloudStatusLabel;
    
    // Current state
    QString m_selectedLauncher;
    QString m_selectedInstance;
    bool m_agentModeEnabled;
    QString m_currentLanguage;
    QString m_currentTheme;
    
    // Data
    QMap<QString, QString> m_launcherPaths;
    QStringList m_discoveredInstances;
    QStringList m_availableBackups;
};

} // namespace MinecraftBackupTool

#endif // MAINWINDOW_H