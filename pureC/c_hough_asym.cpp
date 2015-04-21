#include <string.h>
#include "c_hough_asym.h"
//#include "omp.h"
//! преобразование Хафа
//! image - указатель на изображение
//! r0 - длина(в пикселях) отрезков по которым идет интерполирование,
//! height - высота изображение
//! width - ширина изображения


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
    static int rs;
    static int rs_1,rs1;
    int j=0,i=0;

    rs=r<<1;
    rs_1=rs-1,rs1=rs+1;
    //! указатели на элементы индексной матрицы
    char *m11=index +  (0);
    char *m12=m11 +    (rs1);
    char *m13=m12 +    (rs_1);
    char *m14=m13 +    (rs1);

    char *m21=m14 +    (rs_1);
    char *m22=m21 +    (rs);
    char *m23=m22 +    (rs1);
    char *m24=m23 +    (rs_1);

    m21[rs]=r;

    m22[rs_1]=m11[rs_1]=-(r-1);
    m13[rs_1]=m24[rs_1]=-r;

    m22[rs]=m11[rs]=-r;
    m13[rs]=m24[rs]=r;

//#pragma omp parallel for
    for(i=r;i>=m22[rs_1];i--)
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
    int rowcol=col*row;
    int i,j;
    int m=0;
    for(i=0;i<row;i++)
    {
        for(j=1;j<col;j++){
            m=i*col+j;
            di[m-1]=index[m]-index[m-1];
        }
    }

    di[col-1]=di[col-2];
    di[(rowcol)-1]=di[(rowcol)-2];
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
    static int ij;
    static int size;
    int r1,r2,s;
    int i,j;
    ij=0;
    size=row*col;


    for(int l=0;l<colIndex;l++)
    {
        //! индексы
        r2=(((l+1)<<1)-1)*size;
        r1=r2-size;
        s=l*size;
        for(i=0;i<row;i++)
        {
            vv1[i]=mod((v1[i]+index[0*colIndex + l]),row);
            vv3[i]=mod((vv1[i]+di[0*colIndex + l]),row);
        }
        for(i=0;i<col;i++)
        {
            vv2[i]=mod((v2[i]+index[1*colIndex + l]),col);
            vv4[i]=mod((vv2[i]+di[1*colIndex + l]),col);
        }
        int ij=0;
        for(i=0;i<row;i++)
        {
            for(j=0;j<col;j++)
            {
                ij=i*col+j;

                mB[ij + r1]=mA[ij + s] + mA[(vv1[i])*col+vv2[j] + s];
                mB[ij + r2]=mA[ij + s] + mA[(vv3[i])*col+vv4[j] + s];
            }
        }
    }
}

//!
uchar* fullHoughTransformAsym(THough *f, int r0)
{
    int r=1;

    int colIndex=8*r0,rowIndex=2;
    //! заполнение массива A копиями из исходного изображения
    for(int i=0;i<colIndex;i++)
    {
        copyPartImage(f->mA+i*(f->size),f->image,f->size);
    }

    uchar *x1=f->mB,*x2=f->mA,*x3=0;
    while(r<r0)
    {        
        x3=x1;x1=x2;x2=x3;
        createIndex(f->index,r,colIndex,rowIndex);
        calcDiff(f->di,f->index,colIndex,rowIndex);
        calcArray(x1,x2,f->col,f->row,f->v1,f->v2,f->index,f->di,colIndex,rowIndex);

        r=r<<1;
    }
    return x2;
}

