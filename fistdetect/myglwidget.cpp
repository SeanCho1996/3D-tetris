#include "myglwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <stdlib.h>

const GLfloat pi = 3.14159286f;
const float ASPECT_RATIO = static_cast<float>(300.0 / 230.0);

MyGLWidget::MyGLWidget(QWidget *parent):QGLWidget (parent)
{
    connect(parent,SIGNAL(leftup()),this,SLOT(moveleft()));
    connect(parent,SIGNAL(rightup()),this,SLOT(moveright()));
    connect(parent,SIGNAL(approch()),this,SLOT(rotate()));
    connect(parent,SIGNAL(below()),this,SLOT(movebottom()));
    this->setFocusPolicy(Qt::StrongFocus);

    connect(game,SIGNAL(gameover()),this,SLOT(endgame()));
    connect(game,SIGNAL(accelerate()),this,SLOT(acceleratedescend()));
    connect(parent,SIGNAL(pause()),this,SLOT(pausetimer()));
    connect(parent,SIGNAL(resume()),this,SLOT(resumetimer()));

    initgame();

    timer = new QTimer(this);
    timer->setInterval(timeinterval);
    connect(timer,SIGNAL(timeout()),this,SLOT(descend()));
}

MyGLWidget::~MyGLWidget(){
    timer->stop();
    delete timer;
}

void MyGLWidget::initgame(){
    // reset random seed
    srand((unsigned int)(time(NULL)));

    // reset cherckerbord to 0
    game->initialize();

    //start game
    game->start();

}

void MyGLWidget::initializeGL()
{
    glClearColor(0.2f,0.2f,0.2f,1.0);
    glEnable(GL_DEPTH_TEST);
}

void MyGLWidget::resizeGL(int width, int height)
{
    // 2d condition
    /*
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0 * ASPECT_RATIO,50.0*ASPECT_RATIO,-50.0,50.0,-100.0,100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    */

    // 3d condition
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0f,(float)width/(float)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,-7,40,0,-7,0,0,1,0);
    glRotatef(-50,1,0,0);
    glRotatef(10,0,0,1);
    glPushMatrix();
}

void MyGLWidget::paintGL()
{

    glPopMatrix();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // draw a circle
    /*
    glColor3ub(160,200,150);
    glBegin(GL_POLYGON);
    for(int i = 0; i<100; i++)
    {
        glVertex2f(20.0f*cos(2*pi/100*i),20.0f*sin(2*pi/100*i));
    }
    glEnd();
    */

    // draw the checkerboard
    glColor3ub(255,255,255);
    float width_board = 1.0f;
    glBegin(GL_LINES);
    for(int i = -5;i<6;i++)
    {
        glVertex3f(i*width_board,-15.0f,-5.0f);
        glVertex3f(i*width_board,5.0f,-5.0f);

    }
    for(int i=-15;i<6;i++)
    {
        glVertex3f(-5.0f,i*width_board,-5.0f);
        glVertex3f(5.0f,i*width_board,-5.0f);
    }
    glEnd();

    // draw the existed tetris
    for(int i = 0;i<20;i++){
        for(int j = 0;j<10;j++){
            if(game->checkerboard[i][j] == 1){
                getcolor(game->type_current);
                paintcube(i,j);
            }
            if(game->checkerboard[i][j] == 2){
                glColor3ub(180,180,180);
                paintcube(i,j);
            }
       }
    }

    // draw the next tetris beside the checkerborder
    for(int i = 0;i<4;i++) {
        for(int j = 0;j<4;j++){
            if(game->next_tetris[i][j] == 1){
                getcolor(game->type_next);
                paintcube(i+1,j+12);
            }
        }
    }

    // display current score
    QString str1 = QString("Your Score: %1").arg(game->score);
    QString str2 = QString("Your Level: %1").arg(game->level);
    QFont font;
    font.setFamily("Algerian");
    font.setItalic(true);
    font.setBold(true);
    font.setPointSize(16);
    font.setPixelSize(16);
    glColor3ub(255,255,255);
    renderText(50,50,str1,font);
    renderText(50,70,str2,font);


}

void MyGLWidget::getcolor(int type)
{
    switch (type) {
    case 0:{
       glColor3ub(255,0,0);
       break;
    }
    case 1:{
       glColor3ub(255,165,0);
       break;
    }
    case 2:{
       glColor3ub(255,255,0);
       break;
    }
    case 3:{
       glColor3ub(0,255,0);
       break;
    }
    case 4:{
       glColor3ub(127,255,0);
       break;
    }
    case 5:{
       glColor3ub(0,0,255);
       break;
    }
    case 6:{
       glColor3ub(139,0,255);
       break;
    }
    default:
        break;
    }
}

void MyGLWidget::paintcube(int i,int j)
{
    glBegin(GL_QUADS);

        glVertex3f(-5.0f+j,5.0f-i,-5.0f);
        glVertex3f(-4.0f+j,5.0f-i,-5.0f);
        glVertex3f(-4.0f+j,4.0f-i,-5.0f);
        glVertex3f(-5.0f+j,4.0f-i,-5.0f);

        glVertex3f(-5.0f+j,5.0f-i,-5.0f);
        glVertex3f(-4.0f+j,5.0f-i,-5.0f);
        glVertex3f(-4.0f+j,5.0f-i,-4.0f);
        glVertex3f(-5.0f+j,5.0f-i,-4.0f);

        glVertex3f(-5.0f+j,5.0f-i,-4.0f);
        glVertex3f(-4.0f+j,5.0f-i,-4.0f);
        glVertex3f(-4.0f+j,4.0f-i,-4.0f);
        glVertex3f(-5.0f+j,4.0f-i,-4.0f);

        glVertex3f(-5.0f+j,4.0f-i,-5.0f);
        glVertex3f(-4.0f+j,4.0f-i,-5.0f);
        glVertex3f(-4.0f+j,4.0f-i,-4.0f);
        glVertex3f(-5.0f+j,4.0f-i,-4.0f);

        glVertex3f(-5.0f+j,5.0f-i,-5.0f);
        glVertex3f(-5.0f+j,4.0f-i,-5.0f);
        glVertex3f(-5.0f+j,4.0f-i,-4.0f);
        glVertex3f(-5.0f+j,5.0f-i,-4.0f);

        glVertex3f(-4.0f+j,5.0f-i,-5.0f);
        glVertex3f(-4.0f+j,4.0f-i,-5.0f);
        glVertex3f(-4.0f+j,4.0f-i,-4.0f);
        glVertex3f(-4.0f+j,5.0f-i,-4.0f);
        glEnd();
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Up:
        {
            game->movement(UP);
            break;
        }
        case Qt::Key_Left:
        {
            game->movement(LEFT);
            break;
        }
        case Qt::Key_Right:
        {
            game->movement(RIGHT);
            break;
        }
        case Qt::Key_Down:
        {
            game->movement(DOWN);
            break;
        }
        case Qt::Key_Space:
        {
            game->movement(BOTTOM);
            break;
        }
        default:
        {
            // Ignorer l'evenement
            event->ignore();
            return;
        }
    }

        // Acceptation de l'evenement et mise a jour de la scene
        event->accept();
        updateGL();
}

void MyGLWidget::descend()
{
    game->movement(DOWN);
    timer->setInterval(timeinterval);
    updateGL();
}

void MyGLWidget::endgame()
{
    ggDialog dialog(this);
    dialog.setscore(game->score);
    dialog.exec();

}

void MyGLWidget::acceleratedescend()
{
    timeinterval = timeinterval*0.8;
    game->level++;
}

void MyGLWidget::pausetimer()
{
    int remaining = timer->remainingTime();
    timer->stop();
    timer->setInterval(remaining);
}

void MyGLWidget::resumetimer()
{
    timer->start();
}

void MyGLWidget::moveleft()
{
    game->movement(LEFT);
    updateGL();
}

void MyGLWidget::moveright()
{
    game->movement(RIGHT);
    updateGL();
}

void MyGLWidget::rotate()
{
    game->movement(UP);
    updateGL();
}

void MyGLWidget::movebottom()
{
    game->movement(BOTTOM);
    updateGL();
}
