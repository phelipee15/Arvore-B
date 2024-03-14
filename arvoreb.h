#ifndef ARVORE_B
#define ARVORE_B

// Ordem da Árvore
#define ORDEM 9


/*
    Estrutura da Árvore
*/


typedef struct pagina {
    int tamanho;
    long itens[ORDEM-1];
    struct pagina *filhos[ORDEM];
    int flag;
} Arvb;

Arvb *raiz;


/*
    Funções
*/


// Função para Criar e Inicializar
Arvb *criarArv();

// Funções Auxiliares
int vaziaOrdem(Arvb *arv); // Retorna se a Página existe (true || false)
int vazia(Arvb *arv); // Retorna se a Página está vazia (true || false)
int cheia(Arvb *arv); // Retorna se a Página está cheia (true || false)
int tamanho(Arvb *arv); // Retorna o tamanho da Página

// Funções de Inserção
void dividir(Arvb *origem, Arvb *destino); // Recebe a página e divide os elementos na metade com outra página
void swap(long *a,long *b); // Trocar o item de A por B e vise-versa
void inserirOrd(Arvb *arv, long item); // Insere os itens de forma ordenada
int buscabinariaFilho(Arvb *arv, Arvb *filho, int esq, int dir); // Buscar a posição do filho no pai
int buscaProximo(Arvb *arv, long item, int esq, int dir); // Retorna a busca da posição de onde se encontra o item
int numeroFilho(Arvb *pai, Arvb *filho); // Retorna a posição do filho em relação ao pai
void inserirFilho(Arvb *pai, Arvb *filho, int lugar); // Insere um novo filho
void split(Arvb *arv, Arvb *pai); // Separa os itens da Árvore(Página) e reoganiza
void inserir(Arvb *arv,Arvb *pai, int long); // Inserir um item na Árvore

// Funções de Pesquisa
int buscarItemPag(Arvb *arv, long item, int inicio, int fim); // Retorna se o item está presente na Página
int buscarProxFilho(Arvb *arv, long item, int inicio, int fim); // Retorna a busca da posição de onde se encontra o filho para procurar o item
int possui(Arvb *arv, long item); // Retorna se existe o item na Página
void pesquisa(Arvb *arv, long item); // Retorna a Árvore(Página) onde se encontra o item

// Funções de Remoção
void adicionarPai(Arvb *pai, Arvb *filho, int local); // Remove os itens da Página pai e insere na Página filha
int suporta(Arvb *a, Arvb *b); // Retorna se suporta adicionar uma Página B na Página A e um item da Página pai
int metade(Arvb *arv); // Retorna se está abaixo da capacidade mínima
void unir(Arvb *a,Arvb *b); // Une a Página A com a B e remove a B
void underSplit(Arvb *arv, Arvb *pai); // Faz o inverso do split
void substituir(Arvb *arv, long *item, Arvb *pai); // Caso precise apagar um item de uma Página que possua filhos, vai substituir o item com número mais próximo(sucessor)
int removerItem(Arvb *arv, long item); // Remove o item da Página
void remover(Arvb *arv, Arvb *pai, long item); // Remover Página

// Funções de Imprimir
void visite(Arvb *arv, int altura); // Visita a Página e imprime ela
void printArv(Arvb *arv, int altura); // Imprime a Árvore toda

// Funções de Backup
void abrirArquivo(char *nome, char *modo); // Abre um arquivo em um modo ou para salvar ou carregar a Árvore
void salvarArvore(Arvb *arv, FILE *file); // Função para salvar a Árvore B em um arquivo
void carregarArvore(Arvb *arv, FILE *file); // Função para carregar a Árvore B de um arquivo

// Função de Apagar
void apagar(Arvb *arv); // Apagar toda a Árvore

// Função Menu e outros
long randomLong(long max); // Gerar um número tipo long
void menu(char *endereco); // Menu

#endif