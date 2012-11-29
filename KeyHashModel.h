#ifndef KEYHASHMODEL_H
#define KEYHASHMODEL_H
#include <QAbstractListModel>
#include "syncia/SynciaEnvironment.h"

class KeyHashModel : public QAbstractListModel {
    Q_OBJECT
public:
    KeyHashModel(QObject* parent=0);

    void Add(const syncia::database::FileKeyHash& key_hash);
    syncia::database::FileKeyHash GetKeyHash(const QModelIndex& index);
    void Clear();

    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex&) const;
    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:

    syncia::database::FileKeyHashList key_hash_list;

};


#endif // KEYHASHMODEL_H
