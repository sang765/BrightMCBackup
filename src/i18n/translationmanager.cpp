#include "translationmanager.h"

#include <QObject>
#include <QTranslator>
#include <QLocale>
#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

namespace MinecraftBackupTool {

TranslationManager::TranslationManager(QObject *parent)
    : QObject(parent)
    , m_translator(std::make_unique<QTranslator>())
    , m_currentLanguage("en")
{
    // Initialize language names
    m_languageNames["en"] = "English";
    m_languageNames["vi"] = "Tiếng Việt";
    
    setupDefaultTranslations();
}

void TranslationManager::loadTranslations()
{
    QStringList translationPaths = {
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/i18n",
        ":/i18n", // Resource path
        "i18n" // Relative path
    };
    
    for (const QString &path : translationPaths) {
        QDir dir(path);
        if (!dir.exists()) continue;
        
        QStringList translationFiles = dir.entryList(QStringList() << "*.qm", QDir::Files);
        
        for (const QString &file : translationFiles) {
            QString langCode = file.section('.', 0, 0).section('_', -1);
            if (m_languageNames.contains(langCode)) {
                if (m_translator->load(file, path)) {
                    QCoreApplication::installTranslator(m_translator.get());
                    emit translationLoaded(langCode);
                    break;
                }
            }
        }
    }
    
    // Load default language
    switchLanguage("en");
}

bool TranslationManager::switchLanguage(const QString &languageCode)
{
    // Remove previous translator
    if (m_currentLanguage != languageCode) {
        QCoreApplication::removeTranslator(m_translator.get());
    }
    
    QStringList translationPaths = {
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/i18n",
        ":/i18n",
        "i18n"
    };
    
    QString translationFile = QString("minecraftbackup_%1.qm").arg(languageCode);
    
    for (const QString &path : translationPaths) {
        QString fullPath = path + "/" + translationFile;
        if (QFile(fullPath).exists()) {
            if (m_translator->load(fullPath)) {
                QCoreApplication::installTranslator(m_translator.get());
                m_currentLanguage = languageCode;
                emit languageChanged(languageCode);
                return true;
            }
        }
    }
    
    emit translationError(QString("Translation file not found for language: %1").arg(languageCode));
    return false;
}

QStringList TranslationManager::getAvailableLanguages() const
{
    return m_languageNames.keys();
}

void TranslationManager::setupDefaultTranslations()
{
    // Fallback translations if .qm files are not available
    // This ensures the app works even without external translation files
}

} // namespace MinecraftBackupTool