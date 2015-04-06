#ifndef GL_FUNC_H
#define GL_FUNC_H

#define GLM_PRECISION_HIGHP_FLOAT
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
//#include "ibuffer.h"
#include <limits.h>
#include <QDomElement>
///////////////////////////////////////////////////////////////////
#ifndef	EPS
    #define	EPS	0.000000000000001f
#endif

#ifndef	M_PI
    #define M_PI 3.14159265358979323846f
#endif

#ifndef	GravA
    #define GravA 9.81
#endif

#define RadtoGrad 180.0/M_PI
#define GradToRad M_PI/180
//////////////////////////////////////////////////////////////////
//! ���������� ��� ���������
//#define TRENAG_COMPILE
//! ����� ���������
const double gradToRad =   M_PI/180.0;
const double radToGrad =   180.0/M_PI;


class TTable2D
{
public:
    double *m;
    unsigned sizeCol;
    unsigned sizeRow;
};
class TTable1D
{
public:
    double *m;
    unsigned sizeCol;
    unsigned sizeRow;
};
//! ���� ������������ ������������ ���
class TAngle
{
public:
    TAngle(double tau_=0.0,double sigma_=0.0):tau(tau_),sigma(sigma_){}
    void set(double tau_,double sigma_){tau=tau_;sigma=sigma_;}

    //! �������� �� ��������� (������������� ����������� - �����)
    double tau;
    //! �������� �� ����������� (������������� ����������� - �����)
    double sigma;

    const TAngle operator + (const TAngle &angle)const
    {
        TAngle a;
        a.sigma=this->sigma+angle.sigma;
        a.tau=this->tau+angle.tau;
        return a;
    }
    const TAngle operator - (const TAngle &angle)const
    {
        TAngle a;
        a.sigma=this->sigma-angle.sigma;
        a.tau=this->tau-angle.tau;
        return a;
    }

    void operator = (const TAngle& angle)
    {
        this->sigma=angle.sigma;
        this->tau=angle.tau;
    }

    void clear()
    {
        tau=0.0;
        sigma=0.0;
    }
};
//! ����������� ���� ����� ����������� ������� ��������� � ������ ����������������� ������� ���������
void earthTo2Angles(glm::vec3 inVec,TAngle *anglesVec);

//! ������ ����� ������� ������� �� ��� ����
glm::mat3 calcTAngleMatrix(TAngle);

//! �������������� �����(� ������ ������� ���������) � ���������� (��� ���� ������������ ������������ ���)
void earthToSGF(const glm::vec3 &inCoord,           /*���������� ������� ����� �������������*/
                const glm::vec3 &inAircraft,        /*���������� ��������*/
                const glm::mat3 &inMatrixAircraft,  /*������� �������� ��������*/
                TAngle *outAngleSGF);               /*���� ������������ ���*/

//! �������������� ���� �����(������������ ���) � ���������� ����� � ������ ������� ��������� � ����������� � ������� dH
void sgfToEarth(const TAngle &inAngleSGF,
                const glm::vec3 &inAircraft,
                const glm::mat3 &inMatrixAircraft,
                const double &dH,
                glm::vec3 *outCoord);

//! ����������� ������������� �������()
void sgfToEarthDirectionVec(TAngle inAngleSGF,
                            glm::vec3 inAircraft,
                            glm::mat3 &inMatrixAircraft,
                            glm::vec3 *outDirectionVec);

//! �������������� �� ������ �� � ����������������� ���������� ��
void convertEarthToGorizontAircraft(glm::vec3 inAircraft,
                                    glm::mat3 matrixPsi,
                                    glm::vec3 inPoint,
                                    glm::vec3 &outPointGor);

//! �������������� �� ����������������� ���������� �� � ������ ��
void convertGorizontAircraftToEarth(glm::vec3 inPointGor,
                                    glm::vec3 inAircraft,
                                    glm::mat3 matrixPsi,
                                    glm::vec3 &outPointEarth);
