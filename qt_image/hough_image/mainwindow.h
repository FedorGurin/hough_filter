#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QImage>

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
    void openFile(QString name);
public slots:
    void slotOpenFile();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
