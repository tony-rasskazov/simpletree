
#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QVariant>
#include <QHash>

#include <QModelIndex>

#include <QAbstractItemModel>
#include <QSqlDatabase>

class DataItem;

class DataModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    DataModel(const QStringList &headers, QSqlDatabase &db, QObject *parent = 0);
    ~DataModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

    DataItem *getItem(const QModelIndex &index) const;

    QSqlError addConnection(const QString &driver, const QString &dbName, const QString &host,
                                        const QString &user, const QString &passwd, int port);

    void addConnection();


signals:
    void dataItemChanged(DataItem *item);
    void newDataItem(DataItem *item);

public slots:
    void onDataItemChanged(DataItem *item);
    void onNewDataItem(DataItem *item);

private:
    void setupModelData(DataItem *parent);

    DataItem *_rootItem;

    QSqlDatabase &_db;
    bool _dbOk;
};

#endif // DATAMODEL_H
