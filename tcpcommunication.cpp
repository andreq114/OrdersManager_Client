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

void TcpCommunication::setFullScreen(){
    QString tekst = "FULLSCREEN";
    QByteArray block;
    block.append(tekst);
    waitForBytesWritten(3000);
    this->write(block);
}



void TcpCommunication::addNewOrder(){

    QString tekst = "ADD";
    QByteArray block;
    block.append(tekst);
    waitForBytesWritten(3000);
    this->write(block);
}

void TcpCommunication::changeOrderState(int order,int state)
{
    QString tekst = "ORDER/" + QString::number(order)+"/STATE#"+QString::number(state)+"#";    //Dane o numerze zamowienia i stanie wysylane do serwera
    QByteArray block;
    block.append(tekst);
    waitForBytesWritten(3000);
    this->write(block);

}

void TcpCommunication::useSoundSignal(){
    QString tekst = "SOUND";
    QByteArray block;
    block.append(tekst);
    waitForBytesWritten(3000);
    this->write(block);
}

void TcpCommunication::recoverLastOrder(){
    QString tekst = "RECOVER";
    QByteArray block;
    block.append(tekst);
    waitForBytesWritten(3000);
    this->write(block);
}

void TcpCommunication::resetOrders(){
    QString tekst = "RESET";
    QByteArray block;
    block.append(tekst);
    waitForBytesWritten(3000);
    this->write(block);
}



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

