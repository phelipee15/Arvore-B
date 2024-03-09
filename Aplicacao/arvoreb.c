#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "arvoreb.h"

#define METADE ORDEM/2

#pragma region Inicialização e Informações

Arvb *criarArv() { // Inicializa a Árvore e retorna ela
    Arvb *aux;
    aux = (Arvb*)malloc(sizeof(Arvb));
    aux->tamanho = 0;
    for (long i = 0; i < ORDEM-1; i++) {
        aux->itens[i] = criarConta();
    }
    for (long i = 0; i < ORDEM; i++) {
        aux->filhos[i] = NULL;
    }
    return aux;
}

Conta *criarConta() { // Inicializa a Conta e retorna ela
    Conta *aux;
    aux = (Conta*)malloc(sizeof(Conta));
    aux->CPF = 0;
    strcpy(aux->nome, "");
    aux->saldo = 0;
    return aux;
}

long vaziaOrdem(Arvb *arv) { // Retorna se a Página existe (true || false)
    return arv == NULL;
}

long vazia(Arvb *arv) { // Retorna se a Página está vazia (true || false)
    return arv->tamanho == 0;
}

long cheia(Arvb *arv) { // Retorna se a Página está cheia (true || false)
    return arv->tamanho >= ORDEM-1;
}

long tamanho(Arvb *arv) { // Retorna o tamanho da Página
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

void swap(Conta **a, Conta **b) { // Trocar o valor de A por B e vise-versa
    Conta *aux = *a;
    *a = *b;
    *b = aux;
}
 
void inserirOrd(Arvb *arv, long item, char *nome, long saldo) { // Insere os itens de forma ordenada
    long status = 0; // Informa se ja foi feita a troca
    Conta *guardado = criarConta(); // Guarda os dados
    if (vazia(arv)) {
        arv->itens[arv->tamanho]->CPF = item;
        strcpy(arv->itens[arv->tamanho]->nome, nome);
        arv->itens[arv->tamanho]->saldo = saldo;
    } else {
        for (long i = 0; i <= arv->tamanho; i++) {
            if (status == 0) {
                if (item == arv->itens[i]->CPF) {
                    arv->tamanho--;
                    break;
                }
                if (item < arv->itens[i]->CPF) {
                    guardado->CPF = arv->itens[i]->CPF;
                    strcpy(guardado->nome, arv->itens[i]->nome);
                    guardado->saldo = arv->itens[i]->saldo;
                    arv->itens[i]->CPF = item;
                    strcpy(arv->itens[i]->nome, nome);
                    arv->itens[i]->saldo = saldo;
                    status = 1;
                } else if (i == arv->tamanho) {
                    arv->itens[i]->CPF = item;
                    strcpy(arv->itens[i]->nome, nome);
                    arv->itens[i]->saldo = saldo;
                }
            } else {
                swap(&guardado, &arv->itens[i]);
            }
        }
    }
    arv->tamanho++;
}

long buscabinariaFilho(Arvb *arv, Arvb *filho, long inicio, long fim) { // Buscar a posição do filho no pai
    if (inicio == fim) {
        if(arv->filhos[inicio]->itens[0]->CPF == filho->itens[0]->CPF)
            return inicio;
        else
            return -1;
    } else {
        long meio = (inicio + fim)/2;
        if (arv->filhos[meio]->itens[0]->CPF < filho->itens[0]->CPF){
            return buscabinariaFilho(arv, filho, meio+1, fim);
        } else {
            return buscabinariaFilho(arv, filho, inicio, meio);
        }
    }
}

long buscaProximo(Arvb *arv, long item, long inicio, long fim) { // Retorna a busca da posição de onde se encontra o item
    if (inicio == fim) {
        if(arv->itens[inicio]->CPF > item)
            return inicio;
        else
            return inicio+1;
    } else {
        long meio = (inicio + fim)/2;
        if(arv->itens[meio]->CPF > item)
            return buscaProximo(arv, item, inicio, meio);
        else
            return buscaProximo(arv, item, meio+1, fim);
    }
}

long numeroFilho(Arvb *pai, Arvb *filho) { // Retorna a posição do filho em relação ao pai
    return buscabinariaFilho(pai, filho, 0, pai->tamanho);
}

void inserirFilho(Arvb *pai, Arvb *filho, long lugar) { // Insere um novo filho
    Arvb *aux, *aux1 = filho;
    for (long i = lugar+1; i <= pai->tamanho; i++){
        aux = pai->filhos[i];
        pai->filhos[i] = aux1;
        aux1 = aux;
    }
}

void split(Arvb *arv, Arvb *pai) { // Separa os itens da Árvore(Página) e reoganiza
    if (arv == raiz) {
        Arvb *encima = criarArv(), *lado = criarArv();
        inserirOrd(encima, arv->itens[(METADE)]->CPF, arv->itens[(METADE)]->nome, arv->itens[(METADE)]->saldo);
        dividir(arv, lado);
        raiz = encima;
        raiz->filhos[0] = arv;
        raiz->filhos[1] = lado;
    } else {
        Arvb *lado = criarArv();
        inserirOrd(pai, arv->itens[(METADE)]->CPF, arv->itens[(METADE)]->nome, arv->itens[(METADE)]->saldo);
        dividir(arv, lado);
        inserirFilho(pai, lado, numeroFilho(pai, arv));
    }
}

void inserir(Arvb *arv, Arvb *pai, long item, char *nome, long saldo) { // Inserir um item na Árvore
    if (vaziaOrdem(arv)) {
        printf("Nao Inicializada\n");
    } else {
        if (vaziaOrdem(arv->filhos[0]) == 1) { // Caso não possui filhos, indica que a Página é uma folha, então inserimos
            inserirOrd(arv, item, nome, saldo);
        } else { // Caso possua, procuramos aonde inserir
            long i = buscaProximo(arv, item, 0, arv->tamanho-1);
            inserir(arv->filhos[i], arv, item, nome, saldo);
        }
        if (cheia(arv)) { // Se a Página está cheia, efetuamos o split
            split(arv, pai);
        }
   }
}

void lerLinhaAleatoria(FILE *arquivo, char *linha) {
    long posicao = rand() % 100 + 1;
    for (long j = 0; j < posicao; j++) {
        if (fgets(linha, 50, arquivo) == NULL) {
            return;
        }
    }
}

void inserirAleatorio(long qntd) {
    char linhaNome[50];
    char linhaSobrenome[50];
    FILE *nomes = fopen("dic/nomes.txt", "r");
    if (nomes == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }
    FILE *sobrenomes = fopen("dic/sobrenomes.txt", "r");
    if (sobrenomes == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }
    srand(time(NULL));
    for (long i = 0; i < qntd; i++) {
        fseek(nomes, 0, SEEK_SET);
        fseek(sobrenomes, 0, SEEK_SET);
        lerLinhaAleatoria(nomes, linhaNome);
        lerLinhaAleatoria(sobrenomes, linhaSobrenome);
        char nomeCompleto[100];
        char *nome = strtok(linhaNome, ",");
        char *sobrenome = strtok(linhaSobrenome, ",");
        snprintf(nomeCompleto, sizeof(nomeCompleto), "%s %s", nome, sobrenome);
        inserir(raiz, NULL, rand()%1000000000, nomeCompleto, rand()%10000);
    }
    fclose(nomes);
    fclose(sobrenomes);
}

#pragma endregion

#pragma region Funções de Pesquisa

long buscarItemPag(Arvb *arv, long item, long inicio, long fim) { // Retorna se o item está presente na Página
    if (fim >= inicio) {
        long meio = (inicio + fim)/2;
        if (arv->itens[meio]->CPF == item)
            return meio;
        else if (arv->itens[meio]->CPF > item)
            return buscarItemPag(arv, item, inicio, meio-1);
        else
            return buscarItemPag(arv, item, meio+1, fim);
    }
    return -1;
}

long possui(Arvb *arv, long item) {
    return buscarItemPag(arv, item, 0, arv->tamanho-1)!=-1?1:0;
}

long buscarProxFilho(Arvb *arv, long item, long inicio, long fim) { // Retorna a busca da posição de onde se encontra o filho para procurar o item
    if (fim == inicio) {
        if(arv->itens[inicio]->CPF > item)
            return inicio;
        else
            return inicio+1;
    } else {
        long meio = (inicio + fim)/2;
        if(arv->itens[meio]->CPF > item)
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
            long i = buscarProxFilho(arv, item, 0, arv->tamanho-1);
            arv = arv->filhos[i];
        }
    }
}