//! �������������� ��������� �� ����������������� �� � ������ ��
void convertGorizontAircraftToEarth(glm::vec3 inPointGor,
                                    glm::mat3 matrixPsi,
                                    glm::vec3 &outPointEarth);

//! �������� ��� ��������������� ����
void checkSigma(double &sigma,
                double D0x1,
                double D0z1);

//! ���� �������� ������ �� ����
void calcAngleRotateHeadEarth(glm::vec3 &vecAircraft,   /*������ �� ����� �������� ������������ �����*/
                              glm::vec3 &vecDevice,     /*������ �� ����� ����������(����������) �������. ��������*/
                              glm::vec3 &vecOnHead,     /*������ �� ������� ������������ ������ ����������*/
                              glm::vec3 &vecTarget,     /*������ �� ����(����� ��������) ������������ �����*/
                              glm::mat3 &matrixAircraft,/*������� �������� �������� ������������ �����*/
                              TAngle &angle              /*���� �������� ������ ���������� �� ����*/
                              );

void calcAgnleRotateHead(glm::vec3 &vecAircraft,   /*������ �� ����� �������� ������������ �����*/
                         glm::vec3 &vecDevice,     /*������ �� ����� ����������(����������) �������. ��������*/
                         glm::vec3 &vecOnHead,     /*������ �� ������� ������������ ������ ����������*/
                         glm::vec3 &vecTarget,     /*������ �� ����(����� ��������) ������������ �����*/
                         glm::mat3 &matrixAircraft,/*������� �������� �������� ������������ �����*/
                         glm::mat3 &matrixDevice,  /*������� �������� ���������� �����. ��������*/
                         TAngle &angle,               /* ���� �������� ������ ���������� �� ���� tau=(-90;90) sigma=(-180;180)*/
                         TAngle &angleNeg             /* ���� �������� ������ ���������� �� ���� tau=(-180;180) sigma=(-90;90)*/
                         );
//! �������� ������������
double linearInterpolation(double Y,double Y2,double Y1,double X2,double X1);

//! �������������� ����������� � ���������
//! lambda0,fi0 - �������������� ����� ������
//! lambda, fi  - ������� ����������, ������� ����� ����������� � �������������
void convertSphereToDekart(double lambda0   /*���*/,    double fi0  /*���*/,
                           double lambda    /*���*/,    double fi   /*���*/,
                           double &x        /*�����*/,  double &z   /*�����*/);

//! �������������� ���������� � �����������
void convertDekartToSphere(double lambda0   /*���*/,    double fi0  /*���*/,
                           double x         /*�����*/,  double z    /*�����*/,
                           double &lambda   /*���*/,    double &fi  /*���*/);
///////////////////////////////////////////////////////
//! ������� �������������� �������������� � ����������
void convertGeoToDekart(double lambda0,double fi0,
                        double lambda,double fi,
                        double &x,double &z);

//! ������� �������������� �������������� � ����������
void convertDekartToGeo(double lambda0,double fi0,
                        double x,double z,
                        double &lambda,double &fi);
////////////////////////////////////////////////////////

double dTo(glm::vec3 &vecBegin,glm::vec3 &vecEnd);


//! �������������� ��������� ����� � ����� ������� ��������� � ������
//TPoint convertToStartAircraft(TPoint rVectorOut, TPoint rVectorTo);
int searchColIndexInRow(double X,
                        double *mas,
                        int sizeCol);
int searchRowIndexIn2DTable(double Y,           /*���. �������� � ������� 0*/
                            double *mas,        /*��������� �� ������*/
                            unsigned sizeCol,   /*���-�� ��������*/
                            unsigned sizeRow);   /*���-�� �����*/
int searchColIndexIn2DTable(double X,           /*���. �������� � ������ 0*/
                            double *mas,        /*��������� �� ������*/
                            unsigned sizeCol);   /*���-�� ��������*/
//! ����������� ��� ����� ������
double rowInterpolation(double Y,double *masY, int sizeCol,int indexCol=-1, int indexRow=1);
double rowInterpolation(double Y,const TTable1D &table,int indexCol, int indexRow);


