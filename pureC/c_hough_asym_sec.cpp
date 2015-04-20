#include <string.h>
#include "c_hough_asym_sec.h"
//#include "omp.h"
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
void createIndex(char* index,int r,int &col,int &row, int sec)
{
    //кол-во столбцов
    col=8*r;
    //кол-во строк
    row=2;
    static int rs=r<<1;
    static int rs_1=rs-1,rs1=rs+1;
    static int j=0,i=0;
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

    for(i=r;i>=m22[rs_1];i--)
    {
        m23[j]=m12[j]=-r;
        m14[j]=m21[j]= r;

        m22[j]=m11[j]= i;
        m13[j]=m24[j]=-i;

        j++;
    }

//    for(i=1;i<=r;i++)
//        m11
}
//! расчет разностей
void calcDiff(char* di, char* index,int col, int row,int sec)
{
    static int rowcol=col*row;
    static int i,j;
    static int m=0;
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

                mB[ij + r1]=mA[ij + s] + mA[(vv1[i])*col+vv2[j] -1 + s];
                mB[ij + r2]=mA[ij + s] + mA[(vv3[i])*col+vv4[j] -1 + s];
            }
        }
    }
}

//!
uchar* fullHoughTransformAsymSec(uchar* image, int row,int col, int r0,int sec)
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

           //double t2 = omp_get_wtime();
    //! заполнение массива A копиями из исходного изображения
    for(int i=0;i<8*r0;i++)
    {
        copyPartImage(mA+i*(size),image,size);
    }
    int colIndex=8*r0,rowIndex=2;

    uchar *x1=mB,*x2=mA,*x3=0;
    while(r<r0)
    {        
        x3=x1;x1=x2;x2=x3;
        createIndex(index,r,colIndex,rowIndex,1);
        calcDiff(di,index,colIndex,rowIndex,sec);
        calcArray(x1,x2,col,row,v1,v2,index,di,colIndex,rowIndex);

        r=r<<1;
    }
           double t1 = omp_get_wtime();
           timeAll=t1-t2;

    return x1;
}
/*
function [A,index]=full_hough_transform_asym_sec1(x,R0,sec)

%R - степень 2, номер сектора от 1 до 8
%Вариант для реализации на С++, используется один буфер
%Матричные операции реализуются функциями ippiAdd (IPP) или nppiAdd (CUDA)

[N,M]=size(x);


A=zeros(N,M,R0);
A(:,:,1)=x;
R=1;
CFS=R0/2;


while R<R0
    [index,di]=getindex(R,sec);

    for r=1:R
  % Левый верхний угол   источника и приемника

    v1=(abs(index(1,r))-index(1,r))/2;
   v2=(abs(index(1,r))+index(1,r))/2;
   u1=(abs(index(2,r))-index(2,r))/2;
   u2=(abs(index(2,r))+index(2,r))/2;
   v3=v1+(abs(di(1,r))-di(1,r))/2;
   v4=v2+(abs(di(1,r))+di(1,r))/2;
   u3=u1+(abs(di(2,r))-di(2,r))/2;
   u4=u2+(abs(di(2,r))+di(2,r))/2;

%индекс слоя
   rr=CFS*(r-1)*2+1;

      A(v3+1:v3+N-R,u3+1:u3+M-R,rr+CFS)=A(v3+1:v3+N-R,u3+1:u3+M-R,rr)+A(v4+1:v4+N-R,u4+1:u4+M-R,rr);
    A(v1+1:v1+N-R,u1+1:u1+M-R,rr)=A(v1+1:v1+N-R,u1+1:u1+M-R,rr)+A(v2+1:v2+N-R,u2+1:u2+M-R,rr);

   end

  R=2*R;CFS=CFS/2;
end

index=getindex(R,sec);


    function [index,di]=getindex(R,sec)
      index0=[R:-1:-R  -repmat(R,[1 2*R-1])  -R:R repmat(R,[1 2*R-1]);  repmat(R,[1 2*R]) R:-1:-R  repmat(-R,[1 2*R-1]) -R:R-1 ];
  di0=[diff(index0(1,[1:end 1])); diff(index0(2,[1:end 1])) ];
  index=index0(:,(sec-1)*R+(1:R));
  di=di0(:,(sec-1)*R+(1:R));
© 2015 Microsoft Условия Заявление о конфиденциальности и использовании файлов cookie Разработчикам Русский

  */
