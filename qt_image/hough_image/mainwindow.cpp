#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hough_c.h"
#include "QPainter"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dialog=new QFileDialog(this);
    dialog->setNameFilter(tr("*.*"));
    dialog->setFileMode(QFileDialog::AnyFile);
    //dialog->setDirectory(settingVV->dirSaveVar);

    connect(ui->pushButtonOpenFile,SIGNAL(clicked(bool)),this,SLOT(slotOpenFile()));
    connect(ui->pushButtonAHough,SIGNAL(clicked(bool)),this,SLOT(slotOpenAHough()));
}
void MainWindow::openFile(QString name)
{
    QImage im(name);
    QImage i1=im.convertToFormat(QImage::Format_Indexed8);
    image=i1;
    repaint();
}
void MainWindow::slotOpenAHough()
{
    uchar* result= fullHoughTransformAsym(image.bits(),image.height(),image.width(),16);
    QImage res(result,image.width(),image.height(),QImage::Format_Indexed8);
    image=res;
    repaint();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(image.isNull() == false)
        painter.drawImage(0,0, image.scaled(this->size()));
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
