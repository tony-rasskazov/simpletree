#ifndef VEHICLESPEC_H
#define VEHICLESPEC_H

#include "dataitem.h"

class Vehicle;

class VehicleSpec : public DataItem
{
public:
    explicit VehicleSpec(const QString title, int parentId);

    QString dbTableField() const override { return QString("model"); }

};

#endif // VEHICLESPEC_H
