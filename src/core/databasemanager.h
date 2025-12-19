#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QString>
#include <QList>
#include "../models/contact.h"

class DatabaseManager {
public:
    DatabaseManager();
    bool saveToJson(const QList<Contact> &contacts, const QString &path);
    QList<Contact> loadFromJson(const QString &path);
    bool exportToCsv(const QList<Contact> &contacts, const QString &path);
    bool exportToTxt(const QList<Contact> &contacts, const QString &path);
    QList<Contact> importFromCsv(const QString &path);

    QString storeContactPhoto(const QString &sourcePath, const QString &contactId);
    void removeContactPhoto(const QString &photoPath);
};

#endif
