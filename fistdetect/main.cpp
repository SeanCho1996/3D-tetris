#include "fistdetect.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    fistdetect w;
    w.show();

    return a.exec();
}
