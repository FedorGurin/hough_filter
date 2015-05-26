#include "mainwindow.h"
#include <QApplication>
#include <omp.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if defined(_OPENMP)
  printf("Compiled by an OpenMP-compliant implementation.\n");
  printf("The result of omp_get_num_threads %i\n", omp_get_num_threads());
#endif


    MainWindow w;
    w.show();


    return a.exec();
}
