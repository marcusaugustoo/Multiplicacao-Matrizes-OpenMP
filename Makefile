CC = gcc

CFLAGS = -O3 -fopenmp -lm -g -Wall

#Diretório dos fontes
SRC_DIR = src
#Diretório de saída dos arquivos compilados
BIN_DIR = bin


#Nome do executável
EXECUTABLE = matmul
#Caminho completo do executável
TARGET = $(BIN_DIR)/$(EXECUTABLE)

#Encontra automaticamente todos os arquivos .c dentro de SRC_DIR
SOURCES = $(wildcard $(SRC_DIR)/*.c)

#Gera os nomes dos arquivos objeto (.o), mas com o prefixo BIN_DIR/
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

#Alvo padrão: compila o programa
all: $(TARGET)

#Regra de linkagem: junta todos os .o (de bin/) para criar o executável (em bin/)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Regra para criar o diretório bin/
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

run: all

clean:
	rm -rf $(BIN_DIR)