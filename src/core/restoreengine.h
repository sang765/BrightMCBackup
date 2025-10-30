#ifndef RESTOREENGINE_H
#define RESTOREENGINE_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>

namespace MinecraftBackupTool {

class RestoreEngine : public QObject
{
    Q_OBJECT

public:
    explicit RestoreEngine(QObject *parent = nullptr);
    ~RestoreEngine() = default;

    // Restore Operations
    void restoreFromBackup(const QString &backupPath, const QString &instancePath);
    void previewRestore(const QString &backupPath);
    
    // Conflict Resolution
    void setConflictResolution(const QString &resolution);
    bool handleConflicts(const QString &file, const QString &existingPath, const QString &backupPath);
    
    // Backup Validation
    bool validateBackup(const QString &backupPath);
    QStringList getBackupContents(const QString &backupPath);

signals:
    void restoreProgress(int percent);
    void fileRestored(const QString &file);
    void conflictDetected(const QString &file);
    void completed(const QString &message);
    void error(const QString &error);

private:
    void extractBackup(const QString &backupPath);
    void restoreFiles(const QString &sourcePath, const QString &targetPath);
    QString m_conflictResolution;
};

} // namespace MinecraftBackupTool

#endif // RESTOREENGINE_H