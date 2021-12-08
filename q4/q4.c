#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 
//Estrutura que guarda o cabecalho do arquivo de entrada.
  typedef struct{
    char numeroMagico[2];
    int qtdLinhas, qtdColunas;
    int valorMaximo;
  } Args;
  
//Estrutura que guarda o valor de RGB de cada pixel.
  typedef struct{ 
    int R, G, B;
    int index;
  } PixelCor;
  
//Inicializando o vetor de pixels.
PixelCor *bitmap; 

//Inicializando a variável que irá conter a quantidade de threads.
int numThreads;

//Escrevendo as imagens em tons de cinza no arquivo de saída.
void escreveBitmap(){
  int i;

  FILE *arqBitmapCvt = fopen("bitmapcvt.ppm", "w");
  if(arqBitmapCvt == NULL){
    printf("Erro na criacao do arquivo!\n");
    return;
  }

  for(i = 0; i < numThreads; i++){
    fprintf(arqBitmapCvt, "%d %d %d\n",  bitmap[i].R, bitmap[i].G, bitmap[i].B);
  }

  fclose(arqBitmapCvt);
}

//Convertendo as imagens em tons de cinza.
void* converteTonsdeCinza(void*args){
  //Fazendo cast do elemento usado como argumento por cada thread.
  int i = (*(int*)args);

  //Convertendo os pixels.
  int tomCinza = ((bitmap[i].R)*0.30) + ((bitmap[i].G)*0.59) + ((bitmap[i].B)*0.11);
  bitmap[i].R=tomCinza;
  bitmap[i].G=tomCinza;
  bitmap[i].B=tomCinza;
}

//Lendo o arquivo de entrada.
void leBitmap(Args* cabecalho, FILE* arqBitmap){
  int i, j;

  //Lendo as três primeiras linhas do arquivo.
  fscanf(arqBitmap, "%s", cabecalho->numeroMagico);
  fscanf(arqBitmap, "%d %d", &cabecalho->qtdLinhas, &cabecalho->qtdColunas);
  fscanf(arqBitmap, "%d", &cabecalho->valorMaximo);
  
  //Calculando a quantidade de threads.
  numThreads = cabecalho->qtdLinhas*cabecalho->qtdColunas;

  //Alocando o vetor que guarda cada pixel.
  bitmap = (PixelCor*) malloc (numThreads*sizeof (PixelCor)) ;
  if(bitmap == NULL){
    printf("Erro na alocacao da matriz!\n");
  }
    
  //Lendo a vetor do bitmap.
  for(i = 0; i < numThreads; i++){
    fscanf(arqBitmap, "%d %d %d", &bitmap[i].R, &bitmap[i].G, &bitmap[i].B);
  }

  //Fechando o arquivo.
  fclose(arqBitmap);
}
 
int main(){
  //Abrindo o arquivo no formado de leitura.
  FILE *arqBitmap = fopen("bitmap1.ppm", "r"); 
  
  //Verificando a abertura.
  if(arqBitmap == NULL){
    printf("Erro na alocacao.\n");
    exit(0);
  }

  Args cabecalho;
  PixelCor P;
  int i, t;
  
  //Lendo as informações do arquivo.
  leBitmap(&cabecalho, arqBitmap);

  //Alocando o vetor de threads.
  pthread_t *threadsBitmap = (pthread_t*) malloc(numThreads*sizeof(pthread_t));

  //Inicializando o vetor de ids para cada thread.
  int *ids[numThreads];

  for(i = 0; i < numThreads; i++){
    //Alocando o vetor de ids.
    ids[i] = (int *) malloc(sizeof(int));
    *ids[i] = i;

    //Criando as threads.
    t = pthread_create(&threadsBitmap[i], NULL, converteTonsdeCinza,(void *)ids[i]);
    
    //Verificando se a criação de cada thread foi bem sucessida.
    if(t){
      printf("Erro na criação das threads!\n");
      exit(0);
    }
  }

  //Unindo as threads;
  for(i = 0; i < numThreads; i++){
    pthread_join(threadsBitmap[i],NULL);
  }

  //Escrevendo no arquivo o bitmap convertido.
  escreveBitmap();

  for(i = 0; i < numThreads; i++){
    printf("Linha[%d] = %d %d %d\n", i, bitmap[i].R, bitmap[i].G, bitmap[i].B);
  }

  //Desalocando a memória alocada.
  free(threadsBitmap);
  for(int i = 0; i < numThreads; ++i){
    free(ids[i]);
  }
  
  pthread_exit(NULL);
}
