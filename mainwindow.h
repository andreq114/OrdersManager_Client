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

    void readData();
    void displayError(QAbstractSocket::SocketError socketError);
    void on_actionKonfiguracja_triggered();
    void on_actionInformacje_triggered();
    void connectApprove();
    void getOrdersListFromString(QString data);
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void disconnectApprove();

    void on_recoverBUtton_clicked();
    void on_soundButton_clicked();
    void on_addNewOrderButton_clicked();
    void orderButton_clicked();

    void setFullReadyOrd(int order);
    void setPartReadyOrd(int order);
    void setNotReadyOrd(int order);
    void deleteOrd(int order);
    void refTime();



    void on_actionServerFullScreen_triggered();

    void on_actionOrders_reset_triggered();

    void on_actionFullScreen_triggered();

private:
    Ui::MainWindow *ui;

    TcpCommunication *socket;
    OrderActionWindow *orderAction;
    dbmanager *db;

    QVector<int> orders;
    QVector<int> ordersStates;
    QVector<QTime> timeList;

    QVector<QPushButton*> orderButtons;
    QVector<QLabel*> timeLabels;
    QVector<QVBoxLayout*> layouts;

    QFont ordersFont;
    QFont timeFont;

    Thread *myThread;

    int nextOrder = 0;

    void connectSignals();
    //void changeOrderState(int order,int state);
    void refreshOrdersList();
    void addOrdButtons();



};
#endif // MAINWINDOW_H
