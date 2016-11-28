
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QModelIndex>
#include <QSqlDatabase>
#include <QSqlError>

class DataModel;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    QSqlDatabase& db();

public slots:
    void updateActions();

private slots:
    void insertVehicleSpec();
    void insertVehicle();
    void removeVehicle();
    void on_actionApply_changes_triggered();
    void resizeAllColumnsToContents();

    void on_toolButton_newVehicle_clicked();

    void on_toolButton_newVehicleModel_clicked();

    void refresh();

private:
    DataModel *_model;
    QSqlDatabase _db;
};

#endif // MAINWINDOW_H
