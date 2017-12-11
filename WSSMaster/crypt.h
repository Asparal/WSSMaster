#ifndef CRYPT_H
#define CRYPT_H

#include <QByteArray>
#include <QString>
#include <QRegExp>

class crypt
{
public:
    crypt();
    static QByteArray enc(QByteArray);
    static QByteArray dec(QByteArray);
    static bool is_base64(QString string);
};

#endif // CRYPT_H
