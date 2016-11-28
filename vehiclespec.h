#ifndef VEHICLESPEC_H
#define VEHICLESPEC_H

#include "dataitem.h"

class Vehicle;

class VehicleSpec : public DataItem
{
public:
    explicit VehicleSpec(int id, const QString title, QSqlDatabase &db, int parentId);

    QString dbTableField() const override { return QString("model"); }
    QSqlQuery prepareInsertSqlQuery() const override;

private:
    int _parentId;
};

#endif // VEHICLESPEC_H
