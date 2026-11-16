#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#define REPETITIONS 30

typedef struct {
    int count;
    double mean;
    double sum_squared_differences;
} Statistics;

void add_measurement(Statistics *statistics, double value) {
    statistics->count++;
    double difference = value - statistics->mean;
    statistics->mean += difference / statistics->count;
    double updated_difference = value - statistics->mean;
    statistics->sum_squared_differences += difference * updated_difference;
}

double standard_deviation(const Statistics *statistics) {
    if (statistics->count < 2) return 0.0;
    return sqrt(statistics->sum_squared_differences / (statistics->count - 1));
}

// 1. Bubble Sort clássico
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

// 2. Bubble Sort otimizado (saída antecipada com sinalizador)
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
        if (!swapped) break; // Para se o vetor já estiver ordenado
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

// Geração de dados (0: Aleatório, 1: Crescente, 2: Decrescente)
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

int main(void) {
    srand(42);

    int sizes[] = {1000, 5000, 10000, 50000, 100000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    const char *scenario_names[] = {"Aleatório", "Crescente", "Decrescente"};
    FILE *results_file = fopen("results.csv", "w");
    if (results_file == NULL) {
        fprintf(stderr, "Não foi possível criar o arquivo results.csv.\n");
        return 1;
    }

    fprintf(results_file, "cenário;elementos;bubble_clássico_média_ms;bubble_clássico_desvio_padrão_ms;bubble_otimizado_média_ms;bubble_otimizado_desvio_padrão_ms;merge_sort_média_ms;merge_sort_desvio_padrão_ms\n");

    printf("=========================================================================\n");
    printf("   AVALIAÇÃO EXPERIMENTAL: BUBBLE SORT vs BUBBLE SORT OTIMIZADO vs MERGE SORT\n");
    printf("=========================================================================\n\n");

    for (int t = 0; t < 3; t++) {
        printf(">>> CENÁRIO DE ENTRADA: %s <<<\n", scenario_names[t]);
        printf("%-10s | %-29s | %-31s | %-27s\n", "Elementos", "Bubble clássico (média ± desvio)", "Bubble otimizado (média ± desvio)", "Merge Sort (média ± desvio)");
        printf("----------------------------------------------------------------------------------------------------\n");

        for (int s = 0; s < num_sizes; s++) {
            int n = sizes[s];

            Statistics classic = {0, 0.0, 0.0};
            Statistics optimized = {0, 0.0, 0.0};
            Statistics merge_statistics = {0, 0.0, 0.0};

            int *original = (int *)malloc(n * sizeof(int));
            int *work = (int *)malloc(n * sizeof(int));

            for (int r = 0; r < REPETITIONS; r++) {
                generate_data(original, n, t);

                copy_vector(original, work, n);
                add_measurement(&classic, measure_time_ms(bubble_sort_classic, work, n));

                copy_vector(original, work, n);
                add_measurement(&optimized, measure_time_ms(bubble_sort_optimized, work, n));

                copy_vector(original, work, n);
                add_measurement(&merge_statistics, measure_time_ms(merge_sort_wrapper, work, n));
            }

            free(original);
            free(work);

            double classic_standard_deviation = standard_deviation(&classic);
            double optimized_standard_deviation = standard_deviation(&optimized);
            double merge_standard_deviation = standard_deviation(&merge_statistics);

            printf("%-10d | %12.6f ± %-12.6f | %12.6f ± %-12.6f | %12.6f ± %-12.6f\n",
                   n,
                   classic.mean,
                   classic_standard_deviation,
                   optimized.mean,
                   optimized_standard_deviation,
                   merge_statistics.mean,
                   merge_standard_deviation);
            fprintf(results_file, "%s;%d;%.6f;%.6f;%.6f;%.6f;%.6f;%.6f\n",
                    scenario_names[t],
                    n,
                    classic.mean,
                    classic_standard_deviation,
                    optimized.mean,
                    optimized_standard_deviation,
                    merge_statistics.mean,
                    merge_standard_deviation);
        }
        printf("----------------------------------------------------------------------------------------------------\n\n");
    }

    fclose(results_file);
    return 0;
}
