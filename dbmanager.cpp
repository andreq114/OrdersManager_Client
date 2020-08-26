#include "dbmanager.h"


dbmanager::dbmanager(QObject *parent)
{
    startDatabase();
}

void dbmanager::startDatabase(){
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER))
        qDebug()<<"Jest driver";

    db = QSqlDatabase::addDatabase(DRIVER,"database");

    const QString applicationFolder = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
    db.setDatabaseName(applicationFolder+"/databaseclient.sqllite");


    if(!db.open())
        qWarning() << "ERROR: " << db.lastError();
    query = new QSqlQuery(db);


    query->exec("CREATE TABLE IF NOT EXISTS orders (id INTEGER PRIMARY KEY, orderNumber INTEGER, orderState INTEGER,orderTime VARCHAR(10))");

    query->exec("CREATE TABLE IF NOT EXISTS config (id INTEGER PRIMARY KEY, ipAdress VARCHAR(20), port INTEGER)");

    query->exec("CREATE TABLE IF NOT EXISTS next_number (id INTEGER PRIMARY KEY, nextNumber INTEGER)");

    if(!query->isActive())
        qWarning() << "ERROR: " << query->lastError().text();


}

bool dbmanager::addIp_port_toDb(QString ip,int port){

    //Sprawdzamy czy tabela ip/portu jest pusta, wykonywane przy pierwszym uzyciu programu
    query->prepare("SELECT COUNT(*) FROM config");
    if(!query -> exec())
        qWarning() << "ERROR: " << query->lastError().text();
    int rowsInTable = 0;
    if (query->next()) {
        rowsInTable= query->value(0).toInt();
    }
    query->clear();


    //Wkladamy nowy wiersz do tabeli bazy danych z ip i portem
    if(rowsInTable == 0){
        query->prepare("INSERT INTO config(ipAdress,port) VALUES(?,?)");
        query->addBindValue(ip);
        query->addBindValue(port);
        if(!query -> exec())
            qWarning() << "ERROR: " << query->lastError().text();
        query->clear();
    }else{
        //Przy kazdym kolejnym zaktualizowaniu tabeli ip/portu

        query->prepare("UPDATE config SET ipAdress = :ip WHERE id = 1");
        query->bindValue(":ip", ip);
        if(!query -> exec())
            qWarning() << "ERROR: " << query->lastError().text();
        query->clear();


        query->prepare("UPDATE config SET port = :por WHERE id = 1");
        query->bindValue(":por", port);
        if(!query -> exec())
            qWarning() << "ERROR: " << query->lastError().text();
        query->clear();

    }
    return true;
}


QString dbmanager::readIp_fromDb(){
    query->clear();
    if(!query -> exec("SELECT ipAdress FROM config WHERE id = 1"))
        qWarning() << "ERROR: " << query->lastError().text();

    query->first();
    qDebug()<<"IP z bazy "<<query->value(0).toString();

    return query->value(0).toString();
};

int dbmanager::readPort_fromDb(){
    query->clear();
    if(!query -> exec("SELECT port FROM config WHERE id = 1"))
        qWarning() << "ERROR: " << query->lastError().text();

    query->first();
    qDebug()<<"port z bazy "<<query->value(0).toString();

    return query->value(0).toInt();
};


