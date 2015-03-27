#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hough_c.h"
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
}
void MainWindow::openFile(QString name)
{
    QImage im(name);
    QImage i1=im.convertToFormat(QImage::Format_Indexed8);
   // im.bits();
    //QImage
    //uchar *image=new uchar[1024*768];
    uchar* result= fullHoughTransformAsym(i1.bits(),im.height(),im.width(), 8);
    //QImage imResult(result,im.width(),im.height());
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
