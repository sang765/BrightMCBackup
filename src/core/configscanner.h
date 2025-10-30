#ifndef CONFIGSCANNER_H
#define CONFIGSCANNER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QDir>
#include <QFileInfo>

namespace MinecraftBackupTool {

class ConfigScanner : public QObject
{
    Q_OBJECT

public:
    explicit ConfigScanner(QObject *parent = nullptr);
    ~ConfigScanner() = default;

    // Scanning Operations
    QMap<QString, QStringList> scanInstanceConfig(const QString &instancePath);
    QStringList findModConfigs(const QString &instancePath);
    QStringList findGameSettings(const QString &instancePath);
    
    // Configuration Parsing
    QMap<QString, QString> parseConfigFile(const QString &filePath);
    void updateConfigFile(const QString &filePath, const QMap<QString, QString> &config);

signals:
    void scanCompleted(const QString &instancePath, int fileCount);
    void configFound(const QString &filePath, const QString &configType);
    void error(const QString &error);

private:
    void scanDirectory(const QString &path, QStringList &configFiles);
    bool isConfigFile(const QString &filePath);
    QString detectConfigType(const QString &filePath);

private:
    QStringList m_configExtensions;
    QMap<QString, QString> m_configTypeMap;
};

} // namespace MinecraftBackupTool

#endif // CONFIGSCANNER_H