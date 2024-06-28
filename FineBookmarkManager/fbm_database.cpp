#include "fbm_database.h"

FBM_Database::FBM_Database(QObject *parent)
    : QObject{parent}
{
    if (setupDatabase())
        fetchBookmarksData();
}

FBM_Database::~FBM_Database()
{
    if(db.isOpen()) db.close();
}


bool FBM_Database::setupDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("fbm.db");

    if (!db.open()) {
        qDebug() << "Error: Could not connect to database.";
        return false;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS bookmarks ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "title TEXT, "
                    "url TEXT, "
                    "description TEXT, "
                    "tags TEXT, "
                    "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)")) {
        qDebug() << "Error: Could not create table.";
        return false;
    }

    return true;
}

void FBM_Database::fetchBookmarksData()
{
    tableModel = new QSqlQueryModel(this);
    tableModel->setQuery("SELECT title, url FROM bookmarks LIMIT 0, 50", db);
    tableModel->setHeaderData(0, Qt::Horizontal, "Name");
    tableModel->setHeaderData(1, Qt::Horizontal, "Url");

}



bool FBM_Database::addSingleBookmark(const QString &title, const QString &url, const QString &description, const QString &tags) {
    QSqlQuery query;
    query.prepare("INSERT INTO bookmarks (title, url, description, tags) VALUES (:title, :url, :description, :tags)");
    query.bindValue(":title", title);
    query.bindValue(":url", url);
    query.bindValue(":description", description);
    query.bindValue(":tags", tags);

    if (!query.exec()) {
        qDebug() << "Error: Could not insert bookmark.";
        return false;
    }
    return true;
}

QSqlQueryModel *FBM_Database::getTableModel()
{
    return tableModel;
}

void FBM_Database::addJsonBookmark(QJsonObject jsonBookmark)
{
    addSingleBookmark(
        jsonBookmark["name"].toString(),
        jsonBookmark["url"].toString(),
        "","");

}
