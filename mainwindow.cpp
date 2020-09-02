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
    db = new dbmanager(parent);

    socket->setIpAdress(db->readIp_fromDb());                                       //Ustawiamy na poczatku dzialania programu zapisane adresy ip/port z bazy danych
    socket->setPortNumber(db->readPort_fromDb());

    connectSignals();
    addOrdButtons();
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);      //Metoda pozwalajaca scrollowac na tablecie liste jednym palcem lub myszką lewym przyciskiem myszy

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
    delete db;

    for(auto ord:orderButtons)
        delete ord;
    for(auto time:timeLabels)
        delete time;
    for(auto lay:layouts)
        delete lay;
}

void MainWindow::readData()
{
    QByteArray sock = socket->readAll();
    QString data = sock;
    if(data.at(0) == "$"){             //Jesli jest dolar na poczatku, to jest to pierwsze polaczenie, wywolujemy metode potwierdzajaca polaczenie i wycinamy dolara z lancucha znaków
        data.remove(0,1);
        connectApprove();
    }
    getOrdersListFromString(data);     //Wywolujemy metoda do wyłuskania listy zamowien/stanow/czasow z lancucha znaków
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
        timeList.append(QTime::fromString(ordTime,"hh:mm:ss"));
    }
    for(int i=0;i<nextOrderIndTemp.length()-1;i++){
        QString nextOrd = data.mid((nextOrderIndTemp.at(i)+1),nextOrderIndTemp.at(i+1)-nextOrderIndTemp.at(i)-1);
        nextOrder = nextOrd.toInt();
    }

    refreshOrdersList();                                                                    //Odswiezamy po zaktualizowaniu listy liste zamowień wyswietlana na ekranie

    refTime();                                                                              //Odswiezamy czasy

}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
       case QAbstractSocket::RemoteHostClosedError:
           break;
       case QAbstractSocket::HostNotFoundError:
           QMessageBox::information(this, tr("Orders System Client"),
                                    tr("Nie znaleziono hosta.Sprawdź, czy zostało wpisane poprawne ip lub port serwera."));
           break;
       case QAbstractSocket::ConnectionRefusedError:
           QMessageBox::information(this, tr("Orders System Client"),
                                    tr("Połączenie zostało odrzucone.Upewnij się czy serwer jest włączony oraz czy zostało"
                                       " wpisane poprawne ip i port serwera."));
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
        QMessageBox::information(this,"Błąd","Przerwij najpierw aktualne połączenie.");
    }else{
        QDialog dialog(this);

        dialog.setMaximumSize(200,170);
        dialog.setMinimumSize(200,170);

        QFont font;
        font.setPointSize(30);
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

            socket->setIpAdress(ipAdress->text());                                                              //Ustawiamy ip w sockecie

            if(portNumber->text().isEmpty())
                socket->setPortNumber(0);                                                                       //Jesli nie zostało podane, ustawiany jest losowy port
            else
                socket->setPortNumber(portNumber->text().toInt());                                              //Ustawiamy port w sockecie

            db->addIp_port_toDb(socket->getIpAdress(),socket->getPortNumber());                                 //Dodajemy ustawione ip i port do bazy danych
        }
    }
}

void MainWindow::on_actionInformacje_triggered()
{
    QDialog dialog(this);
    QFormLayout lay(&dialog);
    dialog.setMaximumSize(200,150);
    dialog.setMinimumSize(200,150);

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
                                             );
    else
        socket->connectToServer();
}

void MainWindow::on_actionDisconnect_triggered()
{
    if(socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromServer();
    else
        QMessageBox::information(this,"Błąd","Żadne połączenie nie jest aktualnie nawiązane.");
}

void MainWindow::disconnectApprove()
{
    QMessageBox::information(this,"Disconnected","Rozłączono");
}




//Metody do operowaniu na zamówieniach poprzez serwer
void MainWindow::orderButton_clicked()
{

    if(socket->state() == QAbstractSocket::ConnectedState){
        for(int i=0;i<orderButtons.length();i++){
            if(orderButtons.at(i) == sender()){
                orderAction->showWindow(orderButtons.at(i)->text().toInt());
            }
        }
    }
    else
        QMessageBox::information(this,"Błąd","Brak połączenia z serwerem. Upewnij się, że połączenie zostało nawiązane.");


}

void MainWindow::on_recoverBUtton_clicked()
{
    if(socket->state() == QAbstractSocket::ConnectedState){
        if(orders.length()<orderButtons.length())
            socket->recoverLastOrder();
        else
            QMessageBox::information(this,"Błąd","Osiągnięto maksymalną liczbę zamówień jednocześnie.");
    }else{
        QMessageBox::information(this,"Błąd","Brak połączenia z serwerem. Upewnij się, że połączenie zostało nawiązane.");
    }
}

void MainWindow::on_soundButton_clicked()
{
    if(socket->state()==QAbstractSocket::ConnectedState)
        socket->useSoundSignal();
    else
        QMessageBox::information(this,"Błąd","Brak połączenia z serwerem. Upewnij się, że połączenie zostało nawiązane.");
}

void MainWindow::on_addNewOrderButton_clicked()
{
    qDebug()<<"Liczba zamowien";
    if(socket->state() == QAbstractSocket::ConnectedState)
        if(orders.length() < orderButtons.length())
            socket->addNewOrder();
        else
            QMessageBox::information(this,"Błąd","Osiągnięto maksymalną liczbę zamówień jednocześnie.");
    else
        QMessageBox::information(this,"Błąd","Brak połączenia z serwerem. Upewnij się, że połączenie zostało nawiązane.");
}


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
    if(socket->state() == QAbstractSocket::ConnectedState)
        socket->setFullScreen();
    else
        QMessageBox::information(this,"Błąd","Brak połączenia z serwerem. Upewnij się, że połączenie zostało nawiązane.");
}

