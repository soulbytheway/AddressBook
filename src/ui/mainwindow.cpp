#include "mainwindow.h"
#include "contactform.h"
#include <QCoreApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QPixmap>
#include <QStyle>
#include <QGraphicsDropShadowEffect>
#include <algorithm>

MainWindow::MainWindow(QWidget *p) : QMainWindow(p) {
    db = new DatabaseManager();
    QString dbPath = QCoreApplication::applicationDirPath() + "/data/contacts.json";
    contacts = db->loadFromJson(dbPath);
    setupUi();
    refresh();
}

void MainWindow::setupUi() {
    auto *cw = new QWidget(this);
    auto *lay = new QHBoxLayout(cw);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    auto *side = new QWidget();
    side->setObjectName("sidebar");
    side->setFixedWidth(320);
    auto *sLay = new QVBoxLayout(side);
    sLay->setContentsMargins(15, 25, 15, 15);
    sLay->setSpacing(12);

    bookLab = new QLabel("Address Book");
    bookLab->setStyleSheet("font-size: 16px; font-weight: 800; color: #81a1c1; text-transform: uppercase;");

    search = new QLineEdit();
    search->setObjectName("searchEdit");
    search->setPlaceholderText("Search contacts...");

    sortCombo = new QComboBox();
    sortCombo->addItem("Sort: Name (A-Z)", 0);
    sortCombo->addItem("Sort: Name (Z-A)", 1);
    sortCombo->addItem("Sort: Phone Number", 2);
    sortCombo->setStyleSheet("height: 35px;");

    list = new QListWidget();

    auto *bA = new QPushButton("Add Contact");
    bA->setObjectName("addBtn");
    bA->setIcon(style()->standardIcon(QStyle::SP_DirHomeIcon));

    sLay->addWidget(bookLab);
    sLay->addWidget(search);
    sLay->addWidget(sortCombo);
    sLay->addWidget(list);
    sLay->addWidget(bA);

    auto *det = new QWidget();
    auto *dLay = new QVBoxLayout(det);
    dLay->setContentsMargins(40, 40, 40, 40);
    dLay->setSpacing(20);

    pLab = new QLabel();
    pLab->setObjectName("photoLabel");
    pLab->setFixedSize(160, 160);
    pLab->setAlignment(Qt::AlignCenter);

    auto *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 150));
    shadow->setOffset(0, 5);
    pLab->setGraphicsEffect(shadow);

    iLab = new QLabel("Select a contact");
    iLab->setObjectName("nameLabel");
    iLab->setAlignment(Qt::AlignCenter);

    auto *btns = new QHBoxLayout();
    auto *bE = new QPushButton("Edit");
    bE->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    auto *bD = new QPushButton("Delete");
    bD->setObjectName("deleteBtn");
    bD->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    btns->addWidget(bE);
    btns->addWidget(bD);

    auto *ios = new QHBoxLayout();
    auto *bEx = new QPushButton("Export");
    bEx->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    auto *bIm = new QPushButton("Import");
    bIm->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    ios->addWidget(bEx);
    ios->addWidget(bIm);

    dLay->addStretch();
    dLay->addWidget(pLab, 0, Qt::AlignCenter);
    dLay->addWidget(iLab, 0, Qt::AlignCenter);
    dLay->addLayout(btns);
    dLay->addLayout(ios);
    dLay->addStretch();

    lay->addWidget(side);
    lay->addWidget(det);
    setCentralWidget(cw);

    connect(bA, &QPushButton::clicked, this, &MainWindow::add);
    connect(bE, &QPushButton::clicked, this, &MainWindow::edit);
    connect(bD, &QPushButton::clicked, this, &MainWindow::del);
    connect(list, &QListWidget::itemSelectionChanged, this, &MainWindow::sel);
    connect(list, &QListWidget::itemDoubleClicked, this, &MainWindow::edit);
    connect(search, &QLineEdit::textChanged, this, [this]{ refresh(); });
    connect(sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]{ refresh(); });
    connect(bEx, &QPushButton::clicked, this, &MainWindow::exp);
    connect(bIm, &QPushButton::clicked, this, &MainWindow::imp);
}

