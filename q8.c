#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

//Definindo as limitações de acordo com a questão.
#define QTDPASSAGEIROS 20
#define MAXVOLTAS 10
#define VAGASCARRINHO 10

//Criando uma estrutura que representa o nó de cada fila.
typedef struct No{
  int valor;
  struct No *next;
} No;

//Criando uma estrutura que representa a fila.
typedef struct Fila{
  No *header;
  No *tail;
  int size;
} Fila;

//Inicializando o mutex de forma estática.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

//Inicializando o buffer que irá representar as vagas no carrinho.
int buffer[VAGASCARRINHO];
int numVoltas = 0;

//Inicializando a fila de passageiros, o vetor de threads e os ids associados a cada um.
Fila *filaPassageiros;
int *ids[QTDPASSAGEIROS];
pthread_t passageiro[QTDPASSAGEIROS];

//Inicializando a thread do carrinho.
pthread_t carrinho;


//Criando um nó.
No *criaNo(int i, No *nextVal){
  No *n = (No*) malloc(sizeof(No));
  n->valor = i;
  n->next = nextVal;
  return n;
}

//Crinado o header.
No *criaSentinela(No *nextVal){
  No *n = (No*) malloc(sizeof(No));
  n->next = nextVal;
  return n;
}

//Criando uma fila.
Fila *criaFila(){
  Fila *f = (Fila *) malloc(sizeof(Fila));
  f->tail = criaSentinela(NULL);
  f->header = f->tail;
  f->size = 0;
  return f;
}

//Colocando um passageiro na fila.
void colocaFila(int id){
  //Travando a região crítica.
  pthread_mutex_lock(&mutex);

  filaPassageiros->tail->next = criaNo(id, NULL);
  filaPassageiros->tail = filaPassageiros->tail->next;
  (filaPassageiros->size)++;

  //Destravando a região crítica.
  pthread_mutex_unlock(&mutex);
}

//Retirando um passageiro da fila.
int retiraFila(){
  if(filaPassageiros->size == 0){
    printf("A fila esta vazia!");
  }

  int valor = filaPassageiros->header->next->valor;
  filaPassageiros->header->next = filaPassageiros->header->next->next;

  if(filaPassageiros->header->next == NULL){
    filaPassageiros->tail = filaPassageiros->header;
  }

  (filaPassageiros->size)--;
  return valor;
}

void* fCarrinho(){
  while(++numVoltas < MAXVOLTAS){
    printf("Carrinho ta enchendo!!\n");
    for(int i = 0; i < VAGASCARRINHO; i++){
      //Retirando elementos da fila e colocando no buffer.
      buffer[i] = retiraFila();
    }

    printf("Dando a [%d] volta na montanha russa!!\n", numVoltas);

    printf("Saindo do carrinho!!\n");

    for(int i = 0; i < VAGASCARRINHO; i++){
      //Retirando os elementos do carrinho e colocando no final da fila.
      colocaFila(buffer[i]);
    }

    printf("Dando uma volta no parque.....\n");
  }

  pthread_exit(NULL);
}

//Adicionando os passageiros iniciais na fila.
void* fPassageiros(void*args){
  colocaFila((*(int*)args)); 
  pthread_exit(NULL);
}

void* fMontanhaRussa(){
  //Criando o vetor de threads.
  for(int i = 0 ; i < QTDPASSAGEIROS ; i++){
    ids[i] = (int*) malloc(sizeof(int));
    *ids[i] = i;
    pthread_create(&passageiro[i], NULL, fPassageiros, (void *) ids[i]);
  }
  
  //Criando a thread de carrinho.
  pthread_create(&carrinho, NULL, fCarrinho, NULL);

  //Unindo as threads.
  for(int i = 0; i < QTDPASSAGEIROS; i++){
    pthread_join(passageiro[i], NULL);
  }
  pthread_join(carrinho, NULL);

  //Desalocando a memória alocada.
  for(int i = 0;  i < QTDPASSAGEIROS; i++){
  free(ids[i]);
  } 

  pthread_exit(NULL);
}

int main(){
  //Inicializando a fila.
  filaPassageiros =  criaFila();
  
  //Criando a thread da montanha russa.
  pthread_t montanhaRussa;
  pthread_create(&montanhaRussa, NULL, fMontanhaRussa, NULL);
  
  pthread_exit(NULL);
}