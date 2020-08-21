#include "orderactionwindow.h"

OrderActionWindow::OrderActionWindow(QWidget *parent) : QDialog(parent){
    setWindowPreferences();
}

OrderActionWindow::~OrderActionWindow(){
    delete orderNumberLabel;

};

void OrderActionWindow::setWindowPreferences(){
    this->setMinimumSize(500,600);
    this->setMaximumSize(400,400);


    //this->setWindowFlags(Qt::WindowTitleHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);

    setFonts();

    QVBoxLayout *lay = new QVBoxLayout(this);

    orderNumberLabel = new QLabel();
    orderNumberLabel->setAlignment(Qt::AlignCenter);
    orderNumberLabel->setFont(titleFont);
    orderNumberLabel->setMinimumSize(460,100);


    fullReadyButton = new QPushButton(this);
    fullReadyButton->setStyleSheet(GREEN_BACKGROUND);
    fullReadyButton->setFont(buttonsFont);
    fullReadyButton->setMinimumSize(460,120);
    fullReadyButton->setText("Gotowe do odbioru");
    connect(fullReadyButton,SIGNAL(clicked()),this,SLOT(fullReady_clicked()));

    partReadyButton = new QPushButton(this);
    partReadyButton->setStyleSheet(ORANGE_BACKGROUND);
    partReadyButton->setFont(buttonsFont);
    partReadyButton->setMinimumSize(460,120);
    partReadyButton->setText("Częściowo gotowe do odbioru");
    connect(partReadyButton,SIGNAL(clicked()),this,SLOT(partReady_clicked()));

    notReadyButton = new QPushButton(this);
    notReadyButton->setStyleSheet(YELLOW_BACKGROUND);
    notReadyButton->setFont(buttonsFont);
    notReadyButton->setMinimumSize(460,120);
    notReadyButton->setText("W trakcie realizacji");
    connect(notReadyButton,SIGNAL(clicked()),this,SLOT(notReady_clicked()));

    deleteOrderButton = new QPushButton(this);
    deleteOrderButton->setStyleSheet(RED_BACKGROUND);
    deleteOrderButton->setFont(buttonsFont);
    deleteOrderButton->setMinimumSize(460,120);
    deleteOrderButton->setText("Usuń zamówienie");
    connect(deleteOrderButton,SIGNAL(clicked()),this,SLOT(deleteOrder_clicked()));

    lay->addWidget(orderNumberLabel);
    lay->addWidget(fullReadyButton);
    lay->addWidget(partReadyButton);
    lay->addWidget(notReadyButton);
    lay->addWidget(deleteOrderButton);

}
void OrderActionWindow::showWindow(int order){
    this->setWindowTitle(QString::number(order));
    actualOrder = order;
    orderNumberLabel->setText("Zamówienie nr. "+QString::number(order));
    this->show();
}

void OrderActionWindow::setFonts(){
    titleFont.setBold(true);
    titleFont.setPointSize(18);

    buttonsFont.setBold(true);
    buttonsFont.setPointSize(15);
}

void OrderActionWindow::fullReady_clicked(){
    emit setFullReadyOrder(actualOrder);
}
void OrderActionWindow::partReady_clicked(){
    emit setPartReadyOrder(actualOrder);
}
void OrderActionWindow::notReady_clicked(){
    emit setNotReadyOrder(actualOrder);
}
void OrderActionWindow::deleteOrder_clicked(){
    emit deleteOrder(actualOrder);
}

