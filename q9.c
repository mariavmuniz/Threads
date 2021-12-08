#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include<math.h>

//Criando a estrutura que irá representar a região crítica.
typedef struct {
  bool position;
  pthread_mutex_t mutex;
}Vetor;

//Inicializando as variáveis necessárias.
Vetor *vetor;
int t, n;
pthread_t *threads;

void crivo(int x){
  //Travando a região crítica da posição atual.
  pthread_mutex_lock(&vetor[x].mutex);
  if(vetor[x].position){
    //Marcando como falso todos os últimos do valor atual.
    for(int j = 2 * x; j < n; j += x){
      //Travando a região crítica do múltiplo atual.
      pthread_mutex_lock(&vetor[j].mutex);
      vetor[j].position = false;
      //Destravando a região crítica do múltiplo atual.
      pthread_mutex_unlock(&vetor[j].mutex);
    }
  }
  //Destravando a região crítica da posição atual.
  pthread_mutex_unlock(&vetor[x].mutex);

  //Encontrando qual o próximo valor que não está marcado.
  int sqrt_n = sqrt(n) + 1;
  for(int i = x + t; i < sqrt_n; ++i){
    //Travando a região crítica da posição atual.
    pthread_mutex_lock(&vetor[i].mutex);
    bool aux = vetor[i].position;
    int valor = i;
    //Destravando a região crítica da posição atual.
    pthread_mutex_unlock(&vetor[i].mutex); 
    //Analisando se a posição é válida.
    if(aux){
      //Chamando recursivamente a função para a posição válida atual.
      crivo(valor);
    }
  }
}

void *fthread(void* px){
  int x = *((int*)px);
  //Chamando a função que analisa o vetor.
  crivo(x);
  pthread_exit(NULL);
}

int main(){
  scanf(" Numero T: %d", &t);
  scanf(" Numero N: %d", &n);

  //Alocando o vetor.
  vetor = (Vetor*) malloc(n * sizeof(Vetor));
  for(int i = 2; i < n; ++i){
    vetor[i].position = true;
  }

  //Alocando e criando a quantidade t de threads para t elementos do vetor.
  threads = (pthread_t*) malloc(t * sizeof(pthread_t));
  for(int i = 0; i < t; ++i){
    pthread_create(&threads[i], NULL, fthread, (void*) &i);
  }
  
  //Unindo as threads.
  for(int i = 0; i < t; ++i){
    pthread_join(threads[i], NULL);
  }

  puts("Os números primos no vetor são:");
  for(int i = 2; i < n; ++i){
    if(vetor[i].position){
      printf("%d ", i);
    }
  }
  puts("");

  //Desalocando a memória.
  free(vetor);
  free(threads);

  pthread_exit(NULL);
} 