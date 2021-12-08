#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
 
//Definindo a dimensão do vetor.
#define TAM 10
 
//Criando o vetor globalmente.
int vetor[TAM];
 
//Criando a estrutura que serve de argumento para a função que representa cada thread.
typedef struct{ 
  int inicio;
  int tam;
}Par;

//Verificando se os extremos das partições estão ordenados entre si.
void verificaExtremos(int inicio, int fim){ 
  if(vetor[inicio] > vetor[fim] && fim < TAM){
    printf("Os extremos não estão ordenados!\n");
    exit(0);
  }
}

void* verificaOrdenacao(void* args){ 
  //Fazendo cast de cada elemento da estrutura de argumentos.
  int inicio = ((Par*)args) -> inicio;
  int tam = ((Par*)args) -> tam;

  int fim = inicio + tam;

  //Verificando se as partições estão ordenadas internamente entre si.
  if(inicio != fim){
    for(int i = inicio; i < fim - 1; i++){
      if(vetor[i] > vetor[i + 1]){
        printf("O vetor não está ordenado!\n");
        exit(0);
      }
    }
  }

  //Verificando os extremos de cada partição.
  verificaExtremos(inicio, fim);
}

int main(){
  int i;
  //Fazendo a leitura do vetor.
  for (i = 0; i < 10; i++){
    scanf("%d", &vetor[i]);
  }
  
  int n;
  scanf("%d", &n);
  //Inicializando o vetor de threads,
  pthread_t *threads = (pthread_t *) malloc(n*sizeof(pthread_t));
 
  //Calculando o tamanho que cada partição terá considerando que nem toda thread analisará a mesma quantidade de elementos.
  int tam1 = TAM / n;
  int tam2 = TAM - (((TAM / n) * (n - 1)));
 
  //Inicializando a estrutura de argumentos utilizada por cada thread.
  Par args;
  args.inicio = 0;
  args.tam = tam1;

  //Criando as primeiras n- 1 threads que irão analisar N / n posições.
  for(i = 0; i < n - 1; i++){
    int t = pthread_create(&threads[i], NULL, verificaOrdenacao, (void *) &args);
    if(t) printf("Que porra é essa?");
    args.inicio += tam1;
  }
 
  //Atualizando a estrutura de argumentos para a última thread.
  args.tam = tam2;
 
  //Criando a última thread que irá analisar  N - ((N / n) *(n - 1)) posições.
  pthread_create(&threads[i], NULL, verificaOrdenacao, (void*) &args);
 
  //Unindo as threads.
  for(i = 0; i < n; i++){
    pthread_join(threads[i], NULL);
  }

  printf("O vetor está ordenado!\n");

  //Desalocando a memória alocada.
  free(threads);
  pthread_exit(NULL);
}