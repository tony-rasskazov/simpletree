#ifndef VEHICLESPEC_H
#define VEHICLESPEC_H

#include "dataitem.h"

class Vehicle;

class VehicleSpec : public DataItem
{
public:
    explicit VehicleSpec(const QString title, int parentId);
    explicit VehicleSpec(const QString title, const Vehicle *vehicle, DataItem *parent = nullptr);
};

#endif // VEHICLESPEC_H
