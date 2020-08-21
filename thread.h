#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QDebug>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = nullptr);

private:
    short int i = 0;
    void run();
signals:
    void refreshTime();

};

#endif // THREAD_H
