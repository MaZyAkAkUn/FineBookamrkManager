#include "fbm_databaseview.h"

FBM_DatabaseView::FBM_DatabaseView(QWidget *parent)
    : QWidget{parent}
{
    ly_main = new QVBoxLayout(this);


    tv_bookmarksData = new QTableView();
    ly_main->addWidget(tv_bookmarksData);

}

void FBM_DatabaseView::setModel(QSqlQueryModel *tableModel)
{
    tv_bookmarksData->setModel(tableModel);
}

//QTableView *FBM_DatabaseView::getTableView()
//{
//    return tv_bookmarksData;
//}
