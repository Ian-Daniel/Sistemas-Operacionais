#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;

// função criada para ler matriz de arquivo:
vector<vector<int>> lerMatriz(string nomeArquivo)
{
    ifstream arquivo(nomeArquivo);
    int linhas, colunas;
    arquivo >> linhas >> colunas;
    vector<vector<int>> matriz(linhas, vector<int>(colunas));
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            arquivo >> matriz[i][j];
        }
    }
    arquivo.close();
    return matriz;
}

// função para multiplicar matrizes de forma sequencial:
vector<vector<int>> multiplicar(const vector<vector<int>> &A, const vector<vector<int>> &B)
{
    int n1 = A.size();
    int m1 = A[0].size();
    int m2 = B[0].size();
    vector<vector<int>> C(n1, vector<int>(m2, 0));

    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            for (int k = 0; k < m1; k++)
            {
                C[i][j] += A[i][k] * B[k][j]; // cálculo do elemento
            }
        }
    }
    return C;
}

// função para salvar matriz resultado em arquivo, com tempo:
void salvarResultado(string nomeArquivo, const vector<vector<int>> &C, double tempoMs)
{
    ofstream arquivo(nomeArquivo);
    int linhas = C.size();
    int colunas = C[0].size();
    arquivo << linhas << " " << colunas << endl;
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            arquivo << C[i][j] << " ";
        }
        arquivo << endl;
    }
    arquivo << "Tempo(ms): " << tempoMs << endl;
    arquivo.close();
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Uso: ./Sequencial M1.txt M2.txt Resultado.txt" << endl;
        return 1;
    }

    vector<vector<int>> M1 = lerMatriz(argv[1]);
    vector<vector<int>> M2 = lerMatriz(argv[2]);

    auto inicio = chrono::high_resolution_clock::now();
    vector<vector<int>> C = multiplicar(M1, M2);
    auto fim = chrono::high_resolution_clock::now();

    double tempoMs = chrono::duration<double, milli>(fim - inicio).count();

    salvarResultado(argv[3], C, tempoMs);

    cout << "Multiplicação sequencial concluída; Tempo: " << tempoMs << " ms;" << endl;

    return 0;
}