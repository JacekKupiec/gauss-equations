#include <stdio.h>
#include <omp.h>

int main() {
    omp_set_dynamic(1);
    omp_set_nested(1);

    printf("OMP dynamic: %d\nOMP nested: %d", omp_get_dynamic(), omp_get_nested());

#pragma omp parallel 
    {
        printf("%d\n", omp_get_num_threads());
    }

    return 0;
}