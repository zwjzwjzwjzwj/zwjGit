#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    qApp->setApplicationDisplayName("UDP 8888 端");
    qApp->setApplicationName("UDP 8888 端");
    return a.exec();
}
