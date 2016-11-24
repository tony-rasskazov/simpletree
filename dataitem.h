#ifndef DATAITEM_H
#define DATAITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

class DataItem
{
public:
    explicit DataItem(const QVector<QVariant> &data, int id, const QString &dbTableName, const QString &dbChildTableName, DataItem *parent = nullptr);

    //explicit DataItem(const QVector<QVariant> &data, int dbIndex, const QString &dbTableName, const QString &dbChildTableName, DataItem *parent = nullptr);

    ~DataItem();

    DataItem *child(int number);
    int childCount() const;

    int columnCount() const;

    DataItem *parent();

    DataItem* insertChild(DataItem* child, int position = 0, int id = -1);
    DataItem* insertChild(int position, int id, const QString &title, const QString &itemChildTableName, const QString &itemChildsTableName);
    bool removeChildren(int position, int count);

    DataItem *getChildById(int id);

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

    QString title() const;
    void setTitle(const QString &title);


    QString toString() const;

private:
    int _level;
    QString _title;

    QList<DataItem*> _childItems;
    QHash<int, DataItem*> _childItemsById;

    QVector<QVariant> _itemData;
    DataItem *_parentItem;

    QString _dbTableName;
    QString _dbChildTableName;

    int _id;
    int _dbParentIndex;

    bool _changed;
};

#endif // DATAITEM_H
