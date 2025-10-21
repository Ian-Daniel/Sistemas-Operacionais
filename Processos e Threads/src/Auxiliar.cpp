#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// função utilizada para gerar matriz aleatória:
vector<vector<int>> gerarMatriz(int linhas, int colunas)
{
    // cria uma matriz (vetor de vetores) com a quantidade de linhas e colunas especificadas:
    vector<vector<int>> matriz(linhas, vector<int>(colunas));
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            matriz[i][j] = rand() % 10; // neste caso, são aceitos apenas números entre 0 e 9.
        }
    }
    return matriz;
}

// função para salvar matriz em arquivo:
void salvarMatriz(string nomeArquivo, const vector<vector<int>> &matriz)
{
    ofstream arquivo(nomeArquivo);
    int linhas = matriz.size();
    int colunas = matriz[0].size();
    arquivo << linhas << " " << colunas << endl;
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            arquivo << matriz[i][j] << " ";
        }
        arquivo << endl;
    }
    arquivo.close();
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cout << "Uso: ./Auxiliar n1 m1 n2 m2" << endl;
        return 1;
    }

    srand(time(nullptr));

    int n1 = atoi(argv[1]); // número de linhas da primeira matriz;
    int m1 = atoi(argv[2]); // número de colunas da primeira matriz;
    int n2 = atoi(argv[3]); // número de linhas da segunda matriz;
    int m2 = atoi(argv[4]); // número de colunas da segunda matriz.

    if (m1 != n2)
    {
        cout << "Multiplicação impossível: m1 deve ser igual a n2;" << endl;
        return 1;
    }

    vector<vector<int>> M1 = gerarMatriz(n1, m1);
    vector<vector<int>> M2 = gerarMatriz(n2, m2);

    salvarMatriz("M1.txt", M1);
    salvarMatriz("M2.txt", M2);

    cout << "Matrizes geradas e salvas em M1.txt e M2.txt;" << endl;

    return 0;
}