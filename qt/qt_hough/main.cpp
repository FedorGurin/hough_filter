#include <QCoreApplication>
#include "hough_c.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    uchar *image=new uchar[1024*768];
    uchar* result= fullHoughTransformAsym(image,768,1024, 2);
    return a.exec();
}
