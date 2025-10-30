#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>

namespace MinecraftBackupTool {

class FileManager : public QObject
{
    Q_OBJECT

public:
    explicit FileManager(QObject *parent = nullptr);
    ~FileManager() = default;

    // File Operations
    bool copyFile(const QString &source, const QString &destination);
    bool moveFile(const QString &source, const QString &destination);
    bool deleteFile(const QString &filePath);
    bool createDirectory(const QString &path);
    
    // Directory Operations
    QStringList listFiles(const QString &directory, const QString &filter = "*");
    bool copyDirectory(const QString &source, const QString &destination);
    bool removeDirectory(const QString &path);
    
    // Validation
    bool exists(const QString &path);
    bool isDirectory(const QString &path);
    qint64 fileSize(const QString &filePath);
    
    signals:
    void progress(int percent);
    void operationCompleted(const QString &message);
    void error(const QString &error);

private:
    void emitProgress(int percent);
};

} // namespace MinecraftBackupTool

#endif // FILEMANAGER_H