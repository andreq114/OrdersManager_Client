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

void TcpCommunication::connectSignals(){


}

void TcpCommunication::readDataFromServer(){
//    QByteArray sock = this->readAll();
//    qDebug()<<"Odebralem cos";
//    QString data = sock;
//    qDebug()<<sock;
//    if(data.at(0) == "$"){
//        emit connectOk();
//    }
//    emit refOrders(data);
//    //ui->statusLabel->setText(nextFortune);
//   // ui->connectButton->setEnabled(true);
}
