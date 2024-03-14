#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>

#include "arvoreb.h"

#define METADE ORDEM/2

#pragma region Inicialização e Informações

Arvb *criarArv() { // Inicializa a Árvore e retorna ela
    Arvb *aux;
    aux = (Arvb*)malloc(sizeof(Arvb));
    aux->tamanho = 0;
    for (int i = 0; i < ORDEM; i++) {
        aux->filhos[i] = NULL;
    }
    return aux;
}

int vaziaOrdem(Arvb *arv) { // Retorna se a Página existe (true || false)
    return arv == NULL;
}

int vazia(Arvb *arv) { // Retorna se a Página está vazia (true || false)
    return arv->tamanho == 0;
}

int cheia(Arvb *arv) { // Retorna se a Página está cheia (true || false)
    return arv->tamanho >= ORDEM-1;
}

int tamanho(Arvb *arv) { // Retorna o tamanho da Página
    return arv->tamanho;
}

#pragma endregion

#pragma region Funções de Inserção

void dividir(Arvb *origem, Arvb *destino) { // Recebe a página e divide os elementos na metade com outra página
    long i = METADE+1;
    for(; i < ORDEM-1; i++){
        destino->filhos[(destino->tamanho)] = origem->filhos[i];
        origem->filhos[i] = NULL;
        destino->itens[(destino->tamanho)++] = origem->itens[i];
    }
    destino->filhos[(destino->tamanho)] = origem->filhos[i];
    origem->tamanho = ORDEM - METADE - 1;
}

void swap(long *a,long *b) { // Trocar o valor de A por B e vise-versa
    long c = *a;
    *a = *b;
    *b = c;
}
 
void inserirOrd(Arvb *arv, long item) { // Insere os itens de forma ordenada
    long status = 0; // Informa se ja foi feita a troca
    long guardado = 0; // Guarda o número
    if (vazia(arv)) {
        arv->itens[arv->tamanho] = item;
    } else {
        for (long i = 0; i <= arv->tamanho; i++) {
            if (status == 0) {
                if (item==arv->itens[i]) {
                    arv->tamanho--;
                    break;
                }
                if (item < arv->itens[i]) {
                    guardado = arv->itens[i];
                    arv->itens[i] = item;
                    status = 1;
                } else if (i == arv->tamanho) {
                    arv->itens[i] = item;
                }
            }else{
                swap(&guardado, &(arv->itens[i]));
            }
        }
    }
    arv->tamanho++;
}

int buscabinariaFilho(Arvb *arv, Arvb *filho, int inicio, int fim) { // Buscar a posição do filho no pai
    if (inicio == fim) {
        if(arv->filhos[inicio]->itens[0] == filho->itens[0])
            return inicio;
        else
            return -1;
    } else {
        int meio = (inicio + fim)/2;
        if (arv->filhos[meio]->itens[0] < filho->itens[0]){
            return buscabinariaFilho(arv, filho, meio+1, fim);
        } else {
            return buscabinariaFilho(arv, filho, inicio, meio);
        }
    }
}

int buscaProximo(Arvb *arv, long item, int inicio, int fim) { // Retorna a busca da posição de onde se encontra o item
    if (inicio == fim) {
        if(arv->itens[inicio] > item)
            return inicio;
        else
            return inicio+1;
    } else {
        int meio = (inicio + fim)/2;
        if(arv->itens[meio] > item)
            return buscaProximo(arv, item, inicio, meio);
        else
            return buscaProximo(arv, item, meio+1, fim);
    }
}

int numeroFilho(Arvb *pai, Arvb *filho) { // Retorna a posição do filho em relação ao pai
    return buscabinariaFilho(pai, filho, 0, pai->tamanho);
}

void inserirFilho(Arvb *pai, Arvb *filho, int lugar) { // Insere um novo filho
    Arvb *aux, *aux1 = filho;
    for (int i = lugar+1; i <= pai->tamanho; i++){
        aux = pai->filhos[i];
        pai->filhos[i] = aux1;
        aux1 = aux;
    }
}

void split(Arvb *arv, Arvb *pai) { // Separa os itens da Árvore(Página) e reoganiza
    if (arv == raiz) {
        Arvb *encima = criarArv(), *lado = criarArv();
        inserirOrd(encima, arv->itens[(METADE)]);
        dividir(arv, lado);
        raiz = encima;
        raiz->filhos[0] = arv;
        raiz->filhos[1] = lado;
    } else {
        Arvb *lado = criarArv();
        inserirOrd(pai, arv->itens[(METADE)]);
        dividir(arv, lado);
        inserirFilho(pai, lado, numeroFilho(pai, arv));
    }
}

