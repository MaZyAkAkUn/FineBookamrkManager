#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* w_centralWidget = new QWidget();
    this->setCentralWidget(w_centralWidget);
    ly_main = new QVBoxLayout();
    w_centralWidget->setLayout(ly_main);
    this->setLayout(ly_main);

    //TODO: make more universal patterns, to handle diferent companies and  software names
    knowns_pathes["chromium_based"] = QUrl("C:/Users/*/AppData/Local/Microsoft/Edge/User Data/Default/Bookmarks");
    knowns_pathes["gecko_based"] = QUrl("C:/Users/*/AppData/Roaming/Mozilla/Firefox/Profiles/*/places.sqlite");

    pb_openBookmarksFile = new QPushButton("Load File Manualy");
    te_bookmarksFileContent = new QTextEdit();
//    trw_bookmarksTree = new QTreeWidget();
//    trw_bookmarksTree->setColumnCount(2);
//    trw_bookmarksTree->setHeaderLabel("Tree View");
    pb_parseBookmarksFiles = new QPushButton("AutoParse Bookmarks");

    ly_main->addWidget(pb_openBookmarksFile);
    ly_main->addWidget(pb_parseBookmarksFiles);

//    ly_main->addWidget(te_bookmarksFileContent);
//    ly_main->addWidget(trw_bookmarksTree);
    connect(pb_openBookmarksFile,&QPushButton::clicked, this, &MainWindow::openFileDialog);
    connect(pb_parseBookmarksFiles,&QPushButton::clicked, this, &MainWindow::autoParseBookmarksFiles);
    connect(this, &MainWindow::foundNewUrl, this, &MainWindow::processSingleBookmarkedUrl);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setBookmarksDataTable(QWidget *tableWidget)
{
    w_bookmarksDataTable = tableWidget;
    ly_main->addWidget(w_bookmarksDataTable);
}


void MainWindow::openFileDialog()
{
//    std::function<void(const QString&, const QByteArray&)> func = [this](const QString &name, const QByteArray &data) {
//        this->fetchFileContent(name, data);
//    };
//    QFileDialog::getOpenFileContent("Bookmarks",   func);
    auto url = QFileDialog::getOpenFileUrl();
    if (!url.isEmpty()){
        this->parseChromium(url);
    }



}

void MainWindow::autoParseBookmarksFiles()
{

    //chrome C:\Users\username\AppData\Local\Google\Chrome\User Data\Default/Bookmarks ///Json
    //EDge: C:/Users/Andriyko/AppData/Local/Microsoft/Edge/User Data/Default/Bookmarks ///Json
    //MozilaFireFox: C:/Users/Andriyko/AppData/Roaming/Mozilla/Firefox/Profiles/36lro55x.default-release/places.sqlite
    //here we parse knowns pathes of bookmar ks stores

    QList<QPair<QString, QUrl>> foundFiles; //type, path

    foundFiles.append(QPair<QString, QUrl>("chromium", QUrl::fromLocalFile("C:/Users/Andriyko/AppData/Local/Microsoft/Edge/User Data/Default/Bookmarks")));

    QListIterator<QPair<QString, QUrl>> iter(foundFiles);

    while(iter.hasNext()){
        QPair<QString, QUrl> pair =iter.next(); //here our pair
        //so now we can use separated methods to parse diferent breowser engines
        //lets make for google and mozila first
        if(pair.first == QString("gecko")){
            this->parseGecko(pair.second);
        }
        if(pair.first == QString("chromium")){
            this->parseChromium(pair.second);
        }
    };
}

void MainWindow::processSingleBookmarkedUrl(QJsonObject singleBookmarkedUrl)
{
    //here we got a single Url object so we can process it
    //for start simple put it in textEdit widget in formated way
    //Here is a format:
    /*
          "date_added": "13329842664076756",
          "date_last_used": "13360691164523293",
          "guid": "f3cc68f7-debe-4e05-9260-c8a0235c3e37",
          "id": "6",
          "name": "Farms | PancakeSwap",
          "show_icon": false,
          "source": "user_add",
          "type": "url",
          "url": "https://pancakeswap.finance/farms"
     */
    this->te_bookmarksFileContent->append("["+singleBookmarkedUrl["name"].toString()+"]: {"+singleBookmarkedUrl["url"].toString()+"}");

}


