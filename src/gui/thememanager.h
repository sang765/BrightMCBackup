#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QApplication>
#include <QPalette>
#include <QStyleFactory>
#include <QSettings>

namespace MinecraftBackupTool {

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    explicit ThemeManager(QObject *parent = nullptr);
    ~ThemeManager() = default;

    // Theme Management
    void applyTheme(const QString &theme);
    void setDarkMode(bool dark);
    bool isDarkMode() const { return m_currentTheme == "dark"; }
    
    // Color Schemes
    void loadColorSchemes();
    void applyColorScheme(const QString &scheme);
    
    // Settings
    void saveTheme(const QString &theme);
    QString loadSavedTheme() const;

signals:
    void themeChanged(const QString &theme);

private:
    void setupDarkPalette();
    void setupLightPalette();
    void applyQtStyle();

private:
    QString m_currentTheme;
    QPalette m_darkPalette;
    QPalette m_lightPalette;
};

} // namespace MinecraftBackupTool

#endif // THEMEMANAGER_H