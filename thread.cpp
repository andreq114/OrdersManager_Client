#include "thread.h"

Thread::Thread(QObject *parent) : QThread(parent)
{

}

void Thread::run()
{
    while(this->isRunning()){

        emit this->refreshTime();
        this->msleep(1000);


    }
}
