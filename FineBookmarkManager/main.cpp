#include "mainwindow.h"
#include "fbm_database.h"
#include "fbm_databaseview.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    FBM_Database fbm_db;
    FBM_DatabaseView fbm_dbView;
    fbm_dbView.setModel(fbm_db.getTableModel());

    w.setBookmarksDataTable(&fbm_dbView);

    QObject::connect(&w, &MainWindow::foundNewUrl, &fbm_db, &FBM_Database::addJsonBookmark);
    w.show();
    return a.exec();
}
