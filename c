#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define REPETICOES 10

// 1. Bubble Sort Clássico
void bubble_sort_classico(int *v, int n) {
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

// 2. Bubble Sort Otimizado (Parada Antecipada com Flag)
void bubble_sort_otimizado(int *v, int n) {
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                int temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
                trocou = 1;
            }
        }
        if (!trocou) break; // Interrompe se o vetor já estiver ordenado
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

void merge_sort_recursivo(int *v, int l, int r, int *aux) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_recursivo(v, l, m, aux);
        merge_sort_recursivo(v, m + 1, r, aux);
        merge(v, l, m, r, aux);
    }
}

void merge_sort_wrapper(int *v, int n) {
    int *aux = (int *)malloc(n * sizeof(int));
    if (aux != NULL) {
        merge_sort_recursivo(v, 0, n - 1, aux);
        free(aux);
    }
}

// Geração de Dados (0: Aleatório, 1: Crescente, 2: Decrescente)
void gerar_dados(int *v, int n, int tipo) {
    if (tipo == 0) {
        for (int i = 0; i < n; i++) v[i] = rand() % (n * 10);
    } else if (tipo == 1) {
        for (int i = 0; i < n; i++) v[i] = i;
    } else if (tipo == 2) {
        for (int i = 0; i < n; i++) v[i] = n - i;
    }
}

void copiar_vetor(const int *origem, int *destino, int n) {
    memcpy(destino, origem, n * sizeof(int));
}

double medir_tempo_ms(void (*algoritmo)(int *, int), int *vetor, int n) {
    LARGE_INTEGER frequencia;
    LARGE_INTEGER inicio;
    LARGE_INTEGER fim;

    QueryPerformanceFrequency(&frequencia);
    QueryPerformanceCounter(&inicio);
    algoritmo(vetor, n);
    QueryPerformanceCounter(&fim);
    return ((double)(fim.QuadPart - inicio.QuadPart) * 1000.0) / frequencia.QuadPart;
}

int main() {
    srand(42);

    int tamanhos[] = {1000, 5000, 10000, 50000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    const char *tipos_nome[] = {"Aleatorio", "Crescente", "Decrescente"};

    printf("=========================================================================\n");
    printf("   AVALIACAO EXPERIMENTAL: BUBBLE SORT vs BUBBLE OTIMIZADO vs MERGE SORT\n");
    printf("=========================================================================\n\n");

    for (int t = 0; t < 3; t++) {
        printf(">>> CENARIO DE ENTRADA: %s <<<\n", tipos_nome[t]);
        printf("%-10s | %-20s | %-20s | %-20s\n", "Elementos", "Bubble Classico (ms)", "Bubble Otimizado(ms)", "Merge Sort (ms)");
        printf("-------------------------------------------------------------------------\n");

        for (int s = 0; s < num_tamanhos; s++) {
            int n = tamanhos[s];
            
            double tempo_total_classico = 0.0;
            double tempo_total_otimizado = 0.0;
            double tempo_total_merge = 0.0;

            int *original = (int *)malloc(n * sizeof(int));
            int *trabalho = (int *)malloc(n * sizeof(int));

            for (int r = 0; r < REPETICOES; r++) {
                gerar_dados(original, n, t);

                copiar_vetor(original, trabalho, n);
                tempo_total_classico += medir_tempo_ms(bubble_sort_classico, trabalho, n);

                copiar_vetor(original, trabalho, n);
                tempo_total_otimizado += medir_tempo_ms(bubble_sort_otimizado, trabalho, n);

                copiar_vetor(original, trabalho, n);
                tempo_total_merge += medir_tempo_ms(merge_sort_wrapper, trabalho, n);
            }

            free(original);
            free(trabalho);

            printf("%-10d | %-20.6f | %-20.6f | %-20.6f\n", 
                   n, 
                   tempo_total_classico / REPETICOES, 
                   tempo_total_otimizado / REPETICOES, 
                   tempo_total_merge / REPETICOES);
        }
        printf("-------------------------------------------------------------------------\n\n");
    }

    return 0;
}