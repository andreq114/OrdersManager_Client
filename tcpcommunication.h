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
    void connectToServer();                                                 //Łączenie się z serwerem pod podanym ip i portem
    void disconnectFromServer();                                            //Rozłączenie z serwerem
    void setFullScreen();                                                   //Włączenie/wyłączenie trybu pełnoekranowego w serwerze w okienku widocznym dla klientów


    void addNewOrder();                                                     //Komenda wysyłana do serwera o dodanie nowego zamówienia
    void changeOrderState(int order,int state);                             //Komenda wysyłana do serwera o zmiane stanu dla danego zamówienia
    void useSoundSignal();                                                  //Komenda wysyłana do serwera o wywołanie sygnału dźwiękowego
    void recoverLastOrder();                                                //Komenda wysyłana do serwera o przywrócenie usuniętego zamówienia
    void resetOrders();                                                     //Komenda wysyłana do serwera o zresetowanie wszystkich zamówień oraz numeru nastepnego zamówienia

    void setIpAdress(QString ip);                                           //Setter do ustawienia adresu IP serwera z którym będziemy się łączyć
    void setPortNumber(int port);                                           //Setter do ustawienia portu serwera z którym będziemy się łączyć
    QString getIpAdress();                                                  //Getter do pobrania ustawionego adresu IP serwera
    int getPortNumber();                                                    //Getter do pobrania ustawionego portu serwera

signals:
    void disconnect();                                                      //Sygnał wysyłany do mainwindow po rozłączeniu się z serwerem


private slots:

private:
    QString ipAdr = "172.16.108.177";                                       //Adres IP serwera z który będziemy się łączyć
    unsigned short port = 51268;                                            //Port serwera z którym będziemy się łączyc



};

#endif // TCPCOMMUNICATION_H
