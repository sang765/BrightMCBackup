#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <QObject>
#include <QString>
#include <QByteArray>

namespace MinecraftBackupTool {

class Compression : public QObject
{
    Q_OBJECT

public:
    explicit Compression(QObject *parent = nullptr);
    ~Compression() = default;

    // Compression Operations
    QByteArray compressData(const QByteArray &data, const QString &format = "zip");
    QByteArray decompressData(const QByteArray &data, const QString &format = "zip");
    
    // File Operations
    bool compressFile(const QString &inputFile, const QString &outputFile);
    bool decompressFile(const QString &inputFile, const QString &outputFile);
    
    // Archive Operations
    bool createArchive(const QString &archivePath, const QStringList &files);
    bool extractArchive(const QString &archivePath, const QString &destination);
    
    // Utilities
    QStringList getSupportedFormats() const;
    bool isCompressionSupported(const QString &format) const;
    
    signals:
    void progress(int percent);
    void operationCompleted(const QString &message);
    void error(const QString &error);

private:
    void emitProgress(int percent);
};

} // namespace MinecraftBackupTool

#endif // COMPRESSION_H