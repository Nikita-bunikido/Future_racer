#define barriers    5
#define barr_time   10

#define speed   2
#define w       120
#define h       30

int barriers_x[barriers];
int barriers_y[barriers];
int barriers_type[barriers];
int active = 0;

extern int t;

void barriers_setup(void){
    for(int i = 0; i < barriers; ++i){
        barriers_x[i] = w;
        barriers_y[i] = rand()%(h - p[CAR_1]._y) + 10;
        barriers_type[i] = rand()%2;
    }
}

void update_barriers(void){
    if (!(t % barr_time)){
        active += (active < barriers);
    }
    for(int i = 0; i < active; ++i){
        barriers_x[i] -= speed;
        if (barriers_x[i] < - 30)
            barriers_x[i] = w;
    }
}

void draw_barriers(void){
    for(int i = 0; i < active; ++i)
        draw(p[barriers_type[i] + 4], barriers_x[i], barriers_y[i]);
}