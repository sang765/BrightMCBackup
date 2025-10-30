#include "mainwindow.h"
#include "thememanager.h"
#include "launcherdetector.h"
#include "backupengine.h"
#include "restoreengine.h"
#include "translationmanager.h"

#include <QApplication>
#include <QTabWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSplitter>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QSettings>
#include <QIcon>

namespace MinecraftBackupTool {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_agentModeEnabled(false)
    , m_currentLanguage("en")
    , m_currentTheme("light")
{
    // Initialize components
    m_themeManager = std::make_unique<ThemeManager>();
    m_translationManager = std::make_unique<TranslationManager>();
    m_launcherDetector = std::make_unique<LauncherDetector>();
    m_backupEngine = std::make_unique<BackupEngine>();
    m_restoreEngine = std::make_unique<RestoreEngine>();
    
    setupUI();
    setupMenus();
    setupToolbars();
    setupStatusBar();
    setupTrayMenu();
    
    // Load settings
    loadSettings();
    
    // Start launcher detection
    onScanLaunchers();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    // Set window properties
    setWindowTitle(tr("Minecraft Backup Tool"));
    setMinimumSize(1200, 800);
    resize(1400, 900);
    
    // Create main widget and layout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Main horizontal splitter
    m_mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    
    // Left panel - Instances
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    QLabel *instancesLabel = new QLabel(tr("Minecraft Instances"));
    leftLayout->addWidget(instancesLabel);
    
    m_instancesTree = new QTreeWidget();
    m_instancesTree->setHeaderLabel(tr("Instances"));
    m_instancesTree->setContextMenuPolicy(Qt::CustomContextMenu);
    leftLayout->addWidget(m_instancesTree);
    
    QPushButton *refreshButton = new QPushButton(tr("Refresh"));
    leftLayout->addWidget(refreshButton);
    
    // Right panel - Details and Actions
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    
    // Tabs for different views
    QTabWidget *tabWidget = new QTabWidget();
    
    // Backups tab
    QWidget *backupsTab = new QWidget();
    QVBoxLayout *backupsLayout = new QVBoxLayout(backupsTab);
    
    m_backupsList = new QListWidget();
    backupsLayout->addWidget(m_backupsList);
    
    QHBoxLayout *backupButtonsLayout = new QHBoxLayout();
    QPushButton *createBackupBtn = new QPushButton(tr("Create Backup"));
    QPushButton *restoreBackupBtn = new QPushButton(tr("Restore"));
    QPushButton *deleteBackupBtn = new QPushButton(tr("Delete"));
    backupButtonsLayout->addWidget(createBackupBtn);
    backupButtonsLayout->addWidget(restoreBackupBtn);
    backupButtonsLayout->addWidget(deleteBackupBtn);
    backupsLayout->addLayout(backupButtonsLayout);
    
    tabWidget->addTab(backupsTab, tr("Backups"));
    
    // Settings tab
    QWidget *settingsTab = new QWidget();
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsTab);
    
    QGroupBox *cloudGroup = new QGroupBox(tr("Cloud Services"));
    QVBoxLayout *cloudLayout = new QVBoxLayout(cloudGroup);
    
    QPushButton *configureCloudBtn = new QPushButton(tr("Configure Cloud"));
    QPushButton *syncToCloudBtn = new QPushButton(tr("Sync to Cloud"));
    QPushButton *syncFromCloudBtn = new QPushButton(tr("Sync from Cloud"));
    cloudLayout->addWidget(configureCloudBtn);
    cloudLayout->addWidget(syncToCloudBtn);
    cloudLayout->addWidget(syncFromCloudBtn);
    
    QGroupBox *agentGroup = new QGroupBox(tr("Agent Mode"));
    QVBoxLayout *agentLayout = new QVBoxLayout(agentGroup);
    
    QCheckBox *enableAgentCheck = new QCheckBox(tr("Enable Agent Mode"));
    QLabel *agentStatusLabel = new QLabel(tr("Agent Status: Inactive"));
    QPushButton *configureScheduleBtn = new QPushButton(tr("Configure Schedule"));
    agentLayout->addWidget(enableAgentCheck);
    agentLayout->addWidget(agentStatusLabel);
    agentLayout->addWidget(configureScheduleBtn);
    
    settingsLayout->addWidget(cloudGroup);
    settingsLayout->addWidget(agentGroup);
    settingsLayout->addStretch();
    
    tabWidget->addTab(settingsTab, tr("Settings"));
    
    rightLayout->addWidget(tabWidget);
    
    // Progress bar
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    rightLayout->addWidget(m_progressBar);
    
    // Add panels to splitter
    m_mainSplitter->addWidget(leftPanel);
    m_mainSplitter->addWidget(rightPanel);
    m_mainSplitter->setSizes({400, 800});
    
    // Set layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(m_mainSplitter);
    
