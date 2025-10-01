# Compilador e flags:
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17 -pthread

# Estrutura de pastas:
SRC_DIR = src
BIN_DIR = bin
RESULTS_DIR = resultados

# Executáveis:
EXECUTAVEIS = Auxiliar Sequencial ParaleloThreads ParaleloProcessos
BINARIOS = $(addprefix $(BIN_DIR)/, $(EXECUTAVEIS))

# Regra padrão:
all: $(BINARIOS)

# Compila cada programa:
$(BIN_DIR)/Auxiliar: $(SRC_DIR)/Auxiliar.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BIN_DIR)/Sequencial: $(SRC_DIR)/Sequencial.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BIN_DIR)/ParaleloThreads: $(SRC_DIR)/ParaleloThreads.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BIN_DIR)/ParaleloProcessos: $(SRC_DIR)/ParaleloProcessos.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Cria diretórios, caso não existam:
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(RESULTS_DIR):
	mkdir -p $(RESULTS_DIR)

# Limpa apenas executáveis:
clean:
	rm -f $(BIN_DIR)/*

# Limpa tudo (executáveis, resultados e matrizes geradas):
clean-all: clean
	rm -f $(RESULTS_DIR)/* M1.txt M2.txt resultado_*.txt
