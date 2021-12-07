#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define tam 10

pthread_mutex_t mutexEscritoras = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t escrevendo= PTHREAD_MUTEX_INITIALIZER;

int *regiaocritica;

void* fazEscrita(void*args){

  int i = (*(int*)args);
  int posicao;
  char letraAleatoria;

  while(1){

    pthread_mutex_lock(&mutexEscritoras); //trava o mutex

    letraAleatoria = 'A' + (random()%26);
    posicao = rand()%(tam-1);

    regiaocritica[posicao] = letraAleatoria;

    printf("A thread[%d] esta escrevendo a letra [%c] na posicao [%d]\n", i, letraAleatoria, posicao);
    pthread_mutex_unlock(&mutexEscritoras); //destrava o mutex
    system("sleep 01");
  }

}

void* fazLeitura(void*args){

  int i = (*(int*)args);
  int posicao;

    while(1){
      posicao = rand()%(tam-1);
      printf("A thread[%d] esta fazendo a leitura na posicao [%d]\n", i, posicao);
      system("sleep 01");
    }

}


int main(){

  int qtdLeitoras, qtdEscritoras, rc;
  int i;

  pthread_t *leitoras;
  pthread_t *escritoras;

  printf("Digite a quantidade de leitoras: ");
  scanf("%d", &qtdLeitoras);

  printf("Digite a quantidade de escritoras: ");
  scanf("%d", &qtdEscritoras);

  for(i=0;i<5;i++){
    
    char randomLetter = 'A' + (random()%26);
    printf("letra=%c\n", randomLetter);
  }

  int *idEscritoras[qtdEscritoras];
  int *idLeitoras[qtdLeitoras];

  leitoras = (pthread_t *) malloc(qtdLeitoras*sizeof(pthread_t));
  escritoras = (pthread_t *) malloc(qtdEscritoras*sizeof(pthread_t));
  regiaocritica = (int*) malloc(tam*sizeof(int));


  //criando as threads leitoras
    for(i=0;i<qtdLeitoras;i++){
      idLeitoras[i] = (int *) malloc(sizeof(int));
      *idLeitoras[i] = i;
      rc = pthread_create(&leitoras[i], NULL, fazLeitura,(void *)idLeitoras[i]);
      if(rc){
        printf("Erro na criacao das leitoras!\n");
      }
    }

  //criando as threads escritoras
    for(i=0;i<qtdEscritoras;i++){
      idEscritoras[i] = (int *) malloc(sizeof(int));
      *idEscritoras[i] = i;
      rc = pthread_create(&escritoras[i], NULL, fazEscrita,(void *)idEscritoras[i]);
      if(rc){
        printf("Erro na criacao das escritoras!\n");
      }
    }

  // fazendo o join nas threads leitoras e escritoras
    for(i=0;i<qtdLeitoras;i++){
      pthread_join(leitoras[i], NULL);
    }

    for(i=0;i<qtdEscritoras;i++){
      pthread_join(escritoras[i], NULL);
    }

  pthread_exit(NULL);
  free(regiaocritica);

  return 0;
}