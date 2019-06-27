/***********************************************************
  Caget - Problema do caxeiro viajante resolvido via
          algoritmo genetico.
      por:
          N.M.S.. E-mail nfermat@gnu.cc

          Versao  1.6 -   ultima alteracao 12/08/2004
************************************************************
 Principal Alteração:
        1.5
   Mudança de main para o modo de ordenação
   é feita uma ordenação em ordem crescente de aptidão
   se a diferença entre os extremos for menor que o erro
   permitido será finalizado o programa. Caso contrário
   pegaremos os ultimos termos e aplicaremos mutações
   e crossovers repetindo o processo.
   1.6
   alteracao do quicksort
*****************************************02/10/2003*********/
/*

+--------------+-------+---------------------------------------
|   FUNCAO     |ESTADO | ESPECIFICACAO
+--------------+-------+--------------------------------------
|  mutacao     | OK OK | FAZ A MUTACAO EM UM VETOR
|  crossover   | OK OK | CRUZAMENTO ENTRE DOIS VETORES SOLUCAO
|  sorteio     | OK OK | FUNCAO GENERICA DE SORTEIO ENTRE VETORES NAO ALTERADOS
|  aptidao     | OK OK | DIZ A APTIDAO (DISTACIA TOTAL) DO VETOR SOLUCAO
|  inicio      | OK OK | GERA UMA POPULACAO INICIAL
|  elitismo    | OK OK | GARDA OS MELHORES INDIVIDUOS DA POPULACAO
|  main        |       | FUNCAO PRINCIPAL RESPONSAVEL PELO INICIO E TERMINO
|  melhor      | OK OK | RETORNA A MELHOR SOULUCAO DO ESPACO DE SOLUCOES
|  reset       | OK OK | DESMARCA O ESTADO DE MODIFICADO
|  fim         | OK OK | GRAVA EM ARQUIVO TEMPO DE EXECUCAO E O RESULTADO
|  quicksort1  | OK    | ORDENACAO DOS VETORES
|  separa      | OK    | SEPARRACAO DO QUICKSORT
|  puxa2       | OK OK | FAZ A COLETA DE DADOS NO ARQUIVO
|  entrada     | OK OK | FAZ A COLETA DE DADOS JUNTAMENTE COM PUXA2
+--------------+-------+-------------------------------------
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define numcid  400  //numero de cidades envolvidas
#define TESTE   500  //tempo de espera sem evolucao
#define numini  400 //tamanho da populacao de resultados obs deve ser par
#define maximo  9999   //numero maximo de geracoes
#define erroper 100   //erro permitido para o termino
#define MAXCID  500   //numero maximo de cidades

/*
    definicoes do layout arquivo de entrada

    o arquivo de entradas sera definido da seguinte maneira.
    primeira linha -> numero "K" de cidades <= MAXCID
    segunda linha  -> em branco
    proxima linha  -> numero inteiro negativo
    proxima linha  -> numero "X" da cidade
    proximas K linhas -> distancia d(0,n) , n=(0..k-1) obs nesse caso X = 0
    proxima linha  -> em branco
    proxima linha  -> numero inteiro negativo
    proximas K-1 linhas -> distancia d(X,n) , n=(1..k-1)
    proxima linha  -> numero inteiro negativo
    aplica-se um processo indutivo atÃ© k-k+1
*/

//   Definição do vetor inicial
//    Primeiro teste com 5 cidades dado no maximo 5! percursos
//   cidade A ponto inicial
//   Definir
//    para as distancias entre as cidades definimos o vetor

int distancia[MAXCID][MAXCID];

//   o vetor solucao e definido da seguinte maneira
//   numcid Ã© o numero de cidades envolvidas no problema
//************************************************************************
//************************************************************************
struct solucao{
   int   cidade[MAXCID]; // vetor solucao propriamente dito
   int   aptidao; // distancia total da solucao
   int   estado; // verifica se ja foi modificado na geraÃ§Ã£o atual
};
void teste(void)
{
   printf("depura\n");
   getchar();
   exit(1);
}
void teste1(void)
{
   printf("depurando");
   getchar();
}

