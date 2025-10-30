#ifndef ONEDRIVE_H
#define ONEDRIVE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

namespace MinecraftBackupTool {

class OneDrive : public QObject
{
    Q_OBJECT

public:
    explicit OneDrive(QObject *parent = nullptr);
    ~OneDrive() = default;

    // Authentication
    void authenticate(const QString &clientId);
    bool isAuthenticated() const { return m_authenticated; }
    
    // File Operations
    void uploadFile(const QString &localPath, const QString &remotePath);
    void downloadFile(const QString &remotePath, const QString &localPath);
    void syncBackup(const QString &backupPath);

signals:
    void authenticationSuccess();
    void authenticationError(const QString &error);
    void syncCompleted(const QString &message);
    void error(const QString &error);

private:
    QNetworkAccessManager *m_networkManager;
    QString m_accessToken;
    QString m_clientId;
    bool m_authenticated;
};

} // namespace OneDrive

#endif // ONEDRIVE_H