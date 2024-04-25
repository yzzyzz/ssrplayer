#include "managetreelist.h"
#include "HttpClient.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStandardItemModel>
#include <iostream>

ManageTreeList::ManageTreeList(QTreeView* init_list, QObject* parent)
    : QObject { parent }
    , tree_list(init_list)
{
    this->tree_list = init_list;
}

ManageTreeList::~ManageTreeList()
{
}

void ManageTreeList::importToList2(QString data)
{
    // 创建数据模型
    //  QStandardItemModel* model;
    //  tree_list_model->clear();
    tree_list_model = new QStandardItemModel(tree_list);
    QMap<QString, QList<QStringList>> data_map; // 创建QMap对象
    QStringList list1 = data.split("\n");

    // 按照给定的键的顺序构建 QVector
    QVector<QString> keys;
    keys << "我的最爱"
         << "音乐电台"
         << "文化曲艺";

    for (int i = 0; i < list1.size(); i++) {
        qDebug() << list1[i];
        if (list1[i].length() < 10) {
            break;
        }
        QStringList list2 = list1[i].split(",");
        if (data_map.count(list2[2]) > 0) {
            data_map[list2[2]].append(list2);
        } else {
            if (!keys.contains(list2[2])) {
                keys << list2[2];
            }
            QList<QStringList> list_tmp = { list2 };
            data_map.insert(list2[2], list_tmp);
        }
    }
    // 使用基于下标的循环遍历
    for (int i = 0; i < keys.size(); i++) {
        if (!data_map.contains(keys[i])) {
            continue;
        }
        QStandardItem* tmptop = new QStandardItem(keys[i]);
        tmptop->setFlags(tmptop->flags() & ~Qt::ItemIsEditable); // 禁止编辑
        for (int j = 0; j < data_map[keys[i]].size(); j++) {
            // QStringList tmpDataItem = it.value().indexOf(i);
            QList tmpDataItem = data_map[keys[i]][j];
            QStandardItem* newItem = new QStandardItem(tmpDataItem[0]);
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable); // 禁止编辑
            newItem->setData(tmpDataItem[1], Qt::UserRole);
            tmptop->appendRow(newItem);
        }
        tree_list_model->appendRow(tmptop);
    }
    emit updateUiListDone(data_map["我的最爱"]);
    tree_list->setHeaderHidden(true);
    tree_list->setIndentation(14);
    tree_list->setModel(tree_list_model);
}

void ManageTreeList::importToList(QString data)
{

    HttpClient("http://122.51.216.6:8005/radio.php")
        .success([this](const QString& response) {
            importToList2(response);
        })
        .get();
}

void ManageTreeList::switchExpand(const QModelIndex& index)
{
    if (!tree_list->isExpanded(index)) {
        tree_list->expand(index);
    } else {
        tree_list->collapse(index);
    }
}

// void ManageTreeList::removeSelectedFromList()
// {
//     // QList<QListWidgetItem*> removed_items = item_list->selectedItems();
//     // for (QListWidgetItem* item : removed_items) {
//     //     item_list->takeItem(item_list->row(item));
//     //     delete item;
//     // }
// }

// void ManageTreeList::clear()
// {
//     ;
//     // item_list->clear();
// }

// void ManageList::loadList(QSettings& settings, QString list_name)
// {
//     int size = settings.beginReadArray(list_name);
//     for (int row = 0; row < size; row++) {
//         settings.setArrayIndex(row);
//         QString file_name = settings.value("fileName").toString();
//         QString file_path = settings.value("filePath").toString();

//         QListWidgetItem* cur_item = new QListWidgetItem;
//         cur_item->setIcon(QIcon(":/icons/res/music_notec2.png"));
//         cur_item->setText(file_name);
//         cur_item->setData(Qt::UserRole, file_path);
//         item_list->addItem(cur_item);
//     }
//     settings.endArray();
// }

// QListWidget* ManageList::getItem_list() const
// {
//     return item_list;
// }
