#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define TAM 10


int vetor[TAM];
double n;

//estrutura que contém o índice de ínicio e fim da partição
  typedef struct{ 
    int inicio, fim;
  }Par;


//função que configura o par que será passando por parâmentro
  Par createPar(int i, int f){ 
    Par P;
    P.inicio = i;
    P.fim = f;
    return P;
  }

//função que verifica se as partições estão ordenadas entre si
  int merge (int inicio, int fim){ 
    if(vetor[inicio]>= vetor[fim+1]){
      printf("Não está ordenado\n");
      exit(0);
    }
    return 1;
   
  }

//função que verifica se as partições estão ordenadas
  void* verificaOrdenacao(void*args) 
  {
    int i;
    int inicio = ((Par*)args)-> inicio;
    int fim = ((Par*)args)-> fim;
    int ordenado;
    double cont = 0;

    for(i=inicio;i<tam-1;i++){
      if(vetor[i]>=vetor[i+1]){
        printf("Não está ordenado!\n");
        exit(0);
      }
    }
   ordenado =  merge(inicio, fim);
   printf("Esta ordenado!\n");
   
  
  }

int main()
{

  int i, final = 0, rc, idThread = 0;
  double particao = 0;

  pthread_t *threads;

  vetor = (int *)malloc(tam * sizeof(int));

  for (i = 0; i < 10; i++)
  {
    scanf("%d", &vetor[i]);
  }

  scanf("%lf", &n);

  threads = (pthread_t *) malloc(n*sizeof(pthread_t));
  particao = ceil(tam/ n);

  for (i = 0; i < tam; i = i + particao)
  {
    final = i + (particao - 1);

    if (final > tam)
    {
      final = tam - 1;
    }
    Par P = createPar(i, final);
    pthread_create(&threads[idThread], NULL, verificaOrdenacao,&P);
    pthread_join(threads[idThread],NULL);
    idThread++;
  }  

  for(i=0;i<n;i++){
    pthread_join(threads[i],NULL);
  }
     
  pthread_exit(NULL);
  free(vetor);

  return 0;
}

/*
Casos testes:
1=> 1 2 3 4 5 6 7 8 9 10
    3
2=> 1 2 2 4 5 6 7 8 8 10 (ta dando errado)
    3



*/