#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QMessageBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <tcpcommunication.h>
#include <orderactionwindow.h>
#include <QPushButton>
#include <math.h>
#include <QTime>
#include <thread.h>
#include <QScroller>
#include <algorithm>
#include <dbmanager.h>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void readData();                                                                //Metoda odbierajaca potwierdzenie połączenia/liste zamowien,stanów i czasów od serwera przez socket
    void displayError(QAbstractSocket::SocketError socketError);                    //Metoda wyswietlajaca błąd jesli połączenie się nie powiedzie
    void on_actionKonfiguracja_triggered();                                         //Slot po wcisnieciu przycisku konfiguracji, wyswietlajacy okienko do edycji ip/portu
    void on_actionInformacje_triggered();                                           //Slot po wcisnieciu przycisku informacji, wyswietlajacy aktualnie ustawione ip/port
    void on_actionConnect_triggered();                                              //Slot po wcisnieciu przycisku Połącz, wykonujący próbe połączenie z serwerem
    void on_actionDisconnect_triggered();                                           //Slot po wcisnieciu przycisku Rozłącz, kończący połączenie z serwerem
    void disconnectApprove();                                                       //Slot wyświetlający potwierdzenie zakończenia połączenia z serwerem

    void on_recoverBUtton_clicked();                                                //Slot po wcisnieciu przycisku przywroc zamowienie, wysylajacy sygnal do socketa o przywroceni zamowienia
    void on_soundButton_clicked();                                                  //Slot po wcisnieciu przycisku sygnalu dzwiekowego, wysylajacy sygnal do socketa o wywolanie sygnalu dzwiekowego
    void on_addNewOrderButton_clicked();                                            //Slot po wcisnieciu przycisku dodaj nowe zamowienie, wysylajacy sygnal do socketa o dodanie nowego zamowienia
    void orderButton_clicked();                                                     //Slot po wcisnieciu przyciskow zamowien, pokazujacy okno o wybranie akcji na danym zamowieniu

    void setFullReadyOrd(int order);                                                //Sloty akcji na danym zamowieniu, wysylajace sygnal do socketa o wywolanie akcji na danym zamowieniu
    void setPartReadyOrd(int order);
    void setNotReadyOrd(int order);
    void deleteOrd(int order);

    void refTime();                                                                 //Slot polaczony z wątkiem odswiezajacy czas jaki minął od dodania zamowienia



    void on_actionServerFullScreen_triggered();                                     //Slot po wcisnieciu przycisku "Pełny Ekran Serwera", wywołujący metode socketa o włączenie trybu pełnego ekranu w oknie widocznym dla klientów na serwerze

    void on_actionOrders_reset_triggered();                                         //Slot po wcisnieciu przycisku "Resetuj zamówienia", wywołujący metoda socketa o zresetowanie stanu zamówien na serwerze

    void on_actionFullScreen_triggered();                                           //Slot po wcisnieciu przycisku "Pełny Ekran", włączający pełny ekran aktualnej aplikacji

private:
    Ui::MainWindow *ui;

    TcpCommunication *socket;                                                       //Socket do połącznie z serwerem
    OrderActionWindow *orderAction;                                                 //Okienko do wybrania akcji na danym zamówieniu
    dbmanager *db;                                                                  //Klasa  do operowania na bazie danych
    Thread *myThread;                                                               //Klasa z wątkiem odświeżającym czasy od przyjecia danych zamowień

    QVector<int> orders;                                                            //Lista zamówień
    QVector<int> ordersStates;                                                      //Lista stanów danych zamówień
    QVector<QTime> timeList;                                                        //Lista czasów danych zamówień , te trzy listy są ze sobą powiązane indeksami

    QVector<QPushButton*> orderButtons;                                             //Lista przycisków odpowiadajacych danym zamówieniom
    QVector<QLabel*> timeLabels;                                                    //Lista labelów odpowiadajacych czasom danych zamówień, powiązane z buttonami indeksami
    QVector<QVBoxLayout*> layouts;                                                  //Lista layoutów na powyższe przyciski i labele
    QGridLayout *lay;                                                               //Layout na wszystkie layouty z buttonami/labelami

    QFont ordersFont;                                                               //Czcionki dla buttonów i labeli z czasami
    QFont timeFont;


    int nextOrder = 0;                                                              //Numer nastepnego zamowienia

    void connectSignals();                                                          //Metoda tworząca połączenie sygnałów i slotów z wątkiem,socketem i bazą danych
    void refreshOrdersList();                                                       //Metoda odswiezajaca liste zamówień i ich stanów
    void addOrdButtons();                                                           //Metoda dodająca statycznie 50 buttonów na początku programu
    void connectApprove();                                                          //Metoda wyswietlajaca potwierdzenie polaczenia z serwerem
    void getOrdersListFromString(QString data);                                     //Metoda wyłuskająca liste zamówień/stanów/czasów z łańcucha znaków

    void closeEvent(QCloseEvent *event);



};
#endif // MAINWINDOW_H
