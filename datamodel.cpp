#include <QtWidgets>

#include <QSqlQuery>

#include <QDebug>

#include <QSqlResult>
#include <QSqlRecord>

#include "dataitem.h"
#include "datamodel.h"

#include "vehicle.h"
#include "vehiclespec.h"

DataModel::DataModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{

    _rootItem = new DataItem("[root]", -1, "[root]", "[vehicles]");

    _db = QSqlDatabase::addDatabase("QPSQL");
    _db.setHostName("localhost");
    _db.setDatabaseName("vehicles_db");
    _db.setUserName("postgres");
    _db.setPassword("1");
    _dbOk = _db.open();

    qDebug() << (_dbOk ? "TreeModel::TreeModel db connected" : "TreeModel::TreeModel db NOT connected!");

    connect(this, &DataModel::dataItemChanged, this, &DataModel::onDataItemChanged);
    connect(this, &DataModel::newDataItem, this, &DataModel::onNewDataItem);

    setupModelData(_rootItem);
}

DataModel::~DataModel()
{
    delete _rootItem;
}

int DataModel::columnCount(const QModelIndex & /* parent */) const
{
    return 1;//_rootItem->columnCount();
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    DataItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    DataItem *item = static_cast<DataItem*>(index.internalPointer());
    if (item) {
        if (index.column() == 0)
            return Qt::ItemIsSelectable | Qt::ItemIsEditable | QAbstractItemModel::flags(index) ;
        else
            return Qt::ItemIsSelectable | QAbstractItemModel::flags(index) ;
    }
}

DataItem *DataModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        DataItem *item = static_cast<DataItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return _rootItem;
}

void DataModel::onDataItemChanged(DataItem *item)
{
    QSqlQuery q = item->prepareUpdateSqlQuery();
    q.exec();
}

void DataModel::onNewDataItem(DataItem *item)
{
    QSqlQuery q = item->prepareInsertSqlQuery();
    q.exec();

    item->setId(q.lastInsertId().toInt());
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _rootItem->data(section);

    return QVariant();
}

QModelIndex DataModel::index(int row, int column, const QModelIndex &parent) const
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


bool DataModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    DataItem *parentItem = getItem(parent);
    bool success = true;

    beginInsertRows(parent, position, position + rows - 1);
    for (int i = 0; i < rows; i++) {
        DataItem *newItem = nullptr;
        if (parentItem->level() == -1) {
            newItem = parentItem->insertChild(new Vehicle(QString("[new %1]").arg(i), -1, _rootItem));//!!!
        } else if (parentItem->level() == 0) {
            newItem = parentItem->insertChild(new VehicleSpec(-1, QString("[new spec %1]").arg(i), parentItem->id()));//!!!
        }
        if (newItem) emit newDataItem(newItem);
    }
    endInsertRows();

    return success;
}

QModelIndex DataModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DataItem *childItem = getItem(index);
    DataItem *parentItem = childItem->parent();

    if (parentItem == _rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool DataModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    DataItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int DataModel::rowCount(const QModelIndex &parent) const
{
    DataItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

bool DataModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = _rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void DataModel::setupModelData(DataItem *parent)
{
    QSqlQuery vehicles_q = _db.exec("select * from vehicles;");
    QSqlQuery species_q = _db.exec("select * from vehicle_species;");


    while (vehicles_q.next()) {
        DataItem *i = _rootItem->insertChild(
                    new Vehicle(vehicles_q.record().value(1).toString(), vehicles_q.record().value(0).toInt(), _rootItem)
                    );
    }

    while (species_q.next()) {
        Vehicle *v = Vehicle::findVehicleById(species_q.record().value(2).toInt());

        int i_id = species_q.record().value(0).toInt();
        QString v_title = species_q.record().value(1).toString();
        int v_id = species_q.record().value(2).toInt();

        DataItem *i = (v != nullptr) ? v->insertChild(
                                           new VehicleSpec(i_id, v_title, v_id)
                                       )
                                     : nullptr;
    }

}
