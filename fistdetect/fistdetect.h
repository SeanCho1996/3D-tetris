#ifndef FISTDETECT_H
#define FISTDETECT_H

#include <QWidget>
#include <QtWidgets>
#include <QtMultimedia/QCamera>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <cstdio>

using namespace cv;

namespace Ui {
class fistdetect;
}

class fistdetect : public QWidget
{
    Q_OBJECT

public:
    explicit fistdetect(QWidget *parent = nullptr);
    ~fistdetect();

protected:
    void initgame();

private:
    Ui::fistdetect *ui;
    VideoCapture *cam;
    QTimer *timer;
    Rect fistl,fistr;
    int width = 160;
    int height = 90;
    bool webcom_on = false;
    bool blockmove = false;

signals:
    void leftup();
    void rightup();
    void approch();
    void below();
    void pause();
    void resume();

private slots:
    void affiche();

    void on_startButton_clicked();

};

#endif // FISTDETECT_H
