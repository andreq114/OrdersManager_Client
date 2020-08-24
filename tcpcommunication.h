#ifndef TCPCOMMUNICATION_H
#define TCPCOMMUNICATION_H

#include <QTcpSocket>
#include <QHostAddress>

class TcpCommunication : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpCommunication(QObject *parent);
    ~TcpCommunication();
    void connectToServer();
    void disconnectFromServer();
    void setIpAdress(QString ip);
    void setPortNumber(int port);

    void addNewOrder();
    void changeOrderState(int order,int state);
    void useSoundSignal();
    void recoverLastOrder();
    void setFullScreen();

    QString getIpAdress();
    int getPortNumber();

signals:
    void disconnect();


private slots:

private:
    QString ipAdr = "172.16.108.177";
    unsigned short port = 51268;



};

#endif // TCPCOMMUNICATION_H
