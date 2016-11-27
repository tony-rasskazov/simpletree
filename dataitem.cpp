
#include "dataitem.h"

#include <QStringList>

#include <QDebug>

DataItem::DataItem(const QString &title, int id, const QString &dbTableName, const QString &dbChildTableName, DataItem *parent)
    : _level(parent == nullptr ? -1 : parent->_level + 1)
    , _dbTableName(dbTableName)
    , _dbChildTableName(dbChildTableName)
    , _changed(false)
    , _id(id)
    , _title(title)
    , _parentItem(parent)
{

}

DataItem::~DataItem()
{
    qDebug() << "DataItem::~DataItem";
    qDeleteAll(_childItems);
}

DataItem *DataItem::child(int number)
{
    return _childItems.value(number);
}

int DataItem::childCount() const
{
    return _childItems.count();
}

int DataItem::childNumber() const
{
    if (_parentItem)
        return _parentItem->_childItems.indexOf(const_cast<DataItem*>(this));

    return 0;
}


DataItem *DataItem::insertChild(DataItem *item, int position, int id)
{
    _childItems.insert(position, item);
    _childItemsById.insert(id, item);

    return item;
}

DataItem *DataItem::parent()
{
    return _parentItem;
}

bool DataItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > _childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        auto c = _childItems.takeAt(position);
        QSqlQuery q = c->prepareDeleteSqlQuery();
        q.exec();
        //c->
        delete c;
    }

    return true;
}

// вынести в статические методы Vehicle и VehicleModel, для поиска в них уже
DataItem *DataItem::getChildById(int id)
{
    return _childItemsById.value(id, nullptr);
}

QVariant DataItem::data(int column) const
{
    if (_level == -1) {
        return QList<QString>({"vehicle", "model"}).at(column);
    } else
        return column == _level ? _title : QVariant();
}

bool DataItem::setData(int column, const QVariant &value)
{
//    qDebug() << "DataItem::setData " << dbTableName() << "value" << value;

   // if (column < 0 || (column != _level))
    //    return false;

    _title = value.toString();
    _changed = true;

    return true;
}

int DataItem::level() const
{
    return _level;
}

void DataItem::setLevel(const int &level)
{
    _level = level;
}

QString DataItem::dbChildTableName() const
{
    return _dbChildTableName;
}

void DataItem::setDbChildTableName(const QString &dbChildTableName)
{
    _dbChildTableName = dbChildTableName;
}

QString DataItem::dbTableName() const
{
    return _dbTableName;
}

void DataItem::setDbTableName(const QString &dbTableName)
{
    _dbTableName = dbTableName;
}

int DataItem::dbIndex() const
{
    return _id;
}

void DataItem::setDbIndex(int dbIndex)
{
    _id = dbIndex;
}

int DataItem::dbParentIndex() const
{
    return _dbParentIndex;
}

void DataItem::setDbParentIndex(int dbParentIndex)
{
    _dbParentIndex = dbParentIndex;
}

int DataItem::id() const
{
    return _id;
}

QString DataItem::title() const
{
    return _title;
}

void DataItem::setTitle(const QString &title)
{
    _title = title;
}

QString DataItem::toString() const
{
    return QString("(%1,%2,%3,%4)").arg(_id).arg(_title).arg(_dbTableName).arg(_dbChildTableName);
}

QSqlQuery DataItem::prepareDeleteSqlQuery() const
{
    QSqlQuery q;
    q.prepare(QString("DELETE FROM %1 WHERE id=%2").arg(dbTableName()).arg(id()) );
    return q;
}

void DataItem::setId(int id)
{
    _id = id;
}
