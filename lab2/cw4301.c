#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <omp.h>

// Function to read the distance matrix from a file
void read_matrix(const char *filename, int n, int distance[n][n]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File opening failed");
        return;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(file, "%d", &distance[i][j]) != 1) {
                fclose(file);
                perror("File reading failed");
                return;
            }
        }
    }
    fclose(file);
}

// Recursive function to find the shortest path using parallel tasks
void tsp(int currentPos, int visited, int n, int count, int cost, int distance[n][n], int *best_dist, int path[n], int best_path[n], int depth) {
    visited |= (1 << currentPos);
    path[count - 1] = currentPos;

    // Check if all cities have been visited
    if (visited == ((1 << n) - 1)) {
        if (cost < *best_dist) {
            #pragma omp critical
            {
                if (cost < *best_dist) {
                    *best_dist = cost;
                    memcpy(best_path, path, n * sizeof(int));
                }
            }
        }
        return;
    }

    // Parallelize the search by creating tasks for the first few levels of recursion
    if (depth < 2) {  // Experiment with this depth to find the best performance
        for (int i = 0; i < n; i++) {
            if (!(visited & (1 << i))) {
                #pragma omp task firstprivate(i)
                {
                    int new_path[n];
                    memcpy(new_path, path, n * sizeof(int));
                    tsp(i, visited, n, count + 1, cost + distance[currentPos][i], distance, best_dist, new_path, best_path, depth + 1);
                }
            }
        }
        #pragma omp taskwait
    } else {
        // Continue the search sequentially after a certain depth to reduce task overhead
        for (int i = 0; i < n; i++) {
            if (!(visited & (1 << i))) {
                int new_path[n];
                memcpy(new_path, path, n * sizeof(int));
                tsp(i, visited, n, count + 1, cost + distance[currentPos][i], distance, best_dist, new_path, best_path, depth + 1);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: ./ptsm x t filename.txt\n");
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]); // Number of cities
    int num_threads = atoi(argv[2]); // Number of threads
    char *filename = argv[3]; // Distance matrix file

    int distance[n][n];
    read_matrix(filename, n, distance);

    int best_distance = INT_MAX;
    int best_path[n];
    int path[n];
    path[0] = 0; // Start at city 0

    omp_set_num_threads(num_threads);
    #pragma omp parallel
    {
        #pragma omp single
        tsp(0, 0, n, 1, 0, distance, &best_distance, path, best_path, 0);
    }

    printf("Best path: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", best_path[i]);
    }
    printf("Distance: %d\n", best_distance);

    return EXIT_SUCCESS;
}
