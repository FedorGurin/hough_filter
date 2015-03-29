#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QPaintEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //! диалог
    QFileDialog *dialog;
    QImage image;
    uchar* result;
    uchar* output;
    void openFile(QString name);
protected:
    virtual void paintEvent(QPaintEvent *event) ;
public slots:
    void slotOpenFile();
    void slotOpenAHough();
    void slotSpinBox();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
