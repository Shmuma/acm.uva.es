#include <stdio.h>
#include <stdlib.h>


void solve (int n)
{
    int i;
    static char vars[100];

    puts ("program sort(input,output);");
    puts ("var");

    for (i = 0; i < n; i++) {
        vars[i*2] = 'a' + i;
        vars[i*2+1] = ',';
    }
    vars[n*2-1] = 0;
    printf("%s : integer;\n", vars);
    puts("begin");
    printf("  readln(%s);\n", vars);
    puts("end.");
}


int main(int argc, char *argv[])
{
    int count = 0, n;

    scanf ("%d", &count);
    while (count--) {
        scanf ("%d", &n);
        solve (n);
    }
    return 0;
}
