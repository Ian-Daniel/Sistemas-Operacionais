#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// Função para gerar matriz aleatória;
vector<vector<int>> gerarMatriz_(int linhas_, int colunas_) {
    vector<vector<int>> matriz_(linhas_, vector<int>(colunas_));
    for (int i_ = 0; i_ < linhas_; i_++) {
        for (int j_ = 0; j_ < colunas_; j_++) {
            matriz_[i_][j_] = rand() % 10; // neste caso números entre 0 e 9;
        }
    }
    return matriz_;
}

// Função para salvar matriz em arquivo;
void salvarMatriz_(string nomeArquivo_, const vector<vector<int>>& matriz_) {
    ofstream arquivo_(nomeArquivo_);
    int linhas_ = matriz_.size();
    int colunas_ = matriz_[0].size();
    arquivo_ << linhas_ << " " << colunas_ << endl;
    for (int i_ = 0; i_ < linhas_; i_++) {
        for (int j_ = 0; j_ < colunas_; j_++) {
            arquivo_ << matriz_[i_][j_] << " ";
        }
        arquivo_ << endl;
    }
    arquivo_.close();
}

int main(int argc_, char* argv_[]) {
    if (argc_ != 5) {
        cout << "Uso: ./Auxiliar n1 m1 n2 m2" << endl;
        return 1;
    }

    srand(time(nullptr));

    int n1_ = atoi(argv_[1]);
    int m1_ = atoi(argv_[2]);
    int n2_ = atoi(argv_[3]);
    int m2_ = atoi(argv_[4]);

    if (m1_ != n2_) {
        cout << "Multiplicação impossível: m1 deve ser igual a n2;" << endl;
        return 1;
    }

    vector<vector<int>> M1_ = gerarMatriz_(n1_, m1_);
    vector<vector<int>> M2_ = gerarMatriz_(n2_, m2_);

    salvarMatriz_("M1.txt", M1_);
    salvarMatriz_("M2.txt", M2_);

    cout << "Matrizes geradas e salvas em M1.txt e M2.txt;" << endl;

    return 0;
}