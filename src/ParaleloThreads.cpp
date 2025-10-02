#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <pthread.h>
#include <chrono>
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

// estrutura de dados passada para cada thread:
struct ThreadData
{
    int id;
    int inicio;
    int fim;
    const vector<vector<int>> *A;
    const vector<vector<int>> *B;
    vector<vector<int>> *C;
};

// função para criar o diretório de resultados, se necessário:
void ensureResultsDir()
{
    struct stat st;
    if (stat("resultados", &st) != 0)
    {
        mkdir("resultados", 0755); // neste caso cria a pasta "resultados/".
    }
}

// função para ler matriz de arquivo; neste caso lê tamanho seguido dos valores:
vector<vector<int>> lerMatriz(const string &nomeArquivo)
{
    ifstream arq(nomeArquivo);
    if (!arq)
    {
        cerr << "Erro ao abrir arquivo " << nomeArquivo << ";" << endl;
        return {};
    }
    int linhas, colunas;
    arq >> linhas >> colunas;
    vector<vector<int>> matriz(linhas, vector<int>(colunas, 0));
    for (int i = 0; i < linhas; ++i)
    {
        for (int j = 0; j < colunas; ++j)
        {
            arq >> matriz[i][j];
        }
    }
    arq.close();
    return matriz;
}

// função que cada thread executa; calcula o seu bloco e salva arquivo parcial:
void *calcularParte(void *arg)
{
    auto inicioTempo = chrono::high_resolution_clock::now(); // marca início do cálculo da thread.

    ThreadData *dados = (ThreadData *)arg;
    int m1 = (*(dados->A))[0].size();
    int m2 = (*(dados->B))[0].size();
    int n1 = dados->A->size();

    // matriz parcial local, contendo apenas os elementos calculados por esta thread:
    vector<vector<int>> parcial(n1, vector<int>(m2, 0)); // neste caso, zeros, inicialmente.

    for (int idx = dados->inicio; idx < dados->fim; ++idx)
    {
        int i = idx / m2;
        int j = idx % m2;
        int soma = 0;
        for (int k = 0; k < m1; ++k)
        {
            soma += (*(dados->A))[i][k] * (*(dados->B))[k][j]; // cada produto parcial contribui.
        }
        parcial[i][j] = soma;
        // atualiza a matriz compartilhada:
        (*(dados->C))[i][j] = soma;
    }

    auto fimTempo = chrono::high_resolution_clock::now();
    double tempoMs = chrono::duration<double, milli>(fimTempo - inicioTempo).count();

    // salva arquivo parcial iniciando com o tamanho total da matriz:
    string nomeArq = "resultados/resultado_thread_" + to_string(dados->id) + ".txt";
    ofstream out(nomeArq);
    if (!out)
    {
        cerr << "Aviso: não foi possível abrir " << nomeArq << " para escrita;" << endl;
        pthread_exit(nullptr);
    }

    out << n1 << " " << m2 << endl; // cabeçalho com tamanho total.
    for (int i = 0; i < n1; ++i)
    {
        for (int j = 0; j < m2; ++j)
        {
            out << parcial[i][j] << " ";
        }
        out << endl;
    }
    out << "Tempo(ms): " << tempoMs << endl;
    out.close();

    pthread_exit(nullptr);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Uso: ./ParaleloThreads M1.txt M2.txt P" << endl;
        return 1;
    }

    ensureResultsDir(); // cria diretório "resultados/", caso necessário;

    vector<vector<int>> M1 = lerMatriz(argv[1]);
    vector<vector<int>> M2 = lerMatriz(argv[2]);
    if (M1.empty() || M2.empty())
    {
        cerr << "Erro: leitura de matrizes falhou;" << endl;
        return 1;
    }

    int m1 = M1[0].size();
    int n2 = M2.size();
    if (m1 != n2)
    {
        cerr << "Multiplicação impossível: m1 deve ser igual a n2;" << endl;
        return 1;
    }

    int P = atoi(argv[3]);
    if (P <= 0)
        P = 1; // segurança (evitar divisões por zero...).

    int n1 = M1.size();
    int m2 = M2[0].size();
    int totalElementos = n1 * m2;
    int numThreads = (totalElementos + P - 1) / P; // divisão inteira, arredondando para cima.

    vector<vector<int>> C(n1, vector<int>(m2, 0)); // matriz resultado compartilhada.
    vector<pthread_t> threads(numThreads);
    vector<ThreadData> dados(numThreads);

    // cria threads; cada thread processa um bloco de P elementos:
    for (int t = 0; t < numThreads; ++t)
    {
        dados[t].id = t;
        dados[t].inicio = t * P;
        dados[t].fim = min((t + 1) * P, totalElementos);
        dados[t].A = &M1;
        dados[t].B = &M2;
        dados[t].C = &C;

        int rc = pthread_create(&threads[t], nullptr, calcularParte, (void *)&dados[t]);
        if (rc != 0)
        {
            cerr << "Erro ao criar thread " << t << ";" << endl;
        }
    }

    // aguarda todas as threads terminarem:
    for (int t = 0; t < numThreads; ++t)
    {
        pthread_join(threads[t], nullptr);
    }

    // salva o resultado final combinado:
    string nomeFinal = "resultados/resultado_threads_final.txt";
    ofstream outFinal(nomeFinal);
    outFinal << n1 << " " << m2 << endl;
    for (int i = 0; i < n1; ++i)
    {
        for (int j = 0; j < m2; ++j)
        {
            outFinal << C[i][j] << " ";
        }
        outFinal << endl;
    }
    outFinal.close();

    cout << "Multiplicação paralela com threads concluída; arquivos parciais e final criados em resultados/;" << endl;
    return 0;
}