#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <chrono>
#include <algorithm>
using namespace std;

// função para criar o diretório de resultados, se necessário:
void ensure_results_dir_() {
    struct stat st_;
    if (stat("resultados", &st_) != 0) {
        mkdir("resultados", 0755); // nesse caso, cria a pasta "resultados/":
    }
}

// função para ler matriz de arquivo; neste caso: lê linha, coluna e os valores:
vector<vector<int>> lerMatriz_(const string& nomeArquivo_) {
    ifstream arq_(nomeArquivo_);
    if (!arq_) {
        cerr << "Erro ao abrir arquivo " << nomeArquivo_ << ";" << endl;
        return {};
    }
    int linhas_, colunas_;
    arq_ >> linhas_ >> colunas_;
    vector<vector<int>> matriz_(linhas_, vector<int>(colunas_, 0));
    for (int i_ = 0; i_ < linhas_; ++i_) {
        for (int j_ = 0; j_ < colunas_; ++j_) {
            arq_ >> matriz_[i_][j_];
        }
    }
    arq_.close();
    return matriz_;
}

// função executada pelo processo filho para calcular e salvar seu bloco:
void calcularParteProcesso_(int id_, int inicio_, int fim_, const vector<vector<int>>& A_, const vector<vector<int>>& B_) {
    auto inicioTempo_ = chrono::high_resolution_clock::now(); // tempo início do processo.

    int m1_ = A_[0].size();
    int m2_ = B_[0].size();
    int n1_ = A_.size();
    vector<vector<int>> parcial_(n1_, vector<int>(m2_, 0)); // parcial com zeros.

    for (int idx_ = inicio_; idx_ < fim_; ++idx_) {
        int i_ = idx_ / m2_;
        int j_ = idx_ % m2_;
        int soma_ = 0;
        for (int k_ = 0; k_ < m1_; ++k_) {
            soma_ += A_[i_][k_] * B_[k_][j_]; // isso faz com que o elemento seja calculado.
        }
        parcial_[i_][j_] = soma_;
    }

    auto fimTempo_ = chrono::high_resolution_clock::now();
    double tempoMs_ = chrono::duration<double, milli>(fimTempo_ - inicioTempo_).count();

    // salva arquivo parcial iniciando com o tamanho total da matriz, como exigido:
    string nomeArq_ = "resultados/resultado_processo_" + to_string(id_) + ".txt";
    ofstream out_(nomeArq_);
    if (!out_) {
        cerr << "Aviso: não foi possível abrir " << nomeArq_ << " para escrita;" << endl;
        return;
    }

    out_ << n1_ << " " << m2_ << endl;
    for (int i_ = 0; i_ < n1_; ++i_) {
        for (int j_ = 0; j_ < m2_; ++j_) {
            out_ << parcial_[i_][j_] << " ";
        }
        out_ << endl;
    }
    out_ << "Tempo(ms): " << tempoMs_ << endl;
    out_.close();
}

int main(int argc_, char* argv_[]) {
    if (argc_ != 4) {
        cout << "Uso: ./ParaleloProcessos M1.txt M2.txt P" << endl;
        return 1;
    }

    ensure_results_dir_(); // criar "resultados/", caso necessário;

    vector<vector<int>> M1_ = lerMatriz_(argv_[1]);
    vector<vector<int>> M2_ = lerMatriz_(argv_[2]);
    if (M1_.empty() || M2_.empty()) {
        cerr << "Erro: leitura de matrizes falhou;" << endl;
        return 1;
    }

    int m1_ = M1_[0].size();
    int n2_ = M2_.size();
    if (m1_ != n2_) {
        cerr << "Multiplicação impossível: m1 deve ser igual a n2;" << endl;
        return 1;
    }

    int P_ = atoi(argv_[3]);
    if (P_ <= 0) P_ = 1; // segurança (evitar divisões por zero!).

    int n1_ = M1_.size();
    int m2_ = M2_[0].size();
    int totalElementos_ = n1_ * m2_;
    int numProcessos_ = (totalElementos_ + P_ - 1) / P_; // divisão inteira arredondando para cima.

    // cria vários processos; neste caso cada processo calcula um bloco de P_ elementos:
    for (int p_ = 0; p_ < numProcessos_; ++p_) {
        int inicio_ = p_ * P_;
        int fim_ = min((p_ + 1) * P_, totalElementos_);

        pid_t pid_ = fork();
        if (pid_ < 0) {
            cerr << "Erro ao criar processo " << p_ << ";" << endl;
            // continua tentando criar os demais processos:
        } else if (pid_ == 0) {
            // "filho": calcula e salva seu arquivo parcial:
            calcularParteProcesso_(p_, inicio_, fim_, M1_, M2_);
            _exit(0); // garante que o filho termine aqui.
        } else {
            // "pai": continua para criar próximo processo:
        }
    }

    // pai espera todos os filhos terminarem:
    for (int p_ = 0; p_ < numProcessos_; ++p_) {
        wait(nullptr);
    }

    cout << "Multiplicação paralela com processos concluída; arquivos parciais em resultados/;" << endl;
    return 0;
}