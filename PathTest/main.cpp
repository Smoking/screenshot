#include "widget.h"
#include <QApplication>
#include "screen.h"
#include "controlwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Screen s;
    s.show();
//    ControlWidget w;
//    w.show();

    return a.exec();
}
