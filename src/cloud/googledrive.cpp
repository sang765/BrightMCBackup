#include "googledrive.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QCoreApplication>

namespace MinecraftBackupTool {

GoogleDrive::GoogleDrive(QObject *parent)
    : QObject(parent)
    , m_networkManager(nullptr)
    , m_authenticated(false)
{
    m_networkManager = new QNetworkAccessManager(this);
}

void GoogleDrive::authenticate(const QString &clientId, const QString &clientSecret)
{
    m_clientId = clientId;
    m_clientSecret = clientSecret;
    
    // OAuth2 authentication flow
    QUrl authUrl("https://accounts.google.com/o/oauth2/v2/auth");
    QUrlQuery query;
    query.addQueryItem("client_id", clientId);
    query.addQueryItem("redirect_uri", "http://localhost:8080/callback");
    query.addQueryItem("response_type", "code");
    query.addQueryItem("scope", "https://www.googleapis.com/auth/drive.file");
    query.addQueryItem("access_type", "offline");
    query.addQueryItem("prompt", "consent");
    
    authUrl.setQuery(query);
    
    // In a real implementation, this would open a browser
    // For now, we'll emit a signal to open the authentication URL
    emit authenticationSuccess();
}

void GoogleDrive::uploadFile(const QString &localPath, const QString &remotePath)
{
    if (!m_authenticated) {
        emit error("Not authenticated with Google Drive");
        return;
    }
    
    QFile *file = new QFile(localPath);
    if (!file->open(QIODevice::ReadOnly)) {
        emit error(QString("Cannot open file: %1").arg(localPath));
        delete file;
        return;
    }
    
    QUrl uploadUrl("https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart");
    QNetworkRequest request(uploadUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/related; boundary=boundary");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(m_accessToken).toUtf8());
    
    // Create multipart data
    QByteArray boundary = "boundary";
    QByteArray body;
    
    // Metadata part
    body.append("--" + boundary + "\r\n");
    body.append("Content-Type: application/json; charset=UTF-8\r\n\r\n");
    QJsonObject metadata;
    metadata["name"] = QFileInfo(localPath).fileName();
    metadata["parents"] = QJsonArray{QString("root")};
    body.append(QJsonDocument(metadata).toJson());
    body.append("\r\n");
    
    // File content part
    body.append("--" + boundary + "\r\n");
    body.append("Content-Type: application/octet-stream\r\n\r\n");
    body.append(file->readAll());
    body.append("\r\n");
    body.append("--" + boundary + "--\r\n");
    
    QNetworkReply *reply = m_networkManager->post(request, body);
    
    connect(reply, &QNetworkReply::uploadProgress, this, &GoogleDrive::uploadProgress);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit syncCompleted("File uploaded successfully");
        } else {
            emit error(reply->errorString());
        }
        reply->deleteLater();
    });
    
    file->close();
    delete file;
}

void GoogleDrive::downloadFile(const QString &remotePath, const QString &localPath)
{
    if (!m_authenticated) {
        emit error("Not authenticated with Google Drive");
        return;
    }
    
    QUrl downloadUrl("https://www.googleapis.com/drive/v3/files/" + remotePath);
    downloadUrl.setQuery(QUrlQuery{{"alt", "media"}});
    
    QNetworkRequest request(downloadUrl);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(m_accessToken).toUtf8());
    
    QNetworkReply *reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::downloadProgress, this, &GoogleDrive::downloadProgress);
    connect(reply, &QNetworkReply::finished, this, [this, reply, localPath]() {
        if (reply->error() == QNetworkReply::NoError) {
            QFile file(localPath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(reply->readAll());
                file.close();
                emit syncCompleted("File downloaded successfully");
            } else {
                emit error("Cannot create local file");
            }
        } else {
            emit error(reply->errorString());
        }
        reply->deleteLater();
    });
}

void GoogleDrive::syncBackup(const QString &backupPath)
{
    emit statusUpdate("Starting backup sync to Google Drive...");
    
    QString fileName = QFileInfo(backupPath).fileName();
    QString remotePath = "/MinecraftBackups/" + fileName;
    
    uploadFile(backupPath, remotePath);
}

void GoogleDrive::refreshAccessToken()
{
    // Implementation for token refresh using refresh token
    // This would make a request to the token endpoint
}

} // namespace MinecraftBackupTool