#include "math_func.h"

#include <math.h>
//#define USE_MODEL_H
#define USE_SPHERE
//#define USE_GAUSS_KRUGGER

#ifdef USE_MODEL_H
#include "drt\model.h"
#endif

#ifdef USE_GAUSS_KRUGGER
//! �������������� ������� � ������ � ����������
void convertBLtoXY(double fi, double lam, double &n, double &e);
void convertXYtoBL(double n, double e, double &fi, double &lam);
#endif

//#define EPS	0.0001f
/*double limitMinMax(double value,double min,double max)
{
    if(value<min) value=min;
    else if(value>max) value=max;

    return value;
}*/

//! ����������� ������
double limitMax(double value,double max)
{
    if(value>max) value=max;
    return  value;
}
//! ����������� �����
double limitMin(double value,double min)
{
    if(value>min) value=min;
    return  value;
}
//! �������� ������������ ����� ����� ����������
double linearInterpolation(double Y,double Y2,double Y1,double X2,double X1)
{
    double a=0.0,b=0.0;

    a=(Y2-Y1)/(X2-X1);

    b=Y1-a*X1;

    return a*Y+b;
}
int searchColIndexInRow(double X,
                        double *mas,
                        int sizeCol)
{
    for(int i=0;i<sizeCol;i++)
    {
        if(mas[i]>X)
        {
           return i;
        }

    }
}
//! ����� ������� ������ � �������
int searchRowIndexIn2DTable(double Y,           /*���. �������� � ������� 0*/
                            double *mas,        /*��������� �� ������*/
                            unsigned sizeCol,   /*���-�� ��������*/
                            unsigned sizeRow)   /*���-�� �����*/
{
    //! �������� �������
    for(unsigned i=0;i<sizeRow;i++)
    {
        if(mas[i*sizeCol]>=Y)
            return i;
    }
    return 0;
}
//! ����� ������� ������� � ������
int searchColIndexIn2DTable(double X,           /*���. �������� � ������ 0*/
                            double *mas,        /*��������� �� ������*/
                            unsigned sizeCol)   /*���-�� ��������*/
{
    X=limitMinMax(X,mas[1],mas[sizeCol-1]);
    //! �������� �������
    for(unsigned i=1;i<sizeCol;i++)
    {
        if(mas[i]>=X)
            return i;
    }
    return 0;
}
//! ������������ ������ � ����� ������
double rowInterpolation(double Y,const TTable1D &table,int indexCol, int indexRow)
{
    if(indexCol==-1)
    {
        for(unsigned i=0;i<table.sizeCol;i++)
        {
            if(table.m[i]>Y)
            {
                indexCol=i;
                break;
            }

        }
    }

    //! ���� ������ ������� ����� �� ����� �������, �� ������� ������� �������� ����� �������
    if(indexCol<=0) return table.m[table.sizeCol];
    //! ���� ������ ������� ����� �� ������ �������, �� ������� ������� �������� ������ �������
    if(indexCol>(table.sizeCol-1)) return table.m[2*table.sizeCol-1];

    //! ����� ������� �������
    int index=indexCol+indexRow*table.sizeCol;//! ������ ��� ����������� � ���������� �������

    return linearInterpolation(Y,table.m[index],table.m[index-1],table.m[indexCol],table.m[indexCol-1]);
}
//! ������������ ������ � ����� ������
double rowInterpolation(double Y,double *masY, int sizeCol,int indexCol, int indexRow)
{
    if(indexCol==-1)
    {
        for(int i=0;i<sizeCol;i++)
        {
            if(masY[i]>Y)
            {
                indexCol=i;
                break;
            }

        }
    }

    //! ���� ������ ������� ����� �� ����� �������, �� ������� ������� �������� ����� �������
    if(indexCol<=0) return masY[sizeCol];
    //! ���� ������ ������� ����� �� ������ �������, �� ������� ������� �������� ������ �������
    if(indexCol>(sizeCol-1)) return masY[2*sizeCol-1];

    //! ����� ������� �������
    int index=indexCol+indexRow*sizeCol;//! ������ ��� ����������� � ���������� �������

    return linearInterpolation(Y,masY[index],masY[index-1],masY[indexCol],masY[indexCol-1]);
}
double linearInter2DTable(double X,double Y,double *masXY,unsigned sizeRow,unsigned sizeCol,int indexRow,int indexCol)
{

    double y1=0.0,y2=0.0;

    if(indexCol==-1)
    {
        X=limitMinMax(X,masXY[1],masXY[sizeCol-1]);
        //! �������� �������
        for(unsigned i=1;i<sizeCol;i++)
        {
            if(masXY[i]>=X)
            {
                indexCol=i;
                break;
            }
        }
    }
    if(indexRow==-1)
    {
        //! �������� �������
        for(unsigned i=0;i<sizeRow;i++)
        {
            if(masXY[i*sizeCol]>=Y)
            {
                indexRow=i;
                break;
            }
        }
    }
    indexRow=limitMinMax(indexRow-1,1,(int)(sizeRow-2));
    indexCol=limitMinMax(indexCol-1,1,(int)(sizeCol-1));

    y1=rowInterpolation(X,masXY,sizeCol,indexCol+1,indexRow);
    y2=rowInterpolation(X,masXY,sizeCol,indexCol+1,indexRow+1);

    return linearInterpolation(Y,y2,y1,masXY[sizeCol*(indexRow+1)],masXY[sizeCol*indexRow]);

}
double linearInter2DTable(double X, double Y, const TTable2D &table, int indexRow, int indexCol)
{

    double y1=0.0,y2=0.0;

    if(indexCol==-1)
    {
        X=limitMinMax(X,table.m[1],table.m[table.sizeCol-1]);
        //! �������� �������
        for(unsigned i=1;i<table.sizeCol;i++)
        {
            if(table.m[i]>=X)
            {
                indexCol=i;
                break;
            }
        }
    }
    if(indexRow==-1)
    {
        //! �������� �������
        for(unsigned i=0;i<table.sizeRow;i++)
        {
            if(table.m[i*table.sizeCol]>=Y)
            {
                indexRow=i;
                break;
            }
        }
    }
    indexRow=limitMinMax(indexRow-1,1,(int)(table.sizeRow-2));
    indexCol=limitMinMax(indexCol-1,1,(int)(table.sizeCol-1));

    y1=rowInterpolation(X,table.m,table.sizeCol,indexCol+1,indexRow);
    y2=rowInterpolation(X,table.m,table.sizeCol,indexCol+1,indexRow+1);

    return linearInterpolation(Y,y2,y1,table.m[table.sizeCol*(indexRow+1)],table.m[table.sizeCol*indexRow]);

}
//double linearInter2DTable(double X,double Y,boost::multi_array<double, 2> A,int indexRow,int indexCol)
//{

