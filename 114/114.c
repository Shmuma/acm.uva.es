#include <stdlib.h>
#include <stdio.h>

#define DEBUG

int max_x, max_y;

int bumps[60][60];
int values[60][60];
int costs[60][60];
int step;


int rotate (int dir)
{
    switch (dir) {
    case 0:
        return 3;
    case 1:
        return 0;
    case 2:
        return 1;
    case 3:
        return 2;
    }
}

#ifndef DEBUG
void show_field (int x, int y, int xx, int yy, int dir, int score, int ttl)
{
}
#else
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
                    putchar (values[j][i] ? '*' : '#');
                else
                    putchar ('.');
        }
        putchar('\n');
    }
}
#endif


void next_step (int *x, int *y, int *dir, int *got, int *ttl)
{
    int xx = *x, yy = *y;
    
#ifdef DEBUG
    printf ("Step %d: (%d,%d), dir = %d, ttl = %d, score = %d\n", step++, *x, *y, *dir, *ttl, *got);
#endif

    switch (*dir) {
    case 0:
        xx++;
        break;
    case 1:
        yy++;
        break;
    case 2:
        xx--;
        break;
    case 3:
        yy--;
        break;
    }

    show_field (*x, *y, xx, yy, *dir, *got, *ttl);

    /* We'll hit bump */
    if (bumps[xx][yy]) {
        *ttl -= costs[xx][yy];
        if (*ttl > 0)
            *got += values[xx][yy];
        *dir = rotate (*dir);
#ifdef DEBUG
        printf ("Hit bump at %d,%d. New ttl = %d, dir = %d, score = %d\n", xx, yy, *ttl, *dir, *got);
#endif
        return;
    }

#ifdef DEBUG
    printf ("Don't hit anything, new coord (%d,%d)\n", xx, yy);
#endif
    *x = xx;
    *y = yy;
}


int main(int argc, char *argv[])
{
    int p, x, y, val, cost, dir, ttl, wall_cost;
    int score = 0, got;
    int i;

    scanf ("%d %d %d", &max_x, &max_y, &wall_cost);
    scanf ("%d", &p);
    
    max_x--;
    max_y--;

    while (p--) {
        scanf ("%d %d %d %d", &x, &y, &val, &cost);
        bumps[x-1][y-1] = val;
        values[x-1][y-1] = val;
        costs[x-1][y-1] = cost;
    }

    /* Create walls */
    for (i = 0; i <= max_x; i++) {
        bumps[i][0] = bumps[i][max_y] = 1;
        values[i][0] = values[i][max_y] = 0;
        costs[i][0] = costs[i][max_y] = wall_cost;
    }

    for (i = 0; i <= max_y; i++) {
        bumps[0][i] = bumps[max_x][i] = 1;
        values[0][i] = values[max_x][i] = 0;
        costs[0][i] = costs[max_x][i] = wall_cost;
    }

    while (scanf ("%d %d %d %d", &x, &y, &dir, &ttl) == 4) {
        got = 0;
        x--;
        y--;

#ifdef DEBUG
        printf ("\nNew ball @(%d,%d), dir = %d, ttl = %d\n", x, y, dir, ttl);
#endif

        while (ttl--) {
            if (!ttl)
                break;
            next_step (&x, &y, &dir, &got, &ttl);
            if (ttl <= 0)
                break;
        }

        score += got;
        printf ("%d\n", got);
    }

    printf ("%d\n", score);
    return 0;
}
