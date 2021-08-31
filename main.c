#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "header.h"
#include "particles.h"
#include "pictures.h"
#include "barriers.h"
#include "pule.h"

#define DEBUG //for test

#define START           {0, 0}
#define speed           2
#define score_default   "SCORE: "
#define per_default     "| RESTARTING WEAPON..."

#define shadow_offset   -3

char screen[w * h];
char screen_collision[w * h];
const int car_x = 10;
int car_y = 10;
int bl;

int game = 1, t, restart_timer = 0, can_shoot = 1;

int Key[4];
int move = 0;
int background_x = 0;

char score[] = "SCORE: ";

int main(){
    screen[w * h + 1] = '\0';
    srand(time(NULL));
    emmiter_x = car_x + 3;
    emmiter_y = car_y + 6;

    setup_particles();
    barriers_setup();

    char *ens[P] = {"res\\car_1.txt", "res\\car_2.txt", "res\\car_3.txt", 
    "res\\background.txt", "res\\stop.txt", "res\\robot.txt", "res\\lamp.txt", 
    "res\\pule.txt"};

    #ifdef DEBUG
    if (!open(ens)){
        printf("error: can't open pictures");
        system("pause");
    }
    #endif

    // Create Screen Buffer
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
    screen[w * h] = 0;

    while(game){
        memset(screen, 32, w * h * sizeof(char));
        memset(screen_collision, 32, w * h * sizeof(char));
        
        // GAME TICK =======================================
        //Car moving
        move = 0;
        //time
        ++t;
        //moving background
        background_x -= speed;
        //Setting emmiter
        emmiter_x = car_x + 3;
        emmiter_y = car_y + 6;

        Sleep(50);


        // USER INPUT ======================================

        for(int k = 0; k < 4; ++k)
            Key[k] = (0x8000 & GetKeyState((unsigned char)"\x26\x28\x20"[k])) != 0;

        // GAME LOGIC ======================================

        if (Key[1]){
            car_y+= car_y < h - p[CAR_1]._y; //Colision detection with bottom
            move = 1;
        }
        if (Key[0]){
            car_y-= car_y > 10; //Collision detection with top
            move = 1;
        }
        if (Key[2] && can_shoot) { //Shooting
            shoot = 1;
            can_shoot = 0;
        }

        if (!can_shoot){
            restart_timer++;
            if (restart_timer > restart_speed){
                restart_timer = 0;
                can_shoot = 1;
            }
        }

        if (background_x <= -p[BACKGROUND]._x) //background move check
            background_x = -1;

        if ((bl = collision()) >= 0 && !already_shoot){ //shoot to barriers
            barriers_vis[bl] = 0;
            already_shoot = 1;
        }

        update_barriers();
        update_particles();
        pule_update();

        // DRAWING =========================================

        /* Drawing background*/
        draw(p[BACKGROUND], background_x, 1);
        draw(p[BACKGROUND], background_x + p[BACKGROUND]._x - 1, 1);
        draw(p[BACKGROUND], background_x + (p[BACKGROUND]._x * 2) - 1, 1);

        /* Drawing shadow */
        double px_1, py_1;
        for(int px = car_x; px < car_x + p[CAR_1]._x; px++)
            for(int py = car_y + p[CAR_1]._y + shadow_offset; py < car_y + p[CAR_1]._y + shadow_offset + 5; py++)
                if (py < h){
                    px_1 = ((double)(px - car_x) / (double)p[CAR_1]._x - 0.5);
                    py_1 = ((double)(py - car_y - p[CAR_1]._y - shadow_offset) / 5.0 - 0.5);
                        if (sqrt(px_1 * px_1 + py_1 * py_1) < 0.5)
                            screen[py * w + px] = 32;
                }

        /* Drawing a particles */
        draw_particles();

        /* Drawing barriers */
        draw_barriers();

        /*Drawing pule */
        pule_draw();

        /* Drawing a main car*/
        if (!move){
            if ((cos(t * 0.5)) > 0)
                draw(p[CAR_1], car_x, car_y);
            else
                draw(p[CAR_2], car_x, car_y);
        }
        else
            draw(p[CAR_MOVE], car_x, car_y + 1);

        strcpy(score, score_default);
        char sc[50];
        itoa(t / 10, sc, 10);
        strcat(score, sc);
        print(1, 0, score);

        if (!can_shoot)
            print(11, 0, per_default);


        if (game_over()){
            Sleep(5000);
            exit(0);
        }

        COORD start_point = START;
        WriteConsoleOutputCharacter(hConsole, screen, w * h, start_point, &dwBytesWritten);
    }
    return 0;
}