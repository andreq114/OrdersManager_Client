#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new TcpCommunication(this);
    orderAction = new OrderActionWindow();
    myThread = new Thread(this);
    myThread->start();

    connectSignals();
    addOrdButtons();
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);

}

MainWindow::~MainWindow()
{
    delete ui;

    socket->abort();
    delete socket;
    delete orderAction;
    if(myThread->isRunning()){
        myThread->terminate();
        myThread->wait();
    }
    delete myThread;
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
    //CONNECTY KOMUNIKACJI Z SERWEREM
    connect(socket, &QIODevice::readyRead, this, &MainWindow::readData);
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(disconnect()),this,SLOT(disconnectApprove()));

    //CONNECTY Z OKNEM DIALOGOWYM DO WYBORU AKCJI DANEGO ZAMOWIENIA
    connect(orderAction,SIGNAL(setFullReadyOrder(int)),this,SLOT(setFullReadyOrd(int)));
    connect(orderAction,SIGNAL(setPartReadyOrder(int)),this,SLOT(setPartReadyOrd(int)));
    connect(orderAction,SIGNAL(setNotReadyOrder(int)),this,SLOT(setNotReadyOrd(int)));
    connect(orderAction,SIGNAL(deleteOrder(int)),this,SLOT(deleteOrd(int)));

    //CONNECT Z WĄTKIEM
    connect(myThread,SIGNAL(refreshTime()),this,SLOT(refTime()));
}

void MainWindow::connectApprove(){
    QMessageBox::information(this,"Akceptacja","Połączono");
}

void MainWindow::getOrdersListFromString(QString data){
    QVector<int> ordIndTemp;
    QVector<int> stateIndTemp;
    QVector<int> timeIndTemp;
    QVector<int> nextOrderIndTemp;

    orders.clear();
    ordersStates.clear();
    timeList.clear();

    qDebug()<<"Przetwarzam stringa";
    for(int i=0;i<data.length();i++){
        if(data.at(i) == "*")
            ordIndTemp.append(i);
        if(data.at(i) == "^")
            stateIndTemp.append(i);
        if(data.at(i) == "#")
            timeIndTemp.append(i);
        if(data.at(i) == "@")
            nextOrderIndTemp.append(i);
    }
    for(int i=0;i<ordIndTemp.length()-1;i++){
        QString ord = data.mid((ordIndTemp.at(i)+1),ordIndTemp.at(i+1)-ordIndTemp.at(i)-1);
        orders.append(ord.toInt());
    }

    for(int i=0;i<stateIndTemp.length()-1;i++){
        QString ordState = data.mid((stateIndTemp.at(i)+1),stateIndTemp.at(i+1)-stateIndTemp.at(i)-1);
        ordersStates.append(ordState.toInt());
    }

    for(int i=0;i<timeIndTemp.length()-1;i++){
        QString ordTime = data.mid((timeIndTemp.at(i)+1),timeIndTemp.at(i+1)-timeIndTemp.at(i)-1);
        qDebug()<<"Dodaje czas "<<ordTime;
        timeList.append(QTime::fromString(ordTime,"hh:mm:ss"));
    }
    for(int i=0;i<nextOrderIndTemp.length()-1;i++){
        QString nextOrd = data.mid((nextOrderIndTemp.at(i)+1),nextOrderIndTemp.at(i+1)-nextOrderIndTemp.at(i)-1);
        nextOrder = nextOrd.toInt();
    }
    nextOrder = *std::max_element(orders.constBegin(),orders.constEnd())+1;
    qDebug()<<"Przed refreshem";
    refreshOrdersList();
    qDebug()<<"Po refreshu";
    refTime();
    qDebug()<<"Po reftime";


}

/*void MainWindow::changeOrderState(int order,int state){
    qDebug()<<"Wysylam do socketa";
    socket->changeOrderState(order,state);
}*/

void MainWindow::refreshOrdersList(){
    for(int i=0;i<orderButtons.length();i++){
        orderButtons.at(i)->setVisible(false);
        timeLabels.at(i)->setVisible(false);
    }
    for(int i=0;i<orders.length();i++){
        orderButtons.at(i)->setVisible(true);
        timeLabels.at(i)->setVisible(true);
        orderButtons.at(i)->setText(QString::number(orders.at(i)));
        if(ordersStates.at(i) == 1){
            orderButtons.at(i)->setStyleSheet("background-color:rgb(255,255,0);");
            timeLabels.at(i)->setStyleSheet("background-color:rgb(255,255,0);");
        }else if(ordersStates.at(i) == 2){
            orderButtons.at(i)->setStyleSheet("background-color:rgb(255,127,0);");
            timeLabels.at(i)->setStyleSheet("background-color:rgb(255,127,0);");
        }else if(ordersStates.at(i) == 3){
            orderButtons.at(i)->setStyleSheet("background-color:rgb(0,255,0);");
            timeLabels.at(i)->setStyleSheet("background-color:rgb(0,255,0);");
        }
    }

    ui->nextOrderLabel->setText("Następny numer: "+QString::number(nextOrder));
}

