//*******************************************************************************************************************************************
//Olá, meu nome é Marco e este código é um trabalho submetido a disciplina de Estrutura de Dados 1 da Universidade Federal do Espírito Santo
//O trabalho consistia em implemente em C um programa que lê um arquivo texto (no formato .txt) e imprima, em ordem alfabética, as palavras e
//a suas frequências no texto.
//As palavras e suas frequências deverão ser inseridas em uma estrutura de dados chamada Árvore Rubro Negra, cujo código deverá ser implementado
//considerando as funções básicas de inserção e caminhamento.

/*Uma árvore rubro-negra é uma árvore de busca binária onde cada nó tem um atributo de cor, vermelho ou preto. Além dos requisitos ordinários impostos pelas árvores de busca binárias, as árvores rubro-negras tem os seguintes requisitos adicionais:
  1- Um nó é vermelho ou preto.
  2- A raiz é preta. (Esta regra é usada em algumas definições. Como a raiz pode sempre ser alterada de vermelho para preto, mas não sendo válido o oposto, esta regra tem pouco efeito na análise.)Usaremos essa regra
  3- Todas as folhas(nil) são pretas.
  4- Ambos os filhos de todos os nós vermelhos são pretos.
  5- Todo caminho de um dado nó para qualquer de seus nós folhas descendentes contem o mesmo número de nós pretos.
Essas regras asseguram uma propriedade crítica das árvores rubro-negras: que o caminho mais longo da raiz a qualquer folha não seja mais do que duas vezes o caminho mais curto da raiz a qualquer outra folha naquela árvore. O resultado é que a árvore é aproximadamente balanceada.(...) FONTE: Wikipédia*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 1 //definindo as cores dos nós
#define BLACK 0

//entrada: receber o nome do arquivo txt a ser lido, com palavras aleatorias separadas por espaço e em ASCII, ou seja, sem acentuação
//processamento: por meio da árvore rubro negra, conseguir contar a frequencia de ocorrencia de todas as palavras
//saída: imprimir na tela um relatório contendo a palavra e a sua ocorrência em ordem alfabética.

typedef struct s_palavra
{
  char palavra[50];
  int frequencia;
} tipoPalavra;

typedef struct s_no
{
  tipoPalavra info;
  struct s_no *esquerda;
  struct s_no *direita;
  struct s_no *pai;
  int cor;
} NoArvoreRubroNegra;

int criandoArvoreRubroNegra(NoArvoreRubroNegra **raiz) //função que garante que o ponteiro raiz aponte para null quando a árvore for vazia
{
  *raiz = NULL;
  if (*raiz != NULL)
  {
    return -1; //error
  }
  else
  {
    return 0;
  }
}

// rotacao para a esquerda
void rotacaoEsquerda(NoArvoreRubroNegra **raiz, NoArvoreRubroNegra *x) //função que rotaciona a esquerda
{
  if (!x || !x->direita)
    return;
  //ponteiro y armazenando o filho direito de x
  NoArvoreRubroNegra *y = x->direita;

  //armazenando o ponteiro da subárvore esquerda de y como filho direito de x
  x->direita = y->esquerda;

  //atualizando o ponteiro do pai de x à direita
  if (x->direita != NULL)
    x->direita->pai = x;

  //atualizar o ponteiro do pai de y
  y->pai = x->pai;

  // verificando se o pai de x for nulo, caso seja, tornamos y a raiz da árvore
  if (x->pai == NULL)
    (*raiz) = y;

  // armazenando y no lugar de x
  else if (x == x->pai->esquerda)
    x->pai->esquerda = y;
  else
    x->pai->direita = y;

  // tornando x o filho esquerdo de y
  y->esquerda = x;

  //atualizando o ponteiro do pai de x
  x->pai = y;
}

// rotacao direita: analoga a rotação esquerda
void rotacaoDireita(NoArvoreRubroNegra **raiz, NoArvoreRubroNegra *y) //função que rotaciona a direita
{
  if (!y || !y->esquerda)
    return;
  NoArvoreRubroNegra *x = y->esquerda;
  y->esquerda = x->direita;
  if (x->direita != NULL)
    x->direita->pai = y;
  x->pai = y->pai;
  if (x->pai == NULL)
    (*raiz) = x;
  else if (y == y->pai->esquerda)
    y->pai->esquerda = x;
  else
    y->pai->direita = x;
  x->direita = y;
  y->pai = x;
}

void concertaArvore(NoArvoreRubroNegra **raiz, NoArvoreRubroNegra *novoNo) //função que verifica as propriedades da arvore após a inserção de um novo elemento
{
  while (novoNo != *raiz && novoNo != (*raiz)->esquerda && novoNo != (*raiz)->direita && novoNo->pai->cor == RED)
  { //Condição de parada: até que novoNo não seja a raiz e a cor do pai do novoNo seja vermelha
    NoArvoreRubroNegra *y;
    //procurando o tio do novoNo e guardando em y
    if (novoNo->pai && novoNo->pai->pai && novoNo->pai == novoNo->pai->pai->esquerda)
    {
      y = novoNo->pai->pai->direita;
    }
    else
    {
      y = novoNo->pai->pai->esquerda;
    }
    // Se a cor do tio for vermelha
    // (i)  Mudar a cor do pai e do tio para PRETO
    // (ii) Mudar a cor do avô para VERMELHO
    // (iii) Mover novoNo para o avô
    if (!y)
    {
      novoNo = novoNo->pai->pai;
    }
    else if (y->cor == RED)
    {
      y->cor = BLACK;
      novoNo->pai->cor = BLACK;
      novoNo->pai->pai->cor = RED;
      novoNo = novoNo->pai->pai;
    }
    //se o tio do novoNo for BLACk, temos 4 casos
    else
    {
      // Caso Esquerda-Esquerda (LL), faremos o seguinte
      // (i) Trocar a cor dos pais e avós
      // (ii) Giramos a direita do avô
      if (novoNo->pai == novoNo->pai->pai->esquerda &&
          novoNo == novoNo->pai->esquerda)
      {
        int c = novoNo->pai->cor;
        novoNo->pai->cor = novoNo->pai->pai->cor;
        novoNo->pai->pai->cor = c;
        rotacaoDireita(raiz, novoNo->pai->pai);
      }

      // Caso Esquerda-Direita (LR), faremos o seguinte
      // (i) Trocamos a cor do nó atual e do avô
      // (ii) Giramos a esquerda do pai
      // (iii) Giramos a direita do avô
      if (novoNo->pai && novoNo->pai->pai && novoNo->pai == novoNo->pai->pai->esquerda &&
          novoNo == novoNo->pai->direita)
      {
        int c = novoNo->cor;
        novoNo->cor = novoNo->pai->pai->cor;
        novoNo->pai->pai->cor = c;
        rotacaoEsquerda(raiz, novoNo->pai);
        rotacaoDireita(raiz, novoNo->pai->pai);
      }

      // Caso Direita-Direita (RR), faremos o seguinte
      // (i) Trocar a cor dos pais e avós
      // (ii) Giramos a esquerda do avô
      if (novoNo->pai && novoNo->pai->pai &&
          novoNo->pai == novoNo->pai->pai->direita &&
          novoNo == novoNo->pai->direita)
      {
        int c = novoNo->pai->cor;
        novoNo->pai->cor = novoNo->pai->pai->cor;
        novoNo->pai->pai->cor = c;
        rotacaoEsquerda(raiz, novoNo->pai->pai);
      }

      // Caso direita-esquerda (RL), faremos o seguinte
      // (i) Trocamos a cor do nó atual e do avô
      // (ii) Giramos para a direita do pai
      // (iii) Giramos a esquerda do avô
      if (novoNo->pai && novoNo->pai->pai && novoNo->pai == novoNo->pai->pai->direita &&
          novoNo == novoNo->pai->esquerda)
      {
        int c = novoNo->cor;
        novoNo->cor = novoNo->pai->pai->cor;
        novoNo->pai->pai->cor = c;
        rotacaoDireita(raiz, novoNo->pai);
        rotacaoEsquerda(raiz, novoNo->pai->pai);
      }
    }
  }
  (*raiz)->cor = BLACK; //A raiz é sempre negra
}

void caminhaEmOrdem(NoArvoreRubroNegra *RB)
{ //essa função imprime o conteudo do nó depois de percorrer os caminhos possiveis para a esquerda

  if (RB != NULL)
  { //condição de parada da recursão
    caminhaEmOrdem(RB->esquerda);
    printf("%s |%d|\n", strlwr(RB->info.palavra), RB->info.frequencia); //primeiro eu vou para a esquerda e depois eu processo os dados do nó
    caminhaEmOrdem(RB->direita);
  }
}

int inserirArvore(NoArvoreRubroNegra **raiz, tipoPalavra info) //função que insere um nó na árvore rubro negra
{
  //alocando memória para o novo nó
  NoArvoreRubroNegra *novoNo;
  novoNo = (NoArvoreRubroNegra *)malloc(sizeof(NoArvoreRubroNegra));

  if (novoNo == NULL)
  {
    return -1; //erro de alocação de memória
  }
  else
  {
    //preenchendo o novo nó com as informações
    novoNo->info = info;
    novoNo->info.frequencia = 1;

    if (*raiz == NULL)
    { //se a raiz for nula o novoNo será a raiz
      novoNo->cor = BLACK;
      novoNo->direita = NULL;
      novoNo->esquerda = NULL;
      novoNo->pai = NULL;
      (*raiz) = novoNo;
      return 0;
    }
    else
    {
      NoArvoreRubroNegra *paiNovoNo = NULL;
      NoArvoreRubroNegra *noAComparar = (*raiz);

      while (noAComparar != NULL)
      {
        paiNovoNo = noAComparar;
        if (strcmp(novoNo->info.palavra, noAComparar->info.palavra) == 0)
        {
          noAComparar->info.frequencia++;
          return 0;
        }
        else if (strcmp(novoNo->info.palavra, noAComparar->info.palavra) < 0)
        {
          noAComparar = noAComparar->esquerda;
        }
        else
        {
          noAComparar = noAComparar->direita;
        }
      }
      novoNo->pai = paiNovoNo;
      if (strcmp(novoNo->info.palavra, paiNovoNo->info.palavra) > 0)
      {
        paiNovoNo->direita = novoNo;
      }
      else
      {
        paiNovoNo->esquerda = novoNo;
      }
      novoNo->cor = RED;
      novoNo->direita = NULL;
      novoNo->esquerda = NULL;
      concertaArvore(raiz, novoNo);
      return 0;
    }
  }
}

int lerArquivo(char *nomeArquivo) /* lerArquivo: função que recebe o nome de um arquivo preexistente como parâmetro, o lê e manda inserir na arvore palavra por palavra*/
{
  NoArvoreRubroNegra *raiz;
  tipoPalavra info;
  FILE *arquivo;
  int retorno;

  //fazendo o ponteiro raiz apontar para NULL:
  retorno = criandoArvoreRubroNegra(&raiz);
  if (retorno == 0)
  {
    printf("\nArvore criada com sucesso!!!\n");
  }
  else
  {
    printf("\nArvore não pode ser criada!!!\n");
  }

  //abertura do arquivo em modo 'r':
  arquivo = fopen(nomeArquivo, "r");
  if (arquivo == NULL)
  {
    printf("Error na abertura do arquivo\n"); //error arquivo não encontrado
  }
  else
  {
    while ((fscanf(arquivo, "%s", &info.palavra) != EOF)) //condição de parada: o fscanf encontrar EOF
    {

      retorno = inserirArvore(&raiz, info);
      if (retorno == -1)
      {
        printf("\nError de alocacao de memoria!!!\n");
      }
    }
  }
  printf("\nRelatorio de palvras no arquivo informado:\n");
  caminhaEmOrdem(raiz);
  return 0;
}

int main() //função principal do programa
{
  char nomeArquivo[100]; //variavel para armazenar o nome do arquivo informado pelo usuario
  printf("\n!!!AVISO: para que o programa funcione corretamente o arquivo nao pode conter palvras com letras maiusculas, pontuacao ou acentuacao.\n\n");
  printf("Informe o arquivo a ser lido: ");
  scanf("%s", &nomeArquivo);
  lerArquivo(&nomeArquivo);

  return 0;
}