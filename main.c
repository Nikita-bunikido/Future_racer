#include <stdio.h>
#include <windows.h>
#include <math.h>

#define P       3
#define MAX     100000
#define w       120
#define h       30

char screen[w * h + 1];
int car_x = 10, car_y = 1;

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
            if (a.picture[py * a._x + px] != 'a' && (px + o_x) < w && (py + o_y) < h)
                screen[(py + o_y) * w + (px + o_x)] = a.picture[py * a._x + px];
}

int main(){
    char *ens[P] = {"res\\car_1.txt", "res\\car_2.txt", "res\\background.txt"};
    if (!open(ens))
        printf("error: can't open pictures");

    screen[w * h] = 0;
    int game = 1, t;

    int Key[4];
    int move = 0;

    while(game){
        memset(screen, 32, w * h * sizeof(char));
        
        // GAME TICK =======================================
        move = 0;
        Sleep(50);
        ++t;


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

        // DRAWING =========================================
        draw(p[2], 0, 1);
        draw(p[2], p[2]._x - 1, 1);
        /* Drawing a main car*/
        if (!move)
            draw(p[0], car_x, car_y);
        else
            draw(p[1], car_x, car_y + 1);

        
        printf("%s", screen);
    }
    return 0;
}