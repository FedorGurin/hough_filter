#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "c_hough_asym.h"
#include "QPainter"
#include "omp.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    result=0;
    output=0;
    dialog=new QFileDialog(this);
    dialog->setNameFilter(tr("*.*"));
    dialog->setFileMode(QFileDialog::AnyFile);
    //dialog->setDirectory(settingVV->dirSaveVar);


    connect(ui->pushButtonOpenFile,SIGNAL(clicked(bool)),this,SLOT(slotOpenFile()));
    connect(ui->pushButtonAHough,SIGNAL(clicked(bool)),this,SLOT(slotOpenAHough()));
    connect(ui->spinBoxAcc,SIGNAL(valueChanged(int)),this,SLOT(slotSelectImage()));
    connect(ui->spinBoxLevel,SIGNAL(valueChanged(int)),this,SLOT(slotSelectImage()));
}
void MainWindow::slotSpinBox()
{
    repaint();
}
void MainWindow::slotSelectImage()
{
    //image.setColorCount(1);

   /* image.setColor(0,QRgb(Qt::black));
    image.setColor(0,QRgb(Qt::white));*/
    QVector<QRgb> rgbs;

    rgbs.append(qRgb(0,0,0));
    rgbs.append(qRgb(255,255,255));
    image.setColorTable(rgbs);
    int y=image.colorCount();

    int sizeImage=image.width()*image.height();
    int r=ui->spinBoxAcc->value();
    if(result != 0)
    {
        for(int x=0;x<image.width();x++)
        {
            for(int y=0;y<image.height();y++)
            {
                if(result[y*image.width() + x + sizeImage*r]>ui->spinBoxLevel->value())
                    image.setPixel(x,y,1);
                else
                    image.setPixel(x,y,0);
            }
        }
    }
    repaint();
}

void MainWindow::openFile(QString name)
{
    //! загрузить файл
    image.load(name);
    //! преобразовать файл
    image=image.convertToFormat(QImage::Format_Indexed8,Qt::MonoOnly);

    //! содание структуры фильтра
    filtr=new THough(image.bits(),image.height(),image.width());
    //! перерисование файла
    repaint();
}
void MainWindow::slotOpenAHough()
{
    ui->spinBoxAcc->setMinimum(0);
    ui->spinBoxAcc->setMaximum(8*ui->spinBoxR0->value()-1);
    double t1=omp_get_wtime();
    //! локальное преобразование Хаффа
    result= fullHoughTransformAsym(filtr,ui->spinBoxR0->value());
    double t2 = omp_get_wtime();
    //! время на расчет преобразования Фильтра Хаффа
    ui->label_3->setText(QString::number((t2-t1)));


    repaint();
}
//! перерисовка
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(image.isNull() == false){

        painter.drawImage(image.rect(), image,image.rect());
    }
}

void MainWindow::slotOpenFile()
{
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setNameFilter(tr("*.*"));

    dialog->selectFile("");
    dialog->exec();

    QStringList fileNames=dialog->selectedFiles();
    if(fileNames.size()!=0)
    {
        if(fileNames.size()!=0)
        {
            openFile(fileNames[0]);
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
