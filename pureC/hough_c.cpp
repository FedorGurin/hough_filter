#include <string.h>
//! преобразование Хафа
//! image - указатель на изображение
//! r0 - длина(в пикселях) отрезков по которым идет интерполирование,
//! height - высота изображение
//! width - ширина изображения
typedef unsigned char uchar;
#define R_MAX 10
//! размеры изображения
#define ROW_MAX 768
#define COL_MAX 1024
//! копирование части изображения
void copyPartImage(uchar* to,uchar *from,int size)
{
    memcpy((void*)to,(void*)from,sizeof(uchar)*size);
}
//! создание индексов
void createIndex(uchar* index,int r)
{
    //! создаем индексы
    static uchar m11[2*R_MAX+1];
    static uchar m12[2*R_MAX-1];
    static uchar m13[2*R_MAX+1];//! тоже что и m11 только ином порядке
    static uchar m14[2*R_MAX-1];//только сознаком минус

    static uchar m21[2*R_MAX];
    static uchar m22[2*R_MAX+1];
    static uchar m23[2*R_MAX-1];
    static uchar m24[2*R_MAX];

    //m11
    for(int i=r;i<-r;i--)
        m11[i]=i;
    //m12
    for(int i=0;i<2*r-1;i++)
        m12[i]=-r;
    for(int i=0;i<(2*r+1);i++)
        m14=m11;

}
//! расчет разностей
void calcDiff(uchar* di, uchar* index,int col, int row)
{
    for(int i=0;i<row;i++)
    {
        for(int j=1;j<col;j++)
            di[i*row+j-1]=index[i*row+j]-index[i*row+j-1];
    }
}
//! расчет
void calcArray(int r,
               uchar *mA,uchar *mB,int col,int row,
               uchar *index,uchar* di,int rowIndex,int colIndex)
{
    static uchar vv1[ROW_MAX];
    static uchar vv3[ROW_MAX];

    static uchar vv2[COL_MAX];
    static uchar vv4[COL_MAX];

    for(int l=0;l<col;l++)
    {
        float r1=2*l;
        float r2=2*l+1;
        for(int i=0;i<colIndex;i++)
        {
            vv1[i]=v1[i]+index[0*rowIndex + r]%col;
            vv3[i]=vv1[i]+di[0*rowIndex + r]%col;
        }
        for(int i=0;i<rowIndex;i++)
        {
            vv2[i]=v2[i]+index[1*rowIndex + r]%row;
            vv4[i]=vv2[i]+index[1*rowIndex + r]%row;
        }
        for(int i=0;i<row;i++)
        {
            for(int j=0;j<col;j++)
            {
                mB[i*row+j*col + r1*(row*col)]=mA[i*row+j*col + r*(row*col)]+
                                               mA[(vv1[i]+1)*row+(vv2[j]+1)*col + r*(row*col)];
                mB[i*row+j*col + r2*(row*col)]=mA[i*row+j*col + r*(row*col)]+
                                               mA[(vv3[i]+1)*row+(vv4[j]+1)*col + r*(row*col)];
            }
        }
    }
}

//!
uchar* fullHoughTransformAsym(uchar* image, int row,int col, float r0)
{
    int r=1;

    static uchar v1[ROW_MAX];//можно улучшить
    static uchar v2[COL_MAX];
    //!  инициализация массивов
    for(int i=0;i<row;i++)
        v1[i]=i;
    for(int j=0;j<col;j++)
        v2[i]=j;


    uchar *index=0;
    uchar *di=0;

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

    while(r<r0)
    {
        createIndex(index,r);
        calcDiff(di,index);
        calcArray(r,mA,mB,row,col,index,di,);

        r=r<<1;
        if(r>=r0)
            return mB;

        createIndex(index,r);
        calcDiff(di,index);
        calcArray(r,mB,mA,row,col,index,di,);

        r=r<<1;
        if(r>=r0)
            return mA;
    }

}



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
