#ifndef FILA_H
#define FILA_H
#include <stdbool.h>



// Estrutura para representar um processo
typedef struct {
    int pid;
    int instante_de_ativacao;
    int tempo_execucao;
    int tempo_ativa_io;
    int tempo_io;
} Processo;

// Estrutura para representar a fila
typedef struct {
    Processo* processos;
    int frente;
    int tras;
    int tamanho;
    int capacidade;
} Fila;

// Funções para manipular a fila
Fila* criar_fila(int capacidade);
bool fila_vazia(Fila* f);
bool fila_cheia(Fila* f);
void enfileirar(Fila* f, Processo p);
Processo desenfileirar(Fila* f);
void imprimir_fila(Fila* f);

//Funçoes para manipular processos
extern int analisar_entrada_pronto(int tempo_atual, Fila* fila_novos_processos, Fila* fila_alta_prioridadel);
extern int analisa_prontidao(int tempo_atual, Fila* fila_novos_processos, Fila* fila_alta_prioridade);
extern bool preempcao(Fila* fila_ioDisco, Fila* fila_io_fita, Fila* fila_io_impressora, Fila* fila_alta_prioridade, Fila* fila_baixa_prioridad, int tempo_atual);
extern bool analisar_io_processo(Fila* fila_ioDisco, Fila* fila_io_fita, Fila* fila_io_impressora, Processo* p, int tempo_atual);

#endif