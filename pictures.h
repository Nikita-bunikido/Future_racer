#define P       6
#define MAX     100000

struct pict {
    char picture[MAX];
    int _x;
    int _y;
} p[P];

enum ps {
    CAR_1 = 0,
    CAR_MOVE,
    CAR_2,
    BACKGROUND,
    BARRIER,
    ROBOT
};

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