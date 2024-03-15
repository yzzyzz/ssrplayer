#include "managelist.h"

#include "HttpClient.h"

#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <iostream>

ManageList::ManageList(QListWidget* init_list, QObject* parent)
    : QObject { parent }
    , item_list(init_list)
{
}

ManageList::~ManageList()
{
}

void ManageList::importToList(const QDir& dir, QString format)
{
    // Match designated format
    static QRegularExpression re(format, QRegularExpression::CaseInsensitiveOption);

    // register imported files
    for (const QFileInfo& file : dir.entryInfoList(QDir::Files)) {
        if (!re.match(file.fileName()).hasMatch())
            continue;
        auto items = item_list->findItems(file.fileName(), Qt::MatchExactly);
        if (!items.isEmpty())
            continue;

        QListWidgetItem* item = new QListWidgetItem;
        item->setIcon(QIcon(":/icons/res/music_notec2.png"));
        item->setText(file.fileName());
        item->setData(Qt::UserRole, file.absoluteFilePath());
        item_list->addItem(item);
    }
}

void ManageList::importToList2(QString sData)
{
    QStringList list1 = sData.split("\n");
    item_list->clear();
    for (int i = 0; i < list1.size(); i++) {
        qDebug() << list1[i];
        if (list1[i].length() < 10) {
            break;
        }
        QStringList list2 = list1[i].split(",");
        QListWidgetItem* item = new QListWidgetItem;
        item->setIcon(QIcon(":/icons/res/music_notec2.png"));
        // QString Showtitle = QString::number(i + 1) + "." + list2[0];
        item->setText(QString("%1").arg(i, 3) + " " + list2[0]);
        item->setData(Qt::UserRole, list2[1]);
        item_list->addItem(item);
    }
}

void ManageList::importLiveList()
{
    // [[1]] GET 请求无参数
    HttpClient("http://122.51.216.6:8005/radio.php")
        .success([this](const QString& response) {
            importToList2(response);
        })
        .get();
}

void ManageList::removeSelectedFromList()
{
    QList<QListWidgetItem*> removed_items = item_list->selectedItems();
    for (QListWidgetItem* item : removed_items) {
        item_list->takeItem(item_list->row(item));
        delete item;
    }
}

void ManageList::clear()
{
    item_list->clear();
}

int ManageList::getRow(QListWidgetItem* item)
{
    return item_list->row(item);
}

void ManageList::updateUIonItemChange(QListWidgetItem* cur_item, QListWidgetItem* new_item)
{
    cur_item->setSelected(false);
    new_item->setSelected(true);
    item_list->scrollToItem(new_item);
}

void ManageList::saveList(QSettings& settings, QString list_name)
{
    settings.beginWriteArray(list_name);
    for (int row = 0; row < item_list->count(); row++) {
        settings.setArrayIndex(row);
        QListWidgetItem* cur_item = item_list->item(row);
        settings.setValue("fileName", cur_item->text());
        settings.setValue("filePath", cur_item->data(Qt::UserRole));
    }
    settings.endArray();
}

void ManageList::loadList(QSettings& settings, QString list_name)
{
    int size = settings.beginReadArray(list_name);
    for (int row = 0; row < size; row++) {
        settings.setArrayIndex(row);
        QString file_name = settings.value("fileName").toString();
        QString file_path = settings.value("filePath").toString();

        QListWidgetItem* cur_item = new QListWidgetItem;
        cur_item->setIcon(QIcon(":/icons/res/music_notec2.png"));
        cur_item->setText(file_name);
        cur_item->setData(Qt::UserRole, file_path);
        item_list->addItem(cur_item);
    }
    settings.endArray();
}

void ManageList::setItem_list(QListWidget* newMusic_list)
{
    item_list = newMusic_list;
}

QListWidget* ManageList::getItem_list() const
{
    return item_list;
}