void inicio     (struct solucao *, int, int);
void aptidao    (struct solucao *, int, int);
void crossover  (struct solucao *, int, int, int);
void mutacao    (struct solucao *, int, int );
int  sorteio    (struct solucao *, int );
void elitismo   (struct solucao *, int);
int  melhor     (struct solucao *, int);
void reset      (struct solucao *, int);
int  entrada    (char *, int *);
int  puxa2      (FILE *, int *);
void fim        (struct solucao *, int, int, int, unsigned long int);
void quicksort1 (int *, int *, int, int);
int  separa     (int *, int *, int, int);
int Particiona  (int *, int *, int, int);
void QuickSort  (int *, int *, int, int);
void swap_A     (int *, int *);
void swap_B     (int *, int *);

/*************************************************************
 **********************  MAIN ********************************
 *************************************************************/
int main(void){
   int sortudo,a, sortudo2=0, i,alea, num_cid;
   time_t t_inicial;
   unsigned long int geracao;
   int ordenado0[numini];// endereco dos oordenados
   int ordenado1[numini];// ordenados por aptidao
   struct solucao espaco[numini];// espaco de soulucoes propriamente dito
   t_inicial = time(NULL);
   if (!entrada("entrada.nms", &num_cid))
      return(0);
   for(i=0; i<numini; i++)
   {
      ordenado0[i]=0;
      ordenado1[i]=0;
   }

   srand((unsigned) time(NULL) / 2);
   geracao = 0;
   printf("inicio \n");
   inicio(espaco, num_cid, numini);
   do
   {
           for(i = 0; i < numini; i++)
           {
                   ordenado0[i] = i;
                   ordenado1[i] = espaco[i].aptidao;
           }
      QuickSort(ordenado0, ordenado1, 0, numini-1);
      for(i=0 ; i<numini; i++)
//         printf("i = %d   %d \n",ordenado0[i], ordenado1[i]);
      if((-ordenado1[0] + ordenado1[numini/2]) < erroper)
      {
         break;
      }// if teste final
      i=0;
      a= numini/2;
      for(i = a; i < numini-1; i++)
      {
         if((rand() % 2) == 1)
         {
            crossover(espaco, ordenado0[i] , ordenado0[i+1], num_cid);
            i++;
//            printf("cros\n ");
         }
         else
         {

            mutacao(espaco, ordenado0[i], num_cid);
//            printf("muta %d\n",i);

         }
      }
      geracao=geracao+1;

      i = melhor(espaco, numini);
    //  printf("geracao %ld - melhor %ld\n",geracao, espaco[melhor(espaco,numini)].aptidao);

      
   } while(geracao < maximo);


   printf("geracao %d \n",geracao);
   fim(espaco, time(NULL)-t_inicial, ordenado0[0], num_cid, geracao);
   return(0);
} //main
//************************************************************************
//  a funcao aptidao eh defivina do seguinte modo
void aptidao(struct solucao *soluc, int quem, int num_cid){
   int resul=0;
   int i;
   for(i=0;i<num_cid-1;i++)
      resul = resul +distancia[soluc[quem].cidade[i]][soluc[quem].cidade[i+1]];
   soluc[quem].aptidao = resul+ distancia[soluc[quem].cidade[0]][soluc[quem].cidade[num_cid-1]];
}
//////*******************************************************************
// Para formar a populacao inicial usaremos a seguinte funcao
void inicio(struct solucao *espaco, int num_cid, int num_inicial){
   int sortudo; // numero selecionado aleatoriamente entre 0 e numcid-1
   int quantos=0; // numero de cidades sorteadas
   int i,j=0,k=0;
   int teste=1;
   srand((unsigned) time(NULL)/2);
   for(i=0;i<num_inicial;i++) // variacao da solucao no espaco de solucoes
   {
      j = 0;
      quantos=0;
      while (j < num_cid)  // variac.da posic da cid no vetor solu
      {
         sortudo = rand()%num_cid;
         for (k = 0; k < quantos; k++)//ver se cid.ja fora sort.
            if (espaco[i].cidade[k] == sortudo)
            {
               teste = 0;
               }
         if (teste){
            espaco[i].cidade[j] = sortudo;
            espaco[i].estado = 0;
            quantos++;
            j++;
         }
         teste=1;
      }
                aptidao(espaco, i, num_cid);
   }
} // fim de inicio

//***********************************************************************

// definiremos a rotina de crossover do seguinte modo
// nessa rotina pegaremos dois vetores pai e mae definidos como inteiros
// e faremos um cruzamento de modo a gerar vetores diferentes