#pragma endregion

#pragma region Funções de Remoção

void adicionarPai(Arvb *pai, Arvb *filho, long local) { // Remove os itens da Página pai e insere na Página filha
    filho->itens[filho->tamanho++] = pai->itens[local];
    for (long i = local; i < pai->tamanho-1; i++) {
        pai->itens[i] = pai->itens[i+1];
        pai->filhos[i+1] = pai->filhos[i+2];
    }
    pai->tamanho--;
}

long suporta(Arvb *a, Arvb *b) { // Retorna se suporta adicionar uma Página B na Página A e um item da Página pai
    return tamanho(a) + tamanho(b) < ORDEM-2;
}

long metade(Arvb *arv) { // Retorna se está abaixo da capacidade mínima
    return arv->tamanho < METADE;
}

void unir(Arvb *a, Arvb *b) { // Une a Página A com a B e remove a B
    printf("iniciada a uniao\n");
    if (vaziaOrdem(a) || vaziaOrdem(b)) {
        printf("nao pode unir\n");
        exit(0);
    }
    long i = a->tamanho, j = 0;
    for(; i<(a->tamanho + b->tamanho + 1); i++) {
        a->itens[i] = b->itens[j];
        a->filhos[i] = b->filhos[j++];
    }
    a->tamanho += b->tamanho;
    b->tamanho = 0;
}

