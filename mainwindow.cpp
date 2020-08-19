#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new TcpCommunication(this);
    connectSignals();
    addOrdButtons();
    connectButtonsWithSlots();
}

MainWindow::~MainWindow()
{
    delete ui;

    socket->abort();
    delete socket;
}

void MainWindow::readData()             //Odczytywanie danych z serwera, domyslnie odczyt listy zamowien i stanów
{
    QByteArray sock = socket->readAll();
    qDebug()<<"Odebralem cos";
    QString data = sock;
    qDebug()<<sock;
    if(data.at(0) == "$"){
        data.remove(0,1);
        connectApprove();
    }
    qDebug()<<data;
    getOrdersListFromString(data);
}

void MainWindow::connectSignals(){
    connect(socket, &QIODevice::readyRead, this, &MainWindow::readData);
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(disconnect()),this,SLOT(disconnectApprove()));
}

void MainWindow::connectApprove(){
    QMessageBox::information(this,"Akceptacja","Połączono");
}

void MainWindow::getOrdersListFromString(QString data){
    QVector<int> ordIndTemp;
    QVector<int> stateIndTemp;
    orders.clear();
    ordersStates.clear();

    qDebug()<<"Przetwarzam stringa";
    for(int i=0;i<data.length();i++){
        if(data.at(i) == "*")
            ordIndTemp.append(i);
        if(data.at(i) == "^")
            stateIndTemp.append(i);
    }
    for(int i=0;i<ordIndTemp.length()-1;i++){
        QString ord = data.mid((ordIndTemp.at(i)+1),ordIndTemp.at(i+1)-ordIndTemp.at(i)-1);
        orders.append(ord.toInt());
    }

    for(int i=0;i<stateIndTemp.length()-1;i++){
        QString ord = data.mid((stateIndTemp.at(i)+1),stateIndTemp.at(i+1)-stateIndTemp.at(i)-1);
        orders.append(ord.toInt());
    }
    refreshOrdersList();


}

void MainWindow::changeOrderState(int order,int state){
    qDebug()<<"Wysylam do socketa";
    socket->changeOrderState(order,state);
}

void MainWindow::refreshOrdersList(){

}

void MainWindow::addOrdButtons(){
    QPushButton *button;
    QLabel *time;
    QVBoxLayout *vlay;
    for(int i=0;i<50;i++){
        button = new QPushButton(this);
        button->setStyleSheet("background-color:rgb(0,255,0);border:none;");
        button->setMinimumSize(120,80);
        button->setMaximumWidth(250);
        button->setText("ORDER");


        time = new QLabel("empty");
        time->setStyleSheet("background-color:rgb(0,255,0);");
        time->setMinimumSize(100,20);
        time->setMaximumHeight(20);
        time->setAlignment(Qt::AlignCenter);

        vlay = new QVBoxLayout;
        vlay->setSpacing(0);

        timeLabels.append(time);
        orderButtons.append(button);
        layouts.append(vlay);
        vlay->addWidget(time);
        vlay->addWidget(button);


    }

    QGridLayout *lay = new QGridLayout;
    int row = 0;
    int col = 0;

    for(int i=1;i<=layouts.length();i++){

        row = static_cast<int>(ceil((static_cast<double>(i)/5)))-1;
        qDebug()<<row;
        col = (i-1)%5;
        qDebug()<<col;

        lay->addLayout(layouts.at(i-1),row,col);
        ui->ordersArea->setLayout(lay);



    }

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


}

void MainWindow::on_actionKonfiguracja_triggered()
{
    qDebug()<<socket->state();
    if(socket->state() == QAbstractSocket::ConnectedState){
        QMessageBox::information(this,"Błąd","Przerwij najpierw aktualne połączenie");
    }else{
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

void MainWindow::on_actionConnect_triggered()
{
    if(socket->state() == QAbstractSocket::ConnectedState)
        QMessageBox::information(this,"Błąd","Zostało już nawiązane połączenie z serwerem."
                                             "Zakończ obecne połączenie aby nawiązać inne,");
    else
        socket->connectToServer();
}

void MainWindow::on_actionDisconnect_triggered()
{
    if(socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromServer();
    else
        QMessageBox::information(this,"Błąd","Brak połączenia które można zakończyć.");
}

void MainWindow::disconnectApprove(){
    QMessageBox::information(this,"Disconnected","Rozłączono");
}

void MainWindow::on_recoverBUtton_clicked()
{
    socket->recoverLastOrder();
}

void MainWindow::on_soundButton_clicked()
{
    socket->useSoundSignal();
}

void MainWindow::on_addNewOrderButton_clicked()
{
    socket->addNewOrder();
}



void MainWindow::connectButtonsWithSlots(){
    connect(orderButtons.at(0),SIGNAL(clicked()),this,SLOT(ord1_clicked()));
    connect(orderButtons.at(1),SIGNAL(clicked()),this,SLOT(ord2_clicked()));
}

void MainWindow::ord1_clicked(){
    qDebug()<<"Button pierwszy";
}

void MainWindow::ord2_clicked(){
    qDebug()<<"Button drugi";
}
