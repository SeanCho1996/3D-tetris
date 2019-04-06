#include "fistdetect.h"
#include "ui_fistdetect.h"
#include "myglwidget.h"
#include <QDebug>
#include <stdlib.h>
#include <time.h>

fistdetect::fistdetect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fistdetect)
{
    ui->setupUi(this);

    initgame();

    connect(timer,SIGNAL(timeout()),this,SLOT(affiche()));
}

fistdetect::~fistdetect()
{

    timer->stop();
    delete timer;
    delete ui;
    delete cam;
}

void fistdetect::initgame()
{

    ui->label->setText(QString("click on start to begin").arg(width).arg(height));

    timer = new QTimer(this);
    timer->setInterval(100);

}


void fistdetect::affiche()
{
    CascadeClassifier face_cascade;
        if( !face_cascade.load( "../fistdetect/fist2.xml" ) )
        {
            qDebug()<<"Error loading haarcascade"<<endl;

        }

    if (cam->isOpened()) {
            Mat image,frame_gray;
            std::vector<Rect> fists;

            if (cam->read(image)) {   // Capture a frame

                // Flip to get a mirror effect
                flip(image,image,1);

                // Invert Blue and Red color channels
                cvtColor(image,frame_gray,CV_BGR2GRAY);

                //image processing
//                namedWindow("原始图", WINDOW_NORMAL);
//                imshow("原始图", frame_gray);

//                int length = frame_gray.cols;
//                int height = frame_gray.rows;

//                Mat gaussien = (Mat_<char>(3,3) << 1,1,1,1,2,1,1,1,1);
//                Rect rect_head(length/2-100,0,200,height/2);
//                Mat head = frame_gray(rect_head);

//                Mat result;
//                filter2D(head,head,head.depth(),gaussien);
//                namedWindow("gausse", WINDOW_NORMAL);
//                imshow("gausse", frame_gray);


                // detect fists
                face_cascade.detectMultiScale( frame_gray, fists, 1.1, 4, 0|CV_HAAR_SCALE_IMAGE, Size(60, 60) );

                if(webcom_on == true){
                // draw rectangulars
                if (fists.size()== 2 )
                {
                    // Draw green rectangle
                    for (int i=0;i<2;i++)
                        rectangle(image,fists[i],Scalar(0,255,0),2);

                    // comparison of fists' positions
                    if (fists[0].x < fists[1].x){
                        fistl = fists[0];
                        fistr = fists[1];
                    }
                    else {
                        fistr = fists[0];
                        fistl = fists[1];
                    }
                    if(fistl.y < fistr.y - fistr.height/2){
                        if(!blockmove)
                            emit leftup();
                        blockmove = true;
                        //qDebug()<<"signal transfered";
                    }
                    else if(fistl.y > fistr.y + fistr.height/2){
                        if(!blockmove)
                            emit rightup();
                        blockmove = true;
                    }
                    else if(fistr.x - fistl.x < fistr.width*2){
                        if(!blockmove)
                            emit approch();
                        blockmove = true;
                    }
                    else if(fistl.y > 240 && fistr.y > 240){
                        if(!blockmove)
                            emit below();
                        blockmove = true;
                    }
                    else {
                        blockmove = false;
                    }
                    }
                }
                cvtColor(image,image,CV_BGR2RGB);


//                            else if(fistl.y > fistr.y + fistr.height/2)
//                                cout << "right fist above"<<endl;
//                            else if(fistr.x - fistl.x < fistr.width*2)
//                                cout << "approach"<<endl;

                // Convert to Qt image
                QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
                img = img.scaled(ui->label->width(),ui->label->height(),Qt::IgnoreAspectRatio, Qt::FastTransformation);

                    // Display on label
                    ui->label->setPixmap(QPixmap::fromImage(img));
                    // Resize the label to fit the image
                    ui->label->resize(ui->label->pixmap()->size());
                }

            }
            else {
                ui->label->setText("Error capturing the frame");
            }



}

void fistdetect::on_startButton_clicked()
{


    webcom_on = !webcom_on;
    if (webcom_on == false){
        ui->startButton->setText("start");
        int remainingtime = timer->remainingTime();
        timer->setInterval(remainingtime);
        timer->stop();
        emit pause();
        //delete cam;
    }
    else {
        ui->startButton->setText("pause");
        timer->start();
        cam = new VideoCapture(0);
        timer->setInterval(100);
        emit resume();
    }

}