void MainWindow::refresh() {
    list->clear();
    QString filter = search->text();
    int sortType = sortCombo->currentData().toInt();

    std::sort(contacts.begin(), contacts.end(), [sortType](const Contact &a, const Contact &b){
        if (sortType == 0) return QString::localeAwareCompare(a.name(), b.name()) < 0;
        if (sortType == 1) return QString::localeAwareCompare(a.name(), b.name()) > 0;
        return a.phone() < b.phone();
    });

    for(const auto &c : contacts) {
        if(filter.isEmpty() || c.name().contains(filter, Qt::CaseInsensitive)) {
            auto *item = new QListWidgetItem(c.name());
            item->setData(Qt::UserRole, c.id());
            list->addItem(item);
        }
    }
}

void MainWindow::sel() {
    if(!list->currentItem()) return;
    QString id = list->currentItem()->data(Qt::UserRole).toString();
    auto it = std::find_if(contacts.begin(), contacts.end(), [&](const Contact &c){ return c.id() == id; });

    if(it != contacts.end()) {
        QString info = QString(
                           "<div style='text-align: center;'>"
                           "<span style='font-size: 24px; color: #eceff4; font-weight: bold;'>%1</span><br><br>"
                           "<span style='font-size: 16px; color: #81a1c1;'>Phone: %2</span><br>"
                           "<span style='font-size: 14px; color: #abb2bf;'>Email: %3</span>"
                           "</div>"
                           ).arg(it->name(), it->phone(), it->email().isEmpty() ? "Not specified" : it->email());
        iLab->setText(info);

        if(!it->photoPath().isEmpty() && QFile::exists(it->photoPath())) {
            pLab->setPixmap(QPixmap(it->photoPath()).scaled(160, 160, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            pLab->setText("");
        } else {
            pLab->clear(); pLab->setText("NO IMAGE");
        }
    }
}

void MainWindow::add() {
    ContactForm f(this);
    if(f.exec() == QDialog::Accepted) {
        Contact c = f.getContactData();
        if(!c.photoPath().isEmpty()) c.setPhotoPath(db->storeContactPhoto(c.photoPath(), c.id()));
        contacts.append(c);
        db->saveToJson(contacts, QCoreApplication::applicationDirPath() + "/data/contacts.json");
        refresh();
    }
}

void MainWindow::edit() {
    if(!list->currentItem()) return;
    QString id = list->currentItem()->data(Qt::UserRole).toString();
    auto it = std::find_if(contacts.begin(), contacts.end(), [&](const Contact &c){ return c.id() == id; });

    if(it != contacts.end()) {
        ContactForm f(this);
        f.setContactData(*it);

        QString oldPhoto = it->photoPath();

        if(f.exec() == QDialog::Accepted) {
            Contact u = f.getContactData();

            if(u.photoPath() != oldPhoto && !oldPhoto.isEmpty()) {
                db->removeContactPhoto(oldPhoto);
            }

            if(!u.photoPath().isEmpty() && !u.photoPath().contains("assets/photos"))
                u.setPhotoPath(db->storeContactPhoto(u.photoPath(), it->id()));

            *it = u;
            db->saveToJson(contacts, QCoreApplication::applicationDirPath() + "/data/contacts.json");
            refresh(); sel();
        }
    }
}

void MainWindow::del() {
    if(!list->currentItem()) return;
    if(QMessageBox::question(this, "Confirmation", "Delete contact?") == QMessageBox::Yes) {
        QString id = list->currentItem()->data(Qt::UserRole).toString();

        auto it = std::find_if(contacts.begin(), contacts.end(), [&](const Contact &c){ return c.id() == id; });

        if (it != contacts.end()) {
            db->removeContactPhoto(it->photoPath());

            contacts.erase(it);
            db->saveToJson(contacts, QCoreApplication::applicationDirPath() + "/data/contacts.json");
            refresh();
            pLab->clear(); pLab->setText("NO IMAGE");
            iLab->setText("Select a contact");
        }
    }
}

void MainWindow::exp() {
    QString p = QFileDialog::getSaveFileName(this, "Export", QCoreApplication::applicationDirPath() + "/data/", "JSON (*.json);;CSV (*.csv);;TXT (*.txt)");
    if(p.isEmpty()) return;
    if(p.endsWith(".json")) db->saveToJson(contacts, p);
    else if(p.endsWith(".csv")) db->exportToCsv(contacts, p);
    else db->exportToTxt(contacts, p);
}

void MainWindow::imp() {
    QString p = QFileDialog::getOpenFileName(this, "Import", QCoreApplication::applicationDirPath() + "/data/", "Supported Files (*.json *.csv *.txt)");
    if(p.isEmpty()) return;
    if(p.endsWith(".json")) contacts = db->loadFromJson(p);
    else contacts = db->importFromCsv(p);
    refresh();
}
