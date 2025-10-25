#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "sequencial.h"
#include "par_1D.h"
#include "par_2D.h"


double** alocar_matriz_adjacente(int n) {
    double **mat = malloc(n * sizeof(double*));
    if (!mat) return NULL;
    double *data = malloc((size_t)n * n * sizeof(double));
    if (!data) { free(mat); return NULL; }
    for (int i = 0; i < n; ++i) mat[i] = data + (size_t)i * n;
    return mat;
}

void free_matriz_adjacente(double **mat) {
    if (!mat) return;
    free(mat[0]);
    free(mat);
}

void iniciar_matrizes(double** A, double** B, double** C, int n) {
    srand(time(NULL)); // números diferentes a cada execução
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            //Alterado para gerar números de 0 a 1000
            A[i][j] = (double)(rand() % 1001);  
            B[i][j] = (double)(rand() % 1001);
            C[i][j] = 0.0;
        }
    }
}

//Zerar a matriz C
void zero_matriz(double** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = 0.0;
}


//Função para medir diferença entre duas matrizes
double max_abs_diff(double **X, double **Y, int n) {
    double maxd = 0.0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            double d = fabs(X[i][j] - Y[i][j]);
            if (d > maxd) maxd = d;
        }
    return maxd;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <ordem_da_matriz>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "A ordem da matriz deve ser um inteiro positivo.\n");
        return 1;
    }

    int num_threads = omp_get_max_threads();
    printf("Executando com n = %d e %d nucleos.\n\n", n, num_threads);

    //Alocação
    double **A = alocar_matriz_adjacente(n);
    double **B = alocar_matriz_adjacente(n);
    double **C_seq = alocar_matriz_adjacente(n);
    double **C_1D  = alocar_matriz_adjacente(n);
    double **C_2D  = alocar_matriz_adjacente(n);

    if (!A || !B || !C_seq || !C_1D || !C_2D) {
        fprintf(stderr, "Erro ao alocar memoria.\n");
        free_matriz_adjacente(A);
        free_matriz_adjacente(B);
        free_matriz_adjacente(C_seq);
        free_matriz_adjacente(C_1D);
        free_matriz_adjacente(C_2D);
        return 1;
    }

    iniciar_matrizes(A, B, C_seq, n); 

    struct timespec start, end;
    double time_seq, time_1D, time_2D;


    //Sequencial
    zero_matriz(C_seq, n); 
    clock_gettime(CLOCK_MONOTONIC, &start);
    mul_sequencial(A, B, C_seq, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_seq = (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
    printf("Tempo Sequencial: %.6f segundos\n", time_seq);


    //Paralelo 1D
    zero_matriz(C_1D, n);
    clock_gettime(CLOCK_MONOTONIC, &start);
    paralelo_1D(A, B, C_1D, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_1D = (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
    printf("Tempo Paralelo 1D:  %.6f segundos (Speedup: %.2fx)\n", time_1D, time_seq / time_1D);


    //Paralelo 2D 
    zero_matriz(C_2D, n);
    clock_gettime(CLOCK_MONOTONIC, &start);
    paralelo_2D(A, B, C_2D, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_2D = (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
    printf("Tempo Paralelo 2D:  %.6f segundos (Speedup: %.2fx)\n", time_2D, time_seq / time_2D);
    

    double diff_1D = max_abs_diff(C_seq, C_1D, n);
    double diff_2D = max_abs_diff(C_seq, C_2D, n);
    printf("\n\n");
    printf("Diferenca maxima (Seq x 1D): %e\n", diff_1D);
    printf("Diferenca maxima (Seq x 2D): %e\n", diff_2D);

    // Ajuste da tolerância de erro para aritmética de ponto flutuante
    // Uma tolerância relativa é melhor, mas para esta soma uma tolerância absoluta 
    // pequena (como 1e-9 ou 1e-8) deve ser suficiente.
    if (diff_1D > 1e-9 || diff_2D > 1e-9) {
        fprintf(stderr, "ERRO: Os resultados paralelos nao batem com o sequencial\n");
    } else {
        printf("Resultados corretos!\n");
    }


    free_matriz_adjacente(A);
    free_matriz_adjacente(B);
    free_matriz_adjacente(C_seq);
    free_matriz_adjacente(C_1D);
    free_matriz_adjacente(C_2D);

    return 0;
}