# Multiplicação de Matrizes com OpenMP 


Este projeto implementa algoritmos de multiplicação de matrizes
quadradas nas versões SEQUENCIAL e PARALELAS (1D e 2D),
utilizando OpenMP em C.

O objetivo é comparar o desempenho entre as abordagens e observar
o ganho de speedup obtido entre os algoritmos.

----------------------------------------------------------------------
1. PRÉ-REQUISITOS
----------------------------------------------------------------------

- GCC com suporte a OpenMP
  (Windows: usar MinGW ou WSL)
- Make instalado
- Recomendado: máquina com 8 ou mais processadores

----------------------------------------------------------------------
2. COMPILAÇÃO
----------------------------------------------------------------------

No diretório raiz do projeto, execute:

    make

Isso compilará todos os arquivos da pasta src/ e criará o executável:

    bin/matmul

Para limpar arquivos temporários (.o) e recompilar do zero:

    make clean

----------------------------------------------------------------------
3. EXECUÇÃO
----------------------------------------------------------------------

O programa aceita um argumento: a dimensão N das matrizes quadradas.

Exemplo básico:

    ./bin/matmul 1024

Definindo o número de threads:

    OMP_NUM_THREADS=4 ./bin/matmul 1024

Ou exportando a variável:

    export OMP_NUM_THREADS=8
    ./bin/matmul 2048

----------------------------------------------------------------------
4. SAÍDA ESPERADA
----------------------------------------------------------------------

Tamanho da matriz: 1024 x 1024
Threads utilizadas: 8

[Sequencial]  Tempo: y.y s

[Paralelo 1D] Tempo: x.x s | Speedup: Tx

[Paralelo 2D] Tempo: z.z s | Speedup: Wx

Verificação de corretude: OK

(No caso seria o tempo seria em float, variáveis apenas para demonstrar a 
saída)


----------------------------------------------------------------------