void MainWindow::addOrdButtons()                        //Dodanie przyciskow zamowien przy starcie programu i poczatkowo konfiguracja
{
    QPushButton *button;
    QLabel *time;
    QVBoxLayout *vlay;
    ordersFont.setBold(true);
    ordersFont.setPointSize(20);
    timeFont.setPointSize(15);
    for(int i=0;i<50;i++){
        button = new QPushButton(this);
        button->setStyleSheet("background-color:rgb(255,255,0);border:none;");
        button->setMinimumSize(120,120);
        button->setMaximumWidth(350);
        button->setText(QString::number(0));
        button->setVisible(false);
        button->setFont(ordersFont);

        connect(button,SIGNAL(clicked()),this,SLOT(orderButton_clicked()));


        time = new QLabel("empty");
        time->setStyleSheet("background-color:rgb(0,255,0);");
        time->setMinimumSize(120,20);
        time->setMaximumHeight(20);
        time->setAlignment(Qt::AlignCenter);
        time->setVisible(false);
        time->setFont(timeFont);

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
        row = static_cast<int>(ceil((static_cast<double>(i)/4)))-1;
        col = (i-1)%4;

        lay->addLayout(layouts.at(i-1),row,col);
        ui->ordersArea->setLayout(lay);
    }


 }

void MainWindow::refTime(){

    for(int i=0;i<timeList.length();i++){
        QString czas2;
        QString czas;
        if(QTime::currentTime() >= timeList.at(i))          //Bez tego warunku,czas zamowienia z poprzedniego dnia ( po polnocy ) byl by ujemny
            czas = QTime::fromMSecsSinceStartOfDay(QTime::currentTime().msecsSinceStartOfDay()-timeList.at(i).msecsSinceStartOfDay()).toString();       //Aktualna godzina minus godzina przyjecia zamowienia
        else
            czas = QTime::fromMSecsSinceStartOfDay(QTime::currentTime().msecsSinceStartOfDay()-timeList.at(i).msecsSinceStartOfDay()+86400000).toString();      //Jesli zamowienie przyjeto przed polnoca a jest po polnocy, to godzina przyjecia zamowienia minus aktualna godzina plus 24h
        for(int i=0;i<czas.length();i++){                   //Ucinamy godziny, wyswietlamy tylko minuty i sekundy
            if(i > 2)
                czas2.append(czas.at(i));
        }
        timeLabels.at(i)->setText(czas2);
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

void MainWindow::disconnectApprove()                    //Potwierdzenie rozlaczenia z serwerem
{
    QMessageBox::information(this,"Disconnected","Rozłączono");
}


//SLOT PO WYBRANIU DANEGO ZAMOWIENIA
void MainWindow::orderButton_clicked(){

    for(int i=0;i<orderButtons.length();i++){
        if(orderButtons.at(i) == sender()){
            qDebug()<<QString::number(i);
            orderAction->showWindow(orderButtons.at(i)->text().toInt());
        }
    }

}

//AKCJE PRZYWRACANIA OSTATNIEGO ZAMOWIENIA,SYGNALU DZWIEKOWEGO I DODANIA NOWEGO ZAMOWIENIA
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


//METODY PO WYBRANIU AKCJI NA DANYM ZAMOWIENIA
void MainWindow::setFullReadyOrd(int order){
    qDebug()<<"Ustawiam jako gotowe zamówienie numer "<<order;
    orderAction->close();
    socket->changeOrderState(order,3);
}
void MainWindow::setPartReadyOrd(int order){
    qDebug()<<"Ustawiam jako prawie gotowe zamówienie numer "<<order;
    orderAction->close();
    socket->changeOrderState(order,2);
}
void MainWindow::setNotReadyOrd(int order){
    qDebug()<<"Ustawiam jako nie gotowe zamówienie numer "<<order;
    orderAction->close();
    socket->changeOrderState(order,1);
}
void MainWindow::deleteOrd(int order){
    qDebug()<<"Usuwam zamówienie numer "<<order;
    orderAction->close();
    socket->changeOrderState(order,4);
}

void MainWindow::on_actionServerFullScreen_triggered()
{
    socket->setFullScreen();
}