void inserir(Arvb *arv, Arvb *pai, long item) { // Inserir um item na Árvore
    if (vaziaOrdem(arv)) {
        printf("Nao Inicializada\n");
    } else {
        if (vaziaOrdem(arv->filhos[0]) == 1) { // Caso não possui filhos, indica que a Página é uma folha, então inserimos
            inserirOrd(arv, item);
        } else { // Caso possua, procuramos aonde inserir
            int i = buscaProximo(arv, item, 0, arv->tamanho-1);
            inserir(arv->filhos[i], arv, item);
        }
        if (cheia(arv)) { // Se a Página está cheia, efetuamos o split
            split(arv, pai);
        }
   }
}

#pragma endregion

#pragma region Funções de Pesquisa

int buscarItemPag(Arvb *arv, long item, int inicio, int fim) { // Retorna se o item está presente na Página
    if (fim >= inicio) {
        int meio = (inicio + fim)/2;
        if (arv->itens[meio] == item)
            return meio;
        else if (arv->itens[meio] > item)
            return buscarItemPag(arv, item, inicio, meio-1);
        else
            return buscarItemPag(arv, item, meio+1, fim);
    }
    return -1;
}

int possui(Arvb *arv, long item) {
    return buscarItemPag(arv, item, 0, arv->tamanho-1)!=-1?1:0;
}

int buscarProxFilho(Arvb *arv, long item, int inicio, int fim) { // Retorna a busca da posição de onde se encontra o filho para procurar o item
    if (fim == inicio) {
        if(arv->itens[inicio] > item)
            return inicio;
        else
            return inicio+1;
    } else {
        int meio = (inicio + fim)/2;
        if(arv->itens[meio] > item)
            return buscarProxFilho(arv, item, inicio, meio);
        else
            return buscarProxFilho(arv, item, meio+1, fim);
    }
}

void pesquisa(Arvb *arv, long item) { // Retorna a Árvore(Página) onde se encontra o item
    while (arv) {
        if (vaziaOrdem(arv)) {
            printf("Arvore nao iniciada\n");
            break;
        } else if (vazia(arv)) {
            printf("Arvore sem elementos\n");
            break;
        } else if (possui(arv, item)) {
            printf("possui na lista\n");
            break;
        } else if (vaziaOrdem(arv->filhos[0])) {
            printf("Nao possui filhos\n");
            break;
        } else {
            int i = buscarProxFilho(arv, item, 0, arv->tamanho-1);
            arv = arv->filhos[i];
        }
    }
}

#pragma endregion

#pragma region Funções de Remoção

void adicionarPai(Arvb *pai, Arvb *filho, int local) { // Remove os itens da Página pai e insere na Página filha
    filho->itens[filho->tamanho++] = pai->itens[local];
    for (int i = local; i < pai->tamanho-1; i++) {
        pai->itens[i] = pai->itens[i+1];
        pai->filhos[i+1] = pai->filhos[i+2];
    }
    pai->tamanho--;
}

int suporta(Arvb *a, Arvb *b) { // Retorna se suporta adicionar uma Página B na Página A e um item da Página pai
    return tamanho(a) + tamanho(b) < ORDEM-2;
}

int metade(Arvb *arv) { // Retorna se está abaixo da capacidade mínima
    return arv->tamanho < METADE;
}

void unir(Arvb *a,Arvb *b) { // Une a Página A com a B e remove a B
    printf("iniciada a uniao\n");
    if (vaziaOrdem(a) || vaziaOrdem(b)) {
        printf("nao pode unir\n");
        exit(0);
    }
    int i = a->tamanho, j = 0;
    for(; i<(a->tamanho + b->tamanho + 1); i++) {
        a->itens[i] = b->itens[j];
        a->filhos[i] = b->filhos[j++];
    }
    a->tamanho += b->tamanho;
    b->tamanho = 0;
}

void underSplit(Arvb *arv, Arvb *pai) { // Faz o inverso do split
    int local = numeroFilho(pai, arv); // Descobre a localização do filho na Página pai
    if (local < tamanho(pai)-1 && local != 0) {
        // Verifica se de acordo com a posicão a pagina vai ter vizinhos tanto na esquerda quanto na direita, caso seja maior que 0 e menor do que o tamanho ela possui dos dois vizinhos
        if (suporta(arv, pai->filhos[local-1])) {
            Arvb *aux = pai->filhos[local-1];
            adicionarPai(pai, pai->filhos[local-1], local-1);
            unir(aux, arv);
            if (vazia(raiz)) { // Caso a raiz fique vazia muda a raiz para o filho
                free(raiz);
                raiz = aux;
            }
        }
        if (suporta(arv, pai->filhos[local+1])) {
            Arvb *aux = pai->filhos[local+1];
            adicionarPai(pai, arv, local);
            unir(arv, aux);
            if (vazia(raiz)) {
                free(raiz);
                raiz = arv;
            }
        }
    }
    else if (local == 0) { // Caso seja o 0, só possui o vizinho da direita
        if (suporta(arv,pai->filhos[local+1])) {
            Arvb *aux = pai->filhos[local+1];
            adicionarPai(pai, arv, local);
            unir(arv, aux);
            if (cheia(arv))
                split(arv, pai);
            }
            if (vazia(raiz)){
                free(raiz);
                raiz = arv;
            }
    } else { // Caso seja o ultimo so possui o vizinho da esquerda
        if (suporta(arv, pai->filhos[local-1])) {
            Arvb *aux = pai->filhos[local-1];
            adicionarPai(pai, pai->filhos[local-1], local-1);
            unir(aux, arv);
            if (vazia(raiz)) {
                free(raiz);
                raiz = aux;
            }
        }
    }
    
}

void substituir(Arvb *arv, long *item, Arvb *pai) { // Caso precise apagar um item de uma Página que possua filhos, vai substituir o item com número mais próximo(sucessor)
    if (vaziaOrdem(arv->filhos[0])) {
        swap(&(arv->itens[arv->tamanho-1]), item);
        arv->tamanho--;
    } else {
        substituir(arv->filhos[arv->tamanho], item, arv);
    }
    if (metade(arv))
        underSplit(arv, pai);
}

int removerItem(Arvb *arv, long item) { // Remove o item da Página
    long status = 0;
    if (vaziaOrdem(arv->filhos[0])) {
        for (long i = 0; i < arv->tamanho; i++) {
            if (arv->itens[i] == item) { // Coloca o item no final da Página e diminui o tamanho dela
                swap(&(arv->itens[i]), &(arv->itens[i+1]));
                status = 1;
            }
        }
    }
    if (status == 1)
        arv->tamanho--;
    return status;
}

void remover(Arvb *arv, Arvb *pai, long item) { // Remover um item da Árvore
    if (vaziaOrdem(arv)) {
        printf("arvore nao inicida ainda\n");
    } else {
        if (vazia(arv)) {
            printf("nao possui elementos para imprimir\n");
        } else {
            if (vaziaOrdem(arv->filhos[0])) { // Caso ela nao possua filhos
               removerItem(arv, item) ? printf("removeu\n"):printf("nao existe\n");
            } else {
                for (long i = 0; i < arv->tamanho; i++) {
                    if(item == arv->itens[i]){ // Executa a função de troca
                        substituir(arv->filhos[i], &(arv->itens[i]), arv);
                        break;
                    } else if (item < arv->itens[i]) {
                        remover(arv->filhos[i], arv, item);
                        break;
                    } else if (i == arv->tamanho-1) {
                        remover(arv->filhos[i+1], arv, item);
                        break;
                    }
               }
            }
            if (metade(arv) && arv != raiz && vazia(arv) !=1 ) { // Vê se está abaixo da capacidade mínima
                underSplit(arv, pai);
            }
        }
    }
}

void apagar(Arvb *arv){ // Apagar toda a Árvore
    if (vaziaOrdem(arv)) {
        printf("nao iniciada\n");
    } else {
        if (vazia(arv)) {
            printf("nao possue elementos\n");
        } else {
            // Se não tem filhos
            if (vaziaOrdem(arv->filhos[0])) {
                    free(arv);
            } else {
                for (int i = 0; i < arv->tamanho; i++) {
                    apagar(arv->filhos[i]);
                }
                if(arv == raiz)
                    arv->tamanho = 0;
                else
                    free(arv);
            }
        }
    }
    raiz = criarArv();
}

#pragma endregion

#pragma region Funções de Imprimir

void visite(Arvb *arv, int altura) { // Visita a Página e imprime ela
     if(arv == raiz)
        printf("Raiz%*s", 10, "");
    else
        printf("Filho[%d]%*s", altura, altura*3 +10, "|");
    for (int i = 0; i < arv->tamanho; i++) {
        printf("%d ", arv->itens[i]);
    }
    printf("\n");
}

void printArv(Arvb *arv, int altura) { // Imprime a Árvore toda
    if (vaziaOrdem(arv)) {
        printf("Não iniciada a Arvore\n");
    } else {
        if (vazia(arv)) {
            printf("Vazia\n");
        } else {
            visite(arv, altura);
            if (vaziaOrdem(arv->filhos[0]) == 0) {
                for (int i = 0; i <= arv->tamanho; i++) {
                    printArv(arv->filhos[i], altura+1);
                }
                altura++;
            }
        }
    }
}

#pragma endregion

#pragma region Funções de Backup

void abrirArquivo(char *nome, char *modo) { // Abre um arquivo em um modo ou para salvar ou carregar a Árvore
    FILE *file = fopen(nome, modo);
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nome);
        return;
    }
    printf("Modo:%s", modo);
    if (modo == "w")
        salvarArvore(raiz, file);
    else if (modo == "r")
        carregarArvore(raiz, file);
    fclose(file);
}

void salvarArvore(Arvb *arv, FILE *file) { // Função para salvar a Árvore B em um arquivo
    if (arv == NULL)
        return;
    if (file == NULL) {
        FILE *file = fopen("arvore_b.txt", "w");
        if (file == NULL)
            return;
        else
            salvarArvore(arv, file);
    }
    fprintf(file, "%d(%d)\n", arv->tamanho, !vaziaOrdem(arv->filhos[0]));
    for (long i = 0; i < arv->tamanho; i++) {
        fprintf(file, "%d\n", arv->itens[i]);
    }
    for (long i = 0; i < arv->tamanho + 1; i++) {
        salvarArvore(arv->filhos[i], file);
    }
}

void carregarArvore(Arvb *arv, FILE *file) { // Função para carregar a Árvore B de um arquivo
    long tamanho, tem_filhos;
    if (fscanf(file, "%d(%d)\n", &tamanho, &tem_filhos) == EOF) {
        arv = NULL;
        return;
    }
    arv->tamanho = tamanho;
    for (long i = 0; i < arv->tamanho; i++) {
        fscanf(file, "%d", &(arv->itens[i]));
    }
    if (tem_filhos) {
        for (long i = 0; i < tamanho + 1; i++) {
            arv->filhos[i] = criarArv();
            carregarArvore(arv->filhos[i], file);
        }
    }
}

#pragma endregion

#pragma region Função Menu

long randomLong(long max) {
    return (long)((double)rand() / ((double)RAND_MAX + 1) * (max + 1));
}

void menu(char *endereco) { // Função Menu
    char escolha = '0';
    while (escolha != 'N') {
        printf("Escolha uma opcao:\n");
        printf("[1] - Inserir\n");
        printf("[2] - Inserir aleatorio\n");
        printf("[3] - Imprimir\n");
        printf("[4] - Pesquizar\n");
        printf("[5] - Remover\n");
        printf("[6] - Apagar arvore\n");
        printf("[7] - Carregar arvore\n");
        printf("[8] - Salvar arvore\n");
        printf("[9] - Sair\n");
        fflush(stdin);
        scanf("%c", &escolha);

        // Escolhas
        switch (escolha) {

            case '1':
                printf("Digite um numero:\n");
                long numero;
                scanf("%d", &numero);
                inserir(raiz, NULL, numero);
                break;
            
            case '2':
                printf("Digite a quantidade de numeros\n");
                long quantidade;
                scanf("%d", &quantidade);
                srand(time(NULL));
                for (long i = 1; i < quantidade; i++) {
                    inserir(raiz, NULL, randomLong(i * 10));
                }
                break;
            
            case '3':
                printArv(raiz, 0);
                break;
            
            case '4':
                printf("Digite o numero que quer pesquisar\n");
                long pesquisar;
                scanf("%d", &pesquisar);
                pesquisa(raiz, pesquisar);
                break;
            
            case '5': 
                printf("Digite o item para ser removido\n");
                long item;
                scanf("%d", &item);
                remover(raiz, NULL, item);
                break;
            
            case '6':
                apagar(raiz);
                break;

            case '7':
                abrirArquivo("arvore_b.txt", "r");
                break;

            case '8':
                abrirArquivo("arvore_b.txt", "w");
                break;
            
            case '9':
                printf("Saindo\n");
                *endereco = '1';
                //exit(0);
                escolha = 'N';
                break;
            
            default:
                printf("Opcao invalida\n");
            
        }

        if (escolha == 'N')
            break;
        scanf("%c", &escolha);

    }
}

#pragma endregion