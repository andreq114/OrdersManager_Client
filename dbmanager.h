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
    void startDatabase();

    QString readIp_fromDb();
    int readPort_fromDb();
    bool addIp_port_toDb(QString ip,int port);


private:
    QString dbPath = "";
    QSqlQuery *query;
    QSqlDatabase db;
};

#endif // DBMANAGER_H
