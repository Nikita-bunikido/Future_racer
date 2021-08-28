#include <stdlib.h>

#define particles_max       100

#define w       120
#define h       30

extern char screen[];
extern int car_x;
extern int car_y;

char state[] = " `.,:;%%#@";
int emmiter_x;
int emmiter_y;

struct {
    int _x;
    int _y;
    double life_time;
} particles[particles_max];

void draw_particles(void){
    int i;
    for(i = 0; i < particles_max; ++i)
        if (particles[i]._x > 0 && particles[i]._y > 0 && particles[i]._x < w - 1){
            screen[particles[i]._y * w + particles[i]._x] = state[(int)particles[i].life_time];
            screen[particles[i]._y * w + particles[i]._x + 20] = state[(int)particles[i].life_time];
        }
}

void setup_particles(void){
    for(int i = 0; i < particles_max; i++){
        particles[i]._x = emmiter_x;
        particles[i]._y = emmiter_y;
        particles[i].life_time = 8.0f;
    }
}

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
    }
}