void crossover(struct solucao espaco[], int pai, int mae, int num_cid) {
   int quantos, quem[MAXCID]; // numero de posicoes de pai, vetor de teste
   int i,j,k;
   int filho1[MAXCID], filho2[MAXCID];
   int sortudo, repetido1, repetido2;  //posicao sorteada, teste de repeti
   srand((unsigned) time(NULL)/2);
   for(i=0;i<num_cid;i++) {
       quem[i] = 0;
       filho1[i] = -1;
       filho2[i] = -1;
   }
   quantos =1+ rand()%(num_cid-1);
   i = 0;
   while(i < quantos) {
      sortudo = rand()%(num_cid+1);
      if (!quem[sortudo]) {
            quem[sortudo]=1;
            i++;
      } // if
   }// while
   for(i=0;i<num_cid;i++) {
      if(quem[i]) {
         filho1[i]=espaco[pai].cidade[i];
         filho2[i]=espaco[mae].cidade[i];
      }
   }
   for(i=0;i<num_cid;i++) {
      repetido1=0;
      for(j=0; j<num_cid; j++)
         if(filho1[j] == espaco[mae].cidade[i])
            repetido1 = 1;
      if(repetido1!=1)
      {
         for(j=0; j<num_cid; j++)
            if(filho1[j] == -1){
               repetido1 = 0;
               filho1[j] = espaco[mae].cidade[i];
               j=num_cid+1;
            }  // if filho 1
      }
    }// for i
   for(i=0;i<num_cid;i++) {
     repetido2 = 0;
      for(j=0; j<num_cid; j++)
         if(filho2[j] == espaco[pai].cidade[i])
            repetido2 = 1;
      if(!repetido2)
         for(j=0; j<num_cid; j++)
            if(filho2[j] == -1){
               repetido2 = 0;
               filho2[j] = espaco[pai].cidade[i];
               j=num_cid+1;
            } // if filho 2
   }// for i
   for (i=0; i<num_cid; i++){
      espaco[pai].cidade[i] = filho1[i];
      espaco[mae].cidade[i] = filho2[i];
   }
   espaco[pai].estado = 1;
   espaco[mae].estado = 1;
   aptidao(espaco, pai, num_cid);
   aptidao(espaco, mae, num_cid);
} // fim de crossover

//***********************************************************************
// definiremos a funcao mutacao do seguinte modo
// receberemos o vetor pai e seu tamanho
// faremos um sorteio aleatorio do valor dos genes a serem mudados do cromossomo
// apos sortearemos quais os cromossomos que seram alterados

void mutacao(struct solucao *espaco, int pai, int num_cid)
{
   int sortudo, quantos, achei=0, troca=0;
   int i=0,j,k=0;
   srand((unsigned) time(NULL)/2);
   quantos = rand()%(num_cid+1);
   int quem[MAXCID];
   while (i < quantos){
      achei = 0;
      sortudo = rand()%num_cid;
      for(j=0; j<i; j++)
      {
         if(quem[j] == sortudo)
         {
            achei = 1;
         }
      }
      if(!achei){
         quem[i] = sortudo;
         i++;
      }// if
   } // while
   k=1;
   for(k=1; k < quantos ; k++)
   {
      troca = espaco[pai].cidade[quem[k-1]];
      espaco[pai].cidade[quem[k-1]] = espaco[pai].cidade[quem[k]];
      espaco[pai].cidade[quem[k]] = troca;
      k++;
   }
   espaco[pai].estado = 1;
   return;
} // fim de mutacao
//**********************************************************************
// Funcao Sorteio . Escolhe aleatoriamente uma solucao verficando seu estado
int sorteio(struct solucao espaco[], int tamanho)
{
   int sortudo;
   srand((unsigned) time(NULL)/2);
   while( 1)
   {
      sortudo = rand()%tamanho;
      if(!espaco[sortudo].estado)
         break;
   }// while
   return(sortudo);
}//sorteio

//******************************************************
void elitismo(struct solucao *espaco, int num_inicial)
{
   espaco[melhor(espaco, num_inicial)].estado = 1;
}

//*****************
int melhor(struct solucao *espaco, int num_inicial)
{
   int o=0,i;
   for(i = 0; i < num_inicial; i++)
      if(espaco[i].aptidao < espaco[o].aptidao)
         o = i;
   return(o);
}// melhor

//********************

void reset(struct solucao *espaco, int num_inicial)
{
   int i;
   for(i = 0; i < num_inicial; i++)
      espaco[i].estado = 0;
}//reset

//********************

