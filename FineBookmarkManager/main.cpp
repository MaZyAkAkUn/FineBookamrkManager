#include "mainwindow.h"
#include "fbm_database.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    FBM_Database fbm_db;
    QObject::connect(&w, &MainWindow::foundNewUrl, &fbm_db, &FBM_Database::addJsonBookmark);
    w.show();
    return a.exec();
}
