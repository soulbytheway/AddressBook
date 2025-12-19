#ifndef CONTACT_H
#define CONTACT_H
#include <QString>
#include <QJsonObject>
#include <QUuid>

class Contact {
private:
    QString m_id, m_name, m_phone, m_email, m_photoPath;
public:
    Contact();
    Contact(QString name, QString phone, QString email, QString photo);
    QString id() const { return m_id; }
    QString name() const { return m_name; }
    QString phone() const { return m_phone; }
    QString email() const { return m_email; }
    QString photoPath() const { return m_photoPath; }
    void setName(const QString &n) { m_name = n; }
    void setPhone(const QString &p) { m_phone = p; }
    void setEmail(const QString &e) { m_email = e; }
    void setPhotoPath(const QString &path) { m_photoPath = path; }
    QJsonObject toJson() const;
    static Contact fromJson(const QJsonObject &json);
};
#endif
