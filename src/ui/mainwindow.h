#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QList>
#include "../core/databasemanager.h"
#include "../models/contact.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    DatabaseManager *db;
    QList<Contact> contacts;

    QLineEdit *search;
    QListWidget *list;
    QLabel *pLab;
    QLabel *iLab;
    QLabel *bookLab;
    QComboBox *sortCombo;

    void setupUi();
    void refresh();

public:
    MainWindow(QWidget *p = nullptr);

private slots:
    void add();
    void edit();
    void del();
    void sel();
    void exp();
    void imp();
};

#endif
