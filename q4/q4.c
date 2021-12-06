#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct{
  char numeroMagico[2];
  int qtdLinhas, qtdColunas;
  int valorMaximo;
} Args;


typedef struct{
  double R, G, B;
  int index;
} PixelCor;

PixelCor *bitmap; //vetor que fica cada pixel
int numThreads;  //quantidade de threads


PixelCor createPar(double r, double g, double b, int i){ //função que configura o par que será passando por parâmentro
  PixelCor P;
  P.R = r;
  P.G = g;
  P.B = b;
  P.index = i;
  return P;
}

void* converteTonsdeCinza(void*args){ //função que converte para os tons de cinza

  double r = ((PixelCor*)args)-> R;
  double g = ((PixelCor*)args)-> G;
  double b = ((PixelCor*)args)-> B;
  int i = ((PixelCor*)args)-> index;

  double tomCinza = 0;

  tomCinza = (r*0.30) + (g*0.59) + (b*0.11);

  bitmap[i].R=tomCinza;
  bitmap[i].G=tomCinza;
  bitmap[i].B=tomCinza;

  printf("Cinza=%.0lf\n", tomCinza);

}

void leBitmap(Args* cabecalho, FILE* arqBitmap){
   
  int i, j;
  int rc;
  PixelCor P;
  pthread_t *threadsBitmap;

  // lendo as duas primeiras linhas do arquivo
    fscanf(arqBitmap, "%s", cabecalho->numeroMagico);
    fscanf(arqBitmap, "%d %d", &cabecalho->qtdLinhas, &cabecalho->qtdColunas);
    fscanf(arqBitmap, "%d", &cabecalho->valorMaximo);

  //alocando a Vetor e as Threads do bitmap
    numThreads = cabecalho->qtdLinhas*cabecalho->qtdColunas;
    bitmap = (PixelCor*) malloc (numThreads*sizeof (PixelCor)) ;
    threadsBitmap = (pthread_t*) malloc(numThreads*sizeof(pthread_t));

  //lendo a vetor do bitmap e criando as threads
    for(i=0;i<numThreads;i++){
      fscanf(arqBitmap, "%lf %lf %lf", &bitmap[i].R, &bitmap[i].G, &bitmap[i].B);
      P = createPar(bitmap[i].R, bitmap[i].G, bitmap[i].B, i);
      rc = pthread_create(&threadsBitmap[i], NULL, converteTonsdeCinza,&P);
    }
  //prints de checagem 
    printf("numThreads=%d\n", numThreads);
    printf("numeroMagico=%s\n", cabecalho->numeroMagico); 
    printf("qtdLinhas=%d\n", cabecalho->qtdLinhas); 
    printf("qtdColunas=%d\n", cabecalho->qtdColunas); 
    printf("valorMaximo=%d\n", cabecalho->valorMaximo); 


    for(i=0;i<numThreads;i++){
      printf("linha[%d] =%.0lf %.0lf %.0lf\n", i, bitmap[i].R, bitmap[i].G, bitmap[i].B );
    }

    fclose(arqBitmap);
}

int main(){

  Args cabecalho;

  FILE *arqBitmap; //criar a variável para manipular o arquivo


  arqBitmap = fopen("bitmap1.ppm", "r"); //abertura do arquivo de input

  if(arqBitmap == NULL){
    printf("Erro na alocacao\n");
  }
  else{
    leBitmap(&cabecalho, arqBitmap);
    
  }

  free(bitmap);
  pthread_exit(NULL);

  return 0;
}