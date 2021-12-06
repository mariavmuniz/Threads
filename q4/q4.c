#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct{
  char numeroMagico[2];
  int qtdLinhas, qtdColunas;
  int valorMaximo;
} Args;


typedef struct{
  int R, G, B;
}pixelCor;

pixelCor *bitmap; //matriz que fica cada pixel

void leBitmap(Args* cabecalho, FILE* arqBitmap){
   
  int i, j, limite;

  // lendo as duas primeiras linhas do arquivo
    fscanf(arqBitmap, "%s", cabecalho->numeroMagico);
    fscanf(arqBitmap, "%d %d", &cabecalho->qtdLinhas, &cabecalho->qtdColunas);
    fscanf(arqBitmap, "%d", &cabecalho->valorMaximo);

  //alocando a matriz do bitmap
    limite = cabecalho->qtdLinhas*cabecalho->qtdColunas;
    bitmap = malloc (limite*sizeof (int*)) ;

    
    printf("%d\n", limite);
    
  //lendo a matriz de bitmap
    for(i=0;i<limite;i++){
      fscanf(arqBitmap, "%d %d %d", &bitmap[i].R, &bitmap[i].G, &bitmap[i].B);
    }
  //prints de checagem 
    printf("numeroMagico=%s\n", cabecalho->numeroMagico); 
    printf("qtdLinhas=%d\n", cabecalho->qtdLinhas); 
    printf("qtdColunas=%d\n", cabecalho->qtdColunas); 
    printf("valorMaximo=%d\n", cabecalho->valorMaximo); 

    for(i=0;i<limite;i++){
      printf("linha[%d] =%d %d %d\n", i, bitmap[i].R, bitmap[i].G, bitmap[i].B );
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



  return 0;
}