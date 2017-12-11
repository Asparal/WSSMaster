#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include <QObject>
#include <QTcpServer>
#include "crypt.h"
#include "serveur.h"

class socketTCP : public QObject
{
    Q_OBJECT
public:
    explicit socketTCP(QObject *parent = 0);

public slots:
    void sendtext(QTcpSocket*, QString);

signals:
    void newrequest(QTcpSocket* sock);
    void newslave(QString);

private:
    TcpServeur* stcp;

private slots:
    void onReceive(QTcpSocket*, QString);
};

#endif // SOCKETTCP_H