//! �������� ������������ ���������� �������
double linearInter2DTable(double X,double Y,double *masXY,unsigned sizeRow,unsigned sizeCol,int indexRow=-1,int indexCol=-1);
double linearInter2DTable(double X,double Y,const TTable2D &table,int indexRow,int indexCol);



//! ��������� �������� ��� ��������� � ����� ���������� ����
// ���������� � �������� ������ ������������ �����
/* ������������ ��� ������ �������� �������� ��� ������� ������������� E
� ���� ������ ������� ����� �������� E , �� ������������� ������� ����,
 ��� ����� ��������� ��� � ������������ ������������, ���������� ������ � ��������������;

� ���� ������ ������� ������� �������� E , �� ������������� ������� ������,
 ���������� ��� ������ ������������ � ��� �������� �� ������ �����.

 ��� ������ ���������� ������� �� ����� ����� E=0.02
*/
void simulateTrajectoryASP(glm::vec3 inStartASP,       /*���������� ��� � ��������� ������ ������� (�)*/
                           glm::vec3 inSpeedASP,       /*���������� ������� �������� � ��������� ������ (�/�)*/
                           glm::vec3 inEndASP,         /*���������� ����(�������� ����� ������� ���) (�)*/
                           double inTimeMoveASP,       /*����� �������� ���(���.)*/
                           double inE,                 /*������� �������� ������� ������������� ��� (�/�)*/
                           double inCurrentT,          /*������� �������� �������(���.)*/
                           glm::vec3 &curentSpeedASP,  /*������� ���������� �������� ���-�������� ���������� (�/�)*/
                           glm::vec3 &curentCoordASP   /*������� ���������� ���-�������� ���������� (�)*/
                           );


//! �������������� �������� � �������
double gradToRadian(double x);
double radianToGrad(double x);
TAngle radianToGrad(TAngle);
TAngle gradToRadian(TAngle);
double kmToMeter(double x);
double meterToKm(double x);
double cutValues(double x);
double signF(double num,double s);
double signF(double);
double sigmoid(double value);
double limitArc(double x);
//! �������������� [180.0;-180] � [0;360.0]
double psiToKurs(double);
//! �������� ������ �� [180;-180]
double checkPI(double);
double check180(double);
double ugol_ogrMPI_toMPI(double value);
//! �������������� [0;360.0] � [180.0;-180]
double kursToPsi(double);
//! �������� ������ �� [0;-360]
double check2PI(double);
double check360(double);
//! ����������� ������ � �����
/*template<typename T> T limitMinMax(T value,T min,T max)
{
    if(value<min) value=min;
    else if(value>max) value=max;

    return value;
}*/
//! ����������� ������ � ����� c ��������� ������ �� �����������
template<typename T> T limitMinMax(T value,T min,T max,bool *limit=0)
{
    if(limit!=0)
        limit=false;
    if(value<min)
    {
       value=min;
       if(limit!=0)
           *limit=true;
    }
    else if(value>max)
    {
        value=max;
        if(limit!=0)
            *limit=true;
    }
    return value;
}
//! ��������������� �����
double asinh(double x);
//! ����������� ������
double limitMax(double value,double max);
//! ����������� �����
double limitMin(double value,double min);
//! ������ ������ � 2D �������
void readDataToMatrix(const QDomElement &root, const QString &nameMatrix, TTable2D &pM);
//! ������ ������ � 1D �������
void readDataToVector(const QDomElement& root, const QString& nameVector,TTable1D &pM);
//! �������������� ������
class FilterAperiodic
{
public:
    FilterAperiodic(double valueStart_,double k_,double step_,double maxValue_, double minValue_);
    //! ����� �������
    void resetFilter(double valueStart_);
    //! ��������� �������� �������� �������
    double filterValue(double valueCurr);
    //! ���������� �������(� �������� ��������)
    void refresh();
    //! ������� �������� �������� �������
    double setFilterValue(double currentValue);
    //! ������� ��������
    double curValue(){return value;}
    //! �������� ����� ������� � ���������� ���������
    double delta(){return delta_;}
    //! ������ ����������� ��� �����
    void setK(double k_);
private:
    double valueStart;
    double valueCurrent;//�������� �� ����� � ������
    double maxValue;
    double minValue;

