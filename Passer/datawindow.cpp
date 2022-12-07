#include "datawindow.h"
#include "ui_datawindow.h"
#include <QPushButton>
#include <iostream>
#include "sqlitedbmanager.h"
#include <QSqlTableModel>
#include <QMessageBox>
#include <QApplication>
#include <QCloseEvent>
#include <QSqlQueryModel>
#include "addentry.h"
#include "editentry.h"
#include "datainfo.h"

//DataWindow::DataWindow(SqliteDBManager *dbIns, Users *currUser, QWidget *parent) :
DataWindow::DataWindow(SqliteDBManager *dbIns, UserPublicData *currUser, QMainWindow *parent) :
//    QMainWindow(parent),
    parentWin(parent),
    ui(new Ui::DataWindow),
    currentUser(currUser),
    db(dbIns),
    sqlModel(nullptr),
    tableSelectedRowDataId(0)
{
    ui->setupUi(this);
    connect(ui->pbQuit, &QPushButton::clicked, this, &QApplication::quit);
    setWindowTitle("Passer @" + currentUser->username + " - data");
    ui->statusbar->showMessage("Logged as: " + currentUser->username);

//    QSqlTableModel *model = new QSqlTableModel(this, this->db->getDB());

//    sqlModel = new QSqlQueryModel(this);
//    model->setFilter("account_id = " + currentUser->id);
//    sqlmodel->setQuery("SELECT " TABLE_DATA_TITLE ", "
//                       TABLE_DATA "." TABLE_DATA_URL ", "
//                       TABLE_DATA "." TABLE_DATA_USERNAME ", "
//                       TABLE_DATA "." TABLE_DATA_PASSWORD ", "
//                       TABLE_DATA "." TABLE_DATA_DESCRIPTION
//                       " FROM " TABLE_DATA " INNER JOIN " TABLE_USERS
//                       " ON (" TABLE_USERS".id = " TABLE_DATA ".account_id) WHERE "
//                       TABLE_DATA ".account_id = " + QString::number(currentUser->id) + ";");
//    sqlModel = getQueryModel();
//    model->setTable(TABLE_DATA);
//    model->select();
//    ui->tableView->setModel(sqlModel);
    updateTableViewModel(ui->tableView);
    ui->tableView->hideColumn(0);
//    ui->tableView->hideColumn(1);

}

DataWindow::~DataWindow()
{
    delete currentUser;
    delete sqlModel;
    delete ui;
}

void DataWindow::on_pbQuit_clicked()
{

}


void DataWindow::on_pbAdd_clicked()
{
    AddEntry *addRow = new AddEntry(db, currentUser, this);
    addRow->show();
    updateTableViewModel(ui->tableView);

}


void DataWindow::on_pbCancel_clicked()
{
//    parentWidget()->show();
    parentWin->show();
    delete this;
}

void DataWindow::closeEvent (QCloseEvent *event)
{
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Passer",
                                                                    tr("Are you sure you want to exit Passer?\n"),
                                                                    QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
        } else {
            event->accept();
            QApplication::quit();
        }
}

QSqlQueryModel* DataWindow::getQueryModel() {
QSqlQueryModel *sqlmodel = new QSqlQueryModel(this);        // possible memory leak
//        QSqlQueryModel *sqlmodel
//    model->setFilter("account_id = " + currentUser->id);
    sqlmodel->setQuery("SELECT "
                   TABLE_DATA ".id, "
                   TABLE_DATA_TITLE ", "
                   TABLE_DATA "." TABLE_DATA_URL ", "
                   TABLE_DATA "." TABLE_DATA_USERNAME ", "
                   TABLE_DATA "." TABLE_DATA_PASSWORD ", "
                   TABLE_DATA "." TABLE_DATA_DESCRIPTION
                   " FROM " TABLE_DATA " INNER JOIN " TABLE_USERS
                   " ON (" TABLE_USERS".id = " TABLE_DATA ".account_id) WHERE "
                   TABLE_DATA ".account_id = " + QString::number(currentUser->id) + ";");
    return sqlmodel;
}

void DataWindow::updateTableViewModel(QTableView *tb) {
    if (sqlModel){
        delete sqlModel;
    }
    sqlModel = getQueryModel();
//    sqlModel->QAbstractItemView::setHeaderData(1, QT::Horizontal, QObject::tr("Title");
//    sqlModel->QAbstractItemView::setHeaderData(2, QT::Horizontal, QObject::tr("URL");
//    sqlModel->QAbstractItemView::setHeaderData(3, QT::Horizontal, QObject::tr("Username");
//    sqlModel->QAbstractItemView::setHeaderData(4, QT::Horizontal, QObject::tr("Password");
//    sqlModel->QAbstractItemView::setHeaderData(5, QT::Horizontal, QObject::tr("Description");
    tb->setModel(sqlModel);
}

void DataWindow::on_pbRefresh_clicked()
{
    updateTableViewModel(ui->tableView);
}


void DataWindow::on_pbDelete_clicked()
{
//    ui->tableView->remo
    db->deleteDataRow(tableSelectedRowDataId);
    updateTableViewModel(ui->tableView);
}




void DataWindow::on_tableView_clicked(const QModelIndex &index)
{
    tableSelectedRowDataId = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toInt();
    selectedRow = index.row();
//    qDebug() << index.row();
//    qDebug() << "Secected id = " + ui->tableView->model()->data(index, 4).toString();
//    qDebug() << "Secected id = " +    ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toString();
}


void DataWindow::on_pbEdit_clicked()
{
    if(ui->tableView->currentIndex().isValid()){
        EditEntry *editWin = new EditEntry(db, getInfoFromDataSelectedRow(ui->tableView), this);
        editWin->show();
    }
//    selectedRow = 0;
//    getInfoFromDataSelectedRow(ui->tableView, selectedRow);
}

//QVariantList DataWindow::getInfoFromDataSelectedRow(QTableView *table, int row){
//    QVariantList lst;
//    QAbstractItemModel *model = table->model();
//    int columns = model->columnCount();
//    for (int i = 0; i < columns; ++i){
//        lst.append(model->index(selectedRow, i).data().toString());
////            qDebug() << lst[i];
//    }
//    return lst;
//}
DataInfo DataWindow::getInfoFromDataSelectedRow(QTableView *table){
//    QVariantList lst;
    QAbstractItemModel *model = table->model();
    DataInfo data(model->index(selectedRow, 0).data().toInt(),
                  model->index(selectedRow, 1).data().toString(),
                  model->index(selectedRow, 2).data().toString(),
                  model->index(selectedRow, 3).data().toString(),
                  model->index(selectedRow, 4).data().toString(),
                  model->index(selectedRow, 5).data().toString());
//    int columns = model->columnCount();
//    for (int i = 0; i < columns; ++i){
//        lst.append(model->index(selectedRow, i).data().toString());
//            qDebug() << lst[i];
//    }
    return data;
}
