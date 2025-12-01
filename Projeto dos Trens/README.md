# IMD0036 - Sistemas Operacionais
Repositório criado com a finalidade de armazenar o trabalho prático da disciplina Sistemas Operacionais.

# Trabalho Prático 2 – Simulação de Malha Ferroviária (Threads, Mutexes e Sincronização)

## Universidade Federal do Rio Grande do Norte – Instituto Metrópole Digital
**Disciplina:** Sistemas Operacionais;
**Projeto:** Simulação de Malha Ferroviária com 6 Trens (Concorrência e Exclusão Mútua);
**Integrante:** Ian Daniel Varela Marques.

---

## Descrição do projeto

Este projeto implementa uma simulação de malha ferroviária composta por seis trens, cada um executado em uma thread independente. A aplicação utiliza mutexes para controle das regiões críticas da malha e aplica técnicas de sincronização a fim de evitar colisões, impasses e deadlocks. Também permite controlar a velocidade de cada trem individualmente, em conformidade com os requisitos definidos pela disciplina. 

---

## Estrutura do repositório

```
├── src/                      # Código-fonte principal;
│   ├── main.cpp              # Ponto de entrada da aplicação;
│   ├── mainwindow.h          # Declaração da janela principal;
│   ├── mainwindow.cpp        # Implementação da janela principal;
│   ├── mainwindow.ui         # Interface gráfica (Qt Designer);
│   ├── trem.h                # Declaração da classe Trem;
│   ├── trem.cpp              # Implementação da classe Trem;
│   └── Trem.pro              # Arquivo de projeto Qt;
└── README.md
```

---

## Como compilar

O projeto utiliza C++ e o Qt Framework. Para compilar, é necessário possuir o Qt 5.x (ou superior) instalado.

```cd src
qmake Trem.pro
make
```

## Como executar

```
./Trem
```

A aplicação inicia automaticamente a execução dos seis trens e disponibiliza sliders individuais para controle de velocidade, variando de 0 (parado) a 200 (máxima velocidade).

# Regiões críticas e sincronização

O sistema utiliza sete mutexes correspondentes às regiões críticas mínimas exigidas no enunciado. Entre as principais estratégias empregadas estão:

* Uso de tryLock() com timeout para evitar deadlocks;

* Liberação imediata da região após a travessia;

* Verificação prévia de disponibilidade antes de entrar em uma área crítica;

* Execução totalmente assíncrona: cada trem opera de forma independente, sem sincronizações artificiais.

## Funcionalidades implementadas

* Seis threads independentes (uma por trem);

* Controle individual de velocidade;

* Prevenção de colisões e deadlocks;

* Interface gráfica completa desenvolvida com Qt;

* Execução automática dos trens ao iniciar o programa;

* Sincronização com mutexes para cada região crítica da malha.

## Observações

* O projeto segue as especificações da Avaliação de Reposição da disciplina;

* Todo o código é baseado na estrutura disponibilizada pelo professor no SIGAA;

* A solução foi desenvolvida visando máximo paralelismo e ausência de travamentos;

* O repositório deve acompanhar vídeo explicativo e código completo na submissão oficial.

## Referências

* Documentação Qt – Threads e Sincronização;

* Conceitos de exclusão mútua e deadlock (Sistemas Operacionais).
