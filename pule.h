/* @Nikita_Bunikido 2021 */

#define pule_offset     2   //pule offset
#define pule_speed      4   //pule speed
#define restart_speed   40  //restart speed

/* PULE */
int pule_x;         //pule x position
int pule_y;         //pule y position
int shoot = 0;      //shooting bool
int pule_move = 0;  //pule moving bool
int already_shoot;  //already_shoot bool

/* pule_update() - updates pule state (every frame) */
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

/* pule_draw() - draws pule on screen (every frame) */
void pule_draw(void){
    if (shoot || pule_move)
        draw(p[PULE], pule_x, pule_y);
}