#include <stdio.h>
#include <omp.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 5) {
        perror("Please input a, b, x, t, split by blank space.");
        return -1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int x = atoi(argv[3]);
    int t = atoi(argv[4]);

    omp_set_num_threads(t);

    int ans = 0;
    
    #pragma omp parallel for reduction(+:ans)
    for (int i = a; i <= b; i++) {
        if (i % x == 0) {
            ans += 1;
        }
    }

    printf("There are %d numbers divisible by %d in the [%d, %d] range.\n", ans, x, a, b);
    printf("Time taken: time in seconds\n");

    return 0;
}