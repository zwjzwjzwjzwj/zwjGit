#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    qApp->setApplicationDisplayName("UDP 9999 端");
    return a.exec();
}
