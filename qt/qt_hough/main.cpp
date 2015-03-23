#include <QCoreApplication>
#include <QTime>
#include "hough_c.h"
#include "stdio.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTime t1;t1.start();
    uchar *image=new uchar[1024*768];
    uchar* result= fullHoughTransformAsym(image,40,40, 2);
    QTime t2;t2.msecsTo(t1);
    printf("t=%f\n",t2.msecsTo(t1));
    return a.exec();
}
