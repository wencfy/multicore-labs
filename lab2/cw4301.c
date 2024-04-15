#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

// Function to read the distance matrix from a file
void read_matrix(const char *filename, int n, int distance[n][n]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(file, "%d", &distance[i][j]) != 1) {
                fclose(file);
                perror("File reading failed");
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(file);
}

// Recursive function to find the shortest path
void tsp(int currentPos, int visited, int n, int count, int cost, int distance[n][n], int *best_dist, int path[n], int best_path[n]) {
    // Mark this node as visited
    visited |= (1 << currentPos);

    // Set the position in array Path
    path[count - 1] = currentPos;

    // If all nodes are visited
    if (visited == ((1 << n) - 1)) {
        #pragma omp critical
        {
            for (int i = 0; i < n; i++) printf("%d ", path[i]);
            printf("\n");
            if (cost < *best_dist) {
                *best_dist = cost;
                for (int i = 0; i < n; i++) best_path[i] = path[i];
            }
        }
        return;
    }

    // Try to go to every other node that is not visited
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        if (!(visited & (1 << i))) {
            tsp(i, visited, n, count + 1, cost + distance[currentPos][i], distance, best_dist, path, best_path);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: ./ptsm x t filename.txt\n");
        return 1;
    }

    int n = atoi(argv[1]);  // Number of cities
    int num_threads = atoi(argv[2]);  // Number of threads
    char *filename = argv[3];  // Distance matrix file

    int distance[n][n];
    read_matrix(filename, n, distance);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", distance[i][j]);
        }
        printf("\n");
    }

    int best_distance = INT_MAX;
    int best_path[n];

    omp_set_num_threads(num_threads);
    #pragma omp parallel
    {
        // Local path for each thread
        int path[n];
        #pragma omp single
        {
            tsp(0, 0, n, 1, 0, distance, &best_distance, path, best_path);
        }
    }

    printf("Best path: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", best_path[i]);
    }
    printf("Distance: %d\n", best_distance);

    return 0;
}
