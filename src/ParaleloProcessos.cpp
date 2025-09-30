#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>
using namespace std;

// Função para processo filho calcular parte da matriz;
void calcularParteProcesso_(int id_, int inicio_, int fim_, const vector<vector<int>>& A_, const vector<vector<int>>& B_) {
    auto inicioTempo_ = chrono::high_resolution_clock::now();

    int n1_ = A_.size();
    int m1_ = A_[0].size();
    int m2_ = B_[0].size();
    vector<vector<int>> C_(n1_, vector<int>(m2_, 0));

    for (int idx_ = inicio_; idx_ < fim_; idx_++) {
        int i_ = idx_ / m2_;
        int j_ = idx_ % m2_;
        for (int k_ = 0; k_ < m1_; k_++) {
            C_[i_][j_] += A_[i_][k_] * B_[k_][j_];
        }
    }

    auto fimTempo_ = chrono::high_resolution_clock::now();
    double tempoMs_ = chrono::duration<double, milli>(fimTempo_ - inicioTempo_).count();

    // Salvar resultado parcial em arquivo;
    string nomeArq_ = "resultado_processo_" + to_string(id_) + ".txt";
    ofstream arq_(nomeArq_);
    arq_ << "Tempo processo " << id_ << ": " << tempoMs_ << " ms;" << endl;
    arq_.close();
}

int main(int argc_, char* argv_[]) {
    if (argc_ != 4) {
        cout << "Uso: ./ParaleloProcessos M1.txt M2.txt P" << endl;
        return 1;
    }

    // TODO: ler matrizes, dividir em blocos de tamanho P, criar processos com fork() e esperar com wait();

    return 0;
}