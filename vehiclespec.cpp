#include "vehiclespec.h"

#include <QString>
#include <QVector>
#include <QVariant>

#include "vehicle.h"

VehicleSpec::VehicleSpec(const QString title, int parentId)
    : DataItem(QVector<QVariant>({title, parentId}), 0, "vehicle_species", "", Vehicle::findVehicleById(parentId))
{

}
