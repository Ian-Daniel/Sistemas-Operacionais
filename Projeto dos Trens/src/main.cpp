#include "mainwindow.h"
#include <QApplication>

/*
 * Função principal da aplicação;
 * Inicializa o ambiente Qt, cria a janela principal e inicia o loop de eventos;
 *
 * argc, argv → parâmetros da linha de comando repassados ao Qt;
 * QApplication a → gerencia os recursos da aplicação e o loop de eventos;
 * MainWindow w → instância da janela principal da interface gráfica;
 * w.show() → exibe a janela na tela;
 * a.exec() → inicia o loop de eventos da aplicação, mantendo-a em execeção.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Inicializa o sistema Qt;
    MainWindow w;               // Cria a janela principal;
    w.show();                   // Exibe a interface gráfica;

    return a.exec(); // Inicia o loop de eventos da aplicação.
}