//}

//! ����������� ���� ����� ����������� ������� ��������� � ������ ������� ���������
void earthTo2Angles(glm::vec3 inVec,TAngle *anglesVec)
{
    //! ��������� ������ D, �.�. � ������� ��������� ������ D0;
    double length=glm::length(inVec);

    double D0x1,D0y1,D0z1;

    D0x1=inVec.x/length;
    D0y1=inVec.y/length;
    D0z1=inVec.z/length;

    //double d=D0x1*D0x1+D0y1*D0y1+D0z1*D0z1;
    //������� ��������� ��������� ���������� �������   ����� ���� sigma, tau

    double sigma,tau;

    sigma=atan2(D0z1,D0x1);
    checkSigma(sigma,D0x1,D0z1);
    tau=asin(D0y1);

    anglesVec->sigma=sigma;
    anglesVec->tau=tau;
}

//! ������ ����� �������� ������ ������������ ����������
void calcAgnleRotateHead(glm::vec3 &vecAircraft,     /* ������ �� ����� �������� ������������ �����*/
                         glm::vec3 &vecDevice,       /* ������ �� ����� ����������(����������) �������. ��������*/
                         glm::vec3 &vecOnHead,       /* ������ �� ������� ������������ ������ ����������*/
                         glm::vec3 &vecTarget,       /* ������ �� ����(����� ��������) ������������ �����*/
                         glm::mat3 &matrixAircraft,  /* ������� �������� �������� ������������ �����*/
                         glm::mat3 &matrixDevice,    /* ������� �������� ���������� �����. ��������*/
                         TAngle &angle,               /* ���� �������� ������ ���������� �� ���� tau=(-90;90) sigma=(-180;180)*/
                         TAngle &angleNeg             /* ���� �������� ������ ���������� �� ���� tau=(-180;180) sigma=(-90;90)*/)
{
    // ������ ������������ ������� ������������ ����������
    glm::vec3 vecDeviceHead(0.0,0.0,0.0);
    vecDeviceHead=vecDevice+vecOnHead;
    // �������������� ������� ��������
    glm::mat3 matrixDevice_Aircraft=matrixAircraft*matrixDevice;
    //��������� ���������� ������� �������� deltaRxdD   ������������ ��������� �� ����������
    glm::vec3 vecDeltaRxdD=matrixDevice*vecDeviceHead;

    //��������� ���������� ���������������� ������� deltaRp  ������������ ���������� ��
    glm::vec3 vecDeltaRxp=vecTarget-vecAircraft;

    //��������� ���������� ���������������� ������� deltaRd  ������������ ��������� �� ����������
    glm::vec3 vecDeltaRxd=vecDeltaRxp*matrixDevice_Aircraft;

    //��������� ���������� ������� ��������� Dt  ������������ ��������� �� ����������
    glm::vec3 vecDt=vecDeltaRxd-vecDeltaRxdD;
    //����� �������
    double Dt=glm::length(vecDt);
    //��������� ����
    angle.sigma=-atan2(vecDt.z,vecDt.x);
    angle.tau=asin(vecDt.y/Dt);
    //��������� ������ ���� �����
    angleNeg.sigma=-asin(vecDt.z/Dt);
    angleNeg.tau=atan2(vecDt.y,vecDt.x);
}
//! ���������� ����� �������� ������������ �����(����� ��� ��������� ������)
void calcAngleRotateHeadEarth(glm::vec3 &vecAircraft,   /*������ �� ����� �������� ������������ �����*/
                              glm::vec3 &vecDevice,     /*������ �� ����� ����������(����������) �������. ��������*/
                              glm::vec3 &vecOnHead,     /*������ �� ������� ������������ ������ ����������*/
                              glm::vec3 &vecTarget,     /*������ �� ����(����� ��������) ������������ �����*/
                              glm::mat3 &matrixAircraft,/*������� �������� �������� ������������ �����*/
                              TAngle &angle              /*���� �������� ������ ���������� �� ����*/)
{
    // ������ ������������ ������� ������������ ����������
    glm::vec3 vecDeviceHead=vecDevice;/*+vecOnHead;*/

    //��������� ���������� ������� �������� delta_r_d  ������������ ���������� ��
    glm::mat3 tempMatrix=glm::transpose(matrixAircraft);
    glm::vec3 vecDeltaRdp=vecDeviceHead*tempMatrix;

    //��������� ���������� ������� ��������� Dt  ������������ ���������� ��
    glm::vec3 vecDtp=vecTarget-vecAircraft-vecDeltaRdp;

    //����� �������
    double Dt=glm::length(vecDtp);

    //��������� ����
    angle.sigma=-atan2(vecDtp.z,vecDtp.x);
    angle.tau=asin(vecDtp.y/Dt);

    //glm::mat3 tempMatrix1=matrixAircraft*matrixHeadOnAircraft;
    //glm::mat3 tempMatrix2=tempMatrix1*matrixSigmaTau;

    //! ���������� ������ ����(������� ������ ���)
    //angle_tek_target_mu=atan2(tempMatrix2.x[2][1],tempMatrix2.x[1][1]);
}
//! ��������� �������� ��� ��������� � ����� ���������� ����
// ���������� � �������� ������ ������������ �����
void simulateTrajectoryASP(glm::vec3 s0,       /*���������� ��� � ��������� ������ �������*/
                           glm::vec3 v0,       /*���������� ������� �������� � ��������� ������*/
                           glm::vec3 e0,       /*���������� ����(�������� ����� ������� ���)*/
                           double T,            /*����� �������� ���(�����)*/
                           double E,            /*������� �������� ������� ������������� ���*/
                           double t,            /*������� �������� �������*/
                           glm::vec3 &v,       /*������� ���������� �������� ���-�������� ����������*/
                           glm::vec3 &coord   /*������� ���������� ���-�������� ����������*/
                           )
{
    glm::vec3 p;//����������� �������

    double temp1=exp(-E*T);
    double temp2=1.0-temp1;
    double temp3=T-temp2/E;

    double temp4=exp(-E*t);
    double temp5=1.0-temp4;


    //! � ��������� ����
    p=(E*(e0-s0)-v0*temp2)/temp3;
    v=p/E+(v0-p/E)*temp4;
    coord=s0+p*t/E+(1/E)*(v0-p/E)*temp5;
}

