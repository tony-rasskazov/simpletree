#ifndef DATAITEM_H
#define DATAITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

//todo: rename to DataItem
class DataItem
{
public:
    explicit DataItem(const QVector<QVariant> &data, const QString &dbTableName, const QString &dbChildTableName, DataItem *parent = nullptr);

    explicit DataItem(const QVector<QVariant> &data, int dbIndex, const QString &dbTableName, const QString &dbChildTableName, DataItem *parent = nullptr);

    ~DataItem();

    DataItem *child(int number);
    int childCount() const;

    int columnCount() const;

    DataItem *parent();

    bool insertChild(int position, const QString &title, const QString &itemChildTableName, const QString &itemChildsTableName);
    bool removeChildren(int position, int count);

    int childNumber() const;

    bool setData(int column, const QVariant &value);
    QVariant data(int column) const;

    int level() const;
    void setLevel(const int &level);

    QString dbChildTableName() const;
    void setDbChildTableName(const QString &dbChildTableName);

    QString dbTableName() const;
    void setDbTableName(const QString &dbTableName);

    int dbIndex() const;
    void setDbIndex(int dbIndex);

    int dbParentIndex() const;
    void setDbParentIndex(int dbParentIndex);

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

    bool _changed;
};

#endif // DATAITEM_H
