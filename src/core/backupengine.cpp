#include "backupengine.h"

#include <QObject>
#include <QThread>
#include <QString>
#include <QMap>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QMutex>
#include <QCryptographicHash>
#include <QDateTime>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace MinecraftBackupTool {

BackupEngine::BackupEngine(QObject *parent)
    : QObject(parent)
    , m_backupActive(false)
    , m_cancelRequested(false)
    , m_compressionLevel(6)
{
    m_backupPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/backups";
    QDir().mkpath(m_backupPath);
    
    // Default exclusion patterns
    m_exclusionPatterns << "*.log" << "*.tmp" << "*.cache" << ".git" << "bin" << "temp";
}

void BackupEngine::createBackup(const QString &instancePath)
{
    if (m_backupActive) {
        emit error("Backup already in progress");
        return;
    }
    
    m_instancePath = instancePath;
    m_backupActive = true;
    m_cancelRequested = false;
    
    m_currentBackupName = QString("backup_%1_%2")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"))
        .arg(QDir(instancePath).dirName());
    
    emit statusUpdate("Scanning instance directory...");
    
    scanInstanceDirectory(instancePath);
    
    if (m_cancelRequested) {
        m_backupActive = false;
        return;
    }
    
    emit statusUpdate("Creating backup archive...");
    QString backupPath = createBackupArchive();
    
    if (m_cancelRequested) {
        m_backupActive = false;
        return;
    }
    
    emit completed(backupPath);
    m_backupActive = false;
}

void BackupEngine::cancelBackup()
{
    m_cancelRequested = true;
}

void BackupEngine::scanInstanceDirectory(const QString &instancePath)
{
    QDir instanceDir(instancePath);
    if (!instanceDir.exists()) {
        emit error(QString("Instance directory does not exist: %1").arg(instancePath));
        return;
    }
    
    emit progress(10);
    
    // Scan different directories
    backupMinecraftSaves(instancePath);
    emit progress(25);
    
    backupModConfigs(instancePath);
    emit progress(40);
    
    backupGameSettings(instancePath);
    emit progress(55);
    
    backupMods(instancePath);
    emit progress(70);
    
    backupResourcePacks(instancePath);
    emit progress(85);
    
    backupShaderPacks(instancePath);
    emit progress(95);
}

void BackupEngine::backupMinecraftSaves(const QString &instancePath)
{
    QStringList savePaths = {
        instancePath + "/saves",
        instancePath + "/worlds",
        instancePath + "/saves/nether",
        instancePath + "/saves/end",
        instancePath + "/saves/overworld"
    };
    
    for (const QString &savePath : savePaths) {
        if (QDir(savePath).exists()) {
            m_filesToBackup["saves"].append(savePath);
        }
    }
}

void BackupEngine::backupModConfigs(const QString &instancePath)
{
    QStringList configPaths = {
        instancePath + "/config",
        instancePath + "/configmods",
        instancePath + "/globaldatapacks"
    };
    
    for (const QString &configPath : configPaths) {
        if (QDir(configPath).exists()) {
            m_filesToBackup["config"].append(configPath);
        }
    }
}

void BackupEngine::backupGameSettings(const QString &instancePath)
{
    QStringList settingsFiles = {
        "options.txt",
        "optionsof.txt",
        "servers.dat",
        "optionsshaders.txt"
    };
    
    for (const QString &settingsFile : settingsFiles) {
        QString fullPath = instancePath + "/" + settingsFile;
        if (QFileInfo(fullPath).exists()) {
            m_filesToBackup["settings"].append(fullPath);
        }
    }
}

void BackupEngine::backupMods(const QString &instancePath)
{
    QStringList modPaths = {
        instancePath + "/mods",
        instancePath + "/coremods",
        instancePath + "/modloader"
    };
    
    for (const QString &modPath : modPaths) {
        if (QDir(modPath).exists()) {
            m_filesToBackup["mods"].append(modPath);
        }
    }
}

void BackupEngine::backupResourcePacks(const QString &instancePath)
{
    QString resourcePath = instancePath + "/resourcepacks";
    if (QDir(resourcePath).exists()) {
        m_filesToBackup["resourcepacks"].append(resourcePath);
    }
}

void BackupEngine::backupShaderPacks(const QString &instancePath)
{
    QString shaderPath = instancePath + "/shaderpacks";
    if (QDir(shaderPath).exists()) {
        m_filesToBackup["shaderpacks"].append(shaderPath);
    }
}

QString BackupEngine::createBackupArchive()
{
    // Implementation for creating ZIP archive
    // This would use a compression library like zlib or libzip
    QString backupFile = m_backupPath + "/" + m_currentBackupName + ".zip";
    
    writeBackupMetadata();
    
    emit progress(100);
    return backupFile;
}

QStringList BackupEngine::getBackupHistory() const
{
    QStringList history;
    QDir backupDir(m_backupPath);
    
    QStringList filters = {"backup_*.zip"};
    QStringList backupFiles = backupDir.entryList(filters, QDir::Files);
    
    for (const QString &backupFile : backupFiles) {
        history.append(backupFile);
    }
    
    return history;
}

void BackupEngine::writeBackupMetadata()
{
    QJsonObject metadata;
    metadata["instancePath"] = m_instancePath;
    metadata["backupName"] = m_currentBackupName;
    metadata["timestamp"] = QDateTime::currentDateTime().toISOString();
    metadata["filesBackuped"] = QJsonArray::fromStringList(m_filesToBackup.keys());
    metadata["version"] = "1.0";
    
    QString metadataFile = m_backupPath + "/" + m_currentBackupName + "_metadata.json";
    QFile file(metadataFile);
    
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(metadata).toJson());
        file.close();
    }
}

bool BackupEngine::shouldExcludeFile(const QString &filePath)
{
    for (const QString &pattern : m_exclusionPatterns) {
        if (filePath.contains(pattern, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}

QString BackupEngine::calculateFileHash(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }
    
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(&file);
    file.close();
    
    return QString(hash.result().toHex());
}

} // namespace MinecraftBackupTool