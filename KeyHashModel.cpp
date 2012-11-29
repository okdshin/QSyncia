#include "KeyHashModel.h"
#include "qstringlist.h"
#include <QStandardItem>
KeyHashModel::KeyHashModel(QObject* parent) : QAbstractListModel(parent){}

auto KeyHashModel::Add(const syncia::database::FileKeyHash& key_hash) -> void {
    this->key_hash_list.push_back(key_hash);
    std::cout << "added " << key_hash << std::endl;
    reset();
}

auto KeyHashModel::GetKeyHash(const QModelIndex& index) -> syncia::database::FileKeyHash {
    return this->key_hash_list.at(index.row());
}

auto KeyHashModel::Clear() -> void {
    this->key_hash_list.clear();
    reset();
}

auto KeyHashModel::rowCount(const QModelIndex&) const -> int {
    return this->key_hash_list.size();
}

auto KeyHashModel::columnCount(const QModelIndex&) const -> int {
    return 4;
}

auto KeyHashModel::data(const QModelIndex& index, int role) const -> QVariant{
    const auto key_hash = this->key_hash_list.at(index.row());
    if(index.column() == 0 && role == Qt::DisplayRole){ return QString::fromStdString(key_hash.GetKeyword()()); }
    if(index.column() == 1 && role == Qt::DisplayRole){ return QString::fromStdString(key_hash.GetOwnerId()); }
    if(index.column() == 2 && role == Qt::DisplayRole){ return QString::fromStdString(boost::posix_time::to_simple_string(key_hash.GetBirthTime())); }
    if(index.column() == 3 && role == Qt::DisplayRole){ return QString::fromStdString(key_hash.GetHashId()()); }

    return QVariant();
}

auto KeyHashModel::flags(const QModelIndex& index) const -> Qt::ItemFlags {
    return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled;
}

auto KeyHashModel::setData(const QModelIndex &index, const QVariant &value, int role) -> bool {
    emit dataChanged(index, index);
    return true;
}
