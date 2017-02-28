#include "vehicle.h"

#include <QDebug>

QHash<int, Vehicle*> Vehicle::s_byId = QHash<int, Vehicle*>();

Vehicle::Vehicle(const QString _title, int id, QSqlDatabase &db, DataItem *parent)
    : DataItem(_title, id, db, "vehicles", "vehicle_species", parent)
{
    s_byId[_id] = this;
}

Vehicle *Vehicle::findVehicleById(int id)
{
    return s_byId.value(id, 0);
}

QSqlQuery Vehicle::prepareInsertSqlQuery() const {
    QSqlQuery q(_db);

    q.prepare(QString("INSERT INTO %1 (%2) VALUES (?)").arg(dbTableName()).arg(dbTableField()) );
    q.bindValue(0, title());

    return q;
}

void Vehicle::setId(int id)
{
    _id = id;
    s_byId[_id] = this;
}
