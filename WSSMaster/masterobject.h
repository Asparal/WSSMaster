#ifndef MASTEROBJECT_H
#define MASTEROBJECT_H

#include <QObject>
#include "crypt.h"
#include "sockettcp.h"
#include "socketudp.h"
#include <QUdpSocket>
#include <QStringList>
#include <QTimer>
#include <QDir>

class masterobject : public QObject
{
    Q_OBJECT
public:
    explicit masterobject(QObject *parent = 0);

signals:

private:
    socketUDP* udpt;
    socketTCP* tcpt;
    QList <QString> slaves;
    QTimer* fbeacon;
    QTimer* slask;
    int currslave;

private slots:
    void new_slave(QString);
    void back_slave(QString, QString);
    void quit_slave(QString, QString);
    void beacon();
    void ask();
    void parse_response(QString);
};

#endif // MASTEROBJECT_H
