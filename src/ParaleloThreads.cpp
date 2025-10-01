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
struct ThreadData_ {
    int id_;
    int inicio_;
    int fim_;
    const vector<vector<int>>* A_;
    const vector<vector<int>>* B_;
    vector<vector<int>>* C_;
};

// função para criar o diretório de resultados, se necessário:
void ensure_results_dir_() {
    struct stat st_;
    if (stat("resultados", &st_) != 0) {
        mkdir("resultados", 0755); // neste caso cria a pasta "resultados/".
    }
}

// função para ler matriz de arquivo; neste caso lê tamanho seguido dos valores:
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

// função que cada thread executa; calcula o seu bloco e salva arquivo parcial:
void* calcularParte_(void* arg_) {
    auto inicioTempo_ = chrono::high_resolution_clock::now(); // marca início do cálculo da thread.

    ThreadData_* dados_ = (ThreadData_*) arg_;
    int m1_ = (*(dados_->A_))[0].size();
    int m2_ = (*(dados_->B_))[0].size();
    int n1_ = dados_->A_->size();

    // matriz parcial local, contendo apenas os elementos calculados por esta thread:
    vector<vector<int>> parcial_(n1_, vector<int>(m2_, 0)); // neste caso, zeros, inicialmente.

    for (int idx_ = dados_->inicio_; idx_ < dados_->fim_; ++idx_) {
        int i_ = idx_ / m2_;
        int j_ = idx_ % m2_;
        int soma_ = 0;
        for (int k_ = 0; k_ < m1_; ++k_) {
            soma_ += (*(dados_->A_))[i_][k_] * (*(dados_->B_))[k_][j_]; // isso aqui faz com que cada produto parcial contribua.
        }
        parcial_[i_][j_] = soma_;
        // atualiza a matriz compartilhada; caba fazendo com que C_ contenha a junção das partes:
        (*(dados_->C_))[i_][j_] = soma_;
    }

    auto fimTempo_ = chrono::high_resolution_clock::now();
    double tempoMs_ = chrono::duration<double, milli>(fimTempo_ - inicioTempo_).count();

    // salva arquivo parcial iniciando com o tamanho total da matriz, como exigido:
    string nomeArq_ = "resultados/resultado_thread_" + to_string(dados_->id_) + ".txt";
    ofstream out_(nomeArq_);
    if (!out_) {
        cerr << "Aviso: não foi possível abrir " << nomeArq_ << " para escrita;" << endl;
        pthread_exit(nullptr);
    }

    out_ << n1_ << " " << m2_ << endl; // cabeçalho com tamanho total.
    for (int i_ = 0; i_ < n1_; ++i_) {
        for (int j_ = 0; j_ < m2_; ++j_) {
            out_ << parcial_[i_][j_] << " ";
        }
        out_ << endl;
    }
    out_ << "Tempo(ms): " << tempoMs_ << endl;
    out_.close();

    pthread_exit(nullptr);
}

int main(int argc_, char* argv_[]) {
    if (argc_ != 4) {
        cout << "Uso: ./ParaleloThreads M1.txt M2.txt P" << endl;
        return 1;
    }

    ensure_results_dir_(); // cria diretório "resultados/", caso seja necessário;

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
    if (P_ <= 0) P_ = 1; // segurança (evitar as divisões por zero...).

    int n1_ = M1_.size();
    int m2_ = M2_[0].size();
    int totalElementos_ = n1_ * m2_;
    int numThreads_ = (totalElementos_ + P_ - 1) / P_; // divisão inteira, arredondando para cima.

    vector<vector<int>> C_(n1_, vector<int>(m2_, 0)); // matriz resultado compartilhada.
    vector<pthread_t> threads_(numThreads_);
    vector<ThreadData_> dados_(numThreads_);

    // cria threads; neste caso, cada thread processa um bloco de P_ elementos:
    for (int t_ = 0; t_ < numThreads_; ++t_) {
        dados_[t_].id_ = t_;
        dados_[t_].inicio_ = t_ * P_;
        dados_[t_].fim_ = min((t_ + 1) * P_, totalElementos_);
        dados_[t_].A_ = &M1_;
        dados_[t_].B_ = &M2_;
        dados_[t_].C_ = &C_;

        int rc_ = pthread_create(&threads_[t_], nullptr, calcularParte_, (void*) &dados_[t_]);
        if (rc_ != 0) {
            cerr << "Erro ao criar thread " << t_ << ";" << endl;
        }
    }

    // aguarda todas as threads terminarem:
    for (int t_ = 0; t_ < numThreads_; ++t_) {
        pthread_join(threads_[t_], nullptr);
    }

    // salva o resultado final combinado (isto aqui é opcional, mas, optamos por adicionar, uma vez que facilita a verificação);
    string nomeFinal_ = "resultados/resultado_threads_final.txt";
    ofstream outFinal_(nomeFinal_);
    outFinal_ << n1_ << " " << m2_ << endl;
    for (int i_ = 0; i_ < n1_; ++i_) {
        for (int j_ = 0; j_ < m2_; ++j_) {
            outFinal_ << C_[i_][j_] << " ";
        }
        outFinal_ << endl;
    }
    outFinal_.close();

    cout << "Multiplicação paralela com threads concluída; arquivos parciais e final criados em resultados/;" << endl;
    return 0;
}