void underSplit(Arvb *arv, Arvb *pai) { // Faz o inverso do split
    long local = numeroFilho(pai, arv); // Descobre a localização do filho na Página pai
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

void substituir(Arvb *arv, Conta *conta, Arvb *pai) { // Caso precise apagar um item de uma Página que possua filhos, vai substituir o item com número mais próximo(sucessor)
    if (vaziaOrdem(arv->filhos[0])) {
        swap(&(arv->itens[arv->tamanho-1]), &conta);
        arv->tamanho--;
    } else {
        substituir(arv->filhos[arv->tamanho], conta, arv);
    }
    if (metade(arv))
        underSplit(arv, pai);
}

long removerItem(Arvb *arv, long item) { // Remove o item da Página
    long status = 0;
    if (vaziaOrdem(arv->filhos[0])) {
        for (long i = 0; i < arv->tamanho; i++) {
            if (arv->itens[i]->CPF == item) { // Coloca o item no final da Página e diminui o tamanho dela
                swap(&(arv->itens[i]), &(arv->itens[i+1]));
                status = 1;
            }
        }
    }
    if (status == 1) {
        free(arv->itens[arv->tamanho-1]);
        arv->tamanho--;
    }
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
                    if(item == arv->itens[i]->CPF){ // Executa a função de troca
                        substituir(arv->filhos[i], (arv->itens[i]), arv);
                        break;
                    } else if (item < arv->itens[i]->CPF) {
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
                for (long i = 0; i < arv->tamanho; i++) {
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

void visite(Arvb *arv, long altura) { // Visita a Página e imprime ela
    if(arv == raiz)
        printf("Raiz%*s", 10, "");
    else
        printf("Filho[%d]%*s", altura, altura*3 +10, "|");
    for (long i = 0; i < arv->tamanho; i++) {
        printf("%ld(%s) ", arv->itens[i]->CPF, arv->itens[i]->nome);
    }
    printf("\n");
}

void printArv(Arvb *arv, long altura) { // Imprime a Árvore toda
    if (vaziaOrdem(arv)) {
        printf("Não iniciada a Arvore\n");
    } else {
        if (vazia(arv)) {
            printf("Vazia\n");
        } else {
            visite(arv, altura);
            if (vaziaOrdem(arv->filhos[0]) == 0) {
                for (long i = 0; i <= arv->tamanho; i++) {
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
        fprintf(file, "%d\n", arv->itens[i]->CPF);
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
        fscanf(file, "%d", &(arv->itens[i]->CPF));
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
                printf("Informe o CPF:\n");
                long numero;
                scanf("%ld", &numero);
                char nome[30];
                printf("Informe o nome:\n");
                scanf("%s", nome);
                long saldo;
                printf("Informe o saldo:\n");
                scanf("%ld", &saldo);
                inserir(raiz, NULL, numero, nome, saldo);
                break;
            
            case '2':
                printf("Digite a quantidade de contas a ser gerada: \n");
                long quantidade;
                scanf("%ld", &quantidade);
                inserirAleatorio(quantidade);
                break;
            
            case '3':
                printArv(raiz, 0);
                break;
            
            case '4':
                printf("Digite o numero que quer pesquisar\n");
                long pesquisar;
                scanf("%ld", &pesquisar);
                pesquisa(raiz, pesquisar);
                break;
            
            case '5': 
                printf("Digite o item para ser removido\n");
                long item;
                scanf("%ld", &item);
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