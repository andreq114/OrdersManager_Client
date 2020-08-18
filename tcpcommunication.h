#ifndef TCPCOMMUNICATION_H
#define TCPCOMMUNICATION_H

#include <QTcpSocket>
#include <QHostAddress>

class TcpCommunication : public QTcpSocket
{
public:
    explicit TcpCommunication(QObject *parent);
    ~TcpCommunication();
    void connectToServer();
    void changeOrderState(int order,int state);
    void setIpAdress(QString ip);
    void setPortNumber(int port);
    QString getIpAdress();
    int getPortNumber();

private slots:
    void readDataFromServer();
private:
    void connectSignals();


    QString ipAdr = "172.16.108.177";
    unsigned short port = 51268;


};

#endif // TCPCOMMUNICATION_H
