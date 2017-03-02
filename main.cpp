#include <QApplication>
#include "yuntai.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    yuntai w;
    w.show();
    
    return a.exec();
}
