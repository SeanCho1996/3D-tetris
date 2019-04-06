#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#include <QWidget>
#include <QtWidgets>
#include <QGLWidget>
#include <GL/glu.h>
#include <stdlib.h>
#include "tetris.h"
#include <time.h>
#include <iostream>
#include "ggdialog.h"

using namespace std;

class MyGLWidget: public QGLWidget
{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();
protected:
    void initializeGL();
    void resizeGL(int width,int height);
    void paintGL();

    // initialize game
    void initgame();

    // draw a cube
    void paintcube(int i,int j);

    // use different colors for different types of tetris
    void getcolor(int type);


    // keyboard press event
    void keyPressEvent(QKeyEvent *event);


private slots:
    void moveleft();
    void moveright();
    void rotate();
    void movebottom();
    void descend();
    void endgame();
    void acceleratedescend();
    void pausetimer();
    void resumetimer();

private:
    int k;
    tetris *game = new tetris;
    int timeinterval = 1000;
    QTimer *timer;
};

#endif // MYGLWIDGET_H
