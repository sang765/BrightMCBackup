#include "configscanner.h"

#include <QObject>
#include <QMap>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

namespace MinecraftBackupTool {

ConfigScanner::ConfigScanner(QObject *parent)
    : QObject(parent)
{
    // Initialize supported config file extensions
    m_configExtensions = {
        ".cfg", ".conf", ".properties", ".txt", ".json", 
        ".xml", ".ini", ".toml", ".yaml", ".yml"
    };
    
    // Map file patterns to config types
    m_configTypeMap["options"] = "game_settings";
    m_configTypeMap["servers"] = "servers";
    m_configTypeMap["servers.dat"] = "servers";
    m_configTypeMap["optionsshaders"] = "shader_settings";
}

QMap<QString, QStringList> ConfigScanner::scanInstanceConfig(const QString &instancePath)
{
    QMap<QString, QStringList> configMap;
    QStringList allConfigs;
    
    scanDirectory(instancePath, allConfigs);
    
    // Categorize configs
    for (const QString &configFile : allConfigs) {
        QString configType = detectConfigType(configFile);
        configMap[configType].append(configFile);
        emit configFound(configFile, configType);
    }
    
    emit scanCompleted(instancePath, allConfigs.size());
    return configMap;
}

QStringList ConfigScanner::findModConfigs(const QString &instancePath)
{
    QStringList modConfigs;
    QStringList dirs = {
        instancePath + "/config",
        instancePath + "/configmods",
        instancePath + "/defaultconfigs"
    };
    
    for (const QString &dir : dirs) {
        QDir configDir(dir);
        if (configDir.exists()) {
            QStringList files = configDir.entryList(m_configExtensions, QDir::Files);
            for (const QString &file : files) {
                modConfigs.append(configDir.absoluteFilePath(file));
            }
        }
    }
    
    return modConfigs;
}

QStringList ConfigScanner::findGameSettings(const QString &instancePath)
{
    QStringList gameSettings;
    QStringList settingsFiles = {
        "options.txt",
        "optionsof.txt",
        "servers.dat",
        "optionsshaders.txt"
    };
    
    for (const QString &settingsFile : settingsFiles) {
        QString fullPath = instancePath + "/" + settingsFile;
        if (QFileInfo(fullPath).exists()) {
            gameSettings.append(fullPath);
        }
    }
    
    return gameSettings;
}

QMap<QString, QString> ConfigScanner::parseConfigFile(const QString &filePath)
{
    QMap<QString, QString> config;
    QFile file(filePath);
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit error(QString("Cannot open config file: %1").arg(filePath));
        return config;
    }
    
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        
        // Skip empty lines and comments
        if (line.isEmpty() || line.startsWith('#') || line.startsWith('//')) {
            continue;
        }
        
        // Parse key=value pairs
        int equalsPos = line.indexOf('=');
        if (equalsPos > 0) {
            QString key = line.left(equalsPos).trimmed();
            QString value = line.mid(equalsPos + 1).trimmed();
            config[key] = value;
        }
    }
    
    file.close();
    return config;
}

void ConfigScanner::updateConfigFile(const QString &filePath, const QMap<QString, QString> &config)
{
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit error(QString("Cannot write to config file: %1").arg(filePath));
        return;
    }
    
    QTextStream out(&file);
    for (auto it = config.begin(); it != config.end(); ++it) {
        out << it.key() << "=" << it.value() << "\n";
    }
    
    file.close();
}

void ConfigScanner::scanDirectory(const QString &path, QStringList &configFiles)
{
    QDir dir(path);
    if (!dir.exists()) return;
    
    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    
    for (const QFileInfo &entry : entries) {
        if (entry.isDir()) {
            // Recursively scan subdirectories
            scanDirectory(entry.absoluteFilePath(), configFiles);
        } else if (isConfigFile(entry.absoluteFilePath())) {
            configFiles.append(entry.absoluteFilePath());
        }
    }
}

bool ConfigScanner::isConfigFile(const QString &filePath)
{
    QFileInfo info(filePath);
    QString extension = info.suffix().toLower();
    
    return m_configExtensions.contains("." + extension);
}

QString ConfigScanner::detectConfigType(const QString &filePath)
{
    QFileInfo info(filePath);
    QString fileName = info.fileName().toLower();
    
    // Check exact matches first
    if (m_configTypeMap.contains(fileName)) {
        return m_configTypeMap[fileName];
    }
    
    // Check pattern matches
    if (fileName.contains("options")) return "game_settings";
    if (fileName.contains("server")) return "server_settings";
    if (fileName.contains("mod")) return "mod_config";
    if (fileName.contains("shader")) return "shader_settings";
    
    return "other_config";
}

} // namespace MinecraftBackupTool