//! rVectorOut - ; rVectorTo -
/*TPoint convertToStartAircraft(TPoint rVectorOut, TPoint rVectorTo)
{
    TPoint startAircraft;

    startAircraft.x=rVectorTo.x+rVectorOut.x;
    startAircraft.y=rVectorTo.y+rVectorOut.y;
    startAircraft.z=rVectorTo.z+rVectorOut.z;

    return startAircraft;
}*/

//! ����� �����-����� 4 �������
/*double* rk41(short nsdu, double d,double *fb, double *SDU(double f[]))
{
    short i,j;
    double df[10][10];

    static double  f[10];
    double *fout;
    for(j = 0; j < 4; j++)
    {
        if(j==0) fout=SDU(fb);
        else fout=SDU(f);

        for(i = 0; i < nsdu; i++)
        {
            df[i][j] = fout[i] *d;
            switch(j)
            {
            case 0:{f[i] = fb[i] + 0.5 * df[i][j];break;}
            case 1:{f[i] = fb[i] + 0.5 * df[i][j];break;}
            case 2:{f[i] = fb[i] + df[i][j];break;}
            case 3:{f[i] = fb[i]+(df[i][0]+2.0*(df[i][1]+df[i][2])+df[i][3])/6.0;}
            }
        }
    }
    return f;
}*/


double cutValues(double x)//�������� ������� �����
{
    if(fabs(x)<1e-30) return 0.0;
    return x;
}

double signF(double num,double s)
{
    if(s>0.0) num=fabs(num);
    else if(s<0.0)num=-fabs(num);
    return num;
}
double signF(double num)
{
    if(num>=0.0) return 1.0;
    return -1.0;
}
double asinh(double x)
{
    return log(x+sqrt(x*x+1));
}
double sigmoid(double x)
{
    return ((1.0/(1.0+exp(-fabs(x))))-0.5);
}
double limitArc(double x)
{
    if(x<-1.0) return -1.0;
    if(x>1.0) return 1.0;
    return x;
}
double ugol_ogrMPI_toMPI(double value)
{
    return value-2.0*M_PI*signF(value);
}
double gradToRadian(double x)
{
    return (x*M_PI)/180.0;
}
double radianToGrad(double x)
{
     return (x*180)/M_PI;
}
TAngle radianToGrad(TAngle angl)
{
    TAngle a;
    a.sigma=radianToGrad(angl.sigma);
    a.tau=radianToGrad(angl.tau);

    return a;
}
TAngle gradToRadian(TAngle angl_grad)
{
    TAngle a;
    a.sigma=gradToRadian(angl_grad.sigma);
    a.tau=gradToRadian(angl_grad.tau);

    return a;
}
double kmToMeter(double x)
{
   return x/3.6;
}
double meterToKm(double x)
{
    return x*3.6;
}
//! �������� ������ �� [180;-180]
double checkPI(double angle/*� ���*/)
{
    if(angle>M_PI) angle+=-2.0*M_PI;
    else if(angle<-M_PI)angle+=2.0*M_PI;
    return angle;
}
//! �������� ������ �� [0;-360]
double check2PI(double angle/*� ���.*/)
{
    if(angle>(2.0*M_PI)) angle-=2.0*M_PI;
    else if(angle<0.0)angle+=2.0*M_PI;
    return angle;
}
//! �������� ������ �� [180;-180]
double check180(double angle/*� ����*/)
{
    if(angle>180.0) angle+=-2.0*180.0;
    else if(angle<-180.0)angle+=2.0*180.0;
    return angle;
}
//! �������� ������ �� [0;-360]
double check360(double angle/*� ����.*/)
{
    if(angle>(2.0*180.0)) angle-=2.0*180.0;
    else if(angle<0.0)angle+=2.0*180.0;
    return angle;
}
double psiToKurs(double psi_)
{
    psi_=-psi_;
    double kurs_=psi_;
    if(psi_<0.0) kurs_=2*M_PI+psi_;
    return kurs_;
}

