#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fila.h"
#include "processo.c"

#define MAX_PROCESSOS 3
#define FATIA_TEMPO 2
#define TEMPO_DURACAO_DISCO 3
#define TEMPO_DURACAO_FITA 2
#define TEMPO_DURACAO_IMPRESSORA 2

void roundRobin(Fila* fila_ioDisco,  Fila* fila_io_fita,  Fila* fila_io_impressora,  Fila* fila_alta_prioridade, Fila* fila_baixa_prioridade, bool processoE, int* tempo_atual){
    
    Processo p;

    if(processoE)
        p = desenfileirar(fila_alta_prioridade);
    else
        p = desenfileirar(fila_baixa_prioridade);


    for (int i = 0; i < FATIA_TEMPO; i++){

        bool saiuAntesDoQuantum = false;

        (*tempo_atual)++;
        p.tempo_execucao--;

        if(analisar_io_processo(fila_ioDisco, fila_io_fita, fila_io_impressora, p))
            saiuAntesDoQuantum = true;


        if(preempção(fila_ioDisco, fila_io_fita, fila_io_impressora, fila_alta_prioridade, fila_baixa_prioridade, p)){
        //Se ocorrer um pedido de IO e preempção ao mesmo tempo, não será possível enfileirar o processo na fila de baixa prioridade, mas sim
        //na fila de IO, para que o processo seja executado posteriormente.
            if(!saiuAntesDoQuantum){
                enfileirar(fila_baixa_prioridade, p);
            }

            saiuAntesDoQuantum = true;
        }
                

                
        if(saiuAntesDoQuantum){
            printf("Executando processo %d por %d unidades de tempo\n", p.pid, i);

            if(p.tempo_execucao <=0)
                printf("Processo %d concluído\n", p.pid);
            
            return;
        }

        if(p.tempo_execucao <=0){
            printf("Executando processo %d por %d unidades de tempo\n", p.pid, i);
            printf("Processo %d concluído\n", p.pid);

            return;
        }
        else if(i == FATIA_TEMPO){
            printf("Executando processo %d por %d unidades de tempo\n", p.pid, i);
        }else{
            continue;
        }

    }

    enfileirar(fila_baixa_prioridade, p);
    return;
}

int main() {
    srand(time(NULL));

    // Criar filas
    Fila* fila_novos_processos = criar_fila(MAX_PROCESSOS);
    Fila* fila_alta_prioridade = criar_fila(MAX_PROCESSOS);
    Fila* fila_baixa_prioridade = criar_fila(MAX_PROCESSOS);
    Fila* fila_ioDisco = criar_fila(MAX_PROCESSOS);
    Fila* fila_io_fita = criar_fila(MAX_PROCESSOS);
    Fila* fila_io_Impressora = criar_fila(MAX_PROCESSOS);
    FILE* arquivo = fopen("processos.txt", "r");

    // Criar processos
    // Ler os processos do arquivo
    while (!feof(arquivo)) {
        Processo p;
        if (fscanf(arquivo, "%d;%d;%d;%d\n", &p.pid, &p.instante_de_ativacao, &p.tempo_execucao, &p.tempo_io) == 4) {
            p.tempo_ativa_io = rand() % (p.tempo_io + 1);
            enfileirar(fila_novos_processos, p);
        }
    }
    

    if (fila_vazia(fila_novos_processos)) {
        for (int i = 1; i <= MAX_PROCESSOS; i++) {
            Processo p = {
                .pid = i,
                .tempo_execucao = rand() % 10 + 1,
                .tempo_io = rand() % 5 + 1,
                .tempo_ativa_io = rand() % (p.tempo_io + 1)
            };
            enfileirar(fila_novos_processos, p);
        }
    }


    // Simular o escalonamento
    int tempo_atual = 0;
    while (!fila_vazia(fila_alta_prioridade) || !fila_vazia(fila_baixa_prioridade) || !fila_vazia(fila_novos_processos) || !fila_vazia(fila_ioDisco) || !fila_vazia(fila_io_fita) || !fila_vazia(fila_io_Impressora)) {
        
        //analisa o tempo no qual os processos estarão prontos para entrar nas filas de prioridade.
        analisar_entrada_pronto(tempo_atual, fila_novos_processos, fila_alta_prioridade);


            // Verificar fila de alta prioridade
            if (!fila_vazia(fila_alta_prioridade)) {
                roundRobin(fila_ioDisco, fila_io_fita, fila_io_Impressora, fila_alta_prioridade, fila_baixa_prioridade, true, &tempo_atual);

            }
            // Verificar fila de baixa prioridade
            else if (!fila_vazia(fila_baixa_prioridade)) {
                roundRobin(fila_ioDisco, fila_io_fita, fila_io_Impressora, fila_alta_prioridade, fila_baixa_prioridade, false, &tempo_atual);
            }else{
                tempo_atual++;
            }
            
        }

    printf("acabou todos os processos\n");

    return 0;

}
    
