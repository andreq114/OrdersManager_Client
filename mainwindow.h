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


    void on_connectButton_clicked();


    void on_ord1_clicked();

    void on_actionKonfiguracja_triggered();

    void on_actionInformacje_triggered();

private:
    Ui::MainWindow *ui;

    TcpCommunication *socket;

    void connectSignals();
    void changeOrderState(int order,int state);
};
#endif // MAINWINDOW_H
