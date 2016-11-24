#include "vehiclespec.h"

#include <QString>
#include <QVector>
#include <QVariant>

#include "vehicle.h"

VehicleSpec::VehicleSpec(const QString title, const Vehicle *vehicle, DataItem *parent)
    : DataItem(QVector<QVariant>({title, vehicle ? vehicle->title() : "unk"}), 0, "vehicle_species", "", parent)

{

}


VehicleSpec::VehicleSpec(const QString title, int parentId)
    : DataItem(QVector<QVariant>({title, parentId}), 0, "vehicles", "", Vehicle::findVehicleById(parentId))
{

}
