#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM 10

//Inicializando os mutexes de forma estática.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t db = PTHREAD_MUTEX_INITIALIZER;

int *regiaocritica;
int leitoresRegiaoCritica;

void* fazEscrita(void*args){
  //Fazendo cast do elemento usado como argumento para cada thread.
  int i = (*(int*)args);

  int posicao;
  char letraAleatoria;

  while(1){
    //Travando a região crítica.
    pthread_mutex_lock(&mutex);

    letraAleatoria = 'A' + (random() % 26);
    posicao = rand() % (TAM - 1);
    regiaocritica[posicao] = letraAleatoria;

    printf("A thread[%d] esta escrevendo a letra [%c] na posicao [%d].\n", i, letraAleatoria, posicao);
    
    //Destravando a região crítica.
    pthread_mutex_unlock(&mutex);

    //Fornecendo um delay somente para visualização.
    system("sleep 01");
  }
}

void* fazLeitura(void*args){
  //Fazendo cast do elemento usado como argumento para cada thread.
  int i = (*(int*)args);

  int posicao;

  while(1){
    pthread_mutex_lock(&mutex);
    leitoresRegiaoCritica++;

    if(leitoresRegiaoCritica == 1){
      pthread_mutex_lock(&db);
    }

    pthread_mutex_unlock(&mutex);

    posicao = rand()%(TAM - 1);
    printf("A thread[%d] esta fazendo a leitura na posicao [%d].\n", i, posicao);

    pthread_mutex_lock(&mutex);
    
    leitoresRegiaoCritica--;

    if(leitoresRegiaoCritica == 0){
      pthread_mutex_unlock(&db);
    }

    pthread_mutex_unlock(&mutex);

    //Fornecendo um delay somente para visualização.
    system("sleep 01");
  }
}

int main(){
  int qtdLeitoras, qtdEscritoras;
  int i;

  pthread_t *leitoras;
  pthread_t *escritoras;

  int **idLeitoras;
  int **idEscritoras;

  printf("Digite a quantidade de leitoras: ");
  scanf("%d", &qtdLeitoras);

  printf("Digite a quantidade de escritoras: ");
  scanf("%d", &qtdEscritoras);

  //Alocando o vetor de ids para as threads leitoras e escritoras.
  idLeitoras = (int**) malloc(qtdLeitoras * sizeof(int*));
  idEscritoras = (int**) malloc(qtdEscritoras * sizeof(int*));
  
  //Alocando o vetor de ids para as threads leitoras e escritoras.
  leitoras = (pthread_t*) malloc(qtdLeitoras * sizeof(pthread_t));
  escritoras = (pthread_t*) malloc(qtdEscritoras * sizeof(pthread_t));
  
  //Alocando o vetor de região crítica.
  regiaocritica = (int*) malloc(TAM * sizeof(int));

  //Criando as threads leitoras.
  for(i = 0; i < qtdLeitoras; i++){
    //Alocação o vetor de ids.
    idLeitoras[i] = (int*) malloc(sizeof(int));
    *idLeitoras[i] = i;

    //Criando as thereads.
    int t = pthread_create(&leitoras[i], NULL, fazLeitura,(void *)idLeitoras[i]);

    //Identificando se a criação de cada thread foi bem sucessida.
    if(t){
      printf("Erro na criacao das threads leitoras!\n");
      exit(0);
    }
  }

  //Criando as threads escritoras.
  for(i = 0; i < qtdEscritoras; i++){
    //Alocando o vetor de ids.
    idEscritoras[i] = (int *) malloc(sizeof(int));
    *idEscritoras[i] = i;

    //Criando as threads.
    int t = pthread_create(&escritoras[i], NULL, fazEscrita,(void *)idEscritoras[i]);

    //Identificando se cada thread foi bem sucessida.
    if(t){
      printf("Erro na criacao das threads escritoras!\n");
      exit(0);
    }
  }

  free(regiaocritica);
  pthread_exit(NULL);
}