    double value;//������� ��������� �������
    double step;
    double k;

    double delta_;
};
class FiltersAperiodicAngle
{
public:
    FiltersAperiodicAngle(TAngle valueStart_,
                          double k_,
                          double step_,
                          double maxValue_,
                          double minValue_);
    //! ����� �������
    void resetFilter(TAngle valueStart_);
    //! ��������� �������� �������� �������
    TAngle filterValue(TAngle valueCurr);
    //! ���������� �������(� �������� ��������)
    void refresh();
    //! ������� �������� �������� �������
    TAngle setFilterValue(TAngle currentValue);
    //! ������ ����������� ��� �����
    void setK(double k_);

private:

    FilterAperiodic *filterSigma;
    FilterAperiodic *filterTau;
};

//! ����� ������� 6 �������
/*
Butcher I.C. On Runge-Ketta processes of high order,
Y.Austral.Math.Soc., Vol.4, Part 2, p.179-194. - 1964
*/
template<typename T,int nParam>
void odd_Butcher_6order(T* const obj,
                    int nSDU,
                    double *inValue,
                    double *outValue,
                    double step)
{
    double tempSDU[nParam];
    double c2=1.0/2.0,      c3=2.0/3.0,     c4=1.0/3.0,     c5=5.0/6.0,     c6=1.0/6.0,     c7=1.0;

    double a21=1.0/2.0;
    double a31=2.0/9.0,     a32=4.0/9.0;
    double a41=7.0/36.0,    a42=2.0/9.0,    a43=-1.0/12.0;
    double a51=-35.0/144.0, a52=-55.0/36.0, a53=35.0/48.0,  a54=15.0/8.0;
    double a61=-1.0/360.0,  a62=-11.0/36.0, a63=-1.0/8.0,   a64=1.0/2.0,    a65=1.0/10.0;
    double a71=-41.0/260.0, a72=22.0/13.0,  a73=43.0/156.0, a74=-118.0/39.0,a75=32.0/195.0,a76=80.0/39.0;

    double b1=13.0/200.0,   b2=0.0,       b3=11.0/40.0,   b4=11.0/40.0,   b5=4.0/25.0,   b6=4.0/25.0,    b7=13.0/200.0;
    //////////////////////////////////////////////////////////////////////////

    double k1[nParam],k2[nParam],k3[nParam],k4[nParam],k5[nParam],k6[nParam],k7[nParam];

    for(int i=nSDU;i<nParam;i++) tempSDU[i]=inValue[i];

    obj->SDU(inValue,k1);

    for(int i=1;i<nSDU;i++) tempSDU[i]=inValue[i]+step*a21*k1[i];
    tempSDU[0]=inValue[0]+c2*step;
    obj->SDU(tempSDU,k2);

    for(int i=1;i<nSDU;i++) tempSDU[i]=inValue[i]+step*(a31*k1[i]+a32*k2[i]);
    tempSDU[0]=inValue[0]+c3*step;
    obj->SDU(tempSDU,k3);

    for(int i=1;i<nSDU;i++) tempSDU[i]=inValue[i]+step*(a41*k1[i]+a42*k2[i]+a43*k3[i]);
    tempSDU[0]=inValue[0]+c4*step;
    obj->SDU(tempSDU,k4);

    for(int i=1;i<nSDU;i++) tempSDU[i]=inValue[i]+step*(a51*k1[i]+a52*k2[i]+a53*k3[i]+a54*k4[i]);
    tempSDU[0]=inValue[0]+c5*step;
    obj->SDU(tempSDU,k5);

    for(int i=1;i<nSDU;i++) tempSDU[i]=inValue[i]+step*(a61*k1[i]+a62*k2[i]+a63*k3[i]+a64*k4[i]+a65*k5[i]);
    tempSDU[0]=inValue[0]+c6*step;
    obj->SDU(tempSDU,k6);

    for(int i=1;i<nSDU;i++) tempSDU[i]=inValue[i]+step*(a71*k1[i]+a72*k2[i]+a73*k3[i]+a74*k4[i]+a75*k5[i]+a76*k6[i]);
    tempSDU[0]=inValue[0]+c7*step;
    obj->SDU(tempSDU,k7);

    outValue[0]=inValue[0]+step;
    for(int i=1;i<nSDU;i++) outValue[i]=inValue[i]+step*(b1*k1[i]+b2*k2[i]+b3*k3[i]+b4*k4[i]+b5*k5[i]+b6*k6[i]+b7*k7[i]);

}


