#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fila.h"

#define TEMPO_DURACAO_DISCO 3
#define TEMPO_DURACAO_FITA 2
#define TEMPO_DURACAO_IMPRESSORA 2

int analisar_entrada_pronto(int tempo_atual, Fila* fila_novos_processos, Fila* fila_alta_prioridade) {


    int i = fila_novos_processos->frente;
    while (!fila_vazia(fila_novos_processos)) {
        if (fila_novos_processos->processos[i].instante_de_ativacao <= tempo_atual) {
            //printf("%d tempo atual\n", tempo_atual);
            //printf("Processo %d pronto para execução\n", fila_novos_processos->processos[i].pid);

            enfileirar(fila_alta_prioridade, fila_novos_processos->processos[i]);
            //printf("fila alta priori processos\n");
            //imprimir_fila(fila_alta_prioridade);
            i = (i + 1) % fila_novos_processos->capacidade;
            desenfileirar(fila_novos_processos);
            //printf("fila novos processos\n");
           //imprimir_fila(fila_novos_processos);
            
            
        } else {
            break;
        }
    }
    
    return 0;
}


int analisa_prontidao(int tempo_atual, Fila* fila_novos_processos, Fila* fila_alta_prioridade) {
    while (!fila_vazia(fila_novos_processos)) {
        Processo p = desenfileirar(fila_novos_processos);
        if (tempo_atual >= p.instante_de_ativacao) {
            enfileirar(fila_alta_prioridade, p);
        } else {
            enfileirar(fila_novos_processos, p);
            break;
        }
    }
    return 0;
}


bool preempcao(Fila* fila_ioDisco,  Fila* fila_io_fita,  Fila* fila_io_impressora,  Fila* fila_alta_prioridade, Fila* fila_baixa_prioridade) {
    // Verificar processos na fila de I/O
    int percorre = 0;
    bool preempcao = false;

    while (!fila_vazia(fila_ioDisco)) {
        
        Processo io_p = desenfileirar(fila_ioDisco);
        io_p.tempo_io--;
        //printf("tempo io: %d do processo %d\n", io_p.tempo_io, io_p.pid);
        if (io_p.tempo_io > 0) {
            enfileirar(fila_ioDisco, io_p);
            percorre++;
            if(percorre == fila_ioDisco->tamanho)
                break;
        } else {
            enfileirar(fila_baixa_prioridade, io_p);
            //enfileirar(fila_baixa_prioridade, p);
            preempcao = true;
            break;
        }
    }

    
    percorre = 0;

    while (!fila_vazia(fila_io_fita)) {

        Processo io_p = desenfileirar(fila_io_fita);
        io_p.tempo_io--;
        //printf("tempo io: %d do processo %d\n", io_p.tempo_io, io_p.pid);
        if (io_p.tempo_io > 0) {
            enfileirar(fila_io_fita, io_p);
            percorre++;
            if(percorre == fila_io_fita->tamanho)
                break;
        } else {
            enfileirar(fila_alta_prioridade, io_p);
            //enfileirar(fila_baixa_prioridade, p);
            preempcao = true;
            break;
        }
    }

    percorre = 0;

    while (!fila_vazia(fila_io_impressora)) {

        Processo io_p = desenfileirar(fila_io_impressora);
        io_p.tempo_io--;
        //printf("tempo io: %d do processo %d\n", io_p.tempo_io, io_p.pid);
        if (io_p.tempo_io > 0) {
            enfileirar(fila_io_impressora, io_p);
            percorre++;
            if(percorre == fila_io_impressora->tamanho)
                break;
        } else {
            enfileirar(fila_alta_prioridade, io_p);
            //enfileirar(fila_baixa_prioridade, p);
            preempcao = true;
            break;
        }
    }

    /*
    printf("fila disco\n");
    imprimir_fila(fila_ioDisco);
    printf("fila impressora\n");
    imprimir_fila(fila_io_impressora);
    printf("fila FIta\n");
    imprimir_fila(fila_io_fita);
    */
    return preempcao;
}


bool analisar_io_processo(Fila* fila_ioDisco,  Fila* fila_io_fita,  Fila* fila_io_impressora, Processo* p){

    if(p->tempo_ativa_io == 0)
        return false;

    p->tempo_ativa_io--;
    //printf("tempo ativacao io: %d\n", p->tempo_ativa_io);

    if(p->tempo_ativa_io == 0){

        
        //printf("Processo %d solicitou I/O\n", p->pid);
        if(p->tempo_io == TEMPO_DURACAO_DISCO){
            p->tempo_io++;
            enfileirar(fila_ioDisco, *p);
            //imprimir_fila(fila_ioDisco);
        } else if(p->tempo_io == TEMPO_DURACAO_FITA){
            p->tempo_io++;
            enfileirar(fila_io_fita, *p);
            //imprimir_fila(fila_io_fita);
        } else if(p->tempo_io == TEMPO_DURACAO_IMPRESSORA){
            p->tempo_io++;
            enfileirar(fila_io_impressora, *p);
            //imprimir_fila(fila_io_impressora);
        }
        
        return true;
    }

    return false;
}