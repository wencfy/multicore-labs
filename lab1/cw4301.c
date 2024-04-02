#include <stdio.h>
#include <omp.h>
#include <time.h>

int main()
{
    // printf("Please input the number a b x t: (split by blank space)\n");
    // int a, b, x, t;
    // scanf("%d %d %d %d", &a, &b, &x, &t);

    int a = 10;
    int b = 100000000;
    int x = 3;
    int t = 1;

    omp_set_num_threads(t);
    // int max_threads = omp_get_max_threads();
    // printf("max_threads: %d\n", max_threads);

    time_t start_time = time(NULL);
    int seg_cnt = (b - a) / t;
    int ans = 0;
    
    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        
        // check all numbers in [ID * seg_cnt, (ID + 1) * seg_cnt)
        for (int i = ID * seg_cnt; i < (ID + 1) * seg_cnt; i++) {
            if (i % x == 0) {
                ans++;
            }
        }
    }

    time_t end_time = time(NULL);

    printf("There are %d numbers divisible by %d in the [%d, %d] range.\n", ans, x, a, b);
    printf("Time taken: %ld\n", end_time - start_time);

    return 0;
}