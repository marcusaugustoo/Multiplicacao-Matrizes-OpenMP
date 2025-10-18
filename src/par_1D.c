#include "par_1D.h"
#include <omp.h>
#include <string.h> 

#define BLOCK_SIZE 32
#define MIN(a,b) ((a)<(b)?(a):(b))

void paralelo_1D(double **A, double **B, double **C, int n) {
    
    //Paraleliza o loop de bloco mais externo (i_block)
    #pragma omp parallel for schedule(static)
    for (int i_block = 0; i_block < n; i_block += BLOCK_SIZE) {
        for (int j_block = 0; j_block < n; j_block += BLOCK_SIZE) {

            //Bloco temporário é privado para cada thread 
            double C_bloco[BLOCK_SIZE][BLOCK_SIZE];
            memset(C_bloco, 0, sizeof(C_bloco));

            int i_max = MIN(i_block + BLOCK_SIZE, n);
            int j_max = MIN(j_block + BLOCK_SIZE, n);

            for (int k_block = 0; k_block < n; k_block += BLOCK_SIZE) {
                int k_max = MIN(k_block + BLOCK_SIZE, n);

                for (int i = i_block; i < i_max; ++i) {
                    for (int k = k_block; k < k_max; ++k) {
                        double a_ik = A[i][k];
                        for (int j = j_block; j < j_max; ++j) {
                            C_bloco[i - i_block][j - j_block] += a_ik * B[k][j];
                        }
                    }
                }
            }

            //Copia o resultado do bloco temporário para a matriz principal C
            for (int i = i_block; i < i_max; ++i) {
                for (int j = j_block; j < j_max; ++j) {
                    C[i][j] = C_bloco[i - i_block][j - j_block];
                }
            }
        }
    }
}