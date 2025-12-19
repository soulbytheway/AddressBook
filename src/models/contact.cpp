#include "contact.h"
Contact::Contact() { m_id = QUuid::createUuid().toString(QUuid::WithoutBraces); }
Contact::Contact(QString name, QString phone, QString email, QString photo)
    : m_name(name), m_phone(phone), m_email(email), m_photoPath(photo) {
    m_id = QUuid::createUuid().toString(QUuid::WithoutBraces);
}
QJsonObject Contact::toJson() const {
    QJsonObject j; j["id"]=m_id; j["name"]=m_name; j["phone"]=m_phone; j["email"]=m_email; j["photo"]=m_photoPath;
    return j;
}
Contact Contact::fromJson(const QJsonObject &j) {
    Contact c; c.m_id=j["id"].toString(); c.m_name=j["name"].toString();
    c.m_phone=j["phone"].toString(); c.m_email=j["email"].toString(); c.m_photoPath=j["photo"].toString();
    return c;
}
