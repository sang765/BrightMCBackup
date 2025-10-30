#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include <QObject>
#include <QTranslator>
#include <QLocale>
#include <QString>
#include <QCoreApplication>

namespace MinecraftBackupTool {

class TranslationManager : public QObject
{
    Q_OBJECT

public:
    explicit TranslationManager(QObject *parent = nullptr);
    ~TranslationManager() = default;

    // Translation Management
    void loadTranslations();
    bool switchLanguage(const QString &languageCode);
    QStringList getAvailableLanguages() const;
    QString getCurrentLanguage() const { return m_currentLanguage; }

signals:
    void languageChanged(const QString &language);
    void translationLoaded(const QString &language);
    void translationError(const QString &error);

private:
    void setupDefaultTranslations();

private:
    std::unique_ptr<QTranslator> m_translator;
    QString m_currentLanguage;
    QMap<QString, QString> m_languageNames;
};

} // namespace MinecraftBackupTool

#endif // TRANSLATIONMANAGER_H