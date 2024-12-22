#include <stdio.h>
#include <stdlib.h>

typedef struct Aresta {
    int id;
    struct Aresta* prox;
} Aresta;

typedef struct Vertice {
    int id;
    Aresta* arestas;
    struct Vertice* prox;
} Vertice;


Vertice* criarVertice(int id) {
    Vertice* novo = (Vertice*) malloc(sizeof(Vertice));
    if (!novo) {
        printf("erro ao alocar memória.\n");
        return NULL;
    }
    novo->id = id;
    novo->arestas = NULL;
    novo->prox = NULL;
    return novo;
}

Aresta* criarAresta(int id) {
    Aresta* nova = (Aresta*) malloc(sizeof(Aresta));
    if (!nova) {
        printf("erro ao alocar memória.\n");
        return NULL;
    }
    nova->id = id;
    nova->prox = NULL;
    return nova;
}


void inserirVertice(Vertice** grafo, int id) {
    Vertice* novo = criarVertice(id);
    if (novo) {
        novo->prox = *grafo;
        *grafo = novo;
    }
}


void inserirAresta(Vertice* grafo, int origem, int destino) {
    while (grafo && grafo->id != origem) {
        grafo = grafo->prox;
    }
    if (grafo) {
        Aresta* nova = criarAresta(destino);
        if (nova) {
            nova->prox = grafo->arestas;
            grafo->arestas = nova;
        }
    } else {
        printf("vértice de origem %d não encontrado.\n", origem);
    }
}


void removerVertice(Vertice** grafo, int id) {
    Vertice *atual = *grafo, *anterior = NULL;
    while (atual && atual->id != id) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual) {
        if (anterior) {
            anterior->prox = atual->prox;
        } else {
            *grafo = atual->prox;
        }
        while (atual->arestas) {
            Aresta* temp = atual->arestas;
            atual->arestas = temp->prox;
            free(temp);
        }
        free(atual);
    } else {
        printf("vértice %d não encontrado para remoção.\n", id);
    }
}


void removerAresta(Vertice* grafo, int origem, int destino) {
    while (grafo && grafo->id != origem) {
        grafo = grafo->prox;
    }
    if (grafo) {
        Aresta *atual = grafo->arestas, *anterior = NULL;
        while (atual && atual->id != destino) {
            anterior = atual;
            atual = atual->prox;
        }
        if (atual) {
            if (anterior) {
                anterior->prox = atual->prox;
            } else {
                grafo->arestas = atual->prox;
            }
            free(atual);
        } else {
            printf("aresta de %d para %d não encontrada.\n", origem, destino);
        }
    } else {
        printf("vértice de origem %d não encontrado.\n", origem);
    }
}


void exibirGrafo(Vertice* grafo) {
    if (!grafo) {
        printf("nao ha vertices\n");
        return;
    }
    while (grafo) {
        printf("%d:", grafo->id);
        Aresta* aresta = grafo->arestas;
        while (aresta) {
            printf(" %d", aresta->id);
            aresta = aresta->prox;
        }
        printf("\n");
        grafo = grafo->prox;
    }
}


void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// função principal
int main() {
    Vertice* grafo = NULL;
    int opcao, id, origem, destino;

    do {
        printf("\nMenu:\n");
        printf("1. Inserir Vértice\n");
        printf("2. Inserir Aresta\n");
        printf("3. Remover Vértice\n");
        printf("4. Remover Aresta\n");
        printf("5. Exibir Grafo\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("entrada inválida\n");
            limparBuffer();
            continue;
        }

        switch (opcao) {
            case 1:
                printf("digite o ID do vértice: ");
                if (scanf("%d", &id) != 1) {
                    printf("entrada inválida.\n");
                    limparBuffer();
                    continue;
                }
                inserirVertice(&grafo, id);
                break;
            case 2:
                printf("digite o ID de origem: ");
                if (scanf("%d", &origem) != 1) {
                    printf("entrada inválida.\n");
                    limparBuffer();
                    continue;
                }
                printf("digite o ID de destino: ");
                if (scanf("%d", &destino) != 1) {
                    printf("entrada inválida.\n");
                    limparBuffer();
                    continue;
                }
                inserirAresta(grafo, origem, destino);
                break;
            case 3:
                printf("digite o ID do vértice a ser removido: ");
                if (scanf("%d", &id) != 1) {
                    printf("entrada inválida.\n");
                    limparBuffer();
                    continue;
                }
                removerVertice(&grafo, id);
                break;
            case 4:
                printf("digite o ID de origem: ");
                if (scanf("%d", &origem) != 1) {
                    printf("entrada inválida.\n");
                    limparBuffer();
                    continue;
                }
                printf("digite o ID de destino: ");
                if (scanf("%d", &destino) != 1) {
                    printf("entrada inválida\n");
                    limparBuffer();
                    continue;
                }
                removerAresta(grafo, origem, destino);
                break;
            case 5:
                printf("Grafo:\n");
                exibirGrafo(grafo);
                break;
            case 6:
                printf("Encerrando o programa.\n");
                while (grafo) {
                    Vertice* temp = grafo;
                    removerVertice(&grafo, temp->id);
                }
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 6);

    return 0;
}
