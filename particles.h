/* @Nikita_Bunikido 2021 */

#include <stdlib.h>
#include "header.h"

#define particles_max       100     //max particles on screen

extern char screen[];
extern const int car_x;
extern int car_y;
extern int shoot;
extern int pule_move;
extern int pule_x;
extern int pule_y;

char state[] = " `.,:;%%#@";        //one particle state array
int emmiter_x;                      //emmiter x position
int emmiter_y;                      //emmiter y position

/* PARTICLES */
struct {                   //struct particles - for one particle
    int _x;                //particle x position
    int _y;                //particle y position
    double life_time;      //particle current life time
} particles[particles_max], pule_particles[particles_max];  //array of particles, and pule particles

/* draw_particles() - drawing all particles into the screen (every frame) */
void draw_particles(void){
    int i;
    for(i = 0; i < particles_max; ++i)
        if (particles[i]._x > 0 && particles[i]._y > 0 && particles[i]._x < w - 1){
            screen[particles[i]._y * w + particles[i]._x] = state[(int)particles[i].life_time];
            screen[particles[i]._y * w + particles[i]._x + 20] = state[(int)particles[i].life_time];
        }
    if (shoot || pule_move){
        for(i = 0; i < particles_max; ++i)
            if (pule_particles[i]._x > 0 && pule_particles[i]._y > 0 && pule_particles[i]._x < w - 1 && pule_particles[i]._y < h - 1)
                screen[pule_particles[i]._y * w + pule_particles[i]._x] = state[(int)pule_particles[i].life_time];
    }
}

/* setup_particles() - setup all particles before using (at start) */
void setup_particles(void){
    for(int i = 0; i < particles_max; i++){
        particles[i]._x = emmiter_x;
        particles[i]._y = emmiter_y;
        particles[i].life_time = 8.0f;
        pule_particles[i]._x = car_x;
        pule_particles[i]._y = car_y - 1;
        pule_particles[i].life_time = 8.0f;
    }
}

/* setup_pule_particles() - setup all particles specially for pule before using (at start) */
void setup_pule_particles(void){
    for(int i = 0; i < particles_max; i++){
        pule_particles[i]._x = car_x + rand()%4;
        pule_particles[i]._y = car_y - 1;
        pule_particles[i].life_time = 8.0f;
    }
}

/* update_particles() - updating particles states (every frame) */
void update_particles(void){
    int i;
    for(i = 0; i < particles_max; i++){
        particles[i]._x-= rand()%2;
        particles[i]._y-= rand()%2;
        particles[i].life_time-=0.3;
        if (particles[i]._x < 0 || particles[i]._y < 0 || particles[i].life_time == 0){
            particles[i].life_time = 8;
            particles[i]._x = emmiter_x;
            particles[i]._y = emmiter_y;
        }
        if (shoot || pule_move){
            pule_particles[i]._x-= rand()%2;
            pule_particles[i]._y+= rand()%2;
            pule_particles[i].life_time-=0.6;
            if (pule_particles[i]._x < 5 || pule_particles[i]._y < 0 || pule_particles[i].life_time == 0){
                pule_particles[i].life_time = 8;
                pule_particles[i]._x = pule_x;
                pule_particles[i]._y = pule_y;
            }
        }
    }
}