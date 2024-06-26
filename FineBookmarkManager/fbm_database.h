#ifndef FBM_DATABASE_H
#define FBM_DATABASE_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <QJsonObject>
//class which handle DB comunication
class FBM_Database : public QObject
{
    Q_OBJECT
public:
    explicit FBM_Database(QObject *parent = nullptr);

    bool addSingleBookmark(const QString &title, const QString &url, const QString &description, const QString &tags);
signals:
public slots:
    void addJsonBookmark(QJsonObject jsonBookmark);

private:
    bool setupDatabase();
};

#endif // FBM_DATABASE_H
