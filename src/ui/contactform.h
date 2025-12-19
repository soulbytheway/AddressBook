#ifndef CONTACT_FORM_H
#define CONTACT_FORM_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "../models/contact.h"

class ContactForm : public QDialog {
    Q_OBJECT
private:
    QLineEdit *nIn, *pIn, *eIn;
    QLabel *pPrev;
    QPushButton *saveBtn;
    QString curPh;

public:
    ContactForm(QWidget *p = nullptr);
    void setContactData(const Contact &c);
    Contact getContactData() const;

private slots:
    void onPh();
    void validate();
};

#endif
