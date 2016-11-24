#ifndef VEHICLE_H
#define VEHICLE_H

#include "dataitem.h"

#include <QHash>

class Vehicle : public DataItem
{
public:
    explicit Vehicle(const QString _title, int id, DataItem *parent);

    int id() const;
    void setId(int id);

    static Vehicle *findVehicleById(int id);

private:
    int _id;
    static QHash<int, Vehicle*> s_byId;
};

#endif // VEHICLE_H
