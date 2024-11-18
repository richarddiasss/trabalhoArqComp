#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

/*
void inicializar_fila(Fila* f, int capacidade) {
    f->inicio = NULL;
    f->fim = NULL;
    f->tamanho = 0;
    f->capacidade = capacidade;
}

Fila* criar_fila(int capacidade) {
    Fila* f = (Fila*)malloc(sizeof(Fila));
    f->processos = (Processo*)malloc(capacidade * sizeof(Processo));
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
    f->capacidade = capacidade;
    return f;
} 

bool fila_vazia(Fila* f) {
    return f->tamanho == 0;
}

bool fila_cheia(Fila* f) {
    return f->tamanho == f->capacidade;
}

int tamanho_fila(Fila* f) {
    return f->tamanho;
}

void percorrer_fila(Fila* f) {
    for (Processo* p = f->inicio; p != NULL; p = p->proximo) {
        callback(*p);
    }
}

void enfileirar(Fila* f, Processo* p)
*/


Fila* criar_fila(int capacidade) {
    Fila* f = (Fila*)malloc(sizeof(Fila));
    f->processos = (Processo*)malloc(capacidade * sizeof(Processo));
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
    f->capacidade = capacidade;
    return f;
}

bool fila_vazia(Fila* f) {
    return f->tamanho == 0;
}

bool fila_cheia(Fila* f) {
    return f->tamanho == f->capacidade;
}

void enfileirar(Fila* f, Processo p) {
    if (fila_cheia(f)) {
        printf("Erro: Fila cheia\n");
        return;
    }
    f->tras = (f->tras + 1) % f->capacidade;
    f->processos[f->tras] = p;
    f->tamanho++;
}

Processo desenfileirar(Fila* f) {
    if (fila_vazia(f)) {
        printf("Erro: Fila vazia\n");
        // Retornar um processo padrão em caso de fila vazia
        return (Processo){-1, 0, 0};
    }
    Processo p = f->processos[f->frente];
    f->frente = (f->frente + 1) % f->capacidade;
    f->tamanho--;
    return p;
}

void imprimir_fila(Fila* f) {
    
    if(fila_vazia(f)){
        printf("Fila vazia\n");
        return;
    }
    
    printf("Fila: ");
    for (int i = f->frente; i <= f->tras; i++) {
        printf("PID: %d, Tempo de Inicializacao: %d , Tempo de Execucao: %d, Tempo de ativacao I/O: %d ,  Tempo de I/O: %d \n", f->processos[i].pid, f->processos[i].instante_de_ativacao, f->processos[i].tempo_execucao, f->processos[i].tempo_ativa_io, f->processos[i].tempo_io);
    }
    printf("\n");
    
}