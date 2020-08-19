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





    void on_ord1_clicked();

    void on_actionKonfiguracja_triggered();

    void on_actionInformacje_triggered();

private:
    Ui::MainWindow *ui;

    TcpCommunication *socket;
    QVector<int> orders;
    QVector<int> ordersStates;

    void connectSignals();
    void changeOrderState(int order,int state);
    void refreshOrdersList();


private slots:
    void connectApprove();
    void getOrdersListFromString(QString data);
    void on_ord2_clicked();
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void disconnectApprove();
};
#endif // MAINWINDOW_H
