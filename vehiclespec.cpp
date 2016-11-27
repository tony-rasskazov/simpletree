#include "vehiclespec.h"

#include <QString>
#include <QVector>
#include <QVariant>

#include "vehicle.h"

VehicleSpec::VehicleSpec(int id, const QString title, int parentId)
    : DataItem(title, id, "vehicle_species", "", Vehicle::findVehicleById(parentId))
    , _parentId(parentId)
{

}

QSqlQuery VehicleSpec::prepareInsertSqlQuery() const
{

    QSqlQuery q;
    q.prepare(QString("INSERT INTO %1 (%2, v_id) VALUES (:title, :parent_id)").arg(dbTableName()).arg(dbTableField()));//.arg(title()).arg(_parentId) ;
    q.bindValue(":title", title());
    q.bindValue(":parent_id", _parentId);

    return q;
}
