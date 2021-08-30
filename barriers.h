#include "header.h"

#define barriers    10
#define barr_time   8

int barriers_x[barriers];
int barriers_y[barriers];
int barriers_type[barriers];
int barriers_vis[barriers];
int active = 0;

extern int t;
extern int pule_x;
extern int pule_y;
extern int shoot;
extern int pule_move;

void barriers_setup(void){
    for(int i = 0; i < barriers; ++i){
        barriers_x[i] = w;
        barriers_y[i] = rand()%(h - p[CAR_1]._y) + 10;
        barriers_type[i] = rand()%2;
        barriers_vis[i] = 1;
    }
}

void update_barriers(void){
    if (!(t % barr_time)){
        active += (active < barriers);
    }
    for(int i = 0; i < active; ++i){
        barriers_x[i] -= speed;
        if (barriers_x[i] < - 30){
            barriers_y[i] = rand()%(h - p[CAR_1]._y) + 10;
            barriers_x[i] = w;
            barriers_type[i] = rand()%2;
        }
    }
}

void draw_barriers(void){
    for(int i = 0; i < active; ++i)
        if (barriers_vis[i])
            draw(p[barriers_type[i] + 4], barriers_x[i], barriers_y[i]);
}

int collision(void){
    for(int i = 0; i < active; ++i){
        if ((shoot || pule_move) && (pule_x + 4 >= barriers_x[i]) && ((pule_y + 4) >= barriers_y[i]) && 
            (pule_y <= p[barriers_type[i] + 4]._y + barriers_y[i]) && (pule_x <= p[barriers_type[i] + 4]._x + barriers_x[i]) && 
            barriers_vis[i] == 1)
            return i;
    }
    return -1;
}