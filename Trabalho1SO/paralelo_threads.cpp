#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <chrono>
#include <cmath>

using namespace std;

// Estrutura para passar dados para cada thread
struct ThreadData {
    int id_;
    int p_; // número total de threads
    int n1_, m1_, n2_, m2_;
    vector<vector<int>> A_;
    vector<vector<int>> B_;
};

// Função executada por cada thread
void* multiplyPart(void* arg) {
    ThreadData* data_ = (ThreadData*) arg;

    auto start = chrono::high_resolution_clock::now();

    int totalElements = data_->n1_ * data_->m2_;
    int elemsPerThread = ceil((double) totalElements / data_->p_);
    int startIndex = data_->id_ * elemsPerThread;
    int endIndex = min(startIndex + elemsPerThread, totalElements);

    // Abrir arquivo para esta thread
    string filename = "resultado_thread_" + to_string(data_->id_) + ".txt";
    ofstream fout(filename);

    // Calcular apenas os elementos atribuídos
    for (int idx = startIndex; idx < endIndex; idx++) {
        int row = idx / data_->m2_;
        int col = idx % data_->m2_;
        int sum = 0;
        for (int k = 0; k < data_->m1_; k++) {
            sum += data_->A_[row][k] * data_->B_[k][col];
        }
        fout << row << " " << col << " " << sum << "\n";
    }

    auto end = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double, milli>(end - start).count();

    fout << "Tempo(ms): " << elapsed << endl;
    fout.close();

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Uso: " << argv[0] << " <matriz1.txt> <matriz2.txt> <P>" << endl;
        return 1;
    }

    string fileA = argv[1];
    string fileB = argv[2];
    int P = stoi(argv[3]);

    ifstream finA(fileA), finB(fileB);

    int n1, m1, n2, m2;
    finA >> n1 >> m1;
    finB >> n2 >> m2;

    if (m1 != n2) {
        cerr << "Erro: dimensões incompatíveis!" << endl;
        return 1;
    }

    vector<vector<int>> A(n1, vector<int>(m1));
    vector<vector<int>> B(n2, vector<int>(m2));

    for (int i = 0; i < n1; i++)
        for (int j = 0; j < m1; j++)
            finA >> A[i][j];

    for (int i = 0; i < n2; i++)
        for (int j = 0; j < m2; j++)
            finB >> B[i][j];

    finA.close();
    finB.close();

    // Criar threads
    vector<pthread_t> threads(P);
    vector<ThreadData> tdata(P);

    for (int i = 0; i < P; i++) {
        tdata[i] = {i, P, n1, m1, n2, m2, A, B};
        pthread_create(&threads[i], NULL, multiplyPart, &tdata[i]);
    }

    // Esperar todas terminarem
    for (int i = 0; i < P; i++) {
        pthread_join(threads[i], NULL);
    }

    cout << "Execução concluída! Resultados salvos em arquivos resultado_thread_*.txt" << endl;

    return 0;
}
