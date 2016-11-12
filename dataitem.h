#ifndef DATAITEM_H
#define DATAITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

//todo: rename to DataItem
class DataItem
{
public:
    explicit DataItem(const QVector<QVariant> &data, DataItem *parent = nullptr);
    ~DataItem();

    DataItem *child(int number);
    int childCount() const;

    int columnCount() const;

    DataItem *parent();

    bool insertChildren(int position, int count);
    bool removeChildren(int position, int count);

    int childNumber() const;

    bool setData(int column, const QVariant &value);
    QVariant data(int column) const;

    int level() const;
    void setLevel(const int &level);

private:
    int _level;
    QString _title;

    QList<DataItem*> _childItems;
    QVector<QVariant> _itemData;
    DataItem *_parentItem;

    QString _dbTableName;
    QString _dbChildTableName;

    int _dbIndex;
    int _dbParentIndex;
};

#endif // DATAITEM_H
