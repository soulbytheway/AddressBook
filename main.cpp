#include <QApplication>
#include "src/ui/mainwindow.h"
#include "src/ui/stylesheet.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    StyleSheetManager::applyTheme(a);
    MainWindow w;
    w.show();
    return a.exec();
}