void MainWindow::on_actionOrders_reset_triggered()
{
    if(socket->state() == QAbstractSocket::ConnectedState){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Wyjście", "Czy napewno chcesz zresetowac wszystkie zamówienia?",
                                       QMessageBox::Yes|QMessageBox::No);
         if (reply == QMessageBox::Yes) {
           socket->resetOrders();
         } else {

         }

    }
    else
        QMessageBox::information(this,"Błąd","Brak połączenia z serwerem. Upewnij się, że połączenie zostało nawiązane.");
}




//Metody do wykonywania akcji w aktualnym gui
void MainWindow::refreshOrdersList(){
    //Ustawiamy wszystkie buttony/labele jako niewidoczne
    for(int i=0;i<orderButtons.length();i++){
        orderButtons.at(i)->setVisible(false);
        timeLabels.at(i)->setVisible(false);
    }
    //Ustawiamy jako widoczne tylko tyle buttonów i labeli ile jest zamowień, i ustawiamy ich kolory według ich stanów
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

void MainWindow::addOrdButtons()
{
    QPushButton *button;
    QLabel *time;
    QVBoxLayout *vlay;

    ordersFont.setBold(true);
    ordersFont.setPointSize(40);
    timeFont.setPointSize(15);

    for(int i=0;i<50;i++){
        button = new QPushButton(this);
        button->setStyleSheet("background-color:rgb(255,255,0);border:none;");                 //Ustawienia kazdego buttona
        button->setMinimumSize(120,120);
        button->setMaximumSize(350,200);
        button->setText(QString::number(0));
        button->setVisible(false);
        button->setFont(ordersFont);

        connect(button,SIGNAL(clicked()),this,SLOT(orderButton_clicked()));                     //Connect łączący kazdy button ze slotem wywolywanym po kliknieciu go


        time = new QLabel("empty");                                                             //Ustawienia kazdego labela z czasem
        time->setStyleSheet("background-color:rgb(0,255,0);");
        time->setMinimumSize(120,20);
        time->setMaximumHeight(20);
        time->setAlignment(Qt::AlignCenter);
        time->setVisible(false);
        time->setFont(timeFont);

        vlay = new QVBoxLayout;                                                                 //Ustawienia kazdego layouta w którym będzie label/button
        vlay->setSpacing(0);

        timeLabels.append(time);                                                                //Dodajemy każdy label/button/layout do listy
        orderButtons.append(button);
        layouts.append(vlay);

        vlay->addWidget(time);                                                                  //Dodajemy label/button do layouta
        vlay->addWidget(button);
    }

    lay = new QGridLayout;
    int row = 0;
    int col = 0;

    for(int i=1;i<=layouts.length();i++){
        row = static_cast<int>(ceil((static_cast<double>(i)/4)))-1;     //Wiersz się zwiększy, gdy w danym są juz 4 layouty z buttonami/labelami
        col = (i-1)%4;                                                  //Liczba kolumn jest zawsze 4

        lay->addLayout(layouts.at(i-1),row,col);                        //Dodajemy do głównego layoutu layouty z buttonami/labelami
        ui->ordersArea->setLayout(lay);                                 //Dodajemy główny layout do scroll area
    }

 }

void MainWindow::refTime()
{

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

void MainWindow::on_actionFullScreen_triggered()
{
    if(this->isFullScreen()){
        this->setWindowState(Qt::WindowMaximized);
    }else{
        this->setWindowState(Qt::WindowFullScreen);
    }
}



void MainWindow::closeEvent(QCloseEvent *event){
     QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "Wyjście", "Czy napewno zamknąć program?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
        event->accept();
      } else {
        event->ignore();
      }
}
