#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int main(int argc, char **argv) {
    int rows, columns, i, j;
    FILE *f;

    sscanf(argv[1], "%d", &rows);
    sscanf(argv[2], "%d", &columns);

    f = fopen(argv[3], "w");
    assert(f != NULL);
    srand(time(NULL));

    fprintf(f, "%d\n", rows);

    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++)
            fprintf(f, "%lf ", (double)rand() / RAND_MAX);
        putc('\n', f);
    }

    fclose(f);

    return EXIT_SUCCESS;
}