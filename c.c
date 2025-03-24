#include <stdio.h>

enum x {
    argc = 0, A, B, C
};

void f(int argc) {
    return;
}

int main(int argc, char **argv) {
    argc = argc;

    printf("%d\n", argc);

    return 0;
}