double kursToPsi(double kurs)
{
    double psi=kurs;
    if(kurs>M_PI) psi=psi-2.0*M_PI;
    psi=-psi;

    return psi ;
}

//! �������������� �� ������ �� � ����������������� ���������� ��
void convertEarthToGorizontAircraft(glm::vec3 inAircraft,
                                    glm::mat3 matrixPsi,
                                    glm::vec3 inPoint,
                                    glm::vec3 &outPointGor)
{
    //! ������������ ��������
    outPointGor=(inPoint-inAircraft)*matrixPsi;
    //outPointGor=matrixPsi*outPointGor;
}

//! �������������� �� ����������������� ���������� �� � ������ ��
void convertGorizontAircraftToEarth(glm::vec3 inPointGor,
                                    glm::vec3 inAircraft,
                                    glm::mat3 matrixPsi,
                                    glm::vec3 &outPointEarth)
{
    outPointEarth=glm::transpose(matrixPsi)*inPointGor+inAircraft;
}
void convertGorizontAircraftToEarth(glm::vec3 inPointGor,
                                    glm::mat3 matrixPsi,
                                    glm::vec3 &outPointEarth)
{
    outPointEarth=inPointGor*glm::transpose(matrixPsi);
}
//! ������� �������������� �������������� � ����������
void convertGeoToDekart(double lambda0,double fi0,
                        double lambda,double fi,
                        double &x,double &z)
{
#ifdef USE_SPHERE
    convertSphereToDekart(lambda0,fi0,lambda,fi,x,z);
#endif
#ifdef USE_MODEL_H
    count_liner_cord(x,
                     z,
                     radianToGrad(fi),
                     radianToGrad(lambda),
                     radianToGrad(fi0),
                     radianToGrad(lambda0));
#endif
#ifdef USE_GAUSS_KRUGGER
     convertBLtoXY(radianToGrad(fi),radianToGrad(lambda),x,z);
#endif
}

//! ������� �������������� �������������� � ����������
void convertDekartToGeo(double lambda0,double fi0,
                        double x,double z,
                        double &lambda,double &fi)
{
#ifdef USE_SPHERE
        convertDekartToSphere(lambda0,
                              fi0,
                              x,
                              z,
                              lambda,
                              fi);
#endif
#ifdef USE_MODEL_H

        liner_to_geograf(x,
                         z,
                         fi,
                         lambda,
                         radianToGrad(fi0),
                         radianToGrad(lambda0)
                         );
        fi=gradToRadian(fi);
        lambda=gradToRadian(lambda);

#endif
#ifdef USE_GAUSS_KRUGGER
     convertXYtoBL(x,z,fi,lambda);
     fi=gradToRadian(fi);
     lambda=gradToRadian(lambda);
#endif
}

//! �������������� ����������� � ���������
void convertSphereToDekart(double lambda0,double fi0,
                           double lambda,double fi,
                           double &x,double &z)
{

    glm::mat3 Alambda0;
    double cosLambda0=cos(lambda0);
    double sinLambda0=sin(lambda0);

    Alambda0[0][0]=1.0;
    Alambda0[0][1]=0.0;
    Alambda0[0][2]=0.0;

    Alambda0[1][0]=0.0;
    Alambda0[1][1]=cosLambda0;
    Alambda0[1][2]=sinLambda0;

    Alambda0[2][0]=0.0;
    Alambda0[2][1]=-sinLambda0;
    Alambda0[2][2]=cosLambda0;
    /////////////////////////////////////////////////////

    glm::mat3 Afi0;
    double cosfi0=cos(fi0);
    double sinfi0=sin(fi0);

    Afi0[0][0]=cosfi0;
    Afi0[0][1]=-sinfi0;
    Afi0[0][2]=0.0;

    Afi0[1][0]=sinfi0;
    Afi0[1][1]=cosfi0;
    Afi0[1][2]=0.0;

    Afi0[2][0]=0.0;
    Afi0[2][1]=0.0;
    Afi0[2][2]=1.0;
    /////////////////////////////////////////////////////
    glm::mat3 AsG=Alambda0*Afi0;
    ////////////////////////////////////////////////////

    glm::mat3 AgG;

    double cosfi=cos(fi);
    double sinfi=sin(fi);
    double cosLambda=cos(lambda);
    double sinLambda=sin(lambda);

    AgG[0][0]=cosfi;
    AgG[0][1]=-sinfi*cosLambda;
    AgG[0][2]=-sinfi*sinLambda;

    AgG[1][0]=sinfi;
    AgG[1][1]=cosfi*cosLambda;
    AgG[1][2]=cosfi*sinLambda;

    AgG[2][0]=0.0;
    AgG[2][1]=-sinLambda;
    AgG[2][2]=cosLambda;

    glm::mat3 tempAsG=AsG;
    glm::mat3 Asg=AgG*glm::transpose(tempAsG);

    double tetta=asin(Asg[1][0]);
    double psi=atan2(Asg[1][2],Asg[1][1]);

    x=6356767.0*tetta; z=6356767.0*psi;

}

//! �������������� ���������� � �����������
//! ��������� ��������� �� ������������ ������ ��
//!                                             -lambda
//!                                             -fi

//! ��������� ���������� ������� ������������ ��������� ��
//!                                             -x
//!                                             -z
void convertDekartToSphere(double lambda0,double fi0,
                           double x,double z,
                           double &lambda,double &fi)
{
    glm::mat3 Alambda0;
    double cosLambda0=cos(lambda0);
    double sinLambda0=sin(lambda0);

    Alambda0[0][0]=1.0;
    Alambda0[0][1]=0.0;
    Alambda0[0][2]=0.0;

    Alambda0[1][0]=0.0;
    Alambda0[1][1]=cosLambda0;
    Alambda0[1][2]=sinLambda0;

    Alambda0[2][0]=0.0;
    Alambda0[2][1]=-sinLambda0;
    Alambda0[2][2]=cosLambda0;
    /////////////////////////////////////////////////////

    glm::mat3 Afi0;
    double cosfi0=cos(fi0);
    double sinfi0=sin(fi0);

    Afi0[0][0]=cosfi0;
    Afi0[0][1]=-sinfi0;
    Afi0[0][2]=0.0;

    Afi0[1][0]=sinfi0;
    Afi0[1][1]=cosfi0;
    Afi0[1][2]=0.0;

    Afi0[2][0]=0.0;
    Afi0[2][1]=0.0;
    Afi0[2][2]=1.0;
    /////////////////////////////////////////////////////
    glm::mat3 AsG=Afi0*Alambda0;
    ////////////////////////////////////////////////////
    double tetta=x/6356767.0;
    double psi=z/6356767.0;

    glm::mat3 Atetta;
    double cosTetta=cos(tetta);
    double sinTetta=sin(tetta);

    Atetta[0][0]=cosTetta;
    Atetta[0][1]=-sinTetta;
    Atetta[0][2]=0.0;

    Atetta[1][0]=sinTetta;
    Atetta[1][1]=cosTetta;
    Atetta[1][2]=0.0;

    Atetta[2][0]=0.0;
    Atetta[2][1]=0.0;
    Atetta[2][2]=1.0;


    glm::mat3 Apsi;

    double cosPsi=cos(psi);
    double sinPsi=sin(psi);

    Apsi[0][0]=1.0;
    Apsi[0][1]=0.0;
    Apsi[0][2]=0.0;

    Apsi[1][0]=0.0;
    Apsi[1][1]=cosPsi;
    Apsi[1][2]=sinPsi;

    Apsi[2][0]=0.0;
    Apsi[2][1]=-sinPsi;
    Apsi[2][2]=cosPsi;

    glm::mat3 tempMatrix=Atetta*AsG;
    glm::mat3 AgG=Apsi*tempMatrix;

    fi=asin(AgG[1][0]);
    lambda=atan2(AgG[1][2],AgG[1][1]);

}
void sgfToEarthDirectionVec(TAngle inAngleSGF,glm::vec3 inAircraft,glm::mat3 &inMatrixAircraft,glm::vec3 *outDirectionVec)
{
    //===================================================
    glm::mat3 matrixSigma, matrixTau;

    matrixSigma[0][0]=cos(inAngleSGF.sigma);
    matrixSigma[0][1]=0.0;
    matrixSigma[0][2]=-sin(inAngleSGF.sigma);

    matrixSigma[1][0]=0.0;
    matrixSigma[1][1]=1.0;
    matrixSigma[1][2]=0.0;

    matrixSigma[2][0]=sin(inAngleSGF.sigma);
    matrixSigma[2][1]=0.0;
    matrixSigma[2][2]=cos(inAngleSGF.sigma);
    //===================================================

    matrixTau[0][0]=cos(inAngleSGF.tau);
    matrixTau[0][1]=sin(inAngleSGF.tau);
    matrixTau[0][2]=0.0;

    matrixTau[1][0]=-sin(inAngleSGF.tau);
    matrixTau[1][1]=cos(inAngleSGF.tau);
    matrixTau[1][2]=0.0;

    matrixTau[2][0]=0.0;
    matrixTau[2][1]=0.0;
    matrixTau[2][2]=1.0;

    //! �������������� ������� ��������
    glm::mat3 matrixResultRotate;
    glm::mat3 tempMatrix;

    tempMatrix=matrixSigma*inMatrixAircraft;
    matrixResultRotate=matrixTau*tempMatrix;

    double D0xg,D0yg,D0zg;

    D0xg=matrixResultRotate[0][0];
    D0yg=matrixResultRotate[0][1];
    D0zg=matrixResultRotate[0][2];

    /*double u=-inAircraft.y/D0yg;

    outDirectionVec->x=inAircraft.x+u*D0xg;
    outDirectionVec->y=inAircraft.y+u*D0yg;
    outDirectionVec->z=inAircraft.z+u*D0zg;*/

    outDirectionVec->x=D0xg;
    outDirectionVec->y=D0yg;
    outDirectionVec->z=D0zg;
}

//! �������������� �����(� ������ ������� ��������) � ���������� ���(��� ���� ������������ ������������ ���)
void earthToSGF(const glm::vec3 &inCoord,const glm::vec3 &inAircraft, const glm::mat3 &inMatrixAircraft,  TAngle *outAngleSGF)
{
    double Dxg,Dyg,Dzg;

    Dxg=(inCoord.x)-(inAircraft.x);
    Dyg=(inCoord.y)-(inAircraft.y);
    Dzg=(inCoord.z)-(inAircraft.z);

    //! ������� ���������� ������� D  ������������ ��������� ������� ���������
    double Dx1,Dy1,Dz1;
    Dx1=Dxg*inMatrixAircraft[0][0]+Dyg*inMatrixAircraft[0][1]+Dzg*inMatrixAircraft[0][2];
    Dy1=Dxg*inMatrixAircraft[1][0]+Dyg*inMatrixAircraft[1][1]+Dzg*inMatrixAircraft[1][2];
    Dz1=Dxg*inMatrixAircraft[2][0]+Dyg*inMatrixAircraft[2][1]+Dzg*inMatrixAircraft[2][2];

    /*Dx1=Dxg*inMatrixAircraft.x[0][0]+Dyg*inMatrixAircraft.x[1][0]+Dzg*inMatrixAircraft.x[2][0];
    Dy1=Dxg*inMatrixAircraft.x[0][1]+Dyg*inMatrixAircraft.x[1][1]+Dzg*inMatrixAircraft.x[2][1];
    Dz1=Dxg*inMatrixAircraft.x[0][2]+Dyg*inMatrixAircraft.x[1][2]+Dzg*inMatrixAircraft.x[2][2];*/

   // glm::vec3 vec=inMatrixAircraft*
   /* Dx1=Dxg*inMatrixAircraft.x[0][0]+Dyg*inMatrixAircraft.x[1][0]+Dzg*inMatrixAircraft.x[2][0];
    Dy1=Dxg*inMatrixAircraft.x[0][1]+Dyg*inMatrixAircraft.x[1][1]+Dzg*inMatrixAircraft.x[2][1];
    Dz1=Dxg*inMatrixAircraft.x[0][2]+Dyg*inMatrixAircraft.x[1][2]+Dzg*inMatrixAircraft.x[2][2];*/


    //! ��������� ������ D, �.�. � ������� ��������� ������ D0;
    double length=sqrt(Dx1*Dx1+Dy1*Dy1+Dz1*Dz1);

    double D0x1,D0y1,D0z1;
    D0x1=Dx1/length;
    D0y1=Dy1/length;
    D0z1=Dz1/length;
    //double d=D0x1*D0x1+D0y1*D0y1+D0z1*D0z1;
    //������� ��������� ��������� ���������� �������   ����� ���� sigma, tau

    double sigma,tau;

    sigma=atan2(D0z1,D0x1);
    checkSigma(sigma,D0x1,D0z1);
    tau=asin(D0y1);

    outAngleSGF->sigma=sigma;
    outAngleSGF->tau=tau;
}
double dTo(glm::vec3 &vecBegin,glm::vec3 &vecEnd)
{
    return (glm::length(vecEnd)-glm::length(vecBegin));
}
//! �������������� ���� �����(������������ ���) � ������ �������� ��������������� ��������
void sgfToEarth(const TAngle &inAngleSGF,const glm::vec3 &inAircraft,const glm::mat3 &inMatrixAircraft,const double &dH,glm::vec3 *outCoord)
{
    //===================================================
    glm::mat3 matrixSigma, matrixTau;

    matrixSigma[0][0]=cos(inAngleSGF.sigma);
    matrixSigma[0][1]=0.0;
    matrixSigma[0][2]=-sin(inAngleSGF.sigma);

    matrixSigma[1][0]=0.0;
    matrixSigma[1][1]=1.0;
    matrixSigma[1][2]=0.0;

    matrixSigma[2][0]=sin(inAngleSGF.sigma);
    matrixSigma[2][1]=0.0;
    matrixSigma[2][2]=cos(inAngleSGF.sigma);
    //===================================================

    matrixTau[0][0]=cos(inAngleSGF.tau);
    matrixTau[0][1]=sin(inAngleSGF.tau);
    matrixTau[0][2]=0.0;

    matrixTau[1][0]=-sin(inAngleSGF.tau);
    matrixTau[1][1]=cos(inAngleSGF.tau);
    matrixTau[1][2]=0.0;

    matrixTau[2][0]=0.0;
    matrixTau[2][1]=0.0;
    matrixTau[2][2]=1.0;

    //! �������������� ������� ��������
    glm::mat3 matrixResultRotate;
    glm::mat3 tempMatrix;

    tempMatrix=matrixSigma*inMatrixAircraft;
    matrixResultRotate=matrixTau*tempMatrix;

    double D0xg,D0yg,D0zg;

    D0xg=matrixResultRotate[0][0];
    D0yg=matrixResultRotate[0][1];
    D0zg=matrixResultRotate[0][2];

    //double d=D0xg*D0xg+D0yg*D0yg+D0zg*D0zg;
    //if(d>1.0) qDebug("Error in NASP code: function=sgfToEarth, vector!=1.0\n");

    //double u=-inAircraft.y/D0yg;
    double u=(dH-inAircraft.y)/D0yg;

    outCoord->x=inAircraft.x+u*D0xg;
    outCoord->y=inAircraft.y+u*D0yg;
    outCoord->z=inAircraft.z+u*D0zg;
}
void checkSigma(double &sigma,double D0x1, double D0z1)
{
    if(D0x1>0.0) sigma=-atan(D0z1/D0x1);

    if(fabs(D0x1)<EPS && D0z1>0.0) sigma=-M_PI/2.0;
    if(fabs(D0x1)<EPS && D0z1<0.0) sigma=M_PI/2.0;

    if(D0x1<0.0 && D0z1>0.0) sigma=-atan(D0z1/D0x1)-M_PI;
    if(D0x1<0.0 && D0z1<0.0) sigma=-atan(D0z1/D0x1)+M_PI;



    /*if(fabs(D0z1)<EPS && D0x1>0.0) sigma=0.0;
    if(D0z1==0.0 && D0x1<0.0) sigma=M_PI;//+-M_PI*/
}
glm::mat3 calcTAngleMatrix(TAngle angle)
{
    //===================================================
    glm::mat3 matrixSigma, matrixTau;

    matrixSigma[0][0]=cos(angle.sigma);
    matrixSigma[0][1]=0.0;
    matrixSigma[0][2]=-sin(angle.sigma);

    matrixSigma[1][0]=0.0;
    matrixSigma[1][1]=1.0;
    matrixSigma[1][2]=0.0;

    matrixSigma[2][0]=sin(angle.sigma);
    matrixSigma[2][1]=0.0;
    matrixSigma[2][2]=cos(angle.sigma);
    //===================================================

    matrixTau[0][0]=cos(angle.tau);
    matrixTau[0][1]=sin(angle.tau);
    matrixTau[0][2]=0.0;

    matrixTau[1][0]=-sin(angle.tau);
    matrixTau[1][1]=cos(angle.tau);
    matrixTau[1][2]=0.0;

    matrixTau[2][0]=0.0;
    matrixTau[2][1]=0.0;
    matrixTau[2][2]=1.0;

    //! �������������� ������� ��������

    glm::mat3 tempMatrix;


    tempMatrix=matrixTau*matrixSigma;
    return tempMatrix;
}
//! ������ ������ � 2D �������
void readDataToMatrix(const QDomElement& root, const QString& nameMatrix,TTable2D &pM)
{
    QDomElement ele=root.firstChildElement(nameMatrix);
    pM.sizeCol=ele.firstChildElement("size_cols").text().toInt();
    pM.sizeRow=ele.firstChildElement("size_rows").text().toInt();
    pM.m=new double [pM.sizeCol*pM.sizeRow];

    QDomElement row=ele.firstChildElement("row");
    QDomElement column;
    int indexRow=0;
    int indexCol=0;
    do{
        column=row.firstChildElement("column");
        indexCol=0;
        do
        {
            pM.m[indexRow*pM.sizeCol+indexCol]=column.firstChildElement("value").text().toDouble();
            column=column.nextSiblingElement("column");
            indexCol++;
        }while(column.isNull()==false);
        row=row.nextSiblingElement("row");
        indexRow++;
    }while(row.isNull()==false);
}
//! ������ ������ � 2D �������
void readDataToVector(const QDomElement& root, const QString& nameVector,TTable1D &pM)
{
    QDomElement ele=root.firstChildElement(nameVector);
    pM.sizeCol=ele.firstChildElement("size_cols").text().toInt();
    pM.sizeRow=2;
    pM.m=new double [pM.sizeCol*pM.sizeRow];

    QDomElement row=ele.firstChildElement("row");
    QDomElement column;
    int indexRow=0;
    int indexCol=0;
    do{
        column=row.firstChildElement("column");
        indexCol=0;
        do
        {
            pM.m[indexRow*pM.sizeCol+indexCol]=column.firstChildElement("value").text().toDouble();
            column=column.nextSiblingElement("column");
            indexCol++;
        }while(column.isNull()==false);
        row=row.nextSiblingElement("row");
        indexRow++;
    }while(indexRow<2);
}
//! �������������� ������
FilterAperiodic::FilterAperiodic(double valueStart_,double k_,double step_,double maxValue_, double minValue_)
{
    k=k_;
    step=step_;
    maxValue=maxValue_;
    minValue=minValue_;
    value=valueStart_;
    valueStart=valueStart_;
    valueCurrent=valueStart;
    delta_=fabs(valueCurrent-value);
}
//! ����� ������� (������ ������� �������� �������)
void FilterAperiodic::resetFilter(double valueStart_)
{
    value=valueStart_;
}
//! ������� �������� �������� �������
double FilterAperiodic::filterValue(double valueCurr)
{
    value=value+(k*cutValues(valueCurr-value)*step);
    value = limitMinMax(value,minValue,maxValue);
    return value;
}
void FilterAperiodic::setK(double k_)
{
    k=k_;
}
//! ���������� �������(� �������� ��������)
void FilterAperiodic::refresh()
{
    delta_=fabs(valueCurrent-value);
    if(delta_<10e-3)
        value=valueCurrent;
    else
    {
        value=value+(k*cutValues(valueCurrent-value)*step);
        value=limitMinMax(value,minValue,maxValue);
    }
}
//! ��������� �������� �������� �������
double FilterAperiodic::setFilterValue(double v)
{
    valueCurrent=v;
    return value;
}
//! �������������� ������
FiltersAperiodicAngle::FiltersAperiodicAngle(TAngle valueStart_,double k_,double step_,double maxValue_, double minValue_)
{
    filterSigma=new FilterAperiodic(valueStart_.sigma,k_,step_,maxValue_,minValue_);
    filterTau=new FilterAperiodic(valueStart_.tau,k_,step_,maxValue_,minValue_);
}
//! ����� �������
void FiltersAperiodicAngle::resetFilter(TAngle valueStart_)
{

}

