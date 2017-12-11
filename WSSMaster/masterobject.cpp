#include "masterobject.h"

masterobject::masterobject(QObject *parent) : QObject(parent)
{
    udpt = new socketUDP();
    tcpt = new socketTCP();
    fbeacon = new QTimer();
    slask = new QTimer();

    QDir* dir = new QDir();
    system("mkdir -p /var/WSSMaster/Slaves/");
    dir->setCurrent("/var/WSSMaster/Slaves/");
    currslave = 444 + dir->count() + 1;
    dir->~QDir();

    fbeacon->setInterval(15000);
    fbeacon->start();
    slask->setInterval(30000);
    slask->start();

    connect(fbeacon, SIGNAL(timeout()), this, SLOT(beacon()));
    connect(slask, SIGNAL(timeout()), this, SLOT(ask()));
    connect(udpt, SIGNAL(slavedisc(QString)), this, SLOT(new_slave(QString)));
    connect(udpt, SIGNAL(slaveback(QString, QString)), this, SLOT(back_slave(QString, QString)));
    connect(udpt, SIGNAL(slavequit(QString, QString)), this, SLOT(quit_slave(QString, QString)));

    (new QUdpSocket())->writeDatagram(crypt::enc("#444$Firstatt#"), QHostAddress("10.10.10.255"), (quint16) 4441);
}

void masterobject::beacon()
{
    (new QUdpSocket())->writeDatagram(crypt::enc("#444$Beacon#"), QHostAddress("10.10.10.255"), (quint16) 4441);
}

void masterobject::ask()
{
    for(int i = 0; i < slaves.count(); i++)
    {
        QString red = "";
        QTcpSocket* kk = new QTcpSocket();
        kk->connectToHost(QHostAddress(slaves.at(i)), (quint16) 4441);
        if(!kk->waitForConnected(4000)) { qDebug() << "Host Disconnected without saying ! " << kk->errorString(); return; }

        kk->write(crypt::enc("#444$Request#"));
        kk->waitForBytesWritten(1000);

        if(!kk->waitForReadyRead(4000)) { qDebug() << "No response from host ! " << kk->errorString(); return; }

        while(kk->bytesAvailable())
        {
            red += kk->read(kk->bytesAvailable());
            kk->waitForReadyRead(100);
        }
        kk->close();
        red = crypt::dec(red.left(red.length() - 1).toLatin1());
        parse_response(red);
    }
}

void masterobject::parse_response(QString resp)
{
    if(resp.at(0) == '#' && resp.at(resp.length()-1) == '#')
    {
        resp = resp.left(resp.length()-1);
        resp = resp.right(resp.length()-1);
        QStringList list = resp.split("$");

        if(list.count() > 25)
        {
            QString SID = list.at(0);
            QString hostname = list.at(2);
            QString OS = list.at(4);
            QString uptime = list.at(6);
            QString ramt = list.at(8);
            QString ramu = list.at(10);
            QString diskl = list.at(12);
            QString disku = list.at(14);
            QString nbthread = list.at(16);
            QString nbcore = list.at(18);
            QString cpuloadl = list.at(20);
            QString cpuloadll = list.at(22);
            QString cpuloadlll = list.at(24);
            QString cpupercu = list.at(26);
            QString cpupercs = list.at(28);
            QString cpuperci = list.at(30);

            system(QString("echo " + hostname + " >> /var/WSSMaster/Slaves/" + SID + "/hostname").toLatin1());
            system(QString("echo " + OS + " >> /var/WSSMaster/Slaves/" + SID + "/os").toLatin1());
            system(QString("echo " + uptime + " >> /var/WSSMaster/Slaves/" + SID + "/uptime").toLatin1());
            system(QString("echo " + ramt + " >> /var/WSSMaster/Slaves/" + SID + "/ramt").toLatin1());
            system(QString("echo " + ramu + " >> /var/WSSMaster/Slaves/" + SID + "/ramu").toLatin1());
            system(QString("echo " + diskl + " >> /var/WSSMaster/Slaves/" + SID + "/diskl").toLatin1());
            system(QString("echo " + disku + " >> /var/WSSMaster/Slaves/" + SID + "/disku").toLatin1());
            system(QString("echo " + nbthread + " >> /var/WSSMaster/Slaves/" + SID + "/nbthread").toLatin1());
            system(QString("echo " + nbcore + " >> /var/WSSMaster/Slaves/" + SID + "/nbcore").toLatin1());
            system(QString("echo " + cpuloadl + " >> /var/WSSMaster/Slaves/" + SID + "/cpuloadl").toLatin1());
            system(QString("echo " + cpuloadll + " >> /var/WSSMaster/Slaves/" + SID + "/cpuloadll").toLatin1());
            system(QString("echo " + cpuloadlll + " >> /var/WSSMaster/Slaves/" + SID + "/cpuloadlll").toLatin1());
            system(QString("echo " + cpupercu + " >> /var/WSSMaster/Slaves/" + SID + "/cpupercu").toLatin1());
            system(QString("echo " + cpupercs + " >> /var/WSSMaster/Slaves/" + SID + "/cpupercs").toLatin1());
            system(QString("echo " + cpuperci + " >> /var/WSSMaster/Slaves/" + SID + "/cpuperci").toLatin1());
        }
        else qDebug() << "Erreur reception ! " << resp;
    }
}

void masterobject::new_slave(QString s)
{
    QTcpSocket* so = new QTcpSocket();
    so->connectToHost(QHostAddress(s), (quint16) 4441);
    if(!so->waitForConnected()) qDebug() << "Erreur - Host Disconnected" << so->errorString();
    so->write(crypt::enc(QString("#444$NewSlave$" + QString::number(currslave) + "#").toLatin1()));
    so->waitForBytesWritten();
    system("mkdir -p /var/WSSMaster/Slaves/" + QString::number(currslave).toLatin1());
    system(QString("echo 1 > /var/WSSMaster/Slaves/" + QString::number(currslave).toLatin1() + "/active").toLatin1());
    currslave++;
    so->close();
    slaves.append(s);
}

void masterobject::back_slave(QString id, QString s)
{
    QTcpSocket* so = new QTcpSocket();
    so->connectToHost(QHostAddress(s), (quint16) 4441);
    if(!so->waitForConnected()) qDebug() << "Erreur - Host Disconnected" << so->errorString();
    so->write(crypt::enc(QString("#444$NewSlave$" + id + "#").toLatin1()));
    so->waitForBytesWritten();
    so->close();
    system(QString("echo 1 > /var/WSSMaster/Slaves/" + id + "/active").toLatin1());
    slaves.append(s);
}

void masterobject::quit_slave(QString id, QString s)
{
    system(QString("echo 0 > /var/WSSMaster/Slaves/" + id + "/active").toLatin1());
    slaves.removeAll(s);
}