    // Connect signals
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshInstances);
    connect(createBackupBtn, &QPushButton::clicked, this, &MainWindow::onCreateBackup);
    connect(restoreBackupBtn, &QPushButton::clicked, this, &MainWindow::onRestoreBackup);
    connect(deleteBackupBtn, &QPushButton::clicked, this, &MainWindow::onDeleteBackup);
    
    connect(configureCloudBtn, &QPushButton::clicked, this, &MainWindow::onConfigureCloud);
    connect(syncToCloudBtn, &QPushButton::clicked, this, &MainWindow::onSyncToCloud);
    connect(syncFromCloudBtn, &QPushButton::clicked, this, &MainWindow::onSyncFromCloud);
    
    connect(enableAgentCheck, &QCheckBox::toggled, this, &MainWindow::onEnableAgentMode);
    connect(configureScheduleBtn, &QPushButton::clicked, this, &MainWindow::onScheduleBackup);
    
    connect(m_backupEngine.get(), &BackupEngine::progress, this, &MainWindow::onBackupProgress);
    connect(m_backupEngine.get(), &BackupEngine::completed, this, &MainWindow::onBackupCompleted);
    connect(m_backupEngine.get(), &BackupEngine::error, this, &MainWindow::onBackupError);
    
    connect(m_launcherDetector.get(), &LauncherDetector::launcherDetected,
            this, &MainWindow::onLauncherDetected);
}

void MainWindow::setupMenus()
{
    QMenuBar *menuBar = this->menuBar();
    
    // File menu
    QMenu *fileMenu = menuBar->addMenu(tr("&File"));
    QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    
    // Tools menu
    QMenu *toolsMenu = menuBar->addMenu(tr("&Tools"));
    QAction *scanAction = toolsMenu->addAction(tr("&Scan Launchers"));
    connect(scanAction, &QAction::triggered, this, &MainWindow::onScanLaunchers);
    
    QAction *settingsAction = toolsMenu->addAction(tr("&Settings"));
    connect(settingsAction, &QAction::triggered, this, &MainWindow::onOpenSettings);
    
    // Help menu
    QMenu *helpMenu = menuBar->addMenu(tr("&Help"));
    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, tr("About"), 
                          tr("Minecraft Backup Tool v1.0.0\n\n"
                             "A comprehensive backup and restore tool for Minecraft "
                             "settings and mod configurations with cloud integration."));
    });
}

void MainWindow::setupToolbars()
{
    QToolBar *mainToolbar = addToolBar(tr("Main"));
    mainToolbar->addAction(tr("Scan"), this, &MainWindow::onScanLaunchers);
    mainToolbar->addAction(tr("Backup"), this, &MainWindow::onCreateBackup);
    mainToolbar->addAction(tr("Settings"), this, &MainWindow::onOpenSettings);
    
    // Theme and language selectors
    mainToolbar->addSeparator();
    m_themeCombo = new QComboBox();
    m_themeCombo->addItem(tr("Light"), "light");
    m_themeCombo->addItem(tr("Dark"), "dark");
    connect(m_themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
        onThemeChanged(m_themeCombo->itemData(index).toString());
    });
    mainToolbar->addWidget(new QLabel(tr("Theme:")));
    mainToolbar->addWidget(m_themeCombo);
    
    m_languageCombo = new QComboBox();
    m_languageCombo->addItem(tr("English"), "en");
    m_languageCombo->addItem(tr("Tiếng Việt"), "vi");
    connect(m_languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
        QString lang = m_languageCombo->itemData(index).toString();
        onLanguageChanged(lang);
    });
    mainToolbar->addSeparator();
    mainToolbar->addWidget(new QLabel(tr("Language:")));
    mainToolbar->addWidget(m_languageCombo);
}

void MainWindow::setupStatusBar()
{
    m_statusLabel = new QLabel(tr("Ready"));
    statusBar()->addWidget(m_statusLabel);
    
    m_cloudStatusLabel = new QLabel(tr("Cloud: Disconnected"));
    statusBar()->addPermanentWidget(m_cloudStatusLabel);
}

void MainWindow::setupTrayMenu()
{
    // System tray icon will be handled by Application class
}

void MainWindow::applyTheme()
{
    m_themeManager->applyTheme(m_currentTheme);
}

void MainWindow::applyLanguage(const QString &language)
{
    m_translationManager->switchLanguage(language);
    m_currentLanguage = language;
    
    // Update UI translations will be handled by translation manager
    onLanguageChanged(language);
}

