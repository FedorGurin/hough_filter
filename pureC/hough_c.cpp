#include <string.h>
//! преобразование Хофа
//! image - указатель на изображение
//! r0 - длина(в пикселях) отрезков по которым идет интерполирование,
//! height - высота изображение
//! width - ширина изображения
typedef unsigned char uchar;
#define R_MAX 10
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
    static uchar m13[2*R_MAX+1];//! тоже что и m11 только вдурогом порядке
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
        m14=m11

}

void fullHoughTransformAsym(uchar* image, int row,int col, float r0)
{
    int r=1;
    uchar *v1=new uchar[col];
    uchar *v2=new uchar[row];
    uchar *index=0;

    int size=col*row;
    //! содание массива B
    uchar* mB=new uchar[size*r0];
    memset((void*)mB,0,sizeof(uchar)*size*r0);
    //! создание массива A
    uchar* mA=new uchar[size*ro*8];
    memset((void*)mA,0,sizeof(uchar)*size*ro*8);

    //! заполнение массива A копиями из исходного изображения
    for(int i=0;i<(r0*8);i++)
    {
        copyPartImage(mA+i*(size),image,size);
    }

    while(r<r0)
    {
        //! создаем индексы
        uchar m11[2*R_MAX+1];
        uchar m12[2*R_MAX-1];
        uchar m13[2*R_MAX+1];//! тоже что и m11 только вдурогом порядке
        uchar m14[2*R_MAX-1];//только сознаком минус

        uchar m21[2*R_MAX];
        uchar m22[2*R_MAX+1];
        uchar m23[2*R_MAX-1];
        uchar m24[2*R_MAX];

    }

}


function y=full_hough_transform_asym(x,R0)

%R - степень 2


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


   end
    R=2*R;
      if (R>=R0)
        y=B;
      return;
      end

  index=[R:-1:-R  -repmat(R,[1 2*R-1])  -R:R repmat(R,[1 2*R-1]);  repmat(R,[1 2*R]) R:-1:-R  repmat(-R,[1 2*R-1]) -R:R-1 ];

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
