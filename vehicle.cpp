#include "vehicle.h"

QHash<int, Vehicle*> Vehicle::s_byId = QHash<int, Vehicle*>();

Vehicle::Vehicle(const QString _title, int id, DataItem *parent)
    : DataItem(_title, id, "vehicles", "vehicle_species", parent)
{
    s_byId[_id] = this;
}

Vehicle::Vehicle(const QString _title, DataItem *parent)
    : DataItem(_title, -1, "vehicles", "vehicle_species", parent)
{

}

Vehicle *Vehicle::findVehicleById(int id)
{
    return s_byId.value(id, nullptr);
}

QSqlQuery Vehicle::prepareInsertSqlQuery() const {
    QSqlQuery q;

    q.prepare(QString("INSERT INTO %1 (%2) VALUES (?) RETURNING id").arg(dbTableName()).arg(dbTableField()) );
    q.bindValue(0, title());

    return q;
}
