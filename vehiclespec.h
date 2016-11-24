#ifndef VEHICLESPEC_H
#define VEHICLESPEC_H

#include "dataitem.h"

class Vehicle;

class VehicleSpec : public DataItem
{
public:
    explicit VehicleSpec(const QString title, int parentId);

    QString dbTableField() const override { return QString("model"); }
    QSqlQuery prepareInsertSqlQuery() const override {

        QSqlQuery q;
        q.prepare(QString("INSERT INTO %1 (%2, v_id) VALUES (:title, :parent_id)").arg(dbTableName()).arg(dbTableField()));//.arg(title()).arg(_parentId) ;
        q.bindValue(":title", title());
        q.bindValue(":parent_id", _parentId);
        return q;


//        return QString("INSERT INTO %1 (%2, v_id) VALUES (%3, %4)").arg(dbTableName()).arg(dbTableField()).arg(title()).arg(_parentId) ;
    }
//    QString insertSql() const override { return QString("INSERT INTO %1 (%2, v_id) VALUES (%3, %4)").arg(dbTableName()).arg(dbTableField()).arg(title()).arg(_parentId) ; }

private:
    int _parentId;
};

#endif // VEHICLESPEC_H
