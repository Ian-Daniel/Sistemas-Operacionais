#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <chrono>
using namespace std;

struct ThreadData_ {
    int id_;
    int inicio_;
    int fim_;
    const vector<vector<int>>* A_;
    const vector<vector<int>>* B_;
    vector<vector<int>>* C_;
};

// Função que cada thread executará;
void* calcularParte_(void* arg_) {
    auto inicioTempo_ = chrono::high_resolution_clock::now();

    ThreadData_* dados_ = (ThreadData_*) arg_;
    int n1_ = dados_->A_->size();
    int m1_ = (*dados_->A_)[0].size();
    int m2_ = (*dados_->B_)[0].size();

    for (int idx_ = dados_->inicio_; idx_ < dados_->fim_; idx_++) {
        int i_ = idx_ / m2_;
        int j_ = idx_ % m2_;
        for (int k_ = 0; k_ < m1_; k_++) {
            (*(dados_->C_))[i_][j_] += (*(dados_->A_))[i_][k_] * (*(dados_->B_))[k_][j_];
        }
    }

    auto fimTempo_ = chrono::high_resolution_clock::now();
    double tempoMs_ = chrono::duration<double, milli>(fimTempo_ - inicioTempo_).count();

    // salvar resultado parcial em arquivo;
    string nomeArq_ = "resultado_thread_" + to_string(dados_->id_) + ".txt";
    ofstream arq_(nomeArq_);
    arq_ << "Tempo thread " << dados_->id_ << ": " << tempoMs_ << " ms;" << endl;
    arq_.close();

    pthread_exit(nullptr);
}

int main(int argc_, char* argv_[]) {
    if (argc_ != 4) {
        cout << "Uso: ./paralelo_threads M1.txt M2.txt P" << endl;
        return 1;
    }

    // TODO: ler matrizes, dividir elementos em blocos de tamanho P, criar threads e juntar resultados;

    return 0;
}