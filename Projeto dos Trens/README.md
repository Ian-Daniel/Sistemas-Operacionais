# IMD0036 - Sistemas Operacionais
Repositório criado com a finalidade de guardar o **Trabalho Prático 2** da disciplina **Sistemas Operacionais**.

# Trabalho Prático 2 – Sistemas Operacionais: Sincronização e Regiões Críticas

## Universidade Federal do Rio Grande do Norte – Instituto Metrópole Digital
**Disciplina:** Sistemas Operacionais;
**Projeto:** Controle de Trens em Malha Ferroviária (Threads, Mutexes e Semáforos);
**Integrantes:** Ian Daniel Varela Marques; Tiago de Melo Galvão.

---

## Descrição do projeto

Este projeto implementa uma **simulação de malha ferroviária** com **seis trens** independentes, utilizando **threads**, **mutexes** e/ou **semáforos** para o controle de **regiões críticas** e prevenção de **colisões e deadlocks**.  

Cada trem possui uma rota predefinida e uma **velocidade ajustável** através de um controle deslizante (*QSlider*). O objetivo é garantir o tráfego seguro e contínuo de todos os trens, mantendo **máximo paralelismo** entre as execuções.

---

## Objetivos

- Executar cada trem em **threads independentes**, com total assincronismo.  
- Utilizar **no mínimo 7 regiões críticas**, correspondentes aos pontos de colisão da malha.  
- Implementar o controle de velocidade individual de cada trem (intervalo: `0–200`).  
- Evitar **colisões** e **deadlocks** entre os trens.  
- Fazer uso exclusivo de **mutexes e/ou semáforos** para sincronização.  
- Garantir que os trens iniciem automaticamente ao abrir o programa.

---

## Estrutura do repositório

```
├── src/ # Códigos-fonte do projeto
│ ├── main.cpp
│ ├── trem.cpp
│ ├── trem.h
│ ├── mainwindow.cpp
│ └── mainwindow.h
├── ui/ # Interface gráfica (Qt)
│ └── mainwindow.ui
├── assets/ # Recursos visuais (imagens da malha)
│ └── malha.png
├── README.md
└── ProjetoTrens.pro # Arquivo de configuração do Qt
```

## Como Compilar

O projeto foi desenvolvido em **C++** utilizando o **Qt Framework**.  
Para compilar:

1. Abra o projeto no **Qt Creator**.  
2. Selecione o kit de compilação compatível (ex: *Desktop Qt 6.x GCC 64-bit*).  
3. Clique em **Executar (Ctrl+R)** para compilar e iniciar a simulação.

---

## Como executar

1. Ao iniciar o programa, os trens começam a se mover automaticamente.  
2. Ajuste a velocidade de cada trem por meio dos **controles deslizantes** na interface.  
3. Observe a simulação em diferentes composições de velocidade.  

### Parâmetros de velocidade:
- `0`: trem parado.  
- `200`: velocidade máxima (movimento quase imperceptível).  

---

## Demonstração

O vídeo de demonstração deve conter:
1. Todos os trens em **velocidade mínima**.  
2. Todos os trens em **velocidade máxima**.  
3. Diferentes combinações de velocidades (mínimo 5 segundos por caso).  
4. Breve explicação sobre o tratamento de **colisões e deadlocks**.

Duração máxima: **3 minutos**.

---

## Avaliação

Critérios de correção:
- Ausência de colisões e deadlocks ✅  
- Uso adequado de mutexes/semafóros ✅  
- Implementação coerente com o enunciado ✅  
- Interface funcional e organizada ✅  

Pontuação:
- **0 pts:** código inválido ou incompleto  
- **4 pts:** sem colisões, mas com deadlocks  
- **4–10 pts:** solução sem colisões nem deadlocks (nota proporcional à eficiência)

---

## Referências

- [Documentação Qt – QThread](https://doc.qt.io/qt-6/qthread.html)  
- [Mutexes e Semáforos em C++](https://cplusplus.com/reference/mutex/mutex/)  
- [Conceito de Deadlock – Wikipedia](https://en.wikipedia.org/wiki/Deadlock)

---

## Observações

- O código deve estar funcional e comentado conforme as diretrizes da disciplina.  
- A solução deve respeitar integralmente o enunciado fornecido pelo professor.  
- O vídeo explicativo deve acompanhar o envio final no SIGAA.  

---

## Licença

Este projeto é de caráter **acadêmico e didático**, desenvolvido exclusivamente para fins educacionais na UFRN – Instituto Metrópole Digital.