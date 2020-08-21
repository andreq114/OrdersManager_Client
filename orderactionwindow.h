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
    void showWindow(int order);
    void setWindowPreferences();

public slots:
    void fullReady_clicked();
    void partReady_clicked();
    void notReady_clicked();
    void deleteOrder_clicked();


private:
    QLabel *orderNumberLabel;
    QPushButton *fullReadyButton;
    QPushButton *partReadyButton;
    QPushButton *notReadyButton;
    QPushButton *deleteOrderButton;
    QFont titleFont;
    QFont buttonsFont;
    int actualOrder = 0;

    void setFonts();

signals:
    void setFullReadyOrder(int order);
    void setPartReadyOrder(int order);
    void setNotReadyOrder(int order);
    void deleteOrder(int order);

};

#endif // ORDERACTIONWINDOW_H
