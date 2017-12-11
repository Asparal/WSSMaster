#include "socketudp.h"

socketUDP::socketUDP(QObject *parent) : QObject(parent)
{
    udpsocket = new QUdpSocket();
    udpsocket->bind(QHostAddress::Any, 4442);

    connect(udpsocket, SIGNAL(readyRead()), this, SLOT(udpreceive()));
}

void socketUDP::udpreceive()
{
    QByteArray buffer;
    buffer.resize(udpsocket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderport;

    udpsocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderport);
    QString data = QString(crypt::dec(buffer));
    if(data != "")
    {
        if(data.at(0) == '#' && data.at(data.length()-1) == '#')
        {
            data = data.left(data.length()-1);
            data = data.right(data.length()-1);
            QStringList list = data.split("$");
            if(list.at(1) == "newmaster")
            {
                if(list.at(0) == "x")
                {
                    emit slavedisc(sender.toString());
                }
                else emit slaveback(list.at(0), sender.toString());
            }
            else qDebug() << "Erreur - Requete chelou : " << data;
        }
        else qDebug() << "Erreur - Reception chelou : " << data;
    }
    else qDebug() << "donnee de merde...";
}
