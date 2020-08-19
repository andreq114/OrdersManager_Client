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
#include <QPushButton>
#include <math.h>

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

    void ord1_clicked();
    void ord2_clicked();
    /*void ord3_clicked();
    void ord4_clicked();
    void ord5_clicked();
    void ord6_clicked();
    void ord7_clicked();
    void ord8_clicked();
    void ord9_clicked();
    void ord10_clicked();
    void ord11_clicked();
    void ord12_clicked();
    void ord13_clicked();
    void ord14_clicked();
    void ord15_clicked();
    void ord16_clicked();
    void ord17_clicked();
    void ord18_clicked();
    void ord19_clicked();
    void ord20_clicked();
    void ord21_clicked();
    void ord22_clicked();
    void ord23_clicked();
    void ord24_clicked();
    void ord25_clicked();
    void ord26_clicked();
    void ord27_clicked();
    void ord28_clicked();
    void ord29_clicked();
    void ord30_clicked();
    void ord31_clicked();
    void ord32_clicked();
    void ord33_clicked();
    void ord34_clicked();
    void ord35_clicked();
    void ord36_clicked();
    void ord37_clicked();
    void ord38_clicked();
    void ord39_clicked();
    void ord40_clicked();
    void ord41_clicked();
    void ord42_clicked();
    void ord43_clicked();
    void ord44_clicked();
    void ord45_clicked();
    void ord46_clicked();
    void ord47_clicked();
    void ord48_clicked();
    void ord49_clicked();
    void ord50_clicked();*/


private:
    Ui::MainWindow *ui;

    TcpCommunication *socket;
    QVector<int> orders;
    QVector<int> ordersStates;

    QVector<QPushButton*> orderButtons;
    QVector<QLabel*> timeLabels;
    QVector<QVBoxLayout*> layouts;

    void connectSignals();
    void changeOrderState(int order,int state);
    void refreshOrdersList();
    void addOrdButtons();
    void connectButtonsWithSlots();


};
#endif // MAINWINDOW_H
