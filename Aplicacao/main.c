#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>

#include "arvoreb.h"

int main(int argc, char *argv[]) {

    // Criar a Árvore
    raiz = criarArv();

    // Inicializar Menu
    char escolha;
    do {
        menu(&escolha);
    } while (escolha != '1');

    // Apagar a Árvore
    apagar(raiz);

    return 0;

}