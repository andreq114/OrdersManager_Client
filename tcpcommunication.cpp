#include "tcpcommunication.h"

TcpCommunication::TcpCommunication(QObject *parent) : QTcpSocket(parent){

}

TcpCommunication::~TcpCommunication(){

}

void TcpCommunication::connectToServer(){
    this->abort();
    this->connectToHost(QHostAddress(ipAdr),
                             port);
}

void TcpCommunication::disconnectFromServer(){
    disconnectFromHost();
    if(this->state() == QAbstractSocket::UnconnectedState){
        emit disconnect();
    }
}


//METODY DO WYSYLANIA ZADAN DO SERWERA
void TcpCommunication::addNewOrder(){
    qDebug()<<"Dodaje nowe zamówienie";
    QString tekst = "ADD";
    QByteArray block;
    block.append(tekst);
    waitForBytesWritten(3000);
    this->write(block);

    qDebug()<<"Wyslalem do serwera";
}

void TcpCommunication::changeOrderState(int order,int state)//Zmiana stanu danego zamówienia
{
    qDebug()<<"Socket dostał";
    qDebug()<<"Zamowienie numer "<<order;
    QString tekst = "ORDER/" + QString::number(order)+"/STATE#"+QString::number(state)+"#";    //Dane o numerze zamowienia i stanie wysylane do serwera
    QByteArray block;
    block.append(tekst);
    waitForBytesWritten(3000);
    this->write(block);
    qDebug()<<tekst;
    qDebug()<<"Wyslalem do serwera";

}

void TcpCommunication::useSoundSignal(){
    qDebug()<<"Wysylam komende do uzycia sygnalu dzwiekowego";
    QString tekst = "SOUND";
    QByteArray block;
    block.append(tekst);
    waitForBytesWritten(3000);
    this->write(block);
    qDebug()<<tekst;
    qDebug()<<"Wyslalem do serwera";
}

void TcpCommunication::recoverLastOrder(){
    qDebug()<<"Wysylam komende o odzyskanie ostatnio usunietego zamowienia";
    QString tekst = "RECOVER";
    QByteArray block;
    block.append(tekst);
    waitForBytesWritten(3000);
    this->write(block);
    qDebug()<<tekst;
    qDebug()<<"Wyslalem do serwera";
}

void TcpCommunication::setFullScreen(){
    qDebug()<<"Wysylam o wlaczenie/wylaczenie full screena";
    QString tekst = "FULLSCREEN";
    QByteArray block;
    block.append(tekst);
    waitForBytesWritten(3000);
    this->write(block);
    qDebug()<<tekst;
    qDebug()<<"Wyslalem do serwera";
}


//GETTERY i SETTERY
void TcpCommunication::setIpAdress(QString ip){
    ipAdr = ip;
}
void TcpCommunication::setPortNumber(int port){
    this->port = QString::number(port).toUShort();
}
QString TcpCommunication::getIpAdress(){
    return ipAdr;
}
int TcpCommunication::getPortNumber(){
    return this->port;
}

