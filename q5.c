#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//Definindo as dimensões do mapa de bits.
#define N 3
#define M 4

//Criando a estrutura que representa a região compartilhada.
typedef struct{
  int representativo;
  pthread_mutex_t mutex;
} Representativo;

//Criando a estrutura que serve de argumento para a função que representa cada thread.
typedef struct{
  int inicio;
  int tam;
} Args;

//Inicializando a matriz correspondente ao bitmap.
int bitmap[N][M];

//Inicializando o vetor de representativos.
Representativo rps[N * M];

//Analisando se a posição atual está dentro dos limites do mapa e é terra.
bool ehValido(int i, int j){
  return (0 <= i && i < N) && (0 <= j && j < M) && (bitmap[i][j]);
}

//Encontrando o "representativo máximo" de cada posição.
int raiz(int x) {
  return (x == rps[x].representativo ? x : (rps[x].representativo = raiz(rps[x].representativo)));
}

void *uniao(void *pargs){
  //Fazendo cast de cada elemento da estrutura de argumentos.
  int inicio = ((Args*) pargs) -> inicio;
  int tam = ((Args*) pargs) -> tam;
  
  //Criando os pares de orientação no mapa (leste, oeste, norte, sul, sudeste, sudoeste, nordeste, noroeste).
  int dx[] = {0, 0, -1, 1, 1, 1, -1, -1};
  int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};

  for(int i = inicio; i < inicio + tam; ++ i){
    for (int j = 0; j < M; ++j){
      //Travando a região crítica, ou seja, a posição atual do vetor de representativos.
      pthread_mutex_lock(&rps[i * M + j].mutex);
      //Conferindo se a posição atual é terra.
      if(bitmap[i][j]){
        //Analisando todas as posições adjacentes.
        for(int k = 0; k < 8; ++k){
          //Criando as combinações possíveis de orientações para análise de adjacência.
          int x = i + dx[k];
          int y = j + dy[k];
          //Verificando se a posição atual é válida.
          if(ehValido(x, y)){
            int rz1 = raiz(x + M + y);
            int rz2 = raiz(i * M + j);
            if(rz1 != rz2){
              //Unindo os subsets que representam terra.
              rps[rz2].representativo = rz1;
            }
          }
        }
      }
      //Destravando a região crítica.
      pthread_mutex_unlock(&rps[i * M + j].mutex);
    }
  }
}

int main(){
  //Fazendo a leitura do mapa de bits.
  for(int i = 0; i < N; ++i){
    for(int j = 0; j < M; ++j){
      scanf("%d", &bitmap[i][j]);
      //Inicializando cada elemento como seu próprio representativo.
      rps[i * M + j].representativo = i * M + j;
    }
  }

  //Inicializando o vetor de threads.
  int n; 
  scanf("%d", &n);
  pthread_t *threads = (pthread_t*) malloc(n * sizeof(pthread_t));

  //Calculando o tamanho que cada particão terá considerando que a matriz pode não ser quadrada.
  int tam1 = N / n;
  int tam2 = N - ((N / n) *(n - 1));

  //Inicializando a estrutura de argumentos utilizada por cada thread.
  Args args;
  args.inicio = 0;
  args.tam = tam1;

  //Criando as primeiras n - 1 threads que irão analisar N / n linhas.
  int trd;
  for(trd = 0; trd < n - 1; ++trd){
    pthread_create(&threads[trd], NULL, uniao, (void *) &args);
    args.inicio += tam1;
  }
  //Atualizando a estrutura de argumentos para a última thread.
  args.tam = tam2;

  //Criando a última theread que irá analisar  N - ((N / n) *(n - 1)) linhas.
  pthread_create(&threads[trd], NULL, uniao, (void*) &args);

  //Unindo as threads.
  for(int i = 0; i < n; ++i){
    pthread_join(threads[i], NULL);
  }

  //Desalocando a memória.
  free(threads);

  //Contabilizando a quantidade de ilhas.
  int qtd = 0;
  for(int i = 0; i < N; ++i){
    for (int j = 0; j < M; ++j){
      //Verificando quantas posições se mantiveram como seus próprios representativos.
      if((raiz(i * M + j) == i * M + j) && bitmap[i][j]){
        printf("Representativo:%d,%d\n", i, j);
        ++qtd;
      }
    }
  }
  
  printf("A quantidade de ilhas existente no bitmap eh: %d.\n", qtd);
  pthread_exit(NULL);
}