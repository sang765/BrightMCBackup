#include "thememanager.h"

#include <QApplication>
#include <QPalette>
#include <QStyleFactory>
#include <QSettings>
#include <QFile>
#include <QTextStream>

namespace MinecraftBackupTool {

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
    , m_currentTheme("light")
{
    setupDarkPalette();
    setupLightPalette();
    loadColorSchemes();
}

void ThemeManager::applyTheme(const QString &theme)
{
    m_currentTheme = theme;
    
    if (theme == "dark") {
        QApplication::instance()->setPalette(m_darkPalette);
        QApplication::instance()->setStyle(QStyleFactory::create("Fusion"));
    } else {
        QApplication::instance()->setPalette(m_lightPalette);
        QApplication::instance()->setStyle(QStyleFactory::create("Fusion"));
    }
    
    applyQtStyle();
    saveTheme(theme);
    
    emit themeChanged(theme);
}

void ThemeManager::setDarkMode(bool dark)
{
    applyTheme(dark ? "dark" : "light");
}

void ThemeManager::loadColorSchemes()
{
    // Load additional color schemes from resources if needed
    // This can be extended for more theme customization
}

void ThemeManager::applyColorScheme(const QString &scheme)
{
    // Implementation for custom color schemes
    // This would allow users to create their own themes
}

void ThemeManager::saveTheme(const QString &theme)
{
    QSettings settings;
    settings.setValue("ui/theme", theme);
}

QString ThemeManager::loadSavedTheme() const
{
    QSettings settings;
    return settings.value("ui/theme", "light").toString();
}

void ThemeManager::setupDarkPalette()
{
    m_darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    m_darkPalette.setColor(QPalette::WindowText, Qt::white);
    m_darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
    m_darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    m_darkPalette.setColor(QPalette::ToolTipBase, QColor(0, 0, 0));
    m_darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    m_darkPalette.setColor(QPalette::Text, Qt::white);
    m_darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    m_darkPalette.setColor(QPalette::ButtonText, Qt::white);
    m_darkPalette.setColor(QPalette::BrightText, Qt::red);
    m_darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    m_darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    m_darkPalette.setColor(QPalette::HighlightedText, Qt::black);
}

void ThemeManager::setupLightPalette()
{
    m_lightPalette.setColor(QPalette::Window, Qt::white);
    m_lightPalette.setColor(QPalette::WindowText, Qt::black);
    m_lightPalette.setColor(QPalette::Base, Qt::white);
    m_lightPalette.setColor(QPalette::AlternateBase, QColor(242, 242, 242));
    m_lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
    m_lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    m_lightPalette.setColor(QPalette::Text, Qt::black);
    m_lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
    m_lightPalette.setColor(QPalette::ButtonText, Qt::black);
    m_lightPalette.setColor(QPalette::BrightText, Qt::red);
    m_lightPalette.setColor(QPalette::Link, QColor(0, 0, 255));
    m_lightPalette.setColor(QPalette::Highlight, QColor(0, 120, 215));
    m_lightPalette.setColor(QPalette::HighlightedText, Qt::white);
}

void ThemeManager::applyQtStyle()
{
    // Apply additional styling
    QApplication::instance()->setStyleSheet(R"(
        QToolTip { 
            color: black; 
            background-color: white; 
            border: 1px solid black; 
            padding: 2px; 
        }
        QMenuBar::item:selected { 
            background: rgba(0, 120, 215, 0.3); 
        }
    )");
}

} // namespace MinecraftBackupTool