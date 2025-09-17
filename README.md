# IMD0036 - Sistemas Operacionais
Repositório criado com a finalidade de guardar um trabalho prático da disciplina Sistemas Operacionais.

# Trabalho Prático 1 – Sistemas Operacionais: Processos e Threads

## Universidade Federal do Rio Grande do Norte – Instituto Metrópole Digital
**Disciplina:** Sistemas Operacionais;
**Projeto:** Multiplicação de Matrizes – Sequencial e Paralelo (Threads e Processos);
**Integrantes:** Ian Daniel Varela Marques; Tiago de Melo Galvão.

---

## Descrição do projeto

Este projeto tem como objetivo implementar a multiplicação de matrizes de diferentes formas, avaliando o desempenho das abordagens sequencial e paralela (threads e processos). O projeto explora conceitos de **multiprogramação**, **programação paralela** e **pthreads**, permitindo comparar tempos de execução entre as soluções.

As implementações seguem rigorosamente as orientações do professor:

- Todas as classes e variáveis terminam com `_`;
- Código muito bem comentado, com comentários terminando em `;` e uso frequente de expressões como "neste caso" e "isso faz com que".  

---

## Estrutura do repositório

```
├── Auxiliar.cpp # Gera matrizes aleatórias M1 e M2 e salva em arquivos
├── Sequencial.cpp # Multiplicação de matrizes de forma sequencial
├── ParaleloThreads.cpp # Multiplicação de matrizes usando pthreads
├── ParaleloProcessos.cpp # Multiplicação de matrizes usando processos
├── headers/ # Arquivos de cabeçalho (.h) se necessário
├── Makefile # Script para compilar todos os programas
├── resultados/ # Pasta para armazenar arquivos de saída e tempos de execução
└── README.md # Este arquivo
```

---

## Como compilar

O projeto utiliza **C++17** e pode ser compilado usando o `Makefile`. Para compilar todos os programas:

```bash
make all
```

Para compilar um programa específico:

```bash
make Auxiliar
make Sequencial
make ParaleloThreads
make ParaleloProcessos
```

## Como executar

# 1. Gerar Matrizes Aleatórias
   ```bash
   ./Auxiliar n1 m1 n2 m2
   ```
- n1, m1 → dimensões da matriz M1

- n2, m2 → dimensões da matriz M2

- Matrizes serão salvas em arquivos separados para uso posterior.

# 2. Multiplicação Sequencial
  ```bash
  ./Sequencial arquivoM1 arquivoM2 arquivoResultado
  ```
- Salva o resultado em arquivoResultado, incluindo o tempo total de execução.

# 3. Multiplicação Paralela com Threads
   ```bash
   ./ParaleloThreads arquivoM1 arquivoM2 P
   ```
  - ```P``` → número de elementos por thread.

  - Salva arquivos separados para cada segmento da matriz resultado, incluindo o tempo de execução de cada thread.

# 4. Multiplicação Paralela com Processos
   ```bash
   ./ParaleloProcessos arquivoM1 arquivoM2 P
   ```
  - ```P``` → número de elementos por processo.

  - Salva arquivos separados para cada segmento da matriz resultado, incluindo o tempo de execução de cada processo.

## Experimentos

# E1 – Comparação Sequencial vs Paralelo

- Testar com matrizes de tamanhos crescentes (100x100, 200x200, 400x400...);

- Calcular tempo médio em 10 execuções para cada tamanho;

- Gerar gráfico comparando tempos.

# E2 – Ajuste do Valor de P

- Testar diferentes valores de P para threads e processos;

- Calcular tempo médio em 10 execuções;

- Gerar gráfico comparando tempos em função de P.

## Observações

- Código deve ser compilável e comentado conforme as regras do professor;

- Vídeo explicativo de até 2 minutos sobre implementação e análise de resultados;

- Relatório com discussão sobre os experimentos e escolha do P ideal.

## Referências

- [Lei de Gustafson](https://en.wikipedia.org/wiki/Gustafson%27s_law)
