#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

//todo: rename to DataItem
class TreeItem
{
public:
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parent = nullptr);
    ~TreeItem();

    TreeItem *child(int number);
    int childCount() const;

    int columnCount() const;

    TreeItem *parent();

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

    QList<TreeItem*> _childItems;
    QVector<QVariant> _itemData;
    TreeItem *_parentItem;

    QString _dbTableName;
    QString _dbChildTableName;

    int _dbIndex;
    int _dbParentIndex;
};

#endif // TREEITEM_H
