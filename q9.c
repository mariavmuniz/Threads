#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include<math.h>

typedef struct {
  bool position;
  pthread_mutex_t mutex;
}Vetor;

Vetor *vetor;
int t, n;
pthread_t *threads;

void crivo(int x){
  pthread_mutex_lock(&vetor[x].mutex);
  if(vetor[x].position){
    for(int j = 2 * x; j < n; j += x){
      pthread_mutex_lock(&vetor[j].mutex);
      vetor[j].position = false;
      pthread_mutex_unlock(&vetor[j].mutex);
    }
  }
  pthread_mutex_unlock(&vetor[x].mutex);

  int sqrt_n = sqrt(n) + 1;
  for(int i = x + t; i < sqrt_n; ++i){
    pthread_mutex_lock(&vetor[i].mutex);
    bool aux = vetor[i].position;
    int valor = i;
    pthread_mutex_unlock(&vetor[i].mutex); 
    if(aux){
      crivo(valor);
    }
  }
}

void *fthread(void* px){
  int x = *((int*)px);
  crivo(x);
  pthread_exit(NULL);
}

int main(){
  scanf(" Numero T: %d", &t);
  scanf(" Numero N: %d", &n);

  vetor = (Vetor*) malloc(n * sizeof(Vetor));
  for(int i = 2; i < n; ++i){
    vetor[i].position = true;
  }

  threads = (pthread_t*) malloc(t * sizeof(pthread_t));
  for(int i = 0; i < t; ++i){
    pthread_create(&threads[i], NULL, fthread, (void*) &i);
  }
  
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

  free(vetor);
  free(threads);
  pthread_exit(NULL);
} 