int puxa2(FILE *arquivo, int *retorno)
{
   int contr;
   contr = fscanf(arquivo, " %d " , retorno);
   return(contr);
}// puxa2
//********************
int entrada(char *arquivo, int *num_cid)
{
   int i,j;
   int cidade=0;
   int teste=0;
   int num=0;
   int zero=0;
   int distan;
   FILE *fp;
   fp = fopen(arquivo,"r");
   if (fp == NULL)
   {
      printf("Erro ao abrir o arquivo de entrada de dados\n");
      return(0);
   }
   rewind(fp);
   printf("teste %d",num);
//   if(puxa(fp, " %d ", &num) == EOF){
   if(puxa2(fp, &num) == EOF){
      printf( "Arquivo em branco\n");
      return(0);
   }
   printf("eu %d vivo \n",num);
   if(num > MAXCID) {
      printf("Numero de cidades maior que o Maximo Permitido\n");
      return(0);
   }
   i=0;
   j=0;
   for(i = 0; i < num; i++)
   {
      if(puxa2(fp, &teste) == EOF )
         if(i < num-1){
            printf("Erro!!! Fim inesperado de arquivo #2.1#\n");
            return(0);
         }// if i
      if (puxa2(fp, &cidade) == EOF)
         if(i < num-1){
            printf("Erro!!! Fim inesperado de arquivo #2.2#\n");
            return(0);
         }// if i
      printf("teste = %d -- cidade =  %d -- i = %d\n", teste, i, cidade);

      if( zero <= teste ){
         printf("Erro na entrada de dados #1.1#\n");
         return(0);
      }//if teste
      if(cidade != i){
         printf("Erro na entrada de dados #1.2#\n");
         return(0);
      }//if teste

      for(j = i; j < num; j++){
         if(puxa2(fp, &distan) != EOF){
            distancia[i][j] = distan;
            distancia[j][i] = distan;
         }//if
         else{
            printf("Erro!!! Fim inesperado de Arquivo #3#\n");
            return(0);
         }//else
      }//for j
   }//for i

   *num_cid = num;
   return(1);
}// entrada
//****************************************************

void fim(struct solucao *espaco, int tempo, int melhor,
            int num_cid, unsigned long int  gera)
{
   FILE *fp;
   int i;
   fp = fopen("saidas.nms","a+");
   if (fp == NULL)
   {
      printf("ERRO na abertura do arquivo de resultados");
      return;
   }
   fprintf(fp,"Resultado\n");
   fprintf(fp," Tempo de execucao %ld .\n",tempo);
   fprintf(fp," Caminho: ");
   printf(" Caminho: ");
   for(i=0; i < num_cid; i++){
      fprintf(fp," %d ", espaco[melhor].cidade[i]);
      printf(" %d ", espaco[melhor].cidade[i]);
   }
   fprintf(fp, "\nDistancia percorrida: %d\n", espaco[melhor].aptidao);
   printf("\nDistancia percorrida: %d\n", espaco[melhor].aptidao);
   fprintf(fp, "Geracao: %d\n\n\n", gera);
}//fim
//********************************************
int Particiona(int B[], int A[], int p, int r)
{
   int x,teste;
   int i,j;
   x = A[p];
   i = p-1;
   j = r+1;
   while(1)
          {
      do 
      {
         j--;
         teste=A[j];
      }while(teste > x && j>0);
//      while(A[j] <= x  && j>0)
//         --j;
      do
      {
         i++;
         teste = A[i];   
      }while((teste < x) && i<r);   
                   //    >   
      if (i<j)
      {
         swap_A(&A[i], &A[j]);
         swap_B(&B[i], &B[j]);
      }
      else
              return(j);
   }

}
void swap_A(int *x, int *y)
{
   int t;
   t = *y;
   *y = *x;
   *x = t;
//   printf("ta");
}
void swap_B(int *x, int *y)
{
   int t;
   t = *y;
   *y = *x;
   *x = t;
//   printf("tb");
   
}

void QuickSort(int B[], int A[] ,int p,int r)
{
   int q;
//   teste1();
   if (p < r){
      q = Particiona(B, A, p, r);
      QuickSort(B, A, p, q);
//      printf("acabei");
      //teste1();
      QuickSort(B, A, q+1, r);
   }
}
 
//*****************************************************************************
//
//      ***************      **************      ************
//    ***************      **************      ************
//   ***              ***      ***      ***
//   ***               ***        ***      ***
//   *********            ***      ***      *********
//   *********            ***      ***      *********
//   ***               ***      ***      ***
//   ***               ***      ***      ***
//   ***************      **************      ***
//   ***************      **************      ***
// nfermat@gnu.cc
//*****************************************************************************