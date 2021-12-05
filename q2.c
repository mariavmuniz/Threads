#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define tam 10


int *vetor;

typedef struct{ //estrutura que contém o índice de ínicio e fim da partição
  int inicio, fim;
}Par;

Par createPar(int i, int f){ //função que configura o par que será passando por parâmentro
  Par P;
  P.inicio = i;
  P.fim = f;
  return P;
}

void* verificaOrdenacao(void*args) //função que faz a partição do vetor de acordo com o n dado
{
  int i;
  int inicio = ((Par*)args)-> inicio;
  int fim = ((Par*)args)-> fim;

  /*
  for (i = inicio; i < fim + 1; i++)
  {
    printf("%d ", vetor[i]);
  }
  printf("\n");
  */

  for(i=inicio;i<tam-1;i++){
    if(vetor[i]<vetor[i+1]){ 
      //código
    }
    else{
      printf("não está ordenado\n");
      exit(0);
    }
  }
 
}

int main()
{

  int i, final = 0, rc, idThread = 0;
  double particao = 0, n;

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
    rc = pthread_create(&threads[idThread], NULL, verificaOrdenacao,&P);
    pthread_join(threads[idThread],NULL);
    idThread++;
  }  
     
  pthread_exit(NULL);
  free(vetor);

  return 0;
}
