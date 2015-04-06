#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hough_c.h"
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
    connect(ui->spinBoxAcc,SIGNAL(valueChanged(int)),this,SLOT(slotSpinBox()));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(slotSpinBox()));
}
void MainWindow::slotSpinBox()
{
    repaint();
}

void MainWindow::openFile(QString name)
{
    QImage im(name);
    QImage i1=im.convertToFormat(QImage::Format_Indexed8,Qt::MonoOnly);
    image=i1;
    repaint();
}
void MainWindow::slotOpenAHough()
{
    uchar *ptr=image.bits();



    result= fullHoughTransformAsym(image.bits(),image.height(),image.width(),8);
   // double t2 = omp_get_wtime();
    ui->label_3->setText(QString::number((timeAll)));
    output=new uchar[image.height()*image.width()*8*8];
    memcpy(output,result,sizeof(uchar)*image.height()*image.width());

    repaint();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(result != 0)
    {
        int size=sizeof(uchar)*image.width()*image.height();

        for(int i=0;i<image.width()*image.height();i++)
        {
            if((result[i+ui->spinBoxAcc->value()*(size)])>ui->spinBox->value())
                (output[i+ui->spinBoxAcc->value()*(size)])=255;
            else
                (output[i+ui->spinBoxAcc->value()*(size)])=0;
        }
        QImage res(output+ui->spinBoxAcc->value()*(size),image.width(),image.height(),QImage::Format_Indexed8);
        image=res;
    }


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
