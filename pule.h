#define pule_offset     2
#define pule_speed      4
#define restart_speed   40

int pule_x;
int pule_y;
int shoot = 0;
int pule_move = 0;
int already_shoot;

void pule_update(void){
    if (shoot){
        setup_pule_particles();
        pule_x = car_x + 14;
        pule_y = car_y + pule_offset;
        pule_move = 1;
        shoot = !shoot;
    }
    if (pule_move){
        pule_x += pule_speed;
        if (pule_x > w){
            pule_move = 0;
            already_shoot = 0;
        }
    }
}

void pule_draw(void){
    if (shoot || pule_move)
        draw(p[PULE], pule_x, pule_y);
}