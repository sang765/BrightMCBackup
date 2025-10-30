#ifndef BACKUPENGINE_H
#define BACKUPENGINE_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QMap>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QMutex>

namespace MinecraftBackupTool {

class BackupEngine : public QObject
{
    Q_OBJECT

public:
    explicit BackupEngine(QObject *parent = nullptr);
    ~BackupEngine() = default;

    // Backup Operations
    void createBackup(const QString &instancePath);
    void cancelBackup();
    bool isBackupInProgress() const { return m_backupActive; }
    
    // Configuration
    void setBackupPath(const QString &path);
    void setCompressionLevel(int level);
    void addExclusionPattern(const QString &pattern);
    
    // Backup Info
    QStringList getBackupHistory() const;
    QMap<QString, QVariant> getBackupMetadata(const QString &backupName) const;

signals:
    void progress(int percent);
    void fileProgress(const QString &currentFile, int percent);
    void completed(const QString &backupPath);
    void error(const QString &error);
    void statusUpdate(const QString &status);

private:
    void scanInstanceDirectory(const QString &instancePath);
    void backupMinecraftSaves(const QString &instancePath);
    void backupModConfigs(const QString &instancePath);
    void backupGameSettings(const QString &instancePath);
    void backupMods(const QString &instancePath);
    void backupResourcePacks(const QString &instancePath);
    void backupShaderPacks(const QString &instancePath);
    
    QString createBackupArchive();
    void writeBackupMetadata();
    bool shouldExcludeFile(const QString &filePath);
    QString calculateFileHash(const QString &filePath);
    
    QString m_instancePath;
    QString m_backupPath;
    QString m_currentBackupName;
    QMap<QString, QStringList> m_filesToBackup;
    QMap<QString, QString> m_fileHashes;
    QStringList m_exclusionPatterns;
    bool m_backupActive;
    bool m_cancelRequested;
    QMutex m_mutex;
    int m_compressionLevel;
};

} // namespace MinecraftBackupTool

#endif // BACKUPENGINE_H