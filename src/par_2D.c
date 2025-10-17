#include "par_2D.h"
#include <omp.h>
#include <string.h> // Para memset

#define MIN(a,b) ((a)<(b)?(a):(b))
#define BLOCK_SIZE 32

void paralelo_2D(double **A, double **B, double **C, int n) {

    #pragma omp parallel for collapse(2) schedule(static)
    for (int i_block = 0; i_block < n; i_block += BLOCK_SIZE) {
        for (int j_block = 0; j_block < n; j_block += BLOCK_SIZE) {

            double C_bloco[BLOCK_SIZE][BLOCK_SIZE];
            memset(C_bloco, 0, sizeof(C_bloco));

            // Itera sobre os blocos da dimensão 'k'
            for (int k_block = 0; k_block < n; k_block += BLOCK_SIZE) {
                int i_max = MIN(i_block + BLOCK_SIZE, n);
                int j_max = MIN(j_block + BLOCK_SIZE, n);
                int k_max = MIN(k_block + BLOCK_SIZE, n);

                // A ordem 'i, k, j' é intencional para maximizar a reutilização na cache.
                for (int i = i_block; i < i_max; ++i) {
                    for (int k = k_block; k < k_max; ++k) {
                        // O valor A[i][k] é carregado uma vez e reutilizado para toda a
                        // linha do bloco de B. Isso maximiza a localidade temporal.
                        double a_ik = A[i][k];
                        for (int j = j_block; j < j_max; ++j) {
                            C_bloco[i - i_block][j - j_block] += a_ik * B[k][j];
                        }
                    }
                }
            }

            // Copia o resultado do bloco temporário para a matriz principal C.
            int i_max = MIN(i_block + BLOCK_SIZE, n);
            int j_max = MIN(j_block + BLOCK_SIZE, n);
            for (int i = i_block; i < i_max; ++i) {
                for (int j = j_block; j < j_max; ++j) {
                    C[i][j] = C_bloco[i - i_block][j - j_block];
                }
            }
        }
    }
}
