#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fila.h"

#define MAX_PROCESSOS 3
#define FATIA_TEMPO 2
#define TEMPO_DURACAO_DISCO 3
#define TEMPO_DURACAO_FITA 2
#define TEMPO_DURACAO_IMPRESSORA 1

void roundRobin(Fila* fila_ioDisco,  Fila* fila_io_fita,  Fila* fila_io_impressora,  Fila* fila_alta_prioridade, Fila* fila_baixa_prioridade, bool processoE, int* tempo_atual){
    
    Processo p;

    if(processoE)
        p = desenfileirar(fila_alta_prioridade);
    else
        p = desenfileirar(fila_baixa_prioridade);


    for (int i = 0; i < FATIA_TEMPO; i++){

        bool saiuAntesDoQuantum = false;
        bool foiIo = false;
        (*tempo_atual)++;
        p.tempo_execucao--;
        //printf("tempo atual: %d, tempo faltante de execucao: %d e processo: %d \n", (*tempo_atual), p.tempo_execucao, p.pid);

        if(analisar_io_processo(fila_ioDisco, fila_io_fita, fila_io_impressora, &p)){
            saiuAntesDoQuantum = true;
            foiIo = true;
        }


        if(preempcao(fila_ioDisco, fila_io_fita, fila_io_impressora, fila_alta_prioridade, fila_baixa_prioridade)){
        //Se ocorrer um pedido de IO e preempção ao mesmo tempo, não será possível enfileirar o processo na fila de baixa prioridade, mas sim
        //na fila de IO, para que o processo seja executado posteriormente.
            if(!saiuAntesDoQuantum){
                if(p.tempo_execucao > 0)
                    enfileirar(fila_baixa_prioridade, p);
            }

            saiuAntesDoQuantum = true;
        }
                

                
        if(saiuAntesDoQuantum && foiIo){
            printf("Executando processo %d por %d unidades de tempo\n", p.pid, i+1);
            printf("Processo %d solicitou I/O\n", p.pid);

            if(p.tempo_execucao <=0)
                printf("Processo %d concluído no tempo %d\n", p.pid, (*tempo_atual));
    
            return;
        } else if (saiuAntesDoQuantum){

            printf("Processo %d solicitou I/O\n", p.pid);
            printf("Executando processo %d por %d unidades de tempo\n", p.pid, i+1);

            if(p.tempo_execucao <=0)
                printf("Processo %d concluído no tempo %d\n", p.pid, (*tempo_atual));
        }
        

        if(p.tempo_execucao <=0){
            printf("Executando processo %d por %d unidades de tempo\n", p.pid, i+1);
            printf("Processo %d concluído no tempo %d\n", p.pid, (*tempo_atual));

            return;
        }
        else if(i == FATIA_TEMPO - 1){
            printf("Executando processo %d por 2 unidades de tempo\n", p.pid);
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
    FILE* arquivo = fopen("processos3.txt", "r");

    // Criar processos
    // Ler os processos do arquivo
    int i = 0;
    while (!feof(arquivo)) {
        Processo p;
        
        if (fscanf(arquivo, "%d;%d;%d;%d\n", &p.instante_de_ativacao, &p.tempo_execucao, &p.tempo_io, &p.tempo_ativa_io) == 4) {
            p.pid = i;
            i++;
            enfileirar(fila_novos_processos, p);
        }
    }

    //imprimir_fila(fila_novos_processos);
    



    // Simular o escalonamento
    int tempo_atual = 0;
    while (!fila_vazia(fila_alta_prioridade) || !fila_vazia(fila_baixa_prioridade) || !fila_vazia(fila_novos_processos) || !fila_vazia(fila_ioDisco) || !fila_vazia(fila_io_fita) || !fila_vazia(fila_io_Impressora)) {
        
        //analisa o tempo no qual os processos estarão prontos para entrar nas filas de prioridade.
        analisar_entrada_pronto(tempo_atual, fila_novos_processos, fila_alta_prioridade);
        //printf("teste 1\n");
        //imprimir_fila(fila_novos_processos);
        //imprimir_fila(fila_alta_prioridade);

            // Verificar fila de alta prioridade
            if (!fila_vazia(fila_alta_prioridade)){
                roundRobin(fila_ioDisco, fila_io_fita, fila_io_Impressora, fila_alta_prioridade, fila_baixa_prioridade, true, &tempo_atual);

            }
            // Verificar fila de baixa prioridade
            else if (!fila_vazia(fila_baixa_prioridade)) {
                roundRobin(fila_ioDisco, fila_io_fita, fila_io_Impressora, fila_alta_prioridade, fila_baixa_prioridade, false, &tempo_atual);
            }
            else if(!fila_vazia(fila_io_fita) || !fila_vazia(fila_ioDisco) || !fila_vazia(fila_io_Impressora)){
                tempo_atual++;
                preempcao(fila_ioDisco, fila_io_fita, fila_io_Impressora, fila_alta_prioridade, fila_baixa_prioridade);
            }
            else{ 
                tempo_atual++;
            }
        /* 
        printf("fila baixa priori processos\n");
        imprimir_fila(fila_baixa_prioridade);
        printf("fila alta priori processos\n");
        imprimir_fila(fila_alta_prioridade);
        */
            
    }

    printf("acabou todos os processos\n");
    fclose(arquivo);
    return 0;

}
    
