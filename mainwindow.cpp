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

    _model = new DataModel(headers);

    view->setModel(_model);

    connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::updateActions);

    connect(actionsMenu, &QMenu::aboutToShow, this, &MainWindow::updateActions);

    connect(insertVehicleAction, &QAction::triggered, this, &MainWindow::insertVehicle);
    connect(removeVehicleAction, &QAction::triggered, this, &MainWindow::removeVehicle);
    connect(addVehicleModelAction, &QAction::triggered, this, &MainWindow::insertVehicleSpec);

    updateActions();
    resizeAllColumnsToContents();

    view->selectionModel()->select(_model->index(0, 0), QItemSelectionModel::ClearAndSelect);
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

QSqlError MainWindow::addConnection(const QString &driver, const QString &dbName, const QString &host,
                                    const QString &user, const QString &passwd, int port)
{
    static int cCount = 0;

    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver, QString("Browser%1").arg(++cCount));
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    if (!db.open(user, passwd)) {
        err = db.lastError();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(QString("Browser%1").arg(cCount));
    }

    //connectionWidget->refresh();

    return err;
}

void MainWindow::addConnection()
{
    QSqlConnectionDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    if (dialog.useInMemoryDatabase()) {
        QSqlDatabase::database("in_mem_db", false).close();
        QSqlDatabase::removeDatabase("in_mem_db");
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "in_mem_db");
        db.setDatabaseName(":memory:");
        if (!db.open())
            QMessageBox::warning(this, tr("Unable to open database"), tr("An error occurred while "
                                                                         "opening the connection: ") + db.lastError().text());
        QSqlQuery q("", db);
        q.exec("drop table Movies");
        q.exec("drop table Names");
        q.exec("create table Movies (id integer primary key, Title varchar, Director varchar, Rating number)");
        q.exec("insert into Movies values (0, 'Metropolis', 'Fritz Lang', '8.4')");
        q.exec("insert into Movies values (1, 'Nosferatu, eine Symphonie des Grauens', 'F.W. Murnau', '8.1')");
        q.exec("insert into Movies values (2, 'Bis ans Ende der Welt', 'Wim Wenders', '6.5')");
        q.exec("insert into Movies values (3, 'Hardware', 'Richard Stanley', '5.2')");
        q.exec("insert into Movies values (4, 'Mitchell', 'Andrew V. McLaglen', '2.1')");
        q.exec("create table Names (id integer primary key, Firstname varchar, Lastname varchar, City varchar)");
        q.exec("insert into Names values (0, 'Sala', 'Palmer', 'Morristown')");
        q.exec("insert into Names values (1, 'Christopher', 'Walker', 'Morristown')");
        q.exec("insert into Names values (2, 'Donald', 'Duck', 'Andeby')");
        q.exec("insert into Names values (3, 'Buck', 'Rogers', 'Paris')");
        q.exec("insert into Names values (4, 'Sherlock', 'Holmes', 'London')");


        //connectionWidget->refresh();

    } else {
        QSqlError err = addConnection(dialog.driverName(), dialog.databaseName(), dialog.hostName(),
                           dialog.userName(), dialog.password(), dialog.port());

        qDebug() << err;
        if (err.type() != QSqlError::NoError)
            QMessageBox::warning(this, tr("Unable to open database"), tr("An error occurred while "
                                       "opening the connection: ") + err.text());
    }
}


void MainWindow::on_actionConnect_DB_triggered()
{
    addConnection();
}
