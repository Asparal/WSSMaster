#ifndef TCPSERVEUR_H
#define TCPSERVEUR_H

#include <QObject>
#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QList>

class TcpServeur : public QObject
{
    Q_OBJECT
public:
    explicit TcpServeur(int port, QObject *parent = 0);
    QTcpServer* serveur;
    QList <QTcpSocket*> list;
private:
    QString temp;
    bool open;

signals:
    void MessageReceived(QTcpSocket*, QString);

private slots:
    void newConnection();
    void readyRead();
    void disconnected();

public slots:
    void sendtext(QTcpSocket*, QString);
    void forcedeco(QTcpSocket*);
    bool isopen();
};

#endif // TCPSERVEUR_H
