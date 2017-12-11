#include <QCoreApplication>
#include "masterobject.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    masterobject* m = new masterobject();

    return a.exec();
}
