// FileManager implementation
#include "filemanager.h"
#include <QFile>
#include <QDir>
#include <QDebug>

namespace MinecraftBackupTool {

FileManager::FileManager(QObject *parent)
    : QObject(parent)
{
    // Initialize file manager
}

FileManager::~FileManager()
{
    // Cleanup resources
}

bool FileManager::copyFile(const QString &source, const QString &destination)
{
    bool result = QFile::copy(source, destination);
    if (result) {
        emit operationCompleted("File copied successfully");
    } else {
        emit error("Failed to copy file");
    }
    return result;
}

bool FileManager::moveFile(const QString &source, const QString &destination)
{
    bool result = QFile::rename(source, destination);
    if (result) {
        emit operationCompleted("File moved successfully");
    } else {
        emit error("Failed to move file");
    }
    return result;
}

bool FileManager::deleteFile(const QString &filePath)
{
    bool result = QFile::remove(filePath);
    if (result) {
        emit operationCompleted("File deleted successfully");
    } else {
        emit error("Failed to delete file");
    }
    return result;
}

bool FileManager::createDirectory(const QString &path)
{
    bool result = QDir().mkpath(path);
    if (result) {
        emit operationCompleted("Directory created successfully");
    } else {
        emit error("Failed to create directory");
    }
    return result;
}

QStringList FileManager::listFiles(const QString &directory, const QString &filter)
{
    QDir dir(directory);
    QStringList files = dir.entryList(QStringList() << filter, QDir::Files);
    return files;
}

bool FileManager::copyDirectory(const QString &source, const QString &destination)
{
    QDir sourceDir(source);
    if (!sourceDir.exists()) {
        emit error("Source directory does not exist");
        return false;
    }
    
    bool result = true;
    foreach (QString file, sourceDir.entryList(QDir::Files)) {
        emitProgress(0);
        result &= QFile::copy(source + "/" + file, destination + "/" + file);
        emitProgress(25);
    }
    
    foreach (QString dirName, sourceDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir().mkpath(destination + "/" + dirName);
        result &= copyDirectory(source + "/" + dirName, destination + "/" + dirName);
        emitProgress(50);
    }
    
    emitProgress(100);
    return result;
}

bool FileManager::removeDirectory(const QString &path)
{
    bool result = QDir().rmdir(path);
    if (result) {
        emit operationCompleted("Directory removed successfully");
    } else {
        emit error("Failed to remove directory");
    }
    return result;
}

bool FileManager::exists(const QString &path)
{
    return QFile::exists(path);
}

bool FileManager::isDirectory(const QString &path)
{
    return QDir(path).exists();
}

qint64 FileManager::fileSize(const QString &filePath)
{
    return QFileInfo(filePath).size();
}

void FileManager::emitProgress(int percent)
{
    emit progress(percent);
}

} // namespace MinecraftBackupTool