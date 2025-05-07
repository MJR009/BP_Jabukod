/*
    Martin Jabůrek
    Profiling program ackermann.c

    ackermann.jk reimplemented in C
    Plain algorithm implementation with no output for percise measurement.
*/

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
    int ack;

    int m = 0;
    for (; m < 4; m++) {
        for (int n = 0; n <= 5; n++) {
            ack = ackermann(m, n);
        }
    }
    
    m = 4;
    for (int n = 0; n <= 1; n++) {
        ack = ackermann(m, n);
    }
}
