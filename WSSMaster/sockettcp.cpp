#include "sockettcp.h"

socketTCP::socketTCP(QObject *parent) : QObject(parent)
{
    stcp = new TcpServeur(4442);

    connect(stcp, SIGNAL(MessageReceived(QTcpSocket*,QString)), this, SLOT(onReceive(QTcpSocket*,QString)));
}

void socketTCP::onReceive(QTcpSocket* sock, QString mess)
{
    QString data = QString(crypt::dec(mess.toLatin1()));
    if(data != "")
    {
        if(data.at(0) == '#' && data.at(data.length()-1) == '#')
        {
            data = data.left(data.length()-1);
            data = data.right(data.length()-1);
            QStringList list = data.split("$");
            if(list.at(0) == "444")
            {
                if(list.at(1) == "Request") emit newrequest(sock);
                else if(list.at(1) == "NewSlave") emit newslave(list.at(2));
                else qDebug() << "Erreur - Demande chelou : " << data;
            }
            else qDebug() << "Erreur - Sender chelou : " << data;
        }
        else qDebug() << "Erreur - Reception chelou : " << data;
    }
    else qDebug() << "donnee de merde...";
}

void socketTCP::sendtext(QTcpSocket* sock, QString m)
{
    stcp->sendtext(sock, m);
}
