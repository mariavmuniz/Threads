#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Definindo as dimensões do labirinto.
#define N 3
#define M 3

//Criando a estrutura que representa a região compartilhada.
typedef struct {
  int valor;
  bool visitado;
  pthread_mutex_t mutex;
} Celula;

//Criando a estrutura que serve de argumento para a função que representa cada thread.
typedef struct{
  int i;
  int j;
} Args;

//Inicializando o labirinto como a região crítica.
Celula gf[N][M];

//Analisando se a posição atual está dentro dos limites do mapa e não é parede.
bool ehValido(int i, int j){
  return (0 <= i && i < N) && (0 <= j && j < M) && (gf[i][j].valor == 0);
}

void *DFS(void * pargs){
  //Fazendo cast de cada elemento da estrutura de argumentos.
  int i = ((Args*) pargs) -> i;
  int j = ((Args*) pargs) -> j;

  //Travando a regição crítica, ou seja, a posição atual no labirinto.
  pthread_mutex_lock(&gf[i][j].mutex);
  if(!(gf[i][j].visitado)){
    //Inicializando as quatro threads que serão responsáveis pela análise das regiões adjacentes à posição atual.
    pthread_t thread[4];
    
    //Criando os pares de orientação no mapa (leste, oeste, norte, sul).
    int dx[] = {0, 0, -1, 1};
    int dy[] = {1, -1, 0, 0};
    
    gf[i][j].visitado = true;

    //Analisando todas as posições adjacentes.
    for(int k = 0; k < 4; ++k){
      //Criando as combinações possíveis de orientações para análise de adjacência.
      int x = i + dx[k];
      int y = j + dy[k];
      //Verificando se a posição atual é válida e não foi visitada.
      if(ehValido(x, y) && !(gf[x][y].visitado)){
        Args args;
        args.i = x;
        args.j = y;
        //Criando recursivamente as threads associadas ao DFS.
        pthread_create(&thread[k], NULL, DFS, (void*) &args);
      }
    }

    //Unindo as threads.
    for(int k = 0; k < 4; ++k){
        pthread_join(thread[k], NULL);
    }
  }

  //Destravando a região crítica.
  pthread_mutex_unlock(&gf[i][j].mutex);
  pthread_exit(NULL);
}

int main(){
  //Fazendo a leitura do labirinto.
  for(int i = 0; i < N; ++i){
    for(int j = 0; j < M; ++j){
      scanf("%d", &gf[i][j].valor);
      //Setando todas as posições como não visitadas.
      gf[i][j].visitado = false;
      //Inicializando o mutex de cada posição dinamicamente.
      pthread_mutex_init(&gf[i][j].mutex, NULL);
    }
  }

  //Inicializando a estrutura de argumentos utilizada por cada thread.
  Args start;
  start.i = 0;
  start.j = 0;

  //Inicializando a estrutura que representa o destino desejado.
  Args end;
  end.i = 2;
  end.j = 1;

  //Criando a primeira thread que irá começar a chamar recursivamente todas as outras.
  pthread_t thread;
  pthread_create(&thread, NULL, DFS, (void*) &start);

  //Unindo as threads,
  pthread_join(thread, NULL);

  if(gf[end.i][end.j].visitado){
    printf("A saida para a posição [%d][%d] foi encontrada!\n", end.i, end.j);
  } else {
    printf("A saida para a posição [%d][%d] não foi encontrada!\n", end.i, end.j);
  }

  pthread_exit(NULL);
}