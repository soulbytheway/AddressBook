#include "databasemanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QCoreApplication>

DatabaseManager::DatabaseManager() {
    QString base = QCoreApplication::applicationDirPath();
    QDir().mkpath(base + "/data");
    QDir().mkpath(base + "/assets/photos");
}

bool DatabaseManager::saveToJson(const QList<Contact> &contacts, const QString &path) {
    QJsonArray arr;
    for (const auto &c : contacts) arr.append(c.toJson());
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(QJsonDocument(arr).toJson());
    return true;
}

QList<Contact> DatabaseManager::loadFromJson(const QString &path) {
    QList<Contact> list;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return list;
    QJsonArray arr = QJsonDocument::fromJson(f.readAll()).array();
    for (const QJsonValue &v : arr) list.append(Contact::fromJson(v.toObject()));
    return list;
}

bool DatabaseManager::exportToCsv(const QList<Contact> &contacts, const QString &path) {
    QFile f(path); if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&f); out << "Name;Phone;Email\n";
    for (const auto &c : contacts) out << c.name() << ";" << c.phone() << ";" << c.email() << "\n";
    return true;
}

QList<Contact> DatabaseManager::importFromCsv(const QString &path) {
    QList<Contact> list; QFile f(path); if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return list;
    QTextStream in(&f); bool head = true;
    while (!in.atEnd()) {
        QString line = in.readLine(); if (head) { head = false; continue; }
        QStringList fields = line.split(";");
        if (fields.size() >= 2) list.append(Contact(fields[0], fields[1], fields.size() > 2 ? fields[2] : "", ""));
    }
    return list;
}

bool DatabaseManager::exportToTxt(const QList<Contact> &contacts, const QString &path) {
    QFile f(path); if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&f); for (const auto &c : contacts) out << c.name() << " | " << c.phone() << " | " << c.email() << "\n";
    return true;
}

QString DatabaseManager::storeContactPhoto(const QString &src, const QString &id) {
    if (src.isEmpty() || !QFile::exists(src)) return src;
    QString base = QCoreApplication::applicationDirPath();
    QString dest = base + "/assets/photos/" + id + "." + QFileInfo(src).suffix();
    if (src == dest) return dest;
    if (QFile::exists(dest)) QFile::remove(dest);
    if (QFile::copy(src, dest)) return dest;
    return src;
}

void DatabaseManager::removeContactPhoto(const QString &photoPath) {
    if (!photoPath.isEmpty() && photoPath.contains("assets/photos") && QFile::exists(photoPath)) {
        QFile::remove(photoPath);
    }
}
