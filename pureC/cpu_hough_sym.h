#ifndef FILTER_HOUGH_ASYM
#define FILTER_HOUGH_ASYM

typedef unsigned char uchar;

//! радиус
#define R_MAX 32
//! размеры изображения
#define ROW_MAX 1280
#define COL_MAX 1920

typedef struct THoughAsym_
{
    //! настройка фильтра
    THoughAsym_(uchar *image_,int row_,int col_)
    {
        image=image_;

        row=row_;
        col=col_;
        size=row*col;

        //! создание массива B
        mB=new uchar[size*R_MAX*8];
        memset((void*)mB,0,sizeof(uchar)*size*R_MAX*8);

        //! создание массива A
        mA=new uchar[size*R_MAX*8];
        memset((void*)mA,0,sizeof(uchar)*size*R_MAX*8);

        //!  инициализация массивов
        for(int i=0;i<row;i++)
            v1[i]=i;
        for(int j=0;j<col;j++)
            v2[j]=j;
    }
    //! делается только один раз
    void prepare()
    {

    }

    void setImage(uchar *image_)
    {
        image=image_;
    }
    int v1[ROW_MAX];//можно улучшить
    int v2[COL_MAX];
    char index[8*R_MAX*2];
    char di[(8*R_MAX+2)*2];
    uchar* mB;
    uchar* mA;
    uchar* image;
    int row;
    int col;
    int size;

}THough;

uchar* fullHoughTransformAsym(THough *set, int r0);
extern double timeAll;
#endif
