#ifndef DATAITEM_H
#define DATAITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

#include <QSqlQuery>

class DataItem
{
public:
    explicit DataItem(const QString &title, int id, const QString &dbTableName, const QString &dbChildTableName, DataItem *parent = nullptr);

    //explicit DataItem(const QVector<QVariant> &data, int dbIndex, const QString &dbTableName, const QString &dbChildTableName, DataItem *parent = nullptr);

    virtual ~DataItem();

    DataItem *child(int number);
    int childCount() const;

    DataItem *parent();

    DataItem* insertChild(DataItem* child, int position = 0, int id = -1);
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

    int id() const;
    virtual void setId(int id);

    QString title() const;
    void setTitle(const QString &title);

    QString toString() const;

    virtual QString dbTableField() const { return QString("[undef]"); }

    virtual QSqlQuery prepareInsertSqlQuery() const { return QSqlQuery(); }
    QSqlQuery prepareDeleteSqlQuery() const;
    QSqlQuery prepareUpdateSqlQuery() const;


protected:
    int _id;

private:
    int _level;
    QString _title;

    QList<DataItem*> _childItems;
    QHash<int, DataItem*> _childItemsById;

    DataItem *_parentItem;

    QString _dbTableName;
    QString _dbChildTableName;

    int _dbParentIndex;

    bool _changed;
};

#endif // DATAITEM_H
