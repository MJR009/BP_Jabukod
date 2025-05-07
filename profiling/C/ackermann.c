/*
    Martin Jabůrek
    Profiling program ackermann.c

    ackermann.jk reimplemented in C
*/

#include <stdio.h>

int ackermann(int m, int n) {
    if (m == 0) {
        return n + 1;
    } else if ((m > 0) && (n == 0)) {
        return ackermann(m - 1, 1);
    } else {
        return ackermann(m - 1, ackermann(m, n - 1));
    }
}

int main() {
    printf("%s\n", "\t0\t1\t2\t3\t4\t5");

    int m = 0;
    for (; m < 4; m++) {
        printf("%d\t", m);

        for (int n = 0; n <= 5; n++) {
            printf("%d\t", ackermann(m, n));
        }
        printf("\n");
    }
    
    m = 4;
    printf("4\t");
    for (int n = 0; n <= 1; n++) {
        printf("%d\t", ackermann(m, n));
    }

    printf("%s\n", "-\t-\t-\t-\t");
}