void MainWindow::fetchFileContent(const QString &name, const QByteArray &data)
{
    qDebug()<<"File opened: "<<name;
    this->te_bookmarksFileContent->append(QString::fromStdString(data.toStdString()));

}

void MainWindow::make_tree_item(QTreeWidgetItem* parent, const QString &key, const QJsonValue &value) {
    QTreeWidgetItem* newItem = new QTreeWidgetItem(parent);
    newItem->setText(0, key);

    if (value.isObject()) {
        QJsonObject obj = value.toObject();
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            make_tree_item(newItem, it.key(), it.value());
        }
    } else if (value.isArray()) {
        QJsonArray arr = value.toArray();
        for (int i = 0; i < arr.size(); ++i) {
            make_tree_item(newItem, QString("[%1]").arg(i), arr[i]);
        }
    } else {
        newItem->setText(1, value.toString());
    }
}

void MainWindow::simplifyJsonStruct(QJsonDocument &bookmarksDoc)
{
    //here we simplify an chromium`s bookmark file to more convient way
    //so lets start from cleaning folder struct, and extract only urls

    if(bookmarksDoc.isObject()){
        //extract children
        auto rootObj = bookmarksDoc.object()["roots"].toObject();

        for(QString &objKey: rootObj.keys()){
            if(rootObj[objKey].isObject()){
                auto subObj = rootObj[objKey].toObject();
                if(subObj["type"] == "folder"){
                    //we found subfolder
                    parseChildrenSubFolder(subObj);
                }else{
                    emit foundNewUrl(subObj);
                }
            }
        }


    }


}

void MainWindow::parseChildrenSubFolder(QJsonObject &rootObj)
{
    //here we got subfolder, so we need to iterate throught it and extract simple urls
    //so at first, we need to extrac children array
    if(rootObj.contains("children")){
        auto childArray = rootObj["children"].toArray();
        for(auto child: childArray){
            auto childObj = child.toObject();
            if(childObj["type"] == "folder"){
                parseChildrenSubFolder(childObj);
            }else{
                //single url
                emit foundNewUrl(childObj);
            }
        }

    }
}

QTreeWidgetItem* MainWindow::make_tree_top_item(QTreeWidget* parent, const QString &doc_key) {
    QTreeWidgetItem* topItem = new QTreeWidgetItem(parent);
    topItem->setText(0, doc_key);
    parent->addTopLevelItem(topItem);
    return topItem;
}

void MainWindow::parseChromium(const QUrl &path) {
    // Here we parse a specific type of bookmark storage, so let's read the file and parse it.
    // In Chromium-based browsers, we can read bookmarks by this path: C:/Users/Andriyko/AppData/Local/Microsoft/Edge/User Data/Default/Bookmarks
    // in JSON format.
    // So let's do it.
    if (path.isValid()) {
        QFile jsonFile(path.toLocalFile());
        if (jsonFile.open(QFile::ReadOnly)) {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll());
            simplifyJsonStruct(jsonDoc);



//            for (auto  &key:jsonDoc.object().keys()) {

//                QTreeWidgetItem* topItem = make_tree_top_item(trw_bookmarksTree, key);
//                make_tree_item(topItem, key, jsonDoc.object().value(key));
//            }

//            te_bookmarksFileContent->append(QString::fromStdString(jsonDoc.toJson().toStdString()));
        } else {
            te_bookmarksFileContent->append("Error opening file: " + jsonFile.errorString());
        }
    } else {
        te_bookmarksFileContent->append("Invalid URL path.");
    }
}


void MainWindow::parseGecko(const QUrl &path)
{

}

