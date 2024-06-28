#ifndef FBM_DATABASEVIEW_H
#define FBM_DATABASEVIEW_H

#include <QWidget>

#include <QTableView>
#include <QVBoxLayout>
#include <QSqlQueryModel>
//allow view and manipulate database view table
class FBM_DatabaseView : public QWidget
{
    Q_OBJECT
public:
    explicit FBM_DatabaseView(QWidget *parent = nullptr);
    void setModel(QSqlQueryModel* tableModel);
//    QTableView* getTableView();
private:
    QVBoxLayout* ly_main;
    QTableView* tv_bookmarksData;
signals:

};

#endif // FBM_DATABASEVIEW_H
