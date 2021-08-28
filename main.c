#include <stdio.h>
#include <windows.h>
#include <math.h>

#include "particles.h"

#define P       4
#define MAX     100000
#define w       120
#define h       30
#define START   {0, 0}

char screen[w * h + 1];
int car_x = 10;
int car_y = 1;

int game = 1, t;

int Key[4];
int move = 0;
int background_x = 0;

struct pict {
    char picture[MAX];
    int _x;
    int _y;
} p[P];

int open(char *ens[]){
    FILE *f;
    int i, l, end, j;
    char c;
    for(i = 0; i < P; ++i){
        f = fopen(ens[i], "r");
        if (f == NULL){
            printf("%s\t", ens[i]);
            return 0;
        }
        l = end = 0;
        p[i]._y = 1;
        for(j = 0; (c = fgetc(f)) != EOF; j++){
            p[i]._y += (c == '\n') ? 1 : 0;
            if(c == 10){
                end = 1;
                p[i]._x = l;
                j--;
                continue;
            }
            if (!end)
                l++;
            p[i].picture[j] = c;
        }
        fclose(f);
    }
    return 1;
}

void draw(struct pict a, int o_x, int o_y){
    for(int px = 0; px < a._x; px++)
        for(int py = 0; py < a._y; py++)
            if (a.picture[py * a._x + px] != 'a' && (px + o_x) < w && (py + o_y) < h && (o_x + px) >= 0 && (o_y + py) > 0)
                screen[(py + o_y) * w + (px + o_x)] = a.picture[py * a._x + px];
}

int main(){
    emmiter_x = car_x;
    emmiter_y = car_y + 5;
    setup_particles();
    char *ens[P] = {"res\\car_1.txt", "res\\car_2.txt", "res\\car_3.txt", "res\\background.txt"};
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
        background_x -= 2;
        //Setting emmiter
        emmiter_x = car_x + 3;
        emmiter_y = car_y + 6;

        Sleep(50);


        // USER INPUT ======================================

        for(int k = 0; k < 4; ++k)
            Key[k] = (0x8000 & GetKeyState((unsigned char)"\x25\x26\x27\x28"[k])) != 0;

        // GAME LOGIC ======================================

        if (Key[3]){
            car_y++;
            move = 1;
        }
        if (Key[1]){
            car_y--;
            move = 1;
        }

        if (background_x <= -p[3]._x)
            background_x = -1;

        update_particles();

        // DRAWING =========================================

        /* Drawing background*/
        draw(p[3], background_x, 1);
        draw(p[3], background_x + p[3]._x - 1, 1);
        draw(p[3], background_x + (p[3]._x * 2) - 1, 1);

        /* Drawing a particles */
        draw_particles();

        /* Drawing a main car*/
        if (!move){
            if ((cos(t * 0.5)) > 0)
                draw(p[0], car_x, car_y);
            else
                draw(p[2], car_x, car_y);
        }
        else
            draw(p[1], car_x, car_y + 1);

        COORD start_point = START;
        WriteConsoleOutputCharacter(hConsole, screen, w * h, start_point, &dwBytesWritten);
    }
    return 0;
}