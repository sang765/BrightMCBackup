#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QLoggingCategory>
#include "application.h"
#include "i18n/translationmanager.h"

int main(int argc, char *argv[])
{
    // Set application information
    QApplication::setApplicationName("MinecraftBackupTool");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("MinecraftBackupTool");
    QApplication::setOrganizationDomain("minecraftbackuptool.com");
    
    // Enable high DPI support
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    
    // Configure logging
    QLoggingCategory::setFilterRules("*.debug=false\nqt.*.debug=false");
    
    // Create application instance
    MinecraftBackupTool::Application app(argc, argv);
    
    // Initialize translation system
    MinecraftBackupTool::TranslationManager translationManager;
    translationManager.loadTranslations();
    
    // Show main window
    MainWindow mainWindow;
    mainWindow.show();
    
    // Connect translation changes to main window
    QObject::connect(&translationManager, &MinecraftBackupTool::TranslationManager::languageChanged,
                     &mainWindow, &MainWindow::onLanguageChanged);
    
    return app.exec();
}