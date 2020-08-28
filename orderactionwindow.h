#ifndef ORDERACTIONWINDOW_H
#define ORDERACTIONWINDOW_H

#include <QDialog>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#define GREEN_BACKGROUND "background-color:rgb(0,255,0);margin:4px;"
#define YELLOW_BACKGROUND "background-color:rgb(255,255,0);margin:4px;"
#define ORANGE_BACKGROUND "background-color:rgb(254,127,0);margin:4px;"
#define RED_BACKGROUND "background-color:rgb(255,0,0);margin:4px;"


class OrderActionWindow : public QDialog
{
    Q_OBJECT
public:
    OrderActionWindow(QWidget *parent = nullptr);
    ~OrderActionWindow();
    void showWindow(int order);                                             //Metoda do pokazania okienka
    void setWindowPreferences();                                            //Metoda z konfiguracją okienka

public slots:
    void fullReady_clicked();                                               //Metody przy kliknieciu kazdego z buttonów
    void partReady_clicked();
    void notReady_clicked();
    void deleteOrder_clicked();


private:
    QLabel *orderNumberLabel;                                               //Label z numerem zamowienia ktore edytujemy
    QPushButton *fullReadyButton;                                           //Buttony edytujace dane zamowienia w okreslony sposob
    QPushButton *partReadyButton;
    QPushButton *notReadyButton;
    QPushButton *deleteOrderButton;
    QVBoxLayout *lay;                                                       //Layout dla powyzszych


    QFont titleFont;                                                        //Czcionki dla labela i buttonów
    QFont buttonsFont;

    void setFonts();                                                        //Metoda ustawiajaca czcionki dla powyższych


    int actualOrder = 0;                                                    //Numer zamowienia ktore aktualnie edytujemy

signals:
    void setFullReadyOrder(int order);                                      //Sygnały z numerem zamowienia do zmiany stanu wysylane do mainwindow
    void setPartReadyOrder(int order);
    void setNotReadyOrder(int order);
    void deleteOrder(int order);

};

#endif // ORDERACTIONWINDOW_H
