#ifndef VEHICLE_H
#define VEHICLE_H

#include "dataitem.h"

#include <QHash>

class Vehicle : public DataItem
{
public:
    explicit Vehicle(const QString _title, int id, DataItem *parent);
    explicit Vehicle(const QString _title, DataItem *parent);

    /*
    int id() const;
    void setId(int id);
    */

    static Vehicle *findVehicleById(int id);

    QString dbTableField() const override { return QString("title"); }

    QSqlQuery prepareInsertSqlQuery() const override {
        QSqlQuery q;

        q.prepare(QString("INSERT INTO %1 (%2) VALUES (?) RETURNING id").arg(dbTableName()).arg(dbTableField()) );
        q.bindValue(0, title());

        return q;


        //q.prepare(QString("INSERT INTO %1(%2) VALUES(?)").arg(item->dbTableName()).arg(item->dbTableField()));
        //q.bindValue(0, item->title());

        //q.prepare(QString("INSERT INTO %1 (%2) VALUES (:val)").arg(dbTableName()).arg(dbTableField()) );
        //q.bindValue(":val", title());

    }

private:
    static QHash<int, Vehicle*> s_byId;
};

#endif // VEHICLE_H
