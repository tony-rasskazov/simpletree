
#include <QSqlQuery>

#include <QDebug>

#include <QSqlResult>
#include <QSqlRecord>

#include "dataitem.h"
#include "datamodel.h"

#include "vehicle.h"
#include "vehiclespec.h"
#include "mainwindow.h"

#include "qsqlconnectiondialog.h"

DataModel::DataModel(const QStringList &headers, QSqlDatabase &db, QObject *parent)
    : QAbstractItemModel(parent)
    , _db(db)
{

    _rootItem = new DataItem("[root]", -1, _db, "[root]", "[vehicles]");

    /*
    _db = QSqlDatabase::addDatabase("QPSQL");
    _db.setHostName("localhost");
    _db.setDatabaseName("vehicles_db");
    _db.setUserName("postgres");
    _db.setPassword("1");

    _dbOk = _db.open();
    qDebug() << (_dbOk ? "TreeModel::TreeModel db connected" : "TreeModel::TreeModel db NOT connected!");
    */

    connect(this, SIGNAL(dataItemChanged(DataItem*)), this, SLOT(onDataItemChanged(DataItem*)));
    connect(this, SIGNAL(newDataItem(DataItem*)), this, SLOT(onNewDataItem(DataItem*)));

    addConnection();

    setupModelData(_rootItem);
}

DataModel::~DataModel()
{
    delete _rootItem;
}

int DataModel::columnCount(const QModelIndex & /* parent */) const
{
    return 2;//_rootItem->columnCount();
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
    return QAbstractItemModel::flags(index);
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

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
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
    bool success = false;

    beginInsertRows(parent, position, position + rows - 1);
    for (int i = 0; i < rows; i++) {
        DataItem *newItem = 0;

        if (parentItem->level() == -1) {
            newItem = _rootItem->insertChild(new Vehicle(QString("[new %1!]").arg(i), -1, _db, _rootItem));//!!!
        } else if (parentItem->level() == 0) {
            newItem = parentItem->insertChild(new VehicleSpec(-1, QString("[new spec %1]").arg(i), _db, parentItem->id()));//!!!
        }

        if (newItem) {
            success = true;
            emit newDataItem(newItem);
        }
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
    if (!_db.isOpen()) _db.open();
    QSqlQuery vehicles_q = _db.exec("select * from vehicles;");
    QSqlQuery species_q = _db.exec("select * from vehicle_species;");


    while (vehicles_q.next()) {
        DataItem *i = _rootItem->insertChild(
                    new Vehicle(vehicles_q.record().value(1).toString(), vehicles_q.record().value(0).toInt(), _db, _rootItem)
                    );
    }

    while (species_q.next()) {
        Vehicle *v = Vehicle::findVehicleById(species_q.record().value(2).toInt());

        int i_id = species_q.record().value(0).toInt();
        QString v_title = species_q.record().value(1).toString();
        int v_id = species_q.record().value(2).toInt();

        DataItem *i = (v != 0) ? v->insertChild(
                                           new VehicleSpec(i_id, v_title, _db, v_id)
                                       )
                                     : 0;
    }

}

QSqlError DataModel::addConnection(const QString &driver, const QString &dbName, const QString &host,
                                    const QString &user, const QString &passwd, int port)
{
    static int cCount = 0;

    QSqlError err;
    _db = QSqlDatabase::addDatabase(driver, QString("Browser%1").arg(++cCount));
    _db.setDatabaseName(dbName);
    _db.setHostName(host);
    _db.setPort(port);
    if (!_db.open(user, passwd)) {
        err = _db.lastError();
        _db = QSqlDatabase();
        QSqlDatabase::removeDatabase(QString("Browser%1").arg(cCount));
    }

    //connectionWidget->refresh();
    //refresh();

    return err;
}

void DataModel::addConnection()
{
    QSqlConnectionDialog dialog(0);
    if (dialog.exec() != QDialog::Accepted)
        return;

    if (dialog.useInMemoryDatabase()) {
        QSqlDatabase::database("in_mem_db", false).close();
        QSqlDatabase::removeDatabase("in_mem_db");
        _db = QSqlDatabase::addDatabase("QSQLITE", "in_mem_db");
        _db.setDatabaseName(":memory:");
        if (!_db.open())
            qDebug() << tr("Unable to open database") << tr("An error occurred while opening the connection: ") + _db.lastError().text();
        QSqlQuery q("", _db);
        q.exec("drop table vehicles");
        q.exec("drop table vehicle_species");
        q.exec("create table vehicles (id integer primary key autoincrement, title varchar)");
        q.exec("insert into vehicles values (0, 'Metropolis')");
        q.exec("insert into vehicles values (1, 'Nosferatu, eine Symphonie des Grauens')");
        q.exec("insert into vehicles values (2, 'Bis ans Ende der Welt')");
        q.exec("insert into vehicles values (3, 'Hardware')");
        q.exec("insert into vehicles values (4, 'Mitchell')");
        q.exec("create table vehicle_species (id integer primary key autoincrement, model varchar, v_id integer)");
        q.exec("insert into vehicle_species values (0, 'Sala', 'Palmer', 0)");
        q.exec("insert into vehicle_species values (1, 'Christopher', 0)");
        q.exec("insert into vehicle_species values (2, 'Donald', 2)");
        q.exec("insert into vehicle_species values (3, 'Buck', 3)");
        q.exec("insert into vehicle_species values (4, 'Sherlock', 3)");
    } else {
        QSqlError err = addConnection(dialog.driverName(), dialog.databaseName(), dialog.hostName(),
                           dialog.userName(), dialog.password(), dialog.port());

        qDebug() << err;
        if (err.type() != QSqlError::NoError)
            qDebug() << tr("Unable to open database") << tr("An error occurred while "
                                       "opening the connection: ") + err.text();
    }
    //refresh();
}
