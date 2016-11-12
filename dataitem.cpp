
#include "dataitem.h"

#include <QStringList>

#include <QDebug>

DataItem::DataItem(const QVector<QVariant> &data, const QString &dbTableName, const QString &dbChildTableName, DataItem *parent)
    : _level(parent == nullptr ? -1 : parent->_level + 1)
    , _dbTableName(dbTableName)
    , _dbChildTableName(dbChildTableName)
    , _changed(false)
{
    qDebug() << "DataItem::DataItem" << dbTableName << dbChildTableName << "data" << data;
    _parentItem = parent;
    _itemData = data;

    if (_itemData.size() > 0)
        _title = _itemData.at(0).toString();
}

DataItem::DataItem(const QVector<QVariant> &data, int dbIndex, const QString &dbTableName, const QString &dbChildTableName, DataItem *parent)
    : _level(parent == nullptr ? -1 : parent->_level + 1)
    , _dbTableName(dbTableName)
    , _dbChildTableName(dbChildTableName)
    , _changed(false)
    , _dbIndex(dbIndex)
{
    qDebug() << "DataItem::DataItem" << dbTableName << dbChildTableName << "dbIndex" << dbIndex << "data" << data;
    _parentItem = parent;
    _itemData = data;

    if (_itemData.size() > 0)
        _title = _itemData.at(0).toString();
}

DataItem::~DataItem()
{
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

int DataItem::columnCount() const
{
    return _itemData.count();
}

bool DataItem::insertChildren(int position, int count, const QString &itemTableName, const QString &itemChildsTableName)
{
    qDebug() << "DataItem::insertChildren " << itemTableName << itemChildsTableName << _title << "position" <<  position <<  "count" << count;

    if (position < 0 || position > _childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(1);

        data[0] = QString(QObject::tr("[new vehicle %1]")).arg(row);

        DataItem *item = new DataItem(data, itemTableName, itemChildsTableName, this);

        _childItems.insert(position, item);
    }

    return true;
}

DataItem *DataItem::parent()
{
    return _parentItem;
}

bool DataItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > _childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete _childItems.takeAt(position);

    return true;
}

QVariant DataItem::data(int column) const
{
    if (_level == -1) {
        return _itemData.value(column);
    } else
        return column == _level ? _title : QVariant();
}

bool DataItem::setData(int column, const QVariant &value)
{
    qDebug() << "DataItem::setData " << dbTableName() << "value" << value;

   // if (column < 0 || (column != _level))
    //    return false;

    _title = value.toString();

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
    return _dbIndex;
}

void DataItem::setDbIndex(int dbIndex)
{
    _dbIndex = dbIndex;
}

int DataItem::dbParentIndex() const
{
    return _dbParentIndex;
}

void DataItem::setDbParentIndex(int dbParentIndex)
{
    _dbParentIndex = dbParentIndex;
}