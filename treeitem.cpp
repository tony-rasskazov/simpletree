
#include "treeitem.h"

#include <QStringList>

#include <QDebug>

TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
    : _level(parent == nullptr ? -1 : parent->_level + 1)
{
    qDebug() << "TreeItem::TreeItem level" << _level << "data" << data;
    _parentItem = parent;
    _itemData = data;

    if (_itemData.size() > 0)
        _title = _itemData.at(0).toString();
}

TreeItem::~TreeItem()
{
    qDeleteAll(_childItems);
}

TreeItem *TreeItem::child(int number)
{
    return _childItems.value(number);
}

int TreeItem::childCount() const
{
    return _childItems.count();
}

int TreeItem::childNumber() const
{
    if (_parentItem)
        return _parentItem->_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::columnCount() const
{
    return _itemData.count();
}

bool TreeItem::insertChildren(int position, int count)
{
    qDebug() << "TreeItem::insertChildren " << _title << "position" <<  position <<  "count" << count;

    if (position < 0 || position > _childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(1);
        data[0] = QString("vehicle %1").arg(row);
        TreeItem *item = new TreeItem(data, this);
        _childItems.insert(position, item);
    }

    return true;
}

TreeItem *TreeItem::parent()
{
    return _parentItem;
}

bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > _childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete _childItems.takeAt(position);

    return true;
}

QVariant TreeItem::data(int column) const
{
    if (_level == -1) {
        return _itemData.value(column);
    } else
        return column == _level ? _title : QVariant();
}

bool TreeItem::setData(int column, const QVariant &value)
{
    qDebug() << "TreeItem::setData column" << column << "value" << value;

   // if (column < 0 || (column != _level))
    //    return false;

    _title = value.toString();

    return true;
}

int TreeItem::level() const
{
    return _level;
}

void TreeItem::setLevel(const int &level)
{
    _level = level;
}