//! ����� �����-����� 4 ������� (���������)
template<typename T,int nParam /*����� ���-�� ����������*/>
void rk4(T* const obj,      /*������ ������ ������ ��������� ������� SDU*/
         double *inValue,   /*��������� �������� ���, inValue!=0*/
         double *outValue   /*����������� �������� ���, outValue!=0*/,
         int nSDU           /*���-�� ����. ���������*/,
         double step        /*��� ��������������*/
         )
{
    double df[nParam][4];
    double fout[nParam];

    for(int j = 0; j < 4; j++)
    {
        if(j==0)
            obj->SDU(inValue,fout);
        else
            obj->SDU(outValue,fout);

        for(int i = 0; i < nSDU; i++)
        {
            df[i][j] = fout[i] *step;
            switch(j)
            {
            case 0:{outValue[i] = inValue[i] + 0.5 * df[i][j];break;}
            case 1:{outValue[i] = inValue[i] + 0.5 * df[i][j];break;}
            case 2:{outValue[i] = inValue[i] + df[i][j];break;}
            case 3:{outValue[i] = inValue[i] + (df[i][0]+2.0*(df[i][1]+df[i][2])+df[i][3])/6.0;}
            }
        }
        for(int k=nSDU;k<nParam;k++)
        {
            outValue[k]=inValue[k];
        }
    }
}
////! ����� ������ 4-�������
//template<typename T, typename X>
//void adams4(T* const obj,
//            int nSDU,
//            double step)
//{
//    double fout[nSDU];
//    double outValue[nSDU];

//    //! ��������� �����. ��� �����
//    double k_0=55.0/24.0;
//    double k_1=59.0/24.0;
//    double k_2=37.0/24.0;
//    double k_3=9.0/24.0;

//    obj->SDU(obj->buf[0]->current()->masDouble,fout);

//    for(int k=0;k<nSDU;k++)
//    {
//        double fi=obj->buf_p[0]->current()->masDouble[k];
//        double fi_1=obj->buf_p[1]->current()->masDouble[k];
//        double fi_2=obj->buf_p[2]->current()->masDouble[k];
//        double fi_3=obj->buf_p[3]->current()->masDouble[k];

//        double y=obj->buf[0]->current()->masDouble[k];

//        double y1=y+step*(k_0*fi-k_1*fi_1+k_2*fi_2-k_3*fi_3);
//        outValue[k]=y1;
//    }
//    BufferDATA<X> bufferdata;
//    for(int k=0;k<nSDU;k++)
//    {
//        bufferdata.masDouble[k]=outValue[k];
//    }

//    obj->buf.push(bufferdata.value);
//    for(int k=0;k<nSDU;k++)
//    {
//        bufferdata.masDouble[k]=fout[k];
//    }
//    obj->buf_p.push(bufferdata.value);

//}
////! ����� ������ 4-������� (����������-���������)
//template<typename T,typename J,int nParam>
//void adams4_pred_correct(T* const obj,
//            int nSDU,
//            IBuffers<J> *buffers,
//            double step)
//{

//}
//! ����� ������
template<typename T,int nParam>
void euler(T* const obj,
           int nSDU,
           double *inValue,
           double *outValue,
           double step)
{

}
#endif // GL_FUNC_H
