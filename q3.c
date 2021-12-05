#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 3
#define M 3

typedef struct {
  int valor;
  bool visitado;
  pthread_mutex_t mutex;
} Celula;

typedef struct{
  int i;
  int j;
} Args;

Celula gf[N][M];

bool ehValido(int i, int j){
  return (0 <= i && i < N) && (0 <= j && j < M) && (gf[i][j].valor == 0);
}

void *DFS(void * pargs){
  int i = ((Args*) pargs) -> i;
  int j = ((Args*) pargs) -> j;

  pthread_mutex_lock(&gf[i][j].mutex);
  if(!(gf[i][j].visitado)){
    pthread_t thread[4];
    
    //Norte, sul, oeste e leste.
    int dx[] = {0, 0, -1, 1};
    int dy[] = {1, -1, 0, 0};

    gf[i][j].visitado = true;

    for(int k = 0; k < 4; ++k){
      int x = i + dx[k];
      int y = j + dy[k];
      if(ehValido(x, y) && !(gf[x][y].visitado)){
        Args args;
        args.i = x;
        args.j = y;
        pthread_create(&thread[k], NULL, DFS, (void*) &args);
      }
    }

    for(int k = 0; k < 4; ++k){
        pthread_join(thread[k], NULL);
    }
  }
  pthread_mutex_unlock(&gf[i][j].mutex);
  pthread_exit(NULL);
}

int main(){
  for(int i = 0; i < N; ++i){
    for(int j = 0; j < M; ++j){
      scanf("%d", &gf[i][j].valor);
      gf[i][j].visitado = false;
      pthread_mutex_init(&gf[i][j].mutex, NULL);
    }
  }

  Args start;
  start.i = 0;
  start.j = 0;

  Args end;
  end.i = 2;
  end.j = 1;

  pthread_t thread;
  pthread_create(&thread, NULL, DFS, (void*) &start);
  pthread_join(thread, NULL);

  if(gf[end.i][end.j].visitado){
    printf("A saida para a posição [%d][%d] foi encontrada!\n", end.i, end.j);
  } else {
    printf("A saida para a posição [%d][%d] não foi encontrada!\n", end.i, end.j);
  }

  pthread_exit(NULL);
}
