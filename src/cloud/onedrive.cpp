#include "onedrive.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QCoreApplication>

namespace MinecraftBackupTool {

OneDrive::OneDrive(QObject *parent)
    : QObject(parent)
    , m_networkManager(nullptr)
    , m_authenticated(false)
{
    m_networkManager = new QNetworkAccessManager(this);
}

void OneDrive::authenticate(const QString &clientId)
{
    m_clientId = clientId;
    
    // Microsoft Graph API authentication
    QUrl authUrl("https://login.microsoftonline.com/common/oauth2/v2.0/authorize");
    
    // In a real implementation, this would handle OAuth2 flow
    // For now, we'll emit success
    emit authenticationSuccess();
}

void OneDrive::uploadFile(const QString &localPath, const QString &remotePath)
{
    if (!m_authenticated) {
        emit error("Not authenticated with OneDrive");
        return;
    }
    
    // Implementation for OneDrive upload
    QFile *file = new QFile(localPath);
    if (!file->open(QIODevice::ReadOnly)) {
        emit error(QString("Cannot open file: %1").arg(localPath));
        delete file;
        return;
    }
    
    QUrl uploadUrl("https://graph.microsoft.com/v1.0/me/drive/root:/" + remotePath + ":/content");
    QNetworkRequest request(uploadUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(m_accessToken).toUtf8());
    
    QNetworkReply *reply = m_networkManager->put(request, file->readAll());
    
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit syncCompleted("File uploaded to OneDrive successfully");
        } else {
            emit error(reply->errorString());
        }
        reply->deleteLater();
    });
    
    file->close();
    delete file;
}

void OneDrive::downloadFile(const QString &remotePath, const QString &localPath)
{
    if (!m_authenticated) {
        emit error("Not authenticated with OneDrive");
        return;
    }
    
    QUrl downloadUrl("https://graph.microsoft.com/v1.0/me/drive/root:/" + remotePath + ":/content");
    QNetworkRequest request(downloadUrl);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(m_accessToken).toUtf8());
    
    QNetworkReply *reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, this, [this, reply, localPath]() {
        if (reply->error() == QNetworkReply::NoError) {
            QFile file(localPath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(reply->readAll());
                file.close();
                emit syncCompleted("File downloaded from OneDrive successfully");
            } else {
                emit error("Cannot create local file");
            }
        } else {
            emit error(reply->errorString());
        }
        reply->deleteLater();
    });
}

void OneDrive::syncBackup(const QString &backupPath)
{
    emit authenticationSuccess(); // Simulate authentication
    
    QString fileName = QFileInfo(backupPath).fileName();
    QString remotePath = "/MinecraftBackups/" + fileName;
    
    uploadFile(backupPath, remotePath);
}

} // namespace MinecraftBackupTool