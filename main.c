#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define REPETITIONS 10

// 1. Classic Bubble Sort
void bubble_sort_classic(int *v, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                int temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
}

// 2. Optimized Bubble Sort (early exit with flag)
void bubble_sort_optimized(int *v, int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                int temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) break; // Stops if the vector is already sorted
    }
}

// 3. Merge Sort
void merge(int *v, int l, int m, int r, int *aux) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (v[i] <= v[j]) {
            aux[k++] = v[i++];
        } else {
            aux[k++] = v[j++];
        }
    }
    while (i <= m) aux[k++] = v[i++];
    while (j <= r) aux[k++] = v[j++];
    for (i = l; i <= r; i++) {
        v[i] = aux[i];
    }
}

void merge_sort_recursive(int *v, int l, int r, int *aux) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_recursive(v, l, m, aux);
        merge_sort_recursive(v, m + 1, r, aux);
        merge(v, l, m, r, aux);
    }
}

void merge_sort_wrapper(int *v, int n) {
    int *aux = (int *)malloc(n * sizeof(int));
    if (aux != NULL) {
        merge_sort_recursive(v, 0, n - 1, aux);
        free(aux);
    }
}

// Data generation (0: Random, 1: Ascending, 2: Descending)
void generate_data(int *v, int n, int type) {
    if (type == 0) {
        for (int i = 0; i < n; i++) v[i] = rand() % (n * 10);
    } else if (type == 1) {
        for (int i = 0; i < n; i++) v[i] = i;
    } else if (type == 2) {
        for (int i = 0; i < n; i++) v[i] = n - i;
    }
}

void copy_vector(const int *source, int *destination, int n) {
    memcpy(destination, source, n * sizeof(int));
}

double measure_time_ms(void (*algorithm)(int *, int), int *vector, int n) {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    algorithm(vector, n);
    QueryPerformanceCounter(&end);
    return ((double)(end.QuadPart - start.QuadPart) * 1000.0) / frequency.QuadPart;
}

int main() {
    srand(42);

    int sizes[] = {1000, 5000, 10000, 50000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    const char *scenario_names[] = {"Random", "Ascending", "Descending"};

    printf("=========================================================================\n");
    printf("   EXPERIMENTAL EVALUATION: BUBBLE SORT vs OPTIMIZED BUBBLE SORT vs MERGE SORT\n");
    printf("=========================================================================\n\n");

    for (int t = 0; t < 3; t++) {
        printf(">>> INPUT SCENARIO: %s <<<\n", scenario_names[t]);
        printf("%-10s | %-22s | %-24s | %-20s\n", "Elements", "Classic Bubble (ms)", "Optimized Bubble (ms)", "Merge Sort (ms)");
        printf("--------------------------------------------------------------------------\n");

        for (int s = 0; s < num_sizes; s++) {
            int n = sizes[s];

            double total_classic = 0.0;
            double total_optimized = 0.0;
            double total_merge = 0.0;

            int *original = (int *)malloc(n * sizeof(int));
            int *work = (int *)malloc(n * sizeof(int));

            for (int r = 0; r < REPETITIONS; r++) {
                generate_data(original, n, t);

                copy_vector(original, work, n);
                total_classic += measure_time_ms(bubble_sort_classic, work, n);

                copy_vector(original, work, n);
                total_optimized += measure_time_ms(bubble_sort_optimized, work, n);

                copy_vector(original, work, n);
                total_merge += measure_time_ms(merge_sort_wrapper, work, n);
            }

            free(original);
            free(work);

            printf("%-10d | %-22.6f | %-24.6f | %-20.6f\n",
                   n,
                   total_classic / REPETITIONS,
                   total_optimized / REPETITIONS,
                   total_merge / REPETITIONS);
        }
        printf("--------------------------------------------------------------------------\n\n");
    }

    return 0;
}
