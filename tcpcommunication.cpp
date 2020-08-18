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

void TcpCommunication::changeOrderState(int order,int state)//Zmiana stanu danego zamówienia
{
    qDebug()<<"Socket dostał";
    QString tekst = "ORDER/" + QString::number(order)+"/STATE#"+QString::number(state)+"#";    //Dane o numerze zamowienia i stanie wysylane do serwera
    QByteArray block;
    block.append(tekst);
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
    connect(this, &QIODevice::readyRead, this, &TcpCommunication::readDataFromServer);
}

void TcpCommunication::readDataFromServer(){
    QByteArray sock = this->readLine();
    qDebug()<<"Odebralem cos";
    QString data = sock;
    qDebug()<<sock;
    if(sock == "ACCEPT"){
        //QMessageBox::information(this,"Akceptacja","Połączono");
    }else{
        //QMessageBox::information(this,"Ramka",nextFortune);
    }
    //ui->statusLabel->setText(nextFortune);
   // ui->connectButton->setEnabled(true);
}
