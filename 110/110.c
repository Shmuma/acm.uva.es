#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int order[100];
int order_len;
int else_pending;

void fill_vars(int n, char *buf, int *order)
{
    int i;

    for (i = 0; i < n; i++) {
        buf[i*2] = 'a' + (order ? order[n-i-1] : i);
        buf[i*2 + 1] = ',';
    }

    buf[n*2-1] = 0;
}

void swap (int a, int b)
{
    int t = order[a];
    order[a] = order[b];
    order[b] = t;
}

void indent(int n)
{
    while (n--) {
        putchar (' ');
        putchar (' ');
    }
}

inline void insert_entry (int val, int pos)
{
    memmove (order+pos+1, order+pos, (order_len-pos)*sizeof(int));
    order[pos] = val;
    order_len++;
}

inline void remove_entry (int pos)
{
    memmove (order+pos, order+pos+1, (order_len-pos)*sizeof (int));
    order_len--;
}


void generate (int n, int depth)
{
    static char vars[100];
    int i;

    if (depth < n) {
        for (i = 0; i <= order_len; i++) {
            if (depth != i) {
                if (else_pending) {
                    indent (depth);
                    printf ("else ");
                }
                else
                    indent (depth);
                else_pending = 0;
                printf ("if %c < %c then\n", order[i]+'a', 'a'+depth);
            }
            insert_entry (depth, i);
            generate (n, depth+1);
            if (depth > 0)
                else_pending = 1;
            remove_entry (i);
        }
    }
    else {
        if (else_pending) {
            indent (depth-1);
            printf ("else\n");
        }
        fill_vars (n, vars, order);
        indent (depth);
        else_pending = 0;
        printf ("writeln(%s)\n", vars);
    }
}


void solve (int n)
{
    int i;
    static char vars[100];

    puts ("program sort(input,output);");
    puts ("var");

    else_pending = order_len = 0;
    fill_vars (n, vars, NULL);
    printf ("%s : integer;\n", vars);
    puts ("begin");
    printf ("  readln(%s);\n", vars);
    if (n == 1)
        printf ("  writeln(%s)\n", vars);
    else
        generate (n, 0);
    puts ("end.");
}


int main(int argc, char *argv[])
{
    int count = 0, n;

    scanf ("%d", &count);
    while (count--) {
        scanf ("%d", &n);
        solve (n);
        if (count)
            puts ("");
    }
    return 0;
}