void MainWindow::loadSettings()
{
    QSettings settings;
    m_currentTheme = settings.value("ui/theme", "light").toString();
    m_currentLanguage = settings.value("ui/language", "en").toString();
    m_agentModeEnabled = settings.value("agent/enabled", false).toBool();
    
    // Apply loaded settings
    applyTheme();
    applyLanguage(m_currentLanguage);
    
    // Update UI controls
    int themeIndex = m_themeCombo->findData(m_currentTheme);
    if (themeIndex >= 0) m_themeCombo->setCurrentIndex(themeIndex);
    
    int langIndex = m_languageCombo->findData(m_currentLanguage);
    if (langIndex >= 0) m_languageCombo->setCurrentIndex(langIndex);
}

void MainWindow::onLanguageChanged(const QString &language)
{
    // Update window title and other static text
    setWindowTitle(tr("Minecraft Backup Tool"));
    m_statusLabel->setText(tr("Ready"));
    m_cloudStatusLabel->setText(tr("Cloud: Disconnected"));
}

void MainWindow::onThemeChanged(const QString &theme)
{
    m_currentTheme = theme;
    applyTheme();
    
    QSettings settings;
    settings.setValue("ui/theme", theme);
}

void MainWindow::onScanLaunchers()
{
    m_statusLabel->setText(tr("Scanning for Minecraft launchers..."));
    m_launcherDetector->startDetection();
}

void MainWindow::onLauncherDetected(const QString &launcherName, const QString &path)
{
    m_launcherPaths[launcherName] = path;
    refreshInstanceList();
    
    m_statusLabel->setText(tr("Found launcher: %1").arg(launcherName));
}

void MainWindow::onRefreshInstances()
{
    refreshInstanceList();
}

void MainWindow::onSelectInstance(int index)
{
    // Implementation for instance selection
}

void MainWindow::onCreateBackup()
{
    if (m_selectedInstance.isEmpty()) {
        QMessageBox::warning(this, tr("No Selection"), 
                            tr("Please select a Minecraft instance to backup."));
        return;
    }
    
    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_statusLabel->setText(tr("Creating backup..."));
    
    m_backupEngine->createBackup(m_selectedInstance);
}

void MainWindow::onRestoreBackup()
{
    // Implementation for restore
}

void MainWindow::onScheduleBackup()
{
    // Implementation for scheduling
}

void MainWindow::onBackupProgress(int value)
{
    m_progressBar->setValue(value);
}

void MainWindow::onBackupCompleted()
{
    m_progressBar->setVisible(false);
    m_statusLabel->setText(tr("Backup completed successfully"));
    updateBackupList();
    
    showNotification(tr("Backup Complete"), 
                    tr("Backup created successfully for instance: %1").arg(m_selectedInstance));
}

void MainWindow::onBackupError(const QString &error)
{
    m_progressBar->setVisible(false);
    m_statusLabel->setText(tr("Backup failed"));
    
    QMessageBox::critical(this, tr("Backup Error"), error);
}

void MainWindow::onDeleteBackup()
{
    // Implementation for deleting backup
}

void MainWindow::onConfigureCloud()
{
    // Implementation for cloud configuration
}

void MainWindow::onSyncToCloud()
{
    // Implementation for sync to cloud
}

void MainWindow::onSyncFromCloud()
{
    // Implementation for sync from cloud
}

void MainWindow::onCloudStatusChanged(const QString &service, bool connected)
{
    QString status = connected ? tr("Connected") : tr("Disconnected");
    m_cloudStatusLabel->setText(tr("Cloud (%1): %2").arg(service, status));
}

void MainWindow::onEnableAgentMode(bool enabled)
{
    m_agentModeEnabled = enabled;
    QSettings settings;
    settings.setValue("agent/enabled", enabled);
    
    m_statusLabel->setText(enabled ? tr("Agent mode enabled") : tr("Agent mode disabled"));
}

void MainWindow::onAgentBackupScheduled()
{
    // Implementation for scheduled backup
}

void MainWindow::onAgentRestoreScheduled()
{
    // Implementation for scheduled restore
}

void MainWindow::onOpenSettings()
{
    // Implementation for settings dialog
}

void MainWindow::onExportSettings()
{
    // Implementation for export settings
}

void MainWindow::onImportSettings()
{
    // Implementation for import settings
}

void MainWindow::refreshInstanceList()
{
    m_instancesTree->clear();
    
    for (auto it = m_launcherPaths.begin(); it != m_launcherPaths.end(); ++it) {
        QTreeWidgetItem *launcherItem = new QTreeWidgetItem(m_instancesTree);
        launcherItem->setText(0, it.key());
        launcherItem->setData(0, Qt::UserRole, it.value());
        
        // TODO: Add instance detection logic here
        // This would scan the launcher directory for instances/modpacks
    }
}

void MainWindow::updateBackupList()
{
    // Implementation for updating backup list
}

void MainWindow::showNotification(const QString &title, const QString &message)
{
    // Show system notification if available
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        QSystemTrayIcon::showMessage(nullptr, title, message, 
                                   QSystemTrayIcon::Information, 5000);
    }
}

} // namespace MinecraftBackupTool