#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NTHREADS 100
#define MAX 1000000

//Inicializando o mutex de forma estática.
pthread_mutex_t mutex_cont = PTHREAD_MUTEX_INITIALIZER;

//Inicializando a variável de condição.
int contador = 0;

bool iterador(void *threadid){
  //Travando a região crítica.
  pthread_mutex_lock(&mutex_cont);

  //Conferindo se o valor desejado foi antigido.
  if (contador < MAX){
    ++contador;

    //Verificando se a thread atual atingiu o valor desejado após incrementação.
    if (contador == MAX){
      printf("A thread %d chegou em 1m primeiro!\n", *((int*)threadid));
      //Destravando a região crítica e retornando true considerando que a thread atual atingiu o valor desejado.
      pthread_mutex_unlock(&mutex_cont);
      return true;
    }

    //Destravando a região crítica e retornando false considerando que a thread atual não atingiu o valor desejado.
    pthread_mutex_unlock(&mutex_cont);
    return false;
  } else {
    //Destravando a região crítica e retornando true considerando que outra thread atingiu o valor desejado. 
    pthread_mutex_unlock(&mutex_cont);
    return true;
  }
}

void *fthread(void *threadid){
  //Rodando a thread até que o contador não atinja o valor desejado.
  while(!iterador(threadid)){};

  //Terminando a thread.
  pthread_exit(NULL);
}

int main (){
  //Inicializando as threads.
  pthread_t threads[NTHREADS];
  int *ids[NTHREADS];

  //Criando as threads.
  int rc;
  for(int i = 0; i < NTHREADS; ++i){
    ids[i] = (int*) malloc(sizeof(int)); 
    *ids[i] = i;
    rc = pthread_create(&threads[i], NULL, fthread, (void *) ids[i]);
  }

  //Unindo as threads.
  for(int i = 0; i < NTHREADS; ++i){
    pthread_join(threads[i], NULL);    
  }

  //Desalocando a memória utilizada pelo vetor de ID's.
  for(int i = 0;  i < NTHREADS; ++i){
    free(ids[i]);
  }

  //Verificando o valor final do contador.
  printf("Valor final do contador: %d\n", contador);
  pthread_exit(NULL);
}