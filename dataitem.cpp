
#include "dataitem.h"

#include <QStringList>

#include <QDebug>

DataItem::DataItem(const QVector<QVariant> &data, DataItem *parent)
    : _level(parent == nullptr ? -1 : parent->_level + 1)
{
    qDebug() << "TreeItem::TreeItem level" << _level << "data" << data;
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

bool DataItem::insertChildren(int position, int count)
{
    qDebug() << "TreeItem::insertChildren " << _title << "position" <<  position <<  "count" << count;

    if (position < 0 || position > _childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(1);
        data[0] = QString("vehicle %1").arg(row);
        DataItem *item = new DataItem(data, this);
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
    qDebug() << "TreeItem::setData column" << column << "value" << value;

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
