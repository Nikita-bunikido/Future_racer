/* @Nikita_Bunikido 2021 */

#include "header.h"

#define barriers    6           //number of barriers
#define barr_time   20          //barriers visibility on time (start of game)

int barriers_x[barriers];       //barriers x positions array
int barriers_y[barriers];       //barriers y positions array
int barriers_type[barriers];    //barriers types array
int barriers_vis[barriers];     //bariers visibility booleans array
int active = 0;                 //active barriers count

extern int t;
extern int pule_x;
extern int pule_y;
extern int shoot;
extern int pule_move;
extern int go;

/* barriers_setup() - setup barriers positions, types (at start) */
void barriers_setup(void){
    for(int i = 0; i < barriers; ++i){
        barriers_x[i] = w;
        barriers_y[i] = rand()%((h-10)-6+1) + 6;
        barriers_type[i] = rand()%3;
        barriers_vis[i] = 1;
    }
}

/* update_barriers() - updates all barriers (every frame) */
void update_barriers(void){
    if (!(t % barr_time)){
        active += (active < barriers);
    }
    for(int i = 0; i < active; ++i){
        barriers_x[i] -= (!go) ? speed : 0;
        if (barriers_x[i] < - 30){
            barriers_y[i] = rand()%((h-10)-6+1) + 6;
            barriers_x[i] = w;
            barriers_type[i] = rand()%3;
            barriers_vis[i] = 1;
        }
    }
}

/* draw_barriers() - drawing all barriers to screen (every frame) */
void draw_barriers(void){
    for(int i = 0; i < active; ++i)
        if (barriers_vis[i]){
            draw(p[barriers_type[i] + 4], barriers_x[i], barriers_y[i]);
            draw_col(p[barriers_type[i] + 4], barriers_x[i], barriers_y[i]);
        }
}

/* int collision() - colision detection (barrier and pule) (num of barrier if collision detected) (every frame) */
int collision(void){
    for(int i = 0; i < active; ++i){
        if ((shoot || pule_move) &&
            (pule_x + 4 >= barriers_x[i]) &&
            ((pule_y + 4) >= barriers_y[i]) &&
            (pule_y <= p[barriers_type[i] + 4]._y + barriers_y[i]) &&
            (pule_x <= p[barriers_type[i] + 4]._x + barriers_x[i]) &&
            barriers_vis[i] == 1 &&
            (barriers_x[i] > car_x + 10))
            return i;
    }
    return -1;
}

/* int game_over() - check to game over (car and barriers) (1 if game over) (every frame) */
int game_over(void){
    for(int px = car_x + p[CAR_1]._x / 4; px < car_x + p[CAR_1]._x; px++)
        for(int py = car_y + p[CAR_1]._y / 2; py < car_y + p[CAR_1]._y; py++)
            if (p[CAR_1].picture[py * p[CAR_1]._y + px] != 'a' &&
                screen_collision[py * w + px] == '@')
                return 1;
    return 0;
}