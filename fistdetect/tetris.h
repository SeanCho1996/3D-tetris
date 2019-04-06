#ifndef TETRIS_H
#define TETRIS_H
#include <QWidget>
#include <QtWidgets>
#include <QGLWidget>
#include <GL/glu.h>

struct Border{
    int upborder;
    int downborder;
    int leftborder;
    int rightborder;
};

struct TetrisPosition{
    int pos_x;
    int pos_y;
};

enum Directions{
    UP,DOWN,LEFT,RIGHT,BOTTOM
};

class tetris: public QWidget
{
    Q_OBJECT

public:
    tetris(QWidget *parent = nullptr);
    void initialize();
    void start();
    void createteris(int nexttetris[4][4],int type);
    void resettetris();
    void getborder(int tetris[4][4],Border &border);

    void movement(Directions dir);
    void rotate(int tetris[4][4]);
    void fixtetris(int pos_x,int pos_y);
    bool collision(int pos_x,int pos_y,Directions dir);
    void deletefull();

    // different  shapes of tetris
    int matrix[7][4][4]={
        // line
        {
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0}
        },
        // L1
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        // L2
        {
            {0,0,1,0},
            {0,0,1,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        // Z1
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },

        // Z2
        {
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },

        // T
        {
            {0,0,0,0},
            {0,0,1,0},
            {0,1,1,1},
            {0,0,0,0}
        },

        // O
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0}
        }
    };
    // total score
    int number_line_delete = 0;
    int score=0;
    int level = 1;

    // current tetris
    int matrix_tetris[4][4];

    // next tetris
    int next_tetris[4][4];

    // type of tetris
    int type_next,type_current;

    // checkerboard
    int checkerboard[20][10] = {{0}};

    // borders of four directions
    Border border;

    // position(changable) of current tetris
    TetrisPosition pos;
signals:
    void gameover();
    void accelerate();
};

#endif // TETRIS_H
