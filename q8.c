#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define QTDPASSAGEIROS 20
#define MAXVOLTAS 10
#define VAGASCARRINHO 10


typedef struct No{
  int valor;
  struct No *next;
}No;

typedef struct Fila{
  No *header;
  No *tail;
  int size;
}Fila;


int tamFila;
int buffer[VAGASCARRINHO]; //buffer responsável por representar as vagas no carrinho
int numVoltas;

//cria um nó, seta o valor e aponta o tail para NULL
No *criaNo(int i, No *nextVal){
  No *n = (No*) malloc(sizeof(No));
  n->valor = i;
  n->next = nextVal;
  return n;
}

//cria o nó sentinela
No *criaSentinela(No *nextVal){
  No *n = (No*) malloc(sizeof(No));
  n->next = nextVal;
  return n;

}

//Função que faz o setup da fila 
Fila *criaFila(){

  Fila *f = (Fila *) malloc(sizeof(Fila));
  f->tail = criaSentinela(NULL);
  f->header = f->tail;
  f->size = 0;
  return f;
}

//criando a fila de passageiros
Fila *filaPassageiros;
pthread_t *passageiro;
pthread_t carrinho;


void colocaFila(int id){

  filaPassageiros->tail->next = criaNo(id, NULL);
  filaPassageiros->tail = filaPassageiros->tail->next;
  (filaPassageiros->size)++;
  printf("tamFila = %d\n", filaPassageiros->size);
}


int retiraFila(){

  int valor;


  if(filaPassageiros->size == 0){
    printf("A fila esta vazia!");
  }

  valor = filaPassageiros->header->next->valor;
  filaPassageiros->header->next = filaPassageiros->header->next->next;

  if(filaPassageiros->header->next == NULL){
    filaPassageiros->tail = filaPassageiros->header;
  }
  (filaPassageiros->size)--;
  printf("dimiuidoF=%d\n", filaPassageiros->size);

  return valor;
}

void* fCarrinho(){

  int i, id;

  printf("Carrinho ta enchendo!!\n");
  
  
  for(i=0;i<VAGASCARRINHO;i++){
    id = retiraFila();
    buffer[i] = id;
  }
 

  printf("Dando a [%d] volta na montanha russa!!\n", numVoltas);

  printf("Saindo do carrinho!!\n");

  for(i=0;i<VAGASCARRINHO;i++){
    id = buffer[i];
    colocaFila(id);
    buffer[i] = 0;
  }

  numVoltas++;

  printf("Dando uma volta no parque.....\n");

  for(i=0;i<10;i++){
    printf("%d ", buffer[i]);
  }
  printf("\n");  

}

//função responsável por colocar 
void* fPassageiros(void*args){

  int id = (*(int*)args);
  colocaFila(id);
  
}

void* fMontanhaRussa(){

  passageiro = (pthread_t *) malloc(QTDPASSAGEIROS*sizeof(pthread_t));

  numVoltas = 0;

  int i, *ids[QTDPASSAGEIROS];

    for(i = 0 ; i < QTDPASSAGEIROS ; i++){
      ids[i] = (int*) malloc(sizeof(int));
      *ids[i] = i;
      pthread_create(&passageiro[i], NULL, fPassageiros, (void *) ids[i]);
    }
    pthread_create(&carrinho, NULL, fCarrinho, NULL);
    if(numVoltas == MAXVOLTAS){
      for(i = 0;  i < QTDPASSAGEIROS; i++){
      free(ids[i]);
      } 
      exit(0);
    }

  
  
 
}


//Função responsável por imprimir a fila
void imprimeFila()
{
  No *cursor = filaPassageiros->header;
  while (cursor->next != NULL)
  {
    printf("%d\n", cursor->next->valor);
    cursor = cursor->next;
  }

}

int main(){

  filaPassageiros =  criaFila();

  pthread_t montanhaRussa;
 
  //imprimeFila();

  pthread_create(&montanhaRussa, NULL, fMontanhaRussa, NULL);
  

  pthread_exit(NULL);
}