#include "mainwindow.h"


#include <QFile>
#include <QDebug>

#include "datamodel.h"
#include "dataitem.h"
#include "qsqlconnectiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    QStringList headers;
    headers << tr("Vehicle") << tr("Model") << tr("Id");

    _model = new DataModel(headers, _db);
    view->setModel(_model);

//    connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(exitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

//    connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::updateActions);
    connect(view->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateActions()));

//    connect(actionsMenu, &QMenu::aboutToShow, this, &MainWindow::updateActions);
    connect(actionsMenu, SIGNAL(aboutToShow()), this, SLOT(updateActions()));

//    connect(insertVehicleAction, &QAction::triggered, this, &MainWindow::insertVehicle);
    connect(insertVehicleAction, SIGNAL(triggered(bool)), this, SLOT(insertVehicle()));

    connect(removeVehicleAction, SIGNAL(triggered(bool)), this, SLOT(removeVehicle()));

    connect(addVehicleModelAction, SIGNAL(triggered(bool)), this, SLOT(insertVehicle()));

    updateActions();
    resizeAllColumnsToContents();

    view->selectionModel()->select(_model->index(0, 0), QItemSelectionModel::ClearAndSelect);
    //addConnection();
}

void MainWindow::insertVehicleSpec()
{
    QModelIndex index = view->selectionModel()->currentIndex();

    if (!index.isValid()) {
        return;
    }

    DataItem *i = _model->getItem(index);

    QModelIndex child;
    if (i && i->level() == 0) {
        if (!_model->insertRow(0, index))
            return;

        child = _model->index(0, 0, index);
    } else if (i && i->level() == 1) {
        if (!_model->insertRow(0, index.parent()))
            return;

        child = _model->index(0, 0, index.parent());
    }
     if (child.isValid())
       _model->setData(child, QVariant(tr("[new vehicle model]")), Qt::EditRole);
     view->selectionModel()->setCurrentIndex(child, QItemSelectionModel::ClearAndSelect);
//    }
    updateActions();
    resizeAllColumnsToContents();
}

void MainWindow::insertVehicle()
{
    QAbstractItemModel *model = view->model();

    if (!model->insertRow(0))
        return;

    QModelIndex child = model->index(0, 0);
    model->setData(child, QVariant(tr("[new vehicle]")), Qt::EditRole);

    view->selectionModel()->setCurrentIndex(child, QItemSelectionModel::ClearAndSelect);

    updateActions();
    resizeAllColumnsToContents();
}


void MainWindow::removeVehicle()
{
    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

void MainWindow::updateActions()
{
    bool hasSelection = !view->selectionModel()->selection().isEmpty();
    removeVehicleAction->setEnabled(hasSelection);

    bool hasCurrent = view->selectionModel()->currentIndex().isValid();

    if (hasCurrent) {
        view->closePersistentEditor(view->selectionModel()->currentIndex());

        int row = view->selectionModel()->currentIndex().row();
        int column = view->selectionModel()->currentIndex().column();
        if (view->selectionModel()->currentIndex().parent().isValid())
            statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
        else
            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
    }
}

void MainWindow::on_actionApply_changes_triggered()
{
    resizeAllColumnsToContents();
}

void MainWindow::resizeAllColumnsToContents()
{
    for (int column = 0; column < view->model()->columnCount(); ++column)
        view->resizeColumnToContents(column);
}

void MainWindow::on_toolButton_newVehicle_clicked()
{
    insertVehicle();
}

void MainWindow::on_toolButton_newVehicleModel_clicked()
{
    insertVehicleSpec();
}

void MainWindow::refresh()
{
    view->setModel(0);
    delete _model;

    QStringList headers;
    headers << tr("Vehicle") << tr("Model") << tr("Id");

    _model = new DataModel(headers, _db);
    view->setModel(_model);

}

QSqlDatabase &MainWindow::db()
{
    return _db;
}

