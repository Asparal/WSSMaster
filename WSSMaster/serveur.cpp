#include "serveur.h"

TcpServeur::TcpServeur(int port, QObject *parent) : QObject(parent)
{
    open = false;
    serveur = new QTcpServer();
    connect(serveur, SIGNAL(newConnection()), this, SLOT(newConnection()));
    if(serveur->listen(QHostAddress::Any, port)) open = true;
}

void TcpServeur::newConnection()
{
    QTcpSocket* socket = serveur->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    list.append(socket);
}

void TcpServeur::readyRead()
{
    QTcpSocket* sockactuel = qobject_cast<QTcpSocket*>(sender());
    QString read = "";

    while(sockactuel->bytesAvailable())
    {
        read += sockactuel->read(sockactuel->bytesAvailable());
        sockactuel->waitForReadyRead(100);
    }
    emit MessageReceived(sockactuel, read);
}

void TcpServeur::disconnected()
{
    QTcpSocket* sockactuel = qobject_cast<QTcpSocket*>(sender());
    list.removeAll(sockactuel);
    qDebug() << "Deconnexion : " << sockactuel->peerAddress();
}

void TcpServeur::sendtext(QTcpSocket* sock, QString text)
{
    QTextStream flux(sock);
    flux << text << endl;
}

bool TcpServeur::isopen()
{
    return open;
}

void TcpServeur::forcedeco(QTcpSocket* sock)
{
    sock->close();
    list.removeAll(sock);
}
