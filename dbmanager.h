#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

#include <QDebug>

class dbmanager
{
public:
    dbmanager(QObject *parent);
    ~dbmanager();
    void startDatabase();                                           //Początkowa konfiguracja bazy danych

    QString readIp_fromDb();                                        //Metoda do odczytania IP z bazy danych
    int readPort_fromDb();                                          //Metoda do odczytania portu z bazy danych
    bool addIp_port_toDb(QString ip,int port);                      //Metoda do dodania ip/portu do bazy danych


private:
    QSqlQuery *query;
    QSqlDatabase db;
};

#endif // DBMANAGER_H
