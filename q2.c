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

void merge (int inicio, int fim){ //função que verifica se as partições estão ordenadas

  if(vetor[inicio]< vetor[fim+1]){
  }
  else{
    printf("não está ordenado\n");
    exit(0);
  }
  printf("esta ordenado\n");
}

void* verificaOrdenacao(void*args) //função que verifica se as partições estão ordenadas
{
  int i;
  int inicio = ((Par*)args)-> inicio;
  int fim = ((Par*)args)-> fim;

  for(i=inicio;i<tam-1;i++){
    if(vetor[i]<vetor[i+1]){ 
    
    }
    else{
      printf("não está ordenado\n");
      exit(0);
    }
  }
  merge(inicio, fim);
 
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
