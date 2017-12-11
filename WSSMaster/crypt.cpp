#include "crypt.h"

crypt::crypt()
{

}

QByteArray crypt::enc(QByteArray toenc)
{
    QByteArray temp;
    toenc = toenc.toBase64().toBase64();
    for(int i = 0; i < toenc.length(); i++) temp[i] = toenc[toenc.length()-1-i];
    return temp;
}

QByteArray crypt::dec(QByteArray todec)
{
    QByteArray temp;
    for(int i = 0; i < todec.length(); i++) temp[i] = todec[todec.length()-1-i];
    if(crypt::is_base64(QString(temp)))
    {
        if(crypt::is_base64(QString(QByteArray::fromBase64(temp)))) return QByteArray::fromBase64(QByteArray::fromBase64(temp));
        else return "";
    }
    return "";
}

bool crypt::is_base64(QString string)
{
    QRegExp rx("[^a-zA-Z0-9+/=]");

    if(rx.indexIn(string)==-1 && (string.length()%4) == 0 && string.length()>=4){
        return true;
    }

    return false;
}
