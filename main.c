#include <stdio.h>
#include <windows.h>
#include <math.h>

#include "particles.h"
#include "pictures.h"
#include "barriers.h"

#define w       120
#define h       30
#define START   {0, 0}
#define speed   2

#define shadow_offset   -3

char screen[w * h + 1];
const int car_x = 10;
int car_y = 10;

int game = 1, t;

int Key[4];
int move = 0;
int background_x = 0;

int barrier_x;
int barrier_y;

int main(){
    emmiter_x = car_x + 3;
    emmiter_y = car_y + 6;
    barrier_y = car_y;
    barrier_x = 100;

    setup_particles();
    barriers_setup();

    char *ens[P] = {"res\\car_1.txt", "res\\car_2.txt", "res\\car_3.txt", 
    "res\\background.txt", "res\\stop.txt", "res\\robot.txt"};

    if (!open(ens))
        printf("error: can't open pictures");

    // Create Screen Buffer
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
    screen[w * h] = 0;

    while(game){
        memset(screen, 32, w * h * sizeof(char));
        
        // GAME TICK =======================================
        //Car moving
        move = 0;
        //time
        ++t;
        //moving background
        background_x -= speed;
        barrier_x -= speed;
        //Setting emmiter
        emmiter_x = car_x + 3;
        emmiter_y = car_y + 6;

        Sleep(50);


        // USER INPUT ======================================

        for(int k = 0; k < 4; ++k)
            Key[k] = (0x8000 & GetKeyState((unsigned char)"\x25\x26\x27\x28"[k])) != 0;

        // GAME LOGIC ======================================

        if (Key[3]){
            car_y+= car_y < h - p[CAR_1]._y; //Colision detection with bottom
            move = 1;
        }
        if (Key[1]){
            car_y-= car_y > 10; //Collision detection with top
            move = 1;
        }

        if (background_x <= -p[BACKGROUND]._x) //background move check
            background_x = -1;
        if (barrier_x < -10){
            barrier_y = car_y;
            barrier_x = w;
        }

        update_barriers();
        update_particles();

        // DRAWING =========================================

        /* Drawing background*/
        draw(p[BACKGROUND], background_x, 1);
        draw(p[BACKGROUND], background_x + p[BACKGROUND]._x - 1, 1);
        draw(p[BACKGROUND], background_x + (p[BACKGROUND]._x * 2) - 1, 1);

        /* Drawing robot */
        draw(p[ROBOT], barrier_x, barrier_y);


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


        /* Drawing a main car*/
        if (!move){
            if ((cos(t * 0.5)) > 0)
                draw(p[CAR_1], car_x, car_y);
            else
                draw(p[CAR_2], car_x, car_y);
        }
        else
            draw(p[CAR_MOVE], car_x, car_y + 1);

        /* Drawing barriers */
        draw_barriers();

        COORD start_point = START;
        WriteConsoleOutputCharacter(hConsole, screen, w * h, start_point, &dwBytesWritten);
    }
    return 0;
}