#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 
//estrutura que guarda o cabecalho do arquivo de entrada
  typedef struct{
    char numeroMagico[2];
    int qtdLinhas, qtdColunas;
    int valorMaximo;
  } Args;
  
//estrutura que guarda o valor de RGB de cada pixel
  typedef struct{ 
    int R, G, B;
    int index;
  } PixelCor;
  
//vetor que guarda cada pixel 
  PixelCor *bitmap; 

int numThreads;


//Responsável por escrever a imagens em tons de cinza no arquivo de saída
  void escreveBitmap(){

    int i;

    FILE *arqBitmapCvt;

    arqBitmapCvt = fopen("bitmapcvt.ppm", "w"); //abertura do arquivo de input
      if(arqBitmapCvt == NULL){
        printf("Erro na criacao do arquivo!\n");
      }
      else{
        for(i=0;i<numThreads;i++){
        fprintf(arqBitmapCvt, "%d %d %d\n",  bitmap[i].R, bitmap[i].G, bitmap[i].B);
        }
      }

    fclose(arqBitmapCvt);

  }


//Responsável por converter a imagens em tons de cinza
  void* converteTonsdeCinza(void*args){ //função que converte para os tons de cinza
  
    int i = (*(int*)args);
  
    int tomCinza = 0;
  
    tomCinza = ((bitmap[i].R)*0.30) + ((bitmap[i].G)*0.59) + ((bitmap[i].B)*0.11);
  
    bitmap[i].R=tomCinza;
    bitmap[i].G=tomCinza;
    bitmap[i].B=tomCinza;
  
  }

//Responsável por ler o arquivo de entrada 
  void leBitmap(Args* cabecalho, FILE* arqBitmap){
    
    int i, j;

    // lendo as três primeiras linhas do arquivo
      fscanf(arqBitmap, "%s", cabecalho->numeroMagico);
      fscanf(arqBitmap, "%d %d", &cabecalho->qtdLinhas, &cabecalho->qtdColunas);
      fscanf(arqBitmap, "%d", &cabecalho->valorMaximo);
      
    numThreads = cabecalho->qtdLinhas*cabecalho->qtdColunas;

    //alocando o vetor que guarda cada pixel 
      bitmap = (PixelCor*) malloc (numThreads*sizeof (PixelCor)) ;
      if(bitmap == NULL){
        printf("Erro na alocacao da matriz!\n");
      }
      
    //lendo a vetor do bitmap 
      for(i=0;i<numThreads;i++){
        fscanf(arqBitmap, "%d %d %d", &bitmap[i].R, &bitmap[i].G, &bitmap[i].B);
      }

    fclose(arqBitmap);
  }
 
int main(){
 
  Args cabecalho;
 
  FILE *arqBitmap; 

  int i, rc;
  
  pthread_t *threadsBitmap;

  PixelCor P;
 

  arqBitmap = fopen("bitmap1.ppm", "r"); 
  if(arqBitmap == NULL){
    printf("Erro na alocacao\n");
  }
  else{
    leBitmap(&cabecalho, arqBitmap);
    int *ids[numThreads];
    threadsBitmap = (pthread_t*) malloc(numThreads*sizeof(pthread_t));
    for(i=0;i<numThreads;i++){ //criando threads
      ids[i] = (int *) malloc(sizeof(int));
      *ids[i] = i;
      rc = pthread_create(&threadsBitmap[i], NULL, converteTonsdeCinza,(void *)ids[i]);
      if(rc){
        printf("Erro na criação das threads!\n");
      }
    }

    for(i=0;i<numThreads;i++){
      pthread_join(threadsBitmap[i],NULL);
    }

    escreveBitmap();
 
    for(i=0;i<numThreads;i++){
      printf("linha[%d] =%d %d %d\n", i, bitmap[i].R, bitmap[i].G, bitmap[i].B );
    }
    
  }

  free(bitmap);

  pthread_exit(NULL);

}
