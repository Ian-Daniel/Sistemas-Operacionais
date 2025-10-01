#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;

// Função para ler matriz de arquivo;
vector<vector<int>> lerMatriz_(string nomeArquivo_) {
    ifstream arquivo_(nomeArquivo_);
    int linhas_, colunas_;
    arquivo_ >> linhas_ >> colunas_;
    vector<vector<int>> matriz_(linhas_, vector<int>(colunas_));
    for (int i_ = 0; i_ < linhas_; i_++) {
        for (int j_ = 0; j_ < colunas_; j_++) {
            arquivo_ >> matriz_[i_][j_];
        }
    }
    arquivo_.close();
    return matriz_;
}

// Função para multiplicar matrizes de forma sequencial;
vector<vector<int>> multiplicar_(const vector<vector<int>>& A_, const vector<vector<int>>& B_) {
    int n1_ = A_.size();
    int m1_ = A_[0].size();
    int m2_ = B_[0].size();
    vector<vector<int>> C_(n1_, vector<int>(m2_, 0));

    for (int i_ = 0; i_ < n1_; i_++) {
        for (int j_ = 0; j_ < m2_; j_++) {
            for (int k_ = 0; k_ < m1_; k_++) {
                C_[i_][j_] += A_[i_][k_] * B_[k_][j_]; // isso faz com que cada elemento seja calculado;
            }
        }
    }
    return C_;
}

// Função para salvar matriz resultado em arquivo, com tempo;
void salvarResultado_(string nomeArquivo_, const vector<vector<int>>& C_, double tempoMs_) {
    ofstream arquivo_(nomeArquivo_);
    int linhas_ = C_.size();
    int colunas_ = C_[0].size();
    arquivo_ << linhas_ << " " << colunas_ << endl;
    for (int i_ = 0; i_ < linhas_; i_++) {
        for (int j_ = 0; j_ < colunas_; j_++) {
            arquivo_ << C_[i_][j_] << " ";
        }
        arquivo_ << endl;
    }
    arquivo_ << "Tempo(ms): " << tempoMs_ << endl;
    arquivo_.close();
}

int main(int argc_, char* argv_[]) {
    if (argc_ != 4) {
        cout << "Uso: ./sequencial M1.txt M2.txt Resultado.txt" << endl;
        return 1;
    }

    vector<vector<int>> M1_ = lerMatriz_(argv_[1]);
    vector<vector<int>> M2_ = lerMatriz_(argv_[2]);

    auto inicio_ = chrono::high_resolution_clock::now();
    vector<vector<int>> C_ = multiplicar_(M1_, M2_);
    auto fim_ = chrono::high_resolution_clock::now();

    double tempoMs_ = chrono::duration<double, milli>(fim_ - inicio_).count();

    salvarResultado_(argv_[3], C_, tempoMs_);

    cout << "Multiplicação sequencial concluída; Tempo: " << tempoMs_ << " ms;" << endl;

    return 0;
}
