#include "vehicle.h"

QHash<int, Vehicle*> Vehicle::s_byId = QHash<int, Vehicle*>();

Vehicle::Vehicle(const QString _title, int id, DataItem *parent)
    : DataItem(QVector<QVariant>({_title, id}), id, "vehicles", "vehicle_species", parent)
{

    s_byId[_id] = this;
}

Vehicle::Vehicle(const QString _title, DataItem *parent)
    : DataItem(QVector<QVariant>({_title, -1}), -1, "vehicles", "vehicle_species", parent)
{

}

/*
int Vehicle::id() const
{
    return _id;
}

void Vehicle::setId(int id)
{
    s_byId[_id] = nullptr;
    _id = id;
    s_byId[_id] = this;
}
*/

Vehicle *Vehicle::findVehicleById(int id)
{
    return s_byId.value(id, nullptr);
}
