
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QModelIndex>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void updateActions();

private slots:
    void insertVehicleModel();
    void insertVehicle();
    void removeVehicle();
    void on_actionApply_changes_triggered();
};

#endif // MAINWINDOW_H
