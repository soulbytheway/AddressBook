#include "contactform.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QRegularExpression>
#include <QStyle>

ContactForm::ContactForm(QWidget *p) : QDialog(p) {
    setWindowTitle("Contact Editor");
    setMinimumWidth(400);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    auto *formLayout = new QFormLayout();
    formLayout->setSpacing(15);

    nIn = new QLineEdit();
    nIn->setPlaceholderText("Full Name");

    pIn = new QLineEdit();
    pIn->setPlaceholderText("+380XXXXXXXXX");

    eIn = new QLineEdit();
    eIn->setPlaceholderText("email@example.com");

    pPrev = new QLabel();
    pPrev->setFixedSize(100, 100);
    pPrev->setScaledContents(true);
    pPrev->setObjectName("photoLabel");
    pPrev->setAlignment(Qt::AlignCenter);
    pPrev->setStyleSheet("background: #1a1c25; border: 1px solid #3b4252; color: #4c566a;");
    pPrev->setText("IMAGE");

    auto *btnPhoto = new QPushButton("Browse Photo...");
    btnPhoto->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
    connect(btnPhoto, &QPushButton::clicked, this, &ContactForm::onPh);

    formLayout->addRow("Name:", nIn);
    formLayout->addRow("Phone:", pIn);
    formLayout->addRow("Email:", eIn);
    formLayout->addRow(pPrev, btnPhoto);

    mainLayout->addLayout(formLayout);

    saveBtn = new QPushButton("Save Contact");
    saveBtn->setMinimumHeight(40);
    saveBtn->setEnabled(false);
    connect(saveBtn, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(saveBtn);

    connect(nIn, &QLineEdit::textChanged, this, &ContactForm::validate);
    connect(pIn, &QLineEdit::textChanged, this, &ContactForm::validate);
    connect(eIn, &QLineEdit::textChanged, this, &ContactForm::validate);
}

void ContactForm::validate() {
    QRegularExpression nameRex(R"(^[a-zA-Zа-яА-ЯёЁіІїЇєЄґҐ\s']{2,50}$)");
    QRegularExpression phoneRex(R"(^\+?\d{10,15}$)");
    QRegularExpression mailRex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");

    bool nameOk = nameRex.match(nIn->text().trimmed()).hasMatch();
    bool phoneOk = phoneRex.match(pIn->text().trimmed()).hasMatch();
    bool emailOk = eIn->text().isEmpty() || mailRex.match(eIn->text().trimmed()).hasMatch();

    nIn->setStyleSheet(nameOk || nIn->text().isEmpty() ? "" : "border: 1px solid #bf616a;");
    pIn->setStyleSheet(phoneOk || pIn->text().isEmpty() ? "" : "border: 1px solid #bf616a;");
    eIn->setStyleSheet(emailOk ? "" : "border: 1px solid #bf616a;");

    saveBtn->setEnabled(nameOk && phoneOk && emailOk);
}

void ContactForm::onPh() {
    curPh = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg)");
    if(!curPh.isEmpty()) {
        pPrev->setPixmap(QPixmap(curPh));
        pPrev->setText("");
    }
}

void ContactForm::setContactData(const Contact &c) {
    nIn->setText(c.name()); pIn->setText(c.phone()); eIn->setText(c.email());
    curPh = c.photoPath();
    if(!curPh.isEmpty() && QFile::exists(curPh)) {
        pPrev->setPixmap(QPixmap(curPh));
        pPrev->setText("");
    }
    validate();
}

Contact ContactForm::getContactData() const {
    return Contact(nIn->text().trimmed(), pIn->text().trimmed(), eIn->text().trimmed(), curPh);
}
