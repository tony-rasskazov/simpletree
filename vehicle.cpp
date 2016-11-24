#include "vehicle.h"

QHash<int, Vehicle*> Vehicle::s_byId = QHash<int, Vehicle*>();

Vehicle::Vehicle(const QString _title, int id, DataItem *parent)
    : DataItem(QVector<QVariant>({_title, id}), 0, "vehicles", "vehicle_species", parent)
    , _id(id)
{

    s_byId[_id] = this;
}

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

Vehicle *Vehicle::findVehicleById(int id)
{
    return s_byId.value(id, nullptr);
}
