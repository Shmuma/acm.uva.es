#include <stdlib.h>
#include <stdio.h>

#define NDEBUG

int max_x, max_y;

int bumps[60][60];
int values[60][60];
int costs[60][60];
int step;

const int rotate[4] = {
    3, 0, 1, 2
};

const int inc_x[4] = {
    1, 0, -1, 0
};

const int inc_y[4] = {
    0, 1, 0, -1
};


#ifdef DEBUG
void show_field (int x, int y, int xx, int yy, int dir, int score, int ttl)
{
    int i, j;

    for (i = max_y; i >= 0; i--) {
        for (j = 0; j <= max_x; j++) {
            if (j == x && i == y)
                putchar ('@');
            else
                if (j == xx && i == yy)
                    putchar ('_');
            else
                if (bumps[j][i])
                    putchar (bumps[j][i] == 1 ? '*' : '#');
                else
                    putchar ('.');
        }
        putchar('\n');
    }
}
#endif


void next_step (int *x, int *y, int *dir, unsigned long long *got, int *ttl)
{
    int xx = *x, yy = *y;
    
#ifdef DEBUG
    printf ("Step %d: (%d,%d), dir = %d, ttl = %d, score = %d\n", step++, 1 + *x, 1 + *y, *dir, *ttl, *got);
#endif

    (*ttl)--;
    if (*ttl <= 0)
        return;
    xx += inc_x[*dir];
    yy += inc_y[*dir];

#ifdef DEBUG
    show_field (*x, *y, xx, yy, *dir, *got, *ttl);
#endif

    /* We'll hit bump or wall */
    if (bumps[xx][yy]) {
        *ttl -= costs[xx][yy];
        if (*ttl > 0)
            *got += values[xx][yy];
        *dir = rotate [*dir];
#ifdef DEBUG
        printf ("Hit bump at %d,%d. New ttl = %d, dir = %d, score = %d\n", xx+1, yy+1, *ttl, *dir, *got);
#endif
        return;
    }

#ifdef DEBUG
    printf ("Don't hit anything, new coord (%d,%d)\n", xx+1, yy+1);
#endif
    *x = xx;
    *y = yy;
}


int main(int argc, char *argv[])
{
    int p, x, y, val, cost, dir, ttl, wall_cost;
    unsigned long long score = 0, got;
    int i;

    scanf ("%d %d %d", &max_x, &max_y, &wall_cost);
    scanf ("%d", &p);
    
    max_x--;
    max_y--;

    /* Create walls */
    for (i = 0; i <= max_x; i++) {
        bumps[i][0] = bumps[i][max_y] = 2;
        values[i][0] = values[i][max_y] = 0;
        costs[i][0] = costs[i][max_y] = wall_cost;
    }

    for (i = 0; i <= max_y; i++) {
        bumps[0][i] = bumps[max_x][i] = 2;
        values[0][i] = values[max_x][i] = 0;
        costs[0][i] = costs[max_x][i] = wall_cost;
    }

    if (p) {
        while (p--) {
            scanf ("%d %d %d %d", &x, &y, &val, &cost);
            x--;
            y--;
            bumps[x][y] = 1;
            values[x][y] = val;
            costs[x][y] = cost;
        }
    }

    while (scanf ("%d %d %d %d", &x, &y, &dir, &ttl) == 4) {
        got = 0;
        x--;
        y--;

#ifdef DEBUG
        printf ("\nNew ball @(%d,%d), dir = %d, ttl = %d\n", x+1, y+1, dir, ttl);
#endif

        while (ttl > 0) {
            next_step (&x, &y, &dir, &got, &ttl);
            if (ttl <= 0)
                break;
        }

        score += got;
        printf ("%llu\n", got);
    }

    printf ("%llu\n", score);
    return 0;
}
