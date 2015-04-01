#include <string.h>
#include "hough_c.h"
#include "omp.h"
//! преобразование Хафа
//! image - указатель на изображение
//! r0 - длина(в пикселях) отрезков по которым идет интерполирование,
//! height - высота изображение
//! width - ширина изображения

#define R_MAX 32
//! размеры изображения
#define ROW_MAX 1080
#define COL_MAX 1920
double timeAll;
//! копирование части изображения
void copyPartImage(uchar* to,uchar *from,int size)
{
    memcpy((void*)to,(void*)from,sizeof(uchar)*size);
}
//! создание индексной матрицы(ok)
void createIndex(char* index,int r,int &col,int &row)
{
    //кол-во столбцов
    col=8*r;
    //кол-во строк
    row=2;
    int rs=r<<1;
    int j=0;
    //! указатели на элементы индексной матрицы
    char *m11=index +  (0);
    char *m12=m11 +    (rs+1);
    char *m13=m12 +    (rs-1);
    char *m14=m13 +    (rs+1);

    char *m21=m14 +    (rs-1);
    char *m22=m21 +    (rs);
    char *m23=m22 +    (rs+1);
    char *m24=m23 +    (rs-1);

    m21[rs]=r;

    m22[rs-1]=m11[rs-1]=-(r-1);
    m13[rs-1]=m24[rs-1]=-r;

    m22[rs]=m11[rs]=-r;
    m13[rs]=m24[rs]=r;

    for(int i=r;i>=-(r-1);i--)
    {
        m23[j]=m12[j]=-r;
        m14[j]=m21[j]= r;

        m22[j]=m11[j]= i;
        m13[j]=m24[j]=-i;

        j++;
    }
}
//! расчет разностей
void calcDiff(char* di, char* index,int col, int row)
{
    for(int i=0;i<row;i++)
    {
        for(int j=1;j<col;j++)
            di[i*col+j-1]=index[i*col+j]-index[i*col+j-1];
    }
    di[col-1]=di[col-2];
    di[(row*col)-1]=di[(row*col)-2];
}
int mod(int value,int det)
{
    if(value<0)
        return det+value;
    else
        return value%det;
}

//! расчет
void calcArray(uchar *mA,uchar *mB,int col,int row,int *v1,int *v2,
               char *index,char* di,int colIndex,int rowIndex)
{
    static int vv1[ROW_MAX];
    static int vv3[ROW_MAX];

    static int vv2[COL_MAX];
    static int vv4[COL_MAX];

    for(int l=0;l<colIndex;l++)
    {
        //! индексы
        int r2=((l+1)<<1)-1;
        int r1=r2-1;
        for(int i=0;i<row;i++)
        {
            vv1[i]=mod((v1[i]+index[0*colIndex + l]),row);
            vv3[i]=mod((vv1[i]+di[0*colIndex + l]),row);
        }
        for(int i=0;i<col;i++)
        {
            vv2[i]=mod((v2[i]+index[1*colIndex + l]),col);
            vv4[i]=mod((vv2[i]+di[1*colIndex + l]),col);
        }
        for(int i=0;i<row;i++)
        {
            for(int j=0;j<col;j++)
            {
                mB[i*col+j + r1*(row*col)]=mA[i*col+j + l*(row*col)]+
                                               mA[(vv1[i])*col+vv2[j] -1 + l*(row*col)];
                mB[i*col+j + r2*(row*col)]=mA[i*col+j + l*(row*col)]+
                                               mA[(vv3[i])*col+vv4[j] -1 + l*(row*col)];
            }
        }
    }
}

//!
uchar* fullHoughTransformAsym(uchar* image, int row,int col, int r0)
{
    int r=1;

    int v1[ROW_MAX];//можно улучшить
    int v2[COL_MAX];
    char index[8*R_MAX*2];
    char di[(8*R_MAX+2)*2];
    //!  инициализация массивов
    for(int i=0;i<row;i++)
        v1[i]=i;
    for(int j=0;j<col;j++)
        v2[j]=j;

    int size=col*row;

    //! содание массива B
    uchar* mB=new uchar[size*r0*8];
    memset((void*)mB,0,sizeof(uchar)*size*r0*8);
    //! создание массива A
    uchar* mA=new uchar[size*r0*8];
    memset((void*)mA,0,sizeof(uchar)*size*r0*8);

           double t2 = omp_get_wtime();
    //! заполнение массива A копиями из исходного изображения
    for(int i=0;i<8*r0;i++)
    {
        copyPartImage(mA+i*(size),image,size);
    }
    //return mA;

    int colIndex=8*r0,rowIndex=2;

    uchar *x1=mB,*x2=mA,*x3=0;
    while(r<r0)
    {        
        x3=x1;x1=x2;x2=x3;
        createIndex(index,r,colIndex,rowIndex);
        calcDiff(di,index,colIndex,rowIndex);
        calcArray(x1,x2,col,row,v1,v2,index,di,colIndex,rowIndex);

        r=r<<1;
    }
           double t1 = omp_get_wtime();
           timeAll=t1-t2;

    return x1;
}

