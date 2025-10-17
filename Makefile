# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -O3 -march=native -fopenmp -Wall -Wextra

# Diretório com os arquivos fonte
SRC_DIR = src
# Diretório para os objetos compilados
OBJ_DIR = obj

# Nome do executável
TARGET = matmul

# Arquivos fonte dentro de src/
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/sequencial.c \
       $(SRC_DIR)/par_1D.c \
       $(SRC_DIR)/par_2D.c

# Gera lista de objetos correspondentes em obj/
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Cria pasta de objetos se não existir
$(shell mkdir -p $(OBJ_DIR))

# Regra padrão
all: $(TARGET)

# Linkagem final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lm

# Compila cada .c em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(TARGET) $(OBJ_DIR)/*.o

# Execução rápida com exemplo de tamanho
run: $(TARGET)
	OMP_NUM_THREADS=8 ./$(TARGET) 1024

# Testes automáticos com tamanhos grandes
test: $(TARGET)
	for N in 512 1024 2048; do \
		echo "Executando com N=$$N"; \
		OMP_NUM_THREADS=8 ./$(TARGET) $$N; \
		echo ""; \
	done
