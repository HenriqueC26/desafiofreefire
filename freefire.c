#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 20

// ========================================
//              STRUCT PRINCIPAL
// ========================================
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// ========================================
//              FUNÇÕES UTIL
// ========================================

void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Remove newline do fgets
void removerQuebraLinha(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

// Exibir todos os componentes
void mostrarComponentes(Componente v[], int n) {
    printf("\n--- LISTA DE COMPONENTES ---\n");
    for (int i = 0; i < n; i++) {
        printf("%d. Nome: %s | Tipo: %s | Prioridade: %d\n",
               i + 1, v[i].nome, v[i].tipo, v[i].prioridade);
    }
}

// ========================================
//              BUBBLE SORT (NOME)
// ========================================
long bubbleSortNome(Componente v[], int n) {
    long comparacoes = 0;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparacoes++;
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                Componente temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }

    return comparacoes;
}

// ========================================
//          INSERTION SORT (TIPO)
// ========================================
long insertionSortTipo(Componente v[], int n) {
    long comparacoes = 0;

    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;

        while (j >= 0 && strcmp(chave.tipo, v[j].tipo) < 0) {
            comparacoes++;
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }

    return comparacoes;
}

// ========================================
//        SELECTION SORT (PRIORIDADE)
// ========================================
long selectionSortPrioridade(Componente v[], int n) {
    long comparacoes = 0;

    for (int i = 0; i < n - 1; i++) {
        int menor = i;

        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            if (v[j].prioridade < v[menor].prioridade)
                menor = j;
        }

        if (menor != i) {
            Componente temp = v[i];
            v[i] = v[menor];
            v[menor] = temp;
        }
    }

    return comparacoes;
}

// ========================================
//           BUSCA BINÁRIA (NOME)
// ========================================
int buscaBinariaPorNome(Componente v[], int n, char chave[], long *comparacoes) {
    int inicio = 0, fim = n - 1;
    *comparacoes = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;

        int cmp = strcmp(chave, v[meio].nome);

        if (cmp == 0) return meio;

        if (cmp > 0)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }

    return -1;
}

// ========================================
//           MEDIR TEMPO DE EXECUÇÃO
// ========================================
double medirTempo(long (*func)(Componente[], int), Componente v[], int n, long *comparacoes) {
    clock_t inicio = clock();
    *comparacoes = func(v, n);
    return (double)(clock() - inicio) / CLOCKS_PER_SEC;
}

// ========================================
//                MAIN
// ========================================
int main() {
    Componente lista[MAX];
    int total = 0;
    int opcao;

    do {
        printf("\n======= MONTAGEM DA TORRE — MENU =======\n");
        printf("1. Cadastrar componente\n");
        printf("2. Mostrar componentes\n");
        printf("3. Ordenar por nome (Bubble Sort)\n");
        printf("4. Ordenar por tipo (Insertion Sort)\n");
        printf("5. Ordenar por prioridade (Selection Sort)\n");
        printf("6. Busca binária por nome\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparEntrada();

        if (opcao == 1) {
            if (total >= MAX) {
                printf("Limite atingido (20 componentes)\n");
                continue;
            }

            printf("Nome: ");
            fgets(lista[total].nome, 30, stdin);
            removerQuebraLinha(lista[total].nome);

            printf("Tipo: ");
            fgets(lista[total].tipo, 20, stdin);
            removerQuebraLinha(lista[total].tipo);

            printf("Prioridade (1-10): ");
            scanf("%d", &lista[total].prioridade);
            limparEntrada();

            total++;
            printf("Componente cadastrado!\n");
        }

        else if (opcao == 2) {
            mostrarComponentes(lista, total);
        }

        else if (opcao == 3) {
            long comparacoes;
            double tempo = medirTempo(bubbleSortNome, lista, total, &comparacoes);

            printf("\nOrdenado por nome (Bubble Sort)\n");
            printf("Comparações: %ld\n", comparacoes);
            printf("Tempo: %.6f segundos\n", tempo);
            mostrarComponentes(lista, total);
        }

        else if (opcao == 4) {
            long comparacoes = insertionSortTipo(lista, total);

            printf("\nOrdenado por tipo (Insertion Sort)\n");
            printf("Comparações: %ld\n", comparacoes);
            mostrarComponentes(lista, total);
        }

        else if (opcao == 5) {
            long comparacoes = selectionSortPrioridade(lista, total);

            printf("\nOrdenado por prioridade (Selection Sort)\n");
            printf("Comparações: %ld\n", comparacoes);
            mostrarComponentes(lista, total);
        }

        else if (opcao == 6) {
            long compBusca;
            char chave[30];

            printf("Nome para buscar: ");
            fgets(chave, 30, stdin);
            removerQuebraLinha(chave);

            int pos = buscaBinariaPorNome(lista, total, chave, &compBusca);

            printf("Comparações da busca binária: %ld\n", compBusca);

            if (pos >= 0) {
                printf("Componente encontrado!\n");
                printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                       lista[pos].nome, lista[pos].tipo, lista[pos].prioridade);
            } else {
                printf("Componente NÃO encontrado. Lembre-se:\n→ É necessário ordenar por nome antes da busca!\n");
            }
        }

    } while (opcao != 0);

    printf("Encerrando...\n");
    return 0;
}
