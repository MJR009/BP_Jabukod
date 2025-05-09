/*
    Martin Jabůrek
    Profiling program binary_search.c

    binary_search.jk rewritten in C
    Plain algorithm implementation with no output for fair measurement.
*/

int array[100] = { // generated with https://www.random.org/sequences/ and adjusted
    4,   8,  23,  54,  62,  64,  66,  67,  70,  76,
   80, 102, 113, 139, 144, 155, 166, 175, 199, 202,
  204, 209, 217, 250, 252, 274, 275, 276, 286, 292,
  294, 299, 304, 308, 321, 325, 326, 335, 336, 342,
  346, 364, 369, 379, 394, 410, 414, 424, 425, 428,
  435, 448, 459, 463, 467, 472, 483, 502, 513, 527,
  534, 546, 548, 557, 581, 584, 600, 607, 615, 626,
  648, 667, 681, 686, 701, 702, 705, 725, 727, 729, 
  735, 746, 763, 768, 794, 812, 817, 828, 843, 845,
  863, 898, 908, 910, 925, 933, 939, 951, 971, 985
};

int searched[10] = {346, 448, 763,   4, 364, 898, 607, 985, 284, 956};

int binary_search(int n) {
    int left = 0;
    int right = 99;

    while (left <= right) {
        int middle = left + ((right - left) / 2);

        if (array[middle] < n) {
            left = middle + 1;
        } else if (array[middle] > n) {
            right = middle - 1;
        } else {
            return middle;
        }
    }

    return -1;
}

int main() {
    for (int i = 0; i < 10; i++) {
        int n = binary_search( searched[i] );
    }

    return 0;
}
