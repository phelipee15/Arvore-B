#ifndef ARVORE_B
#define ARVORE_B

// Ordem da Árvore
#define ORDEM 10


/*
    Estrutura da Árvore e da Apliação
*/

typedef struct conta {
    long CPF;
    char nome[50];
    long saldo;
} Conta;

typedef struct pagina {
    long tamanho;
    struct conta *itens[ORDEM-1];
    struct pagina *filhos[ORDEM];
} Arvb;

Arvb *raiz;

/*
    Funções
*/


// Funções para Criar e Inicializar
Arvb *criarArv();
Conta *criarConta();

// Funções Auxiliares
long vaziaOrdem(Arvb *arv); // Retorna se a Página existe (true || false)
long vazia(Arvb *arv); // Retorna se a Página está vazia (true || false)
long cheia(Arvb *arv); // Retorna se a Página está cheia (true || false)
long tamanho(Arvb *arv); // Retorna o tamanho da Página

// Funções de Inserção
void dividir(Arvb *origem, Arvb *destino); // Recebe a página e divide os elementos na metade com outra página
void swap(Conta **a, Conta **b); // Trocar o valor de A por B e vise-versa
void inserirOrd(Arvb *arv, long valor, char *nome, long saldo); // Insere os itens de forma ordenada
long buscabinariaFilho(Arvb *arv, Arvb *filho, long esq, long dir); // Buscar a posição do filho no pai
long buscaProximo(Arvb *arv, long item , long esq, long dir); // Retorna a busca da posição de onde se encontra o item
long numeroFilho(Arvb *pai, Arvb *filho); // Retorna a posição do filho em relação ao pai
void inserirFilho(Arvb *pai, Arvb *filho, long lugar); // Insere um novo filho
void split(Arvb *arv, Arvb *pai); // Separa os itens da Árvore(Página) e reoganiza
void inserir(Arvb *arv, Arvb *pai, long valor, char *nome, long saldo); // Inserir um item na Árvore

// Funções de Pesquisa
long buscarItemPag(Arvb *arv, long valor, long inicio, long fim); // Retorna se o item está presente na Página
long buscarProxFilho(Arvb *arv, long item, long inicio, long fim); // Retorna a busca da posição de onde se encontra o filho para procurar o item
long possui(Arvb *arv,long valor); // Retorna se existe o item na Página
void pesquisa(Arvb *arv,long valor); // Retorna a Árvore(Página) onde se encontra o item

// Funções de Remoção
void adicionarPai(Arvb *pai, Arvb *filho, long local); // Remove os itens da Página pai e insere na Página filha
long suporta(Arvb *a, Arvb *b); // Retorna se suporta adicionar uma Página B na Página A e um item da Página pai
long metade(Arvb *arv); // Retorna se está abaixo da capacidade mínima
void unir(Arvb *a, Arvb *b); // Une a Página A com a B e remove a B
void underSplit(Arvb *arv, Arvb *pai); // Faz o inverso do split
void substituir(Arvb *arv, Conta *conta, Arvb *pai); // Caso precise apagar um item de uma Página que possua filhos, vai substituir o item com número mais próximo(sucessor)
long removerItem(Arvb *arv, long valor); // Remove o item da Página
void remover(Arvb *arv, Arvb *pai, long valor); // Remover Página

// Funções de Imprimir
void visite(Arvb *arv, long altura); // Visita a Página e imprime ela
void printArv(Arvb *arv,long altura); // Imprime a Árvore toda

// Funções de Backup
void abrirArquivo(char *nome, char *modo); // Abre um arquivo em um modo ou para salvar ou carregar a Árvore
void salvarArvore(Arvb *arv, FILE *file); // Função para salvar a Árvore B em um arquivo
void carregarArvore(Arvb *arv, FILE *file); // Função para carregar a Árvore B de um arquivo

// Função de Apagar
void apagar(Arvb *arv); // Apagar toda a Árvore

// Função Menu
void menu(char *endereco);

#endif