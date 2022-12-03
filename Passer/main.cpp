#include "initdialog.h"

#include <QApplication>
#include "windowcreate.h"
#include "sqlitedbmanager.h"
#include <QFile>

int main(int argc, char *argv[])
{
    SqliteDBManager *sqliteIns = SqliteDBManager::getInstance();
    sqliteIns->connectToDataBase();
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("C:\\Users\\1234\\Documents\\Passer\\build-Passer-Desktop_Qt_6_4_0_MinGW_64_bit-Debug\\assets\\winic.ico"));
//    QObject::connect( a, SIGNAL(lastWindowClosed()), a, a.quit);
    InitDialog w(sqliteIns);
    w.show();
//    if (w->choseOption()){
//        delete w;
//        WindowCreate *winCreate = new WindowCreate;
//        winCreate->show();
//    }
//    QFile file("passerdb.sqlite"); file.remove();
    return a.exec();
}
