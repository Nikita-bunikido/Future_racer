/* @Nikita_Bunikido 2021 */

#include "header.h"

#define MAX     100000      //Max chars in one picture

/* PICTURES */
struct pict {               //struct pict - for one picture
    char picture[MAX];      //picture array
    int _x;                 //picture x size
    int _y;                 //picture y size
} p[P];                     //array of pictures

enum ps {                   //pictures enum
    CAR_1 = 0,
    CAR_MOVE,
    CAR_2,
    BACKGROUND,
    BARRIER,
    ROBOT,
    LAMP,
    PULE
};

extern char screen_collision[];

/* int open(char *ens[]) - open all files (*ens array of strings), and puts x sizes, y sizes,
   and pictures into array of structs (struct pict p[P];) (1 if successfully) (at start) */
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

/* draw(struct pict a, int o_x, int o_y) - draw (struct pict a) into screen with
   offset x (o_x) and offset y (o_y) (when it needed) */
void draw(struct pict a, int o_x, int o_y){
    for(int px = 0; px < a._x; px++)
        for(int py = 0; py < a._y; py++)
            if (a.picture[py * a._x + px] != 'a' &&
               (px + o_x) < w && (py + o_y) < h &&
               (o_x + px) >= 0 && (o_y + py) > 0)
                screen[(py + o_y) * w + (px + o_x)] = a.picture[py * a._x + px];
}

/* draw_col(struct pict a, int o_x, int o_y) - draw (struct pict a) into collision info
   screen with offset x (o_x) and offset y (o_y) (when it needed) */
void draw_col(struct pict a, int o_x, int o_y){
    for(int px = 0; px < a._x; px++)
        for(int py = 0; py < a._y; py++)
            if (a.picture[py * a._x + px] != 'a' &&
               (px + o_x) < w && (py + o_y) < h &&
                (o_x + px) >= 0 && (o_y + py) > 0)
                screen_collision[(py + o_y) * w + (px + o_x)] = '@';
}

/* print(int x, int y, char* s) - prints (s) string in (int x, int y) position into the
   screen (when it needed) */
void print(int x, int y, char* s){
    int i;
    for(i = 0; s[i] != '\0'; i++)
        screen[y * w + x + i] = s[i];
}