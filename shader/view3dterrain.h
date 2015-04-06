#ifndef VIEW3DTERRAIN_H
#define VIEW3DTERRAIN_H

#include <GL\glew.h>
#include <QtOpenGL\QGLWidget>
#include <QFileDialog>
#include <QVector>
#include <QThread>
#include <QVector3D>



#include <QString>
#include <QTextStream>


#include "gl_func.h"
typedef struct
{
    long ncols;
    long nrows;

    double xllcorner;
    double yllcorner;



    double cellsize;//��� ����� � ��������
    QVector<QVector<double> >data;
    QVector<QVector<QVector3D> > dataVectors;
    QVector3D value(int col,int row)
    {
       /* double x,z;
        convertSphereToDekart(GradToRadian(yllcorner+ncols/2*cellsize),
                              GradToRadian(xllcorner+nrows/2*cellsize),
                              GradToRadian(yllcorner+col*cellsize),
                              GradToRadian(xllcorner+row*cellsize),
                              x,
                              z
                              );

        return QVector3D(x,data[col][row],z);*/
        return dataVectors[col][row];
    }

}TASCII_MAP;

class ThreadParser : public QThread
 {
 public:
    ThreadParser();
    void setParserFile(QTextStream *out,TASCII_MAP *mapH_);
    void run();
 private:

    TASCII_MAP *mapH;
    QTextStream *out;
 };

class View3DTerrain : public QGLWidget
{
    Q_OBJECT;
public:
    View3DTerrain(QWidget *parent=0);
    //! ������� ���� � ������ ������
    void openFileWithMapColor();

    //! ������� ����� ����������� �����
    bool openTerrainMap(QString nameFile);
    //! ������� �������� ����� ����� � �������� �������
    void processLoading();
    bool loadCompleate;
    ~View3DTerrain();
    TASCII_MAP *mapH;
private:

    QFile *file;
    QTextStream *stream;
    QFileDialog *dialog;
    ThreadParser *threadParser;

public slots:
    void slotAccepted();
    //! �������� ������
    void slotThreadFinished();
    //! ������� ���� � ����������
    void openFileWithTerrain();
};

#endif // VIEW3DTERRAIN_H
