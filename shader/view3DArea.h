#ifndef VIEW3DAREA_H
#define VIEW3DAREA_H

#include <GL\glew.h>
#include <QtOpenGL\QGLWidget>
#include <QGLViewer\qglviewer.h>
#include <GL\GL.h>


#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QLocale>
#include <math.h>

#include "view3dTerrain.h"

#include "Matrix3D_D.h"
#include "Vector3D_D.h"
#include "visudp.h"
#include "viewILS.h"
#include "formsettings.h"
#include "TemplateNASP.hpp"
#include <QtOpenGL\QGLShaderProgram>
//! �������� ����������� ����������
typedef struct _TTrajectory
{
    //! ������ � ������������
    QVector<float> x;
    QVector<float> y;
    QVector<float> z;
    //! ���� ����������
    QVector<int>    r;
    QVector<int>    g;
    QVector<int>    b;

}TTrajectory;

//! ��������� ����������� ������ � �������
typedef struct _TDataRow
{
    //! ������������� �������
    int id_obj;
    //! � ����������� �� ���� ����� ������������ 3D ������
    int code;
    //! ������� 3D �������, �/�
    float scale;
    //! �����, ���
    double time;
    //! ����
    int r;
    int g;
    int b;
    //! ���������� �������, �����
    float x_g;
    float y_g;
    float z_g;
    //! ���� ���������� �������, ����
    float psi;
    float gamma;
    float tan;
}TDataRow;

typedef struct
{
    //! ��� �������
    int code;
    //! �������� ����� � ��������
    //Lib3dsFile *file;
}T3DObject;
//! ����� 3D ������������
class view3DArea:public QGLViewer
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
    //! 3D �������� ������� ������������
    view3DArea();
    //! ��������� ������ ������
    void setCameraToObject(int index);
    //! ���������
    void cameraToObject();
    //! ��������� ���� � 3D �������
    //void loadFile(QString nameFile,Lib3dsFile **file3ds);
    //! ������ ���� �������
    void readAllModels();
    //! ����� ������� �� ����
    T3DObject* findObjByCode(int code);
    //! ������������� �����
    //void initScene(Lib3dsFile *file3ds);
    //! ��������� ��������
    //void drawSolidObjects();
    //! ��������� �������
    /*void drawObject(Lib3dsFile *obj,
                    double pos_x,
                    double pos_y,
                    double pos_z,
                    double psi_grad,
                    double gamma_grad,
                    double tan_grad);*/
    //! ������� �������� � �������
    void gradToPixel(double xGrad,
                     double yGrad,
                     int &xPixel,
                     int &yPixel);
    //! ��������� �������
    void drawTerra();
    //! ��������� ����
    void drawSky();
    //! ��������� �����
    void drawILS();
    //! ��������� ���������� ��������
    void drawSymbol();
    //! ��������� ����������
    void drawCircle(TAngle *angle,int radius_,int width_);
    //! ��������� �����������
    void drawCross(TAngle* angle,int radius_,int width_);
    //! ��������� ���������� �� ��������
    void drawTrajectory();
    //! ��������� ������ ���������
    void drawStateLine();
    //! ������ ���������� �����
    void parserTXTFile();
    //! ����� ������� �� ���������
    int searchTimeInterval(double time);
    //! ������ �������������� ������� ��������
    Matrix3D_D signleCalcMatrix(TVis *solid);
    //! ���������� ������ � �� ����
    Vector3D_D vecCameraInTargetSystem;
    Vector3D_D vecCameraInGeoSys;
    //! ������ �����, �����
    double radiusScene;
    //! ����������� �� ���
    LimitPositionKAI *limit;
    //! ��������� �����
    //void renderNode(Lib3dsFile *,Lib3dsNode *node);
    void computePositionOffsets(float &fXOffset, float &fYOffset);
protected :
    virtual void draw();
    virtual void animate();
    virtual void init();
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
private:
    //! ��������� �������, �/�
    float multTime;
    //! ���������� �����, ���
    double globalTime;
    //! ������ ��������, ��
    int dt;//����
    //! ���������� �������
    int globalCount;
    //! ��� ������ TXT ����� � �������
    QFile *file;
    QTextStream *stream;
    QFileDialog *dialog;
    //! ���������� ���������� �����(������ �����)
    QVector<TDataRow> rows;
    //! ������� ������ �� ������� ����������� ������
    TVis *cameraToThisObj;
    //! ������ ������� � �������� ��������� �����
    int cameraToObjIndex;
    //! ������� ��������� ������
    bool fog;
    //! ������ ��� �� �������
    bool cameraToBack;
    //! ������� ��������� ���
    bool ils;
    //! ������� ��������� �����
    bool grid;
    //! ������� ���������� txt ������ �� �����
    bool dataFromTXTFile;
    //! ������� ��������� ������������ �����
    bool vertLine;
    //! ������� ��������� ���������� � �������� ����
    bool info;
    //! ������� ��������� ���������
    bool terra;
    //! ������� ��������� ����
    bool sky;
    //! ������ ��� ��������� ���
    ViewILS *viewILS;

    double curPsi;
    double curGamma;
    double curTeta;
    double curVy;
    double curV;
    double curY;
    double curAlfa;
    double curNya;
    double curFi;
    double curUnt;
    int curIdMan;
    int curTarget;
    int indexCamForTarget;
    //! ��������� �����������
    FormSettings *setting;
    //! ������ 3�� ������ ��������
    QVector<T3DObject> list3DObj;
    //! ������ ����������
    QVector<TTrajectory> trs;

    //! ��������� �� ������������ ������������ �����������
    View3DTerrain *terrain;
    //! ��������� �����
    qglviewer::ManipulatedFrame* light1;

    float current_frame_aircraft;
    float current_frame_target;
    float current_frame_rocket;
    char* camera_name;
    //! ������� ��������������
    VisUDP net;
    //! ��������� �����
    float horStick;
    float verStick;
    float hotStick_tek;
    float verStick_tek;
    //! ������� �� ������ ����������
    FilterAperiodic* horFilter;
    FilterAperiodic* verFilter;
    //! �������������� ���������� ������
    double lam0,fi0;
public slots:
    void slotAccepted();
    void slotFov(double);
    void slotRes(int w,int h);
    void slotReadRes();
    void slotPosWindow(int x,int y);
    //! ��������� ������
    void setFog(bool);
    //! ��������� ������������ �����
    void setVertical(bool);
    //! ������� ��������� ����
    void openTXTFile();
    //! ��������� ���
    void setILS(bool);
    //! ��������� ����������
    void setInfo(bool);
    //! ��������� ���������
    void setTerra(bool value);
    //! ������� ���������
    void setFreq(int);
    //! ��������� �������
    void setMultTime(double);
};

#endif // VIEW3DAREA_H
