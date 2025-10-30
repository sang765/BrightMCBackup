#ifndef AGENTMODE_H
#define AGENTMODE_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QMap>
#include <QString>

namespace MinecraftBackupTool {

class AgentMode : public QObject
{
    Q_OBJECT

public:
    explicit AgentMode(QObject *parent = nullptr);
    ~AgentMode() = default;

    // Agent Control
    void startAgent();
    void stopAgent();
    void pauseAgent();
    
    // Schedule Management
    void setBackupSchedule(const QString &scheduleType);
    void setAutoRestore(bool enabled);
    void setConflictResolution(const QString &resolution);
    
    // Status
    bool isAgentActive() const { return m_agentActive; }
    QString getLastBackupTime() const { return m_lastBackupTime; }

signals:
    void agentStarted();
    void agentStopped();
    void backupScheduled();
    void restoreScheduled();
    void conflictDetected(const QString &file, const QString &description);
    void error(const QString &error);

private slots:
    void onTimerTimeout();
    void checkForUpdates();
    void runScheduledBackup();
    void handleConflicts();

private:
    void setupMonitoring();
    void checkMinecraftUsage();
    void backupIfModified();
    void restoreIfNeeded();

private:
    bool m_agentActive;
    QTimer *m_checkTimer;
    QString m_lastBackupTime;
    QString m_backupSchedule;
    QString m_conflictResolution;
    QMap<QString, QDateTime> m_instanceModificationTimes;
};

} // namespace MinecraftBackupTool

#endif // AGENTMODE_H