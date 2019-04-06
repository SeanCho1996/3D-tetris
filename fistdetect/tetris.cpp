#include "tetris.h"
#include <QDebug>

tetris::tetris(QWidget *parent) : QWidget (parent)
{

}

 void tetris::initialize()
 {
     for (int i = 0;i<20;i++) {
         for(int j = 0;j<10;j++)
         {
             checkerboard[i][j] = 0;
         }

     }
 }

 void tetris::start()
 {
     int type_tetris = rand()%7;
     createteris(next_tetris,type_tetris);
     resettetris();
 }

 void tetris::createteris(int nexttetris[4][4],int type)
 {
     for(int i = 0;i<4;i++){
         for(int j = 0;j<4;j++){
                 nexttetris[i][j] = matrix[type][i][j];
                 type_next = type;
             }
         }
 }

 void tetris::resettetris()
 {
     // generate current tetris
     for(int i = 0;i<4;i++){
         for(int j = 0;j<4;j++){
                 matrix_tetris[i][j] = next_tetris[i][j];
             }
         }
     type_current = type_next;

     // culculate current borders
     getborder(matrix_tetris,border);

     // create next tetris
     int type = rand()%7;
     type_next = type;
     createteris(next_tetris,type);

     // set occurance position = (0,3)
     TetrisPosition position;
     position.pos_x = 10/2-2; // total colums = 10
     position.pos_y = 0;
     pos = position;
 }

 void tetris::getborder(int tetris[4][4], Border &border)
 {
     for(int i=0;i<4;i++)
         for(int j=0;j<4;j++)
             if(tetris[i][j]==1)
             {
                 border.downborder=i;
                 break;
             }
     for(int i=3;i>=0;i--)
         for(int j=0;j<4;j++)
             if(tetris[i][j]==1)
             {
                 border.upborder=i;
                 break;
             }
     for(int j=0;j<4;j++)
         for(int i=0;i<4;i++)
             if(tetris[i][j]==1)
             {
                 border.rightborder=j;
                 break;
             }
     for(int j=3;j>=0;j--)
         for(int i=0;i<4;i++)
             if(tetris[i][j]==1)
             {
                 border.leftborder=j;
                 break;
             }
 }

 void tetris::rotate(int tetris[4][4]){
     int temp_block[4][4];
     for(int i=0;i<4;i++)
         for(int j=0;j<4;j++)
             temp_block[3-j][i]=tetris[i][j];
     for(int i=0;i<4;i++)
         for(int j=0;j<4;j++)
             tetris[i][j]=temp_block[i][j];
 }

 void tetris::movement(Directions dir)
 {
     switch (dir) {
     case UP:{
         // when the rotated tetris collide on the fixed tetris or the border, it cannot rotate
         if(collision(pos.pos_x,pos.pos_y,UP))
             break;

         // when the tetris can rotate
         rotate(matrix_tetris);
         // update checkerboard
         for(int i=0;i<4;i++)
             for(int j=0;j<4;j++)
                 if(checkerboard[pos.pos_y+i][pos.pos_x+j] != 2)
                    checkerboard[pos.pos_y+i][pos.pos_x+j]=matrix_tetris[i][j];
         // because rotation changes the borders, we need to re_calculate the borders for further movements
         getborder(matrix_tetris,border);
         break;
     }
     case DOWN:{
         // when the tetris touch the bottom line(row 20), it's fixed
         if(pos.pos_y+border.downborder==19)
         {
             fixtetris(pos.pos_x,pos.pos_y);
             resettetris();
             deletefull();
             break;
         }

         // when the tetris collide on the fixed tetris, it's fixed
         if(collision(pos.pos_x,pos.pos_y,DOWN))
         {
             fixtetris(pos.pos_x,pos.pos_y);
             resettetris();
             deletefull();
             break;
         }

         // when the tetris can descend
         // clear previous position
         for(int j=border.leftborder;j<=border.rightborder;j++)
             checkerboard[pos.pos_y][pos.pos_x+j]=0;
         // descend
         pos.pos_y+=1;
         // update checkerboard
         for(int i=0;i<4;i++)
             for(int j=border.leftborder;j<=border.rightborder;j++)
                 if(pos.pos_y+i<=19&&checkerboard[pos.pos_y+i][pos.pos_x+j]!=2)
                     checkerboard[pos.pos_y+i][pos.pos_x+j]=matrix_tetris[i][j];
         break;

     }
     case LEFT:{
         // when the tetris touch left border of checherboard or the fixed tetris, it stops
         if(pos.pos_x+border.leftborder==0 || collision(pos.pos_x,pos.pos_y,LEFT))
         {
             break;
         }

         // when the tetris can move to the left
         // clear previous position
         for(int i=border.upborder;i<=border.downborder;i++)
             checkerboard[pos.pos_y+i][pos.pos_x+3]=0;
         // move
         pos.pos_x-=1;
         // update checkerboard
         for(int i=border.upborder;i<=border.downborder;i++)
             for(int j=0;j<4;j++)
                 if(pos.pos_x+j>=0&&checkerboard[pos.pos_y+i][pos.pos_x+j]!=2)
                     checkerboard[pos.pos_y+i][pos.pos_x+j]=matrix_tetris[i][j];
         break;
     }
     case RIGHT:{
         // when the tetris touch left border of checherboard or the fixed tetris, it stops
         if(pos.pos_x+border.rightborder==9 || collision(pos.pos_x,pos.pos_y,RIGHT))
         {
             break;
         }

         // when the tetris can move to the right
         // clear previous position
         for(int i=border.upborder;i<=border.downborder;i++)
             checkerboard[pos.pos_y+i][pos.pos_x]=0;
         // move
         pos.pos_x+=1;
         // update checkerboard
         for(int i=border.upborder;i<=border.downborder;i++)
             for(int j=0;j<4;j++)
                 if(pos.pos_x+j<=9&&checkerboard[pos.pos_y+i][pos.pos_x+j]!=2)
                     checkerboard[pos.pos_y+i][pos.pos_x+j]=matrix_tetris[i][j];
         break;
     }
     case BOTTOM:{
         while(pos.pos_y + border.downborder<19 && !collision(pos.pos_x,pos.pos_y,DOWN)){
             for(int j=border.leftborder;j<=border.rightborder;j++)
             {
                 checkerboard[pos.pos_y][pos.pos_x+j] = 0;
             }
             pos.pos_y++;
             for(int i=0;i<4;i++)
                 for(int j=border.leftborder;j<=border.rightborder;j++)
                     if(pos.pos_y+i<=19&&checkerboard[pos.pos_y+i][pos.pos_x+j]!=2)
                         checkerboard[pos.pos_y+i][pos.pos_x+j]=matrix_tetris[i][j];
         }
         fixtetris(pos.pos_x,pos.pos_y);
         deletefull();
         resettetris();
         break;
     }

     }
     //deletefull();
     for(int j=0;j<9;j++)
         if(checkerboard[0][j]==2)
             emit gameover();
 }

 void tetris::fixtetris(int pos_x, int pos_y)
 {
     for(int i=border.upborder;i<=border.downborder;i++)
         for(int j=border.leftborder;j<=border.rightborder;j++)
             if(matrix_tetris[i][j]==1)
                 checkerboard[pos_y+i][pos_x+j]=2;
 }

 bool tetris::collision(int pos_x, int pos_y, Directions dir)
 {
     // create a test tetris to attempt collision
     int test_matrix[4][4];
     for(int i = 0;i<4;i++){
         for(int j = 0;j<4;j++){
                 test_matrix[i][j] = matrix_tetris[i][j];
             }
         }
     // get the test tetris' shape
     Border test_border;
     getborder(test_matrix,test_border);

     // attempt movement
     switch (dir) {
     case UP:{
         rotate(test_matrix);
         getborder(test_matrix,test_border);
         //qDebug()<<test_border.upborder<<test_border.leftborder<<test_border.downborder<<test_border.rightborder;
         break;
     }
     case DOWN:{
         pos_y = pos_y+1;
         break;
     }
     case LEFT:{
         pos_x = pos_x-1;
         break;
     }
     case RIGHT:{
         pos_x = pos_x+1;
         break;
     }
     }
     for(int i=test_border.upborder;i<=test_border.downborder;i++){
         for(int j=test_border.leftborder;j<=test_border.rightborder;j++){
             //qDebug()<<pos_x+test_border.rightborder;
             if((checkerboard[pos_y+i][pos_x+j]==2&&test_matrix[i][j]==1)||pos_x+test_border.leftborder<0||pos_x+test_border.rightborder>9){

                 return true;}
         }
     }
     return false;
 }

 void tetris::deletefull()
 {
     int i = 19; // number of lines

     while(i>=1)
     {
         bool full = true;

         // line not full
         for (int j = 0;j<10;j++) {
             if(checkerboard[i][j] == 0){
                 full = false;
                 i--;
                 break;
             }
         }

         // line full, move all the lines above one step down
         if(full){
             for (int k = i;k>=1;k--) {
                 for(int j = 0;j<10;j++){
                     checkerboard[k][j] = checkerboard[k-1][j];
                     if(checkerboard[k][j] == 1)
                         checkerboard[k][j] = 2;
                 }
             }

             number_line_delete++;
         }
     }
     int previousscore = score;
     score = number_line_delete*100;
     //qDebug()<<score;
     if(score%100 == 0 && score !=0 && score>previousscore){
         emit accelerate();
     }

 }
