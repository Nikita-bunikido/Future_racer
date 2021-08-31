/*****************************************

            FUTURE RACER
    This is a game written
    on C programming language.
    It uses the command line to
    output information in ascii
    art.
    Management:

    [up, down (^v) arrows] - move
    ESC - exit
    ENTER - continue
    SPACE - shoot

    Developed for OLC_CODEJAM_2021
                    -@Nikita_Bunikido 2021

*****************************************/

#include    <stdio.h>
#include    <windows.h>
#include    <math.h>
#include    <string.h>
#include    <time.h>

#include    "header.h"
#include    "particles.h"
#include    "pictures.h"
#include    "barriers.h"
#include    "pule.h"

#define DEBUG                       /* macro for debug mode */

#define START           {0, 0}      // start point do draw screen
#define end_w           30          // game over message width
#define end_h           8           // game over message height
#define shadow_offset   -3          // car shadow offset

#define score_default   "SCORE: "                       // score output string
#define per_default     "| RESTARTING WEAPON..."        // restarting message
#define gameover_text   "GAME OVER!"                    // game over output string
#define press           "Press [enter] to contune..."   // info output message

/* OUTPUT */
char screen[w * h];             //screen string (output string)
char screen_collision[w * h];   //screen collision (array to collision detection info)

/* CAR */
const int car_x = 10;           //car x position
int car_y = 10;                 //car y position
int can_shoot = 1;              //can car shoot bool
int move = 0;                   //car moving bool

/* COUNTERS */
int t;                          //global time
int restart_timer = 0;          //weapon restart timer
int score_count = 0;            //score counter

/* INPUT */
int Key[5];                     //Key states

/* OTHER */
int bl;                         //index to change barrier visibility (destroy)
int game = 1;                   //main game loop control
int background_x = 0;           //background x position
char score[] = "SCORE: ";       //score string
int go = 0;                     //gameover bool

int main(int argc, char* argv[]){
    srand(time(NULL));
    emmiter_x = car_x + 3;
    emmiter_y = car_y + 6;
    screen[w * h + 1] = '\0';

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
        background_x -= (!go) ? speed : 0;
        //Setting emmiter
        emmiter_x = car_x + 3;
        emmiter_y = car_y + 6;
        //Score
        score_count += !go;

        Sleep(50);


        // USER INPUT ======================================

        for(int k = 0; k < 5; ++k)
            Key[k] = (0x8000 & GetKeyState((unsigned char)"\x26\x28\x20\xD\x1B"[k])) != 0;

        // GAME LOGIC ======================================

        if (Key[1] && !go){
            car_y+= car_y < h - p[CAR_1]._y; //Colision detection with bottom
            move = 1;
        }
        if (Key[0] && !go){
            car_y-= car_y > 10; //Collision detection with top
            move = 1;
        }
        if (Key[2] && can_shoot && !go) { //Shooting
            shoot = 1;
            can_shoot = 0;
        }
        if (Key[4]){ //escape to exit
            game = !game;
        }
        if (Key[3] && go){ //restart
            go  = t = active = shoot = pule_move = background_x = 
            restart_timer = score_count = move = 0;
            car_y = 10;
            barriers_setup();
            setup_particles();
            setup_pule_particles();
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

        if (!go){
            strcpy(score, score_default);
            char sc[50];
            itoa(score_count / 10, sc, 10);
            strcat(score, sc);
            print(1, 0, score); //print score
        }

        if (!can_shoot)
            print(11, 0, per_default);


        if (game_over())
            go = 1;

        if (go){
            for(int px = w / 2 - end_w / 2; px <  w / 2 - end_w / 2 + end_w; px++)
                for(int py = h / 2 - 1; py < h / 2 - 1 + end_h; py++)
                    screen[py * w + px] = 32;
            print(w / 2 - strlen(gameover_text) / 2, h / 2, gameover_text);
            print(w / 2 - strlen(score) / 2, h / 2 + 2, score);
            print(w / 2 - strlen(press) / 2, h / 2 + 5, press);
        }

        COORD start_point = START;
        WriteConsoleOutputCharacter(hConsole, screen, w * h, start_point, &dwBytesWritten);
    }
    return 0;
}