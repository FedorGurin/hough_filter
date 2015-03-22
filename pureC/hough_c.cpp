#include <string.h>
#include "hough_c.h"
//! преобразование Хафа
//! image - указатель на изображение
//! r0 - длина(в пикселях) отрезков по которым идет интерполирование,
//! height - высота изображение
//! width - ширина изображения

#define R_MAX 10
//! размеры изображения
#define ROW_MAX 1080
#define COL_MAX 1920
//! копирование части изображения
void copyPartImage(uchar* to,uchar *from,int size)
{
    memcpy((void*)to,(void*)from,sizeof(uchar)*size);
}
//! создание индексной матрицы(ok)
void createIndex(uchar* index,int r,int &col,int &row)
{
    //кол-во столбцов
    col=8*r;
    //кол-во строк
    row=2;
    int rs=r<<1;
    int j=0;
    //! указатели на элементы индексной матрицы
    uchar *m11=index +  (0);
    uchar *m12=m11 +    (rs+1);
    uchar *m13=m12 +    (rs-1);
    uchar *m14=m13 +    (rs+1);

    uchar *m21=m14 +    (rs-1);
    uchar *m22=m21 +    (rs);
    uchar *m23=m22 +    (rs+1);
    uchar *m24=m23 +    (rs-1);

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
void calcDiff(uchar* di, uchar* index,int col, int row)
{
    for(int i=0;i<row;i++)
    {
        for(int j=1;j<col;j++)
            di[i*col+j-1]=index[i*col+j]-index[i*col+j-1];
    }
}
//! расчет
void calcArray(uchar *mA,uchar *mB,int col,int row,uchar *v1,uchar *v2,
               uchar *index,uchar* di,int colIndex,int rowIndex)
{
    static uchar vv1[ROW_MAX];
    static uchar vv3[ROW_MAX];

    static uchar vv2[COL_MAX];
    static uchar vv4[COL_MAX];

    for(int l=0;l<colIndex;l++)
    {
        //! индексы
        int r2=(l+1)<<1;
        int r1=r2-1;
        for(int i=0;i<colIndex;i++)
        {
            vv1[i]=v1[i]+index[0*rowIndex + l]%row;
            vv3[i]=vv1[i]+di[0*rowIndex + l]%row;
        }
        for(int i=0;i<rowIndex;i++)
        {
            vv2[i]=v2[i]+index[1*rowIndex + l]%col;
            vv4[i]=vv2[i]+index[1*rowIndex + l]%col;
        }
        for(int i=0;i<row;i++)
        {
            for(int j=0;j<col;j++)
            {
                mB[i*col+j + r1*(row*col)]=mA[i*col+j + l*(row*col)]+
                                               mA[(vv1[i]+1)*col+(vv2[j]+1) + l*(row*col)];
                mB[i*col+j + r2*(row*col)]=mA[i*col+j + l*(row*col)]+
                                               mA[(vv3[i]+1)*col+(vv4[j]+1) + l*(row*col)];
            }
        }
    }
}

//!
uchar* fullHoughTransformAsym(uchar* image, int row,int col, int r0)
{
    int r=1;

    uchar v1[ROW_MAX];//можно улучшить
    uchar v2[COL_MAX];
    uchar index[8*R_MAX];
    uchar di[8*R_MAX+2];
    //!  инициализация массивов
    for(int i=0;i<row;i++)
        v1[i]=i;
    for(int j=0;j<col;j++)
        v2[j]=j;

    int size=col*row;

    //! содание массива B
    uchar* mB=new uchar[size*r0];
    memset((void*)mB,0,sizeof(uchar)*size*r0);
    //! создание массива A
    uchar* mA=new uchar[size*r0*8];
    memset((void*)mA,0,sizeof(uchar)*size*(r0<<3));

    //! заполнение массива A копиями из исходного изображения
    for(int i=0;i<(r0<<3);i++)
    {
        copyPartImage(mA+i*(size),image,size);
    }
    int colIndex,rowIndex;

    //createIndex(index,5,colIndex,rowIndex);
    //calcDiff(di,index,colIndex,rowIndex);

    uchar *x1=mB,*x2=mA,*x3=0;
    while(r<r0)
    {
        x3=x1;x1=x2;x2=x3;
        createIndex(index,r,colIndex,rowIndex);
        calcDiff(di,index,colIndex,rowIndex);
        calcArray(x1,x2,col,row,v1,v2,index,di,colIndex,rowIndex);

        r=r<<1;
    }

    return x1;
}


/*
function y=full_hough_transform_asym(x,R0)

%R - степень 2

N- строки
[N,M]=size(x);

B=zeros(N,M,8*R0);

A=repmat(x,[1 1 8*R0]);

R=1;

v1=0:N-1;v2=0:M-1;


while R<R0

  index=[R:-1:-R  -repmat(R,[1 2*R-1])  -R:R repmat(R,[1 2*R-1]);
        repmat(R,[1 2*R]) R:-1:-R  repmat(-R,[1 2*R-1]) -R:R-1 ];

   di=[diff(index(1,[1:end 1])); diff(index(2,[1:end 1])) ];

    for r=1:size(index,2)
    r1=2*r-1;r2=2*r;



    vv1=mod(v1+index(1,r),N);
    vv2=mod(v2+index(2,r),M);
     vv3=mod(vv1+di(1,r),N);
     vv4=mod(vv2+di(2,r),M);

    B(:,:,r1)=A(:,:,r)+A(vv1+1,vv2+1,r);
      B(:,:,r2)=A(:,:,r)+A(vv3+1,vv4+1,r);

!! 1 - строка, а второй элемент это столбец
   end
    R=2*R;
      if (R>=R0)
        y=B;
      return;
      end

  index=[R:-1:-R  -repmat(R,[1 2*R-1])  -R:R repmat(R,[1 2*R-1]);
        repmat(R,[1 2*R]) R:-1:-R  repmat(-R,[1 2*R-1]) -R:R-1 ];

   di=[diff(index(1,[1:end 1])); diff(index(2,[1:end 1])) ];


 for r=1:size(index,2)
    r1=2*r-1;r2=2*r;
     vv1=mod(v1+index(1,r),N);
    vv2=mod(v2+index(2,r),M);
     vv3=mod(vv1+di(1,r),N);
     vv4=mod(vv2+di(2,r),M);

    A(:,:,r1)=B(:,:,r)+B(vv1+1,vv2+1,r);
      A(:,:,r2)=B(:,:,r)+B(vv3+1,vv4+1,r);


       end
        R=2*R;
        if (R>=R0)
              y=A;
            return;
            end

      end
*/
