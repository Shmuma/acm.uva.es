#include <stdio.h>
#include <stdlib.h>

int hull[20][100][2];           /* convex hull of each kingdom */
int hull_size[20];              /* count of points in each hull */
int count;                      /* count of kingdoms */
int hit[20];                    /* affected kingdoms */
int double_area[20];            /* Doubled area of each kingdom */

int base_point[2];              /* base point of convex hull calculation */


/* Find index of leftmost-lowest point */
int extreme_point (int (*points)[100][2], int count)
{
    int i, min_y, res = 0;

    min_y = (*points)[0][1];
    for (i = 1; i < count; i++)
        if ((*points)[i][1] < min_y) {
            min_y = (*points)[i][1];
            res = i;
        }
        else
            if ((*points)[i][1] == min_y) {
                if ((*points)[i][0] < (*points)[res][0])
                    res = i;
            }

    return res;
}


/* Calculate vector product relative to base_point */
int vec_prod (const int *a, const int *b, const int *base)
{
    return (a[0]-base[0])*(b[1]-base[1]) - (a[1]-base[1])*(b[0]-base[0]);
}


int compare_polar_angles (const void *a, const void *b)
{
    return -vec_prod ((const int*)a, (const int*)b, base_point);
}


/* Returns squared length of vector from base point to point a */
int length (const int *a)
{
    int x, y;

    x = a[0] - base_point[0];
    y = a[1] - base_point[1];

    return x*x + y*y;
}


/*
 * Remove duplicates from points list sorted by polar angle. This route returns
 * new count of points in array. First point is base point.
 */
int remove_duplicates (int (*points)[100][2], int count)
{
    int i, j;

    i = 1; 
    j = 2;
    
    while (j < count) {
        if (vec_prod ((*points)[i], (*points)[j], base_point) == 0) {
            if (length ((*points)[i]) < length ((*points)[j])) {
                (*points)[i][0] = (*points)[j][0];
                (*points)[i][1] = (*points)[j][1];
            }
        }
        else {
            i++;
            if (i != j) {
                (*points)[i][0] = (*points)[j][0];
                (*points)[i][1] = (*points)[j][1];
            }
        }
        j++;
    }
    
    return i+1;
}


/* Modifies points array */
int convex_hull (int (*points)[100][2], int count, int out_index)
{
    int i, extreme, j;

    if (!count)
        return;

    extreme = extreme_point (points, count);

    /* swap points */
    base_point[0] = (*points)[extreme][0];
    base_point[1] = (*points)[extreme][1];

    if (extreme != 0) {
        (*points)[extreme][0] = (*points)[0][0];
        (*points)[extreme][1] = (*points)[0][1];
        (*points)[0][0] = base_point[0];
        (*points)[0][1] = base_point[1];
    }

    /* sort points by polar angle */
    qsort ((*points)+1, count-1, sizeof (int[2]), compare_polar_angles);

    /* remove points with equal angle */
    count = remove_duplicates (points, count);

    i = 2;
    j = 3;
    while (j < count) {
        while (vec_prod ((*points)[i], (*points)[j], (*points)[i-1]) <= 0)
            i--;
        i++;
        if (i != j) {
            (*points)[i][0] = (*points)[j][0];
            (*points)[i][1] = (*points)[j][1];
        }
        j++;
    }

    for (j = 0; j < i+1; j++) {
        hull[out_index][j][0] = (*points)[j][0];
        hull[out_index][j][1] = (*points)[j][1];
    }

    return i+1;
}


int calc_double_area (int index)
{
    int res, i;

    hull[index][hull_size[index]][0] = hull[index][0][0];
    hull[index][hull_size[index]][1] = hull[index][0][1];

    res = 0;
    for (i = 1; i <= hull_size[index]; i++)
        res += hull[index][i-1][0] * hull[index][i][1] - hull[index][i][0] * hull[index][i-1][1];

    return res;
}


int attacked (int x, int y, int index)
{
    int i, val = 0, r;
    int pt[2] = { x, y };

    for (i = 1; i < hull_size[index]; i++) {
        r = vec_prod (hull[index][i], hull[index][i-1], pt);
        if (val != 0) {
            if ((val < 0 && r > 0) || (val > 0 && r < 0))
                return 0;
        }
        val = r;
    }

    return 1;
}


int main(int argc, char *argv[])
{
    int n, i;
    static int vertex[100][2];
    int x, y, sum;

    count = 0;

    while (scanf ("%d", &n) == 1 && n != -1) {
        for (i = 0; i < n; i++)
            scanf ("%d %d", &vertex[i][0], &vertex[i][1]);
        /* calculate convex hull of data */
        hull_size[count] = convex_hull (&vertex, n, count);
        /* calculate area of kingdom */
        double_area[count] = calc_double_area (count);
        count++;
    }

    /* missile attacks */
    while (scanf ("%d %d", &x, &y) == 2) {
        /* Find all attacked kingdoms */
        for (i = 0; i < count; i++) {
            if (!hit[i])
                hit[i] = attacked (x, y, i);
        }
    }

    /* Sum areas of kingdoms without power */
    sum = 0;
    for (i = 0; i < count; i++)
        if (hit[i])
            sum += double_area[i];

    printf ("%d%s\n", sum >> 1, sum % 2 ? ".50" : ".00");
    return 0;
}
