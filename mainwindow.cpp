#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //tcpSocket = new QTcpSocket(this);
    socket = new TcpCommunication(this);
    connectSignals();


}




void MainWindow::on_connectButton_clicked()
{
    ui->connectButton->setEnabled(false);
    socket->connectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readData()             //Odczytywanie danych z serwera, domyslnie odczyt listy zamowien i stanów
{

    QByteArray sock = socket->readAll();
    qDebug()<<"Odebralem cos";
    QString nextFortune = sock;
    qDebug()<<sock;
    if(sock == "ACCEPT"){
        QMessageBox::information(this,"Akceptacja","Połączono");
    }else{
        QMessageBox::information(this,"Ramka",nextFortune);
    }
    //ui->statusLabel->setText(nextFortune);
    ui->connectButton->setEnabled(true);
}



void MainWindow::connectSignals(){
    connect(socket, &QIODevice::readyRead, this, &MainWindow::readData);
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
}

void MainWindow::on_ord1_clicked()
{
    qDebug()<<"ord1 klik";
    changeOrderState(ui->ord1->text().toInt(),2);
}

void MainWindow::changeOrderState(int order,int state){
    qDebug()<<"Wysylam do socketa";
    socket->changeOrderState(order,state);
}












void MainWindow::displayError(QAbstractSocket::SocketError socketError){
    switch (socketError) {
       case QAbstractSocket::RemoteHostClosedError:
           break;
       case QAbstractSocket::HostNotFoundError:
           QMessageBox::information(this, tr("Orders System Client"),
                                    tr("The host was not found. Please check the "
                                       "host name and port settings."));
           break;
       case QAbstractSocket::ConnectionRefusedError:
           QMessageBox::information(this, tr("Orders System Client"),
                                    tr("The connection was refused by the peer. "
                                       "Make sure the server is running, "
                                       "and check that the host name and port "
                                       "settings are correct."));
           break;
       default:
           QMessageBox::information(this, tr("Orders System Client"),
                                    tr("The following error occurred: %1.")
                                    .arg(socket->errorString()));
       }

       ui->connectButton->setEnabled(true);
}

void MainWindow::on_actionKonfiguracja_triggered()
{
    QDialog dialog(this);
    QFormLayout lay(&dialog);

    lay.addRow(new QLabel("Adres IP serwera:"));
    QLineEdit *ipAdress = new QLineEdit (&dialog);
    ipAdress->setInputMask("000.000.000.000;_");
    lay.addRow (ipAdress);

    lay.addRow(new QLabel("Port serwera:"));
    QLineEdit *portNumber = new QLineEdit (&dialog);
    portNumber->setInputMask("00000;_");
    lay.addRow(portNumber);

    // Dodaj przycisk Anuluj i OK
    QDialogButtonBox buttonBox (QDialogButtonBox :: Ok | QDialogButtonBox :: Cancel                                ,
        Qt :: Horizontal, & dialog);
    lay.addRow ( & buttonBox);
    QObject :: connect ( & buttonBox, SIGNAL (rejected ()), & dialog, SLOT (reject ()));
    QObject :: connect ( & buttonBox, SIGNAL (accepted ()), & dialog, SLOT (accept ()));
    if (dialog.exec() == QDialog::Accepted) {

        socket->setIpAdress(ipAdress->text());

        if(portNumber->text().isEmpty())
            socket->setPortNumber(0);
        else
         socket->setPortNumber(portNumber->text().toInt());
    }

}

void MainWindow::on_actionInformacje_triggered()
{
    QDialog dialog(this);
    QFormLayout lay(&dialog);

    lay.addRow(new QLabel("Adres IP serwera:"));
    lay.addRow (new QLabel(socket->getIpAdress()));
    lay.addRow(new QLabel("Port serwera:"));
    lay.addRow(new QLabel(QString::number(socket->getPortNumber())));

    // Dodaj przycisk Anuluj i OK
    QDialogButtonBox buttonBox (QDialogButtonBox :: Ok,
        Qt :: Horizontal, & dialog);
    lay.addRow ( & buttonBox);
    QObject :: connect ( & buttonBox, SIGNAL (accepted ()), & dialog, SLOT (reject ()));
    dialog.exec();
}