//! ��������� �������� �������� �������
TAngle FiltersAperiodicAngle::filterValue(TAngle valueCurr)
{
    TAngle angl(0.0,0.0);
    angl.sigma=filterSigma->filterValue(valueCurr.sigma);
    angl.tau=filterTau->filterValue(valueCurr.tau);

    return angl;

}

//! ���������� �������(� �������� ��������)
void FiltersAperiodicAngle::refresh()
{

}

//! ������� �������� �������� �������
TAngle FiltersAperiodicAngle::setFilterValue(TAngle currentValue)
{
    return currentValue;
}

//! ������ ����������� ��� �����
void FiltersAperiodicAngle::setK(double k_)
{
    filterSigma->setK(k_);
    filterTau->setK(k_);
}
////////////////������ � ������
//=============================================================================
//! ������ ������ �����, ��������� ����� � 0...31
//! hBit ������� ���(�����)
//! lBit ������� ���(������)
//! value32 ����� �� �������� �������� ���� � [hBit31 �� lBit0] ������������
template <class Type> Type readCode0(Type value31,  /*����� �� �������� �������� ���*/
                                     int hBit31,    /*������� ���(hight bit)*/
                                     int lBit0)     /*������� ���(low bit)*/
{
    Type tempValue=0;
    if(hBit31<lBit0)
    {
        tempValue=lBit0;
        lBit0=hBit31;
        hBit31=lBit0;
    }
    tempValue=0;
    int sum=(hBit31-lBit0);
    for(int i=0;i<=sum;i++)
    {
        tempValue=(tempValue<<1)|1;
    }
    return ((((value31)&(tempValue<<(lBit0))))>>(lBit0));
}
//! ������ �����, ��������� ����� � 1...32
//! hBit ������� ���(�����)
//! lBit ������� ���(������)
//! value32 ����� �� �������� �������� ���� � [hBit32 �� lBit1] ������������
template <class Type> Type readCode(Type value32,/*����� �� �������� �������� ���*/
                                    int hBit32,/*������� ���(hight bit)*/
                                    int lBit1)/*������� ���(low bit)*/
{
    hBit32--;
    lBit1--;

    return readCode0(value32,hBit32,lBit1);
}
//=============================================================================
//! ������ ����, ��������� ����� � 0...31
template <class Type> Type rBit0(Type value31,  /*����� �� �������� �������� ���*/
                                    int bit)       /*������� ���� (31...0)*/
{
    return readCode0(value31,bit,bit);
}
//===============================================================================================
//! ������ ���� ������������� ������ [hBit31from,lBit0from]
//! �� value32_from � value32_to � ���� � �������� [hBit31to,lBit0to] � ������, ��� ��������� ����� �����������(Intel)
//! [c 31(�������),�� 0(�������)] ���
//! ���� ��������� [hBit31from,lBit0from] � [hBit31to,lBit0to] �� ���������, ��.....
template <class Type> void writeCode(Type &value32_to,
                                     int hBit31to,
                                     int lBit0to,

                                     Type value32_from,
                                     int hBit31from,
                                     int lBit0from)
{
    unsigned mask_from=((0xFFFFFFFF<<lBit0from) & (0xFFFFFFFF>>(31-hBit31from)));
    unsigned mask_to=((0xFFFFFFFF<<lBit0to) & (0xFFFFFFFF>>(31-hBit31to)));

    value32_from=(value32_from & mask_from)<<lBit0to;
    value32_to=(value32_to & ~mask_to) | value32_from;
}
//! ������ ���� ������������� ������ [hBit,lBit]
//! �� value32_from � value32_to � ������, ��� ��������� ����� �����������(Intel)
//! [c 31(�������),�� 0(�������)] ���
template <class Type> void writeCode(Type &value32_to,
                                     Type value32_from,
                                     int hBit31,
                                     int lBit0)
{
    unsigned mask=((0xFFFFFFFF<<lBit0) & (0xFFFFFFFF>>(31-hBit31)));

    value32_from=(value32_from<<(lBit0)) & mask;
    value32_to=(value32_to & ~mask) | value32_from;
}
//! ��������� 32..1
template <class Type> void writeCode32(Type &value32_to,
                                       Type value32_from,
                                       int hBit32,
                                       int lBit1)
{
    hBit32--;
    lBit1--;
    unsigned mask=((0xFFFFFFFF<<hBit32) & (0xFFFFFFFF>>(32-lBit1)));

    value32_from=(value32_from<<(lBit1)) & ~mask;
    value32_to=(value32_to & mask) | value32_from;
}

//! ������ ���� ������������� ������ [hBit,lBit]
//! �� value32_from � value32_to � ������, ��� ��������� �����
//! ����������� ��������� ����; [c 19,�� 4] ���
template <class Type> void writeCodeMKIO(Type &value32_to, /*� �����*/
                                         Type value32_from,/*�� �����*/
                                         int hBit,/*� �������� ����(������������)*/
                                         int lBit)/*�� ������� ��� (������������)*/
{
    hBit=19-hBit;
    lBit=19-lBit;
    unsigned mask=(0xFFFFFFFF<<hBit) & (0xFFFFFFFF>>(31-lBit));

    value32_from=(value32_from<<(hBit)) & mask;
    value32_to=(value32_to & ~mask) | value32_from;
}
