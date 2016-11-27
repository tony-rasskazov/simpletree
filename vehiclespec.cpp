#include "vehiclespec.h"

#include <QString>
#include <QVector>
#include <QVariant>

#include "vehicle.h"

VehicleSpec::VehicleSpec(int id, const QString title, int parentId)
    : DataItem(QVector<QVariant>({title, parentId}), id, "vehicle_species", "", Vehicle::findVehicleById(parentId))
    , _parentId(parentId)
{

}
