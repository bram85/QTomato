#include <QtGui/QApplication>
#include "qtomatotray.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed( false );

    QTomatoTray qtt;

    return a.exec();
}
