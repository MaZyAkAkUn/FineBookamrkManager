#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <functional>
#include <iostream>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <QList>
#include <QUrl>
#include <QMap>
#include <QMapIterator>

#include <QTreeWidget>
#include <QTreeWidgetItem>



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setBookmarksDataTable(QWidget* tableWidget);
private:
    QVBoxLayout* ly_main;
    QPushButton* pb_openBookmarksFile;
    QPushButton* pb_parseBookmarksFiles;

    QTextEdit* te_bookmarksFileContent;
    QTreeWidget* trw_bookmarksTree;
    void fetchFileContent(const QString& name, const QByteArray& data);

    QMap<QString, QUrl> knowns_pathes;

    void parseChromium(const QUrl &path);
    void parseGecko(const QUrl &path);

    QTreeWidgetItem *make_tree_top_item(QTreeWidget *parent, const QString &doc_key);
    void make_tree_item(QTreeWidgetItem *parent, const QString &key, const QJsonValue &value);

    void simplifyJsonStruct(QJsonDocument& bookmarksDoc);
    void parseChildrenSubFolder(QJsonObject& rootObj);

    QWidget* w_bookmarksDataTable;


private slots:
    void openFileDialog();
    void autoParseBookmarksFiles();

    void processSingleBookmarkedUrl(QJsonObject singleBookmarkedUrl);

signals:
    void foundNewUrl(QJsonObject singleBookmarkedUrl);
};
#endif // MAINWINDOW_H
