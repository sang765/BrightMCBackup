#ifndef GOOGLEDRIVE_H
#define GOOGLEDRIVE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QUrl>

namespace MinecraftBackupTool {

class GoogleDrive : public QObject
{
    Q_OBJECT

public:
    explicit GoogleDrive(QObject *parent = nullptr);
    ~GoogleDrive() = default;

    // Authentication
    void authenticate(const QString &clientId, const QString &clientSecret);
    bool isAuthenticated() const { return m_authenticated; }
    
    // File Operations
    void uploadFile(const QString &localPath, const QString &remotePath);
    void downloadFile(const QString &remotePath, const QString &localPath);
    void deleteFile(const QString &remotePath);
    QStringList listFiles(const QString &folderId);
    
    // Backup Operations
    void syncBackup(const QString &backupPath);
    void restoreBackup(const QString &remoteBackupPath);

signals:
    void authenticationSuccess();
    void authenticationError(const QString &error);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void syncCompleted(const QString &message);
    void error(const QString &error);

private:
    QNetworkAccessManager *m_networkManager;
    QString m_accessToken;
    QString m_clientId;
    QString m_clientSecret;
    QString m_refreshToken;
    bool m_authenticated;
    
    void refreshAccessToken();
    void makeAuthenticatedRequest(const QUrl &url, const QString &method, const QByteArray &data);
    
private slots:
    void onAuthFinished(QNetworkReply *reply);
    void onUploadFinished(QNetworkReply *reply);
    void onDownloadFinished(QNetworkReply *reply);
};

} // namespace MinecraftBackupTool

#endif // GOOGLEDRIVE_H