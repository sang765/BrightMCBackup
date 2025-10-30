#include <QObject>
#include <QThread>
#include <QMap>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include <QProcess>

namespace MinecraftBackupTool {

class LauncherDetector : public QObject
{
    Q_OBJECT

public:
    explicit LauncherDetector(QObject *parent = nullptr);
    ~LauncherDetector() = default;

    void startDetection();
    QStringList getDetectedLaunchers() const;
    QString getLauncherPath(const QString &launcherName) const;

signals:
    void launcherDetected(const QString &launcherName, const QString &path);
    void detectionProgress(const QString &message);

private:
    void detectWindowsLaunchers();
    bool checkLauncher(const QString &path, const QString &launcherName);

private:
    QMap<QString, QString> m_detectedLaunchers;
};

} // namespace MinecraftBackupTool