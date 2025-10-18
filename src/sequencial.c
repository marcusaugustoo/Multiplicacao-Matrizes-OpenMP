#include "sequencial.h"
#include <string.h> 

//bloco (tiling)
#define BLOCK_SIZE 32
#define MIN(a,b) ((a)<(b)?(a):(b))

void mul_sequencial(double **A, double **B, double **C, int n) {
    
    //Loops de bloco (tiling)
    for (int i_block = 0; i_block < n; i_block += BLOCK_SIZE) {
        for (int j_block = 0; j_block < n; j_block += BLOCK_SIZE) {

            // Bloco temporário para C. Ajuda a cache.
            double C_bloco[BLOCK_SIZE][BLOCK_SIZE];
            memset(C_bloco, 0, sizeof(C_bloco));

            //Limites reais do bloco 
            int i_max = MIN(i_block + BLOCK_SIZE, n);
            int j_max = MIN(j_block + BLOCK_SIZE, n);

            //Itera sobre os blocos da dimensão k
            for (int k_block = 0; k_block < n; k_block += BLOCK_SIZE) {
                int k_max = MIN(k_block + BLOCK_SIZE, n);

                //Ordem i, k, j para otimização de cache
                for (int i = i_block; i < i_max; ++i) {
                    for (int k = k_block; k < k_max; ++k) {
                        double a_ik = A[i][k];
                        for (int j = j_block; j < j_max; ++j) {
                            C_bloco[i - i_block][j - j_block] += a_ik * B[k][j];
                        }
                    }
                }
            }

            //Copia o resultado do bloco temporário para a matriz principal C.
            for (int i = i_block; i < i_max; ++i) {
                for (int j = j_block; j < j_max; ++j) {
                    C[i][j] = C_bloco[i - i_block][j - j_block];
                }
            }
        }
    }
}