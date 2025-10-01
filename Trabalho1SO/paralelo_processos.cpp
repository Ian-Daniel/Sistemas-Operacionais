#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>
#include <cmath>

using namespace std;

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

    int totalElements = n1 * m2;
    int elemsPerProc = ceil((double) totalElements / P);

    // Criar processos filhos
    for (int id = 0; id < P; id++) {
        pid_t pid = fork();

        if (pid == 0) { 
            // Processo filho
            auto start = chrono::high_resolution_clock::now();

            int startIndex = id * elemsPerProc;
            int endIndex = min(startIndex + elemsPerProc, totalElements);

            string filename = "resultado_proc_" + to_string(id) + ".txt";
            ofstream fout(filename);

            for (int idx = startIndex; idx < endIndex; idx++) {
                int row = idx / m2;
                int col = idx % m2;
                int sum = 0;
                for (int k = 0; k < m1; k++) {
                    sum += A[row][k] * B[k][col];
                }
                fout << row << " " << col << " " << sum << "\n";
            }

            auto end = chrono::high_resolution_clock::now();
            double elapsed = chrono::duration<double, milli>(end - start).count();
            fout << "Tempo(ms): " << elapsed << endl;

            fout.close();
            _exit(0); // garante saída imediata do filho
        }
        else if (pid < 0) {
            cerr << "Erro ao criar processo!" << endl;
            return 1;
        }
    }

    // Pai espera todos os filhos
    for (int i = 0; i < P; i++) {
        wait(NULL);
    }

    cout << "Execução concluída! Resultados salvos em arquivos resultado_proc_*.txt" << endl;

    return 0;
}
