#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define TAM 10

int vetor[TAM];
int n;
int ordenado = 0;

//Estrutura que contém o índice de ínicio e fim da partição.
  typedef struct{ 
    int inicio;
    int tam;
  }Par;

//Função que verifica se as partições estão ordenadas entre si.
  void verificaExtremos(int inicio, int fim){ 
    if(vetor[inicio] > vetor[fim + 1] && fim < TAM){
      printf("Os extremos não estão ordenados!\n");
      exit(0);
    }
    ordenado++;
  }

//Função que verifica se as partições estão ordenadas
  void* verificaOrdenacao(void*args){
    int i;

    int inicio = ((Par*)args) -> inicio;
    int tam = ((Par*)args) -> tam;

    int fim = inicio + tam;

    printf("Na funcao inicio: %d e fim %d\n", inicio, fim);
    if(inicio != fim){
      for(i = inicio; i < fim; i++){
        if(vetor[i] > vetor[i + 1]){
          printf("vetor i:%d      ----      vetor i + 1:%d\n", vetor[i], vetor[i + 1]);
          printf("Não esta ordenado!\n");
          exit(0);
        }
      }
    }

    verificaExtremos(inicio, fim);
    if(ordenado == n){
      printf("Esta ordenado!\n");
    }
  }

int main(){
  int i;

  for (i = 0; i < 10; i++){
    scanf("%d", &vetor[i]);
  }

  scanf("%d", &n);
  pthread_t *threads = (pthread_t *) malloc(n*sizeof(pthread_t));

  int tam1 = TAM / n;
  int tam2 = TAM - (((TAM / n) * (n - 1)));

  Par args;
  args.inicio = 0;
  args.tam = tam1 - 1;
  for(i = 0; i < n - 1; i++){
    printf("Na main inicio: %d, fim: %d\n", args.inicio, (args.inicio + args.tam));
    pthread_create(&threads[i], NULL, verificaOrdenacao, (void *) &args);
    args.inicio += (i < n ? tam1 : tam2);
  }

  //Atualizando a estrutura de argumentos para a última thread.
  args.tam = tam2 - 1;
  printf("Na main inicio: %d, fim: %d\n", args.inicio, (args.inicio + args.tam));

  //Criando a última theread que irá analisar  N - ((N / n) *(n - 1)) linhas.
  pthread_create(&threads[i], NULL, verificaOrdenacao, (void*) &args);

  //Unindo as threads.
  for(i = 0; i < n; i++){
    pthread_join(threads[i], NULL);
  }
 
  pthread_exit(NULL);
}