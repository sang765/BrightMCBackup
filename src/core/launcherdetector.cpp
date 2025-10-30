#include "launcherdetector.h"

#include <QObject>
#include <QThread>
#include <QMap>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include <QProcess>
#include <QCoreApplication>

namespace MinecraftBackupTool {

LauncherDetector::LauncherDetector(QObject *parent)
    : QObject(parent)
{
}

void LauncherDetector::startDetection()
{
    emit detectionProgress("Starting launcher detection...");
    detectWindowsLaunchers();
}

QStringList LauncherDetector::getDetectedLaunchers() const
{
    return m_detectedLaunchers.keys();
}

QString LauncherDetector::getLauncherPath(const QString &launcherName) const
{
    return m_detectedLaunchers.value(launcherName, QString());
}

void LauncherDetector::detectWindowsLaunchers()
{
    QStringList searchPaths = {
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),
        QDir::homePath() + "/.minecraft",
        "C:/Program Files/Minecraft",
        "C:/Program Files (x86)/Minecraft",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Minecraft"
    };
    
    // Check for major Minecraft launchers
    QList<QPair<QString, QString>> launchers = {
        {"Vanilla Launcher", "minecraft-launcher.exe"},
        {"Modrinth", "modrinth.exe"},
        {"CurseForge", "curseforge-launcher.exe"},
        {"ATLauncher", "ATLauncher.exe"},
        {"Prism Launcher", "PrismLauncher.exe"},
        {"MultiMC", "MultiMC.exe"},
        {"Legacy Launcher", "LegacyLauncher.exe"}
    };
    
    for (const QString &searchPath : searchPaths) {
        QDir dir(searchPath);
        if (!dir.exists()) continue;
        
        for (const auto &launcher : launchers) {
            QString launcherPath = dir.filePath(launcher.second);
            if (checkLauncher(launcherPath, launcher.first)) {
                m_detectedLaunchers[launcher.first] = launcherPath;
                emit launcherDetected(launcher.first, launcherPath);
                emit detectionProgress(QString("Found: %1").arg(launcher.first));
            }
        }
    }
    
    emit detectionProgress("Launcher detection completed");
}

bool LauncherDetector::checkLauncher(const QString &path, const QString &launcherName)
{
    QFileInfo info(path);
    return info.exists() && info.isFile();
}

} // namespace MinecraftBackupTool