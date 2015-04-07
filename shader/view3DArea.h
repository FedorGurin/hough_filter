#ifndef VIEW3DAREA_H
#define VIEW3DAREA_H

#include <GL\glew.h>
#include <QtOpenGL\QGLWidget>
//#include <QGLViewer\qglviewer.h>
#include <GL\GL.h>


#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QLocale>
#include <math.h>


#include <QtOpenGL\QGLShaderProgram>



//! класс 3D визуализации
class View3DArea:public QGLWidget
{
    QGLShaderProgram *prg;
    quint32 vb;
    //GLuint theProgram;
    
    GLuint offsetUniform;
    GLuint frustumScaleUnif;
    GLuint zNearUnif, zFarUnif;
    //GLuint vertexBufferObject;
    GLuint vao;
    GLuint perspectiveMatrixUnif;

    Q_OBJECT;
public:
    //! 3D просмотр области визуализации
    View3DArea();
protected :
    virtual void draw();
    virtual void animate();
    virtual void init();
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
private:
  
};

#endif // VIEW3DAREA_H
