#include <QtWidgets>

#include <QSqlQuery>

#include <QDebug>

#include <QSqlResult>
#include <QSqlRecord>

#include "dataitem.h"
#include "treemodel.h"

#include "vehicle.h"
#include "vehiclespec.h"

TreeModel::TreeModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    _rootItem = new DataItem(rootData, -1, "[root]", "vehicles");

    _db = QSqlDatabase::addDatabase("QPSQL");
    _db.setHostName("localhost");
    _db.setDatabaseName("vehicles_db");
    _db.setUserName("postgres");
    _db.setPassword("1");
    _dbOk = _db.open();

    qDebug() << (_dbOk ? "TreeModel::TreeModel db connected" : "TreeModel::TreeModel db NOT connected!");

    connect(this, &TreeModel::dataItemChanged, this, &TreeModel::onDataItemChanged);
    connect(this, &TreeModel::newDataItem, this, &TreeModel::onNewDataItem);

    setupModelData(_rootItem);
}

TreeModel::~TreeModel()
{
    delete _rootItem;
}

int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return 2;//_rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    DataItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    DataItem *item = static_cast<DataItem*>(index.internalPointer());
    if (item && (item->level() == index.column() )) {
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    } else {
        return QAbstractItemModel::flags(index) ^ Qt::ItemIsSelectable;
    }
}

DataItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        DataItem *item = static_cast<DataItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return _rootItem;
}

void TreeModel::onDataItemChanged(DataItem *item)
{
    qDebug() << "TreeModel::dataItemChanged" << item->toString();
    qDebug() << QString("UPDATE %1 set %4 = '%2' WHERE id = %3;").arg(item->dbTableName()).arg(item->title()).arg(item->id()).arg(item->dbTableField()) ;
}

void TreeModel::onNewDataItem(DataItem *item)
{
    qDebug() << "TreeModel::onNewDataItem" << item->toString();

}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    DataItem *parentItem = getItem(parent);

    DataItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}


bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    DataItem *parentItem = getItem(parent);
    bool success = true;

    beginInsertRows(parent, position, position + rows - 1);
    for (int i = 0; i < rows; i++) {
        //todo: refactor it
        DataItem *newItem = parentItem->insertChild(position, -1, QString("[new %1]").arg(i), parentItem->dbChildTableName(), "vehicle_species");//!!!
        emit newDataItem(newItem);
    }
    endInsertRows();

    return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DataItem *childItem = getItem(index);
    DataItem *parentItem = childItem->parent();

    if (parentItem == _rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    DataItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    DataItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    DataItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result) {
        emit dataChanged(index, index);
        emit dataItemChanged(item);
    }

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = _rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void TreeModel::setupModelData(DataItem *parent)
{
    QSqlQuery vehicles_q = _db.exec("select * from vehicles;");
    QSqlQuery species_q = _db.exec("select * from vehicle_species;");

    while (vehicles_q.next()) {
        DataItem *i = _rootItem->insertChild(

                    new Vehicle(vehicles_q.record().value(1).toString(), vehicles_q.record().value(0).toInt(), _rootItem)
                    );
//        emit newDataItem(i);
    }


    while (species_q.next()) {
        Vehicle *v = Vehicle::findVehicleById(species_q.record().value(2).toInt());

        int v_id = species_q.record().value(2).toInt();
        QString v_title = species_q.record().value(1).toString();

        DataItem *i = (v != nullptr) ? v->insertChild(
                                           new VehicleSpec(v_title, v_id)
                                       )
                                     : nullptr;
//        emit newDataItem(i);
    }

}
