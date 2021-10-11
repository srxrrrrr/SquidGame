#include "squid_game.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SquidGame w;
    w.show();
    return a.exec();
}
