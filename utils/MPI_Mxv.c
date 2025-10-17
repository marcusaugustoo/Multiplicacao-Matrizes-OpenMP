#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int N ; // dimensão da matriz (pode ser lida da linha de comando)
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc > 1) {
        N = atoi(argv[1]); // ler dimensão da matriz
    }

    // cada processo terá N_local = N/size linhas
    if (N % size != 0) {
        if (rank == 0)
            printf("Erro: N deve ser divisível por size.\n");
        MPI_Finalize();
        return 0;
    }

    int N_local = N / size;

    // Alocar vetores
    double *A_local = (double*) malloc(N_local * N * sizeof(double));
    double *x = (double*) malloc(N * sizeof(double));
    double *y_local = (double*) malloc(N_local * sizeof(double));
    double *y = NULL;

    if (rank == 0) {
        // gerar matriz e vetor
        srand(rank);
        double *A = (double*) malloc(N * N * sizeof(double));
        y = (double*) malloc(N * sizeof(double));

        for (int i = 0; i < N; i++) {
            x[i] = rand()%(10*N*N); // exemplo simples: 
            for (int j = 0; j < N; j++) {
                A[i*N + j] = rand()%(10*N*N); // matriz com valores simples
            }
        }

        // distribuir linhas da matriz
        MPI_Scatter(A, N_local*N, MPI_DOUBLE,
                    A_local, N_local*N, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);

        free(A);
    } else {
        // receber vetor e linhas da matriz
        MPI_Scatter(NULL, N_local*N, MPI_DOUBLE,
                    A_local, N_local*N, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
    }

    // broadcast do vetor x para todos os processos
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // calcular produto local
    for (int i = 0; i < N_local; i++) {
        y_local[i] = 0.0;
        for (int j = 0; j < N; j++) {
            y_local[i] += A_local[i*N + j] * x[j];
        }
    }

    // reunir resultados
    MPI_Gather(y_local, N_local, MPI_DOUBLE,
               y, N_local, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    // imprimir resultado no processo raiz
    if (rank == 0) {
        printf("Resultado y = A * x:\n");
        for (int i = 0; i < N; i++) {
            printf("%6.2f ", y[i]);
        }
        printf("\n");
        free(y);
    }

    free(A_local);
    free(x);
    free(y_local);

    MPI_Finalize();
    return 0;
}