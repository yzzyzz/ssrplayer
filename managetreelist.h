#ifndef MANAGETREELIST_H
#define MANAGETREELIST_H

#include <QDir>
#include <QObject>
#include <QSettings>
#include <QStandardItemModel>
#include <QTreeView>

QT_BEGIN_NAMESPACE namespace MLT
{
    class ManageTreeList;
}
QT_END_NAMESPACE

class ManageTreeList : public QObject {
    Q_OBJECT

signals:
    void updateUiListDone(QList<QStringList> value);

public:
    explicit ManageTreeList(QTreeView* init_list, QObject* parent = nullptr);
    ~ManageTreeList();

    // operations
    void importToList(QString sData);
    void importToList2(QString sData);

    void switchExpand(const QModelIndex& index);
    // void clear();
    // // int getRow(QListWidgetItem* item);

    // // ui update
    // // void updateUIonItemChange(QListWidgetItem* cur_item, QListWidgetItem* new_item);

    // // save/load
    // void saveList(QSettings& settings, QString list_name);
    // void loadList(QSettings& settings, QString list_name);

    // getters & setters
    //  void setItem_list(QListWidget* newMusic_list);
    //   QListWidget* getItem_list() const;

private:
    QTreeView* tree_list;
    QStandardItemModel* tree_list_model;
};

#endif // MANAGELIST_H
