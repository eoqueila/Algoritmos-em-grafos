#include <stdio.h>
#include <stdlib.h>

typedef struct t_celula{
    struct t_celula *direita, *abaixo;
    int linha, coluna;
    int valor;
}tipo_celula;

typedef struct{
    int m, n;
    tipo_celula *inicio, *fimLinha, *fimColuna;
}tipo_matriz_esparsa;

typedef tipo_matriz_esparsa matriz_esparsa;

matriz_esparsa *cria_matriz_esparsa(int m, int n);
int iniciaCabecas(matriz_esparsa *mat);
int insereCabecaLinha(matriz_esparsa *mat);
int insereCabecaColuna(matriz_esparsa *mat);

int insere(matriz_esparsa *mat, int linha, int coluna, int valor);
int imprimeMatriz(matriz_esparsa *mat);
int apagaMatriz(matriz_esparsa *mat);
matriz_esparsa *somaMatriz(matriz_esparsa *matA, matriz_esparsa *matB);
matriz_esparsa *multiplicaMatriz(matriz_esparsa *matA, matriz_esparsa *matB);
int obtemElementoPeloIndice(matriz_esparsa *mat, int linha, int coluna);


matriz_esparsa *cria_matriz_esparsa(int m, int n){
    matriz_esparsa *mat;
	mat = (matriz_esparsa *)malloc(sizeof(matriz_esparsa));

	if (!mat || m<=0 || n<=0){
        return 0;
	}

	//inicia as variaveis da struct
	mat->inicio = NULL;
	mat->fimLinha = NULL;
	mat->fimColuna = NULL;
	mat->m = m;
	mat->n = n;

	iniciaCabecas(mat);

	return mat;
}

int iniciaCabecas(matriz_esparsa *mat){ //cria a estrutura
    int i=0;

    tipo_celula *cabeca;
    cabeca = (tipo_celula*)malloc(sizeof(tipo_celula));

    if (!cabeca){
        return 0; //erro
    }

    //celula principal
    cabeca->coluna = -1;
    cabeca->linha = -1;

    mat->inicio = cabeca;
    mat->fimLinha = cabeca;
    mat->fimColuna = cabeca;

    for (i = 1; i <= mat->n; i++){ //cabecas colunas, | | | |
        insereCabecaColuna(mat);
    }
    for (i = 1; i <= mat->m; i++){ //cabecas linhas
        insereCabecaLinha(mat);
    }
    return 1;
}
int insereCabecaColuna(matriz_esparsa *mat){ //cabecas colunas, | | | |
    tipo_celula *cabeca;
    cabeca = (tipo_celula*)malloc(sizeof(tipo_celula));

    if (!cabeca){
        return 0; //erro
    }

    cabeca->coluna = -1;
    cabeca->linha = 0;

    mat->fimColuna->direita = cabeca;
    mat->fimColuna = cabeca;

    cabeca->direita = mat->inicio;
    cabeca->abaixo = cabeca;
    return 1;
}

int insereCabecaLinha(matriz_esparsa *mat){
    tipo_celula *cabeca;
    cabeca = (tipo_celula*)malloc(sizeof(tipo_celula));

    if (!cabeca){
        return 0;
    }

    cabeca->coluna = 0;
    cabeca->linha = -1;

    mat->fimLinha->abaixo = cabeca;
    mat->fimLinha = cabeca;

    cabeca->abaixo = mat->inicio;
    cabeca->direita = cabeca;

    return 1;
}

int insere(matriz_esparsa *mat, int linha, int coluna, int valor){
    int i;

    if (!mat || mat->m <= 0 || mat->n <= 0 || !valor){ //esses testes sao feitos tbm na funcao lerMatriz
        return 0;
    }
    if (linha>mat->m || coluna>mat->n || !valor || linha < 1 || coluna < 1){
        return 0;
    }

    tipo_celula *pCelula; //celula temporaria
    tipo_celula *pCelulaColuna; //celula temporaria para coluna
    tipo_celula *pCelulaLinha; //celula temporaria para linha

    pCelula = (tipo_celula*)malloc(sizeof(tipo_celula));

    if (!pCelula){ //memoria cheia
        return 0;
    }

    pCelula->linha = linha;
    pCelula->coluna = coluna;
    pCelula->valor = valor;

    pCelulaLinha = mat->inicio->abaixo; //linha aponta para a primeira cabeca so de linha
    pCelulaColuna = mat->inicio->direita; //linha aponta para a primeira cabeca so de coluna

    //Percorre ate a linha do elemento, percorre nas cabecas ate a informada
    for (i=0; i<linha-1; i++){ //-1 pq ja ta na primeira
        pCelulaLinha = pCelulaLinha->abaixo; //percorre para baixo(linhas) ate achar a correta
    }
    i=0;
    while (i<coluna && pCelulaLinha->direita->linha != -1){
        if (pCelulaLinha->direita->coluna > pCelula->coluna){ //se a celula inserida esta a esquerda
            pCelula->direita = pCelulaLinha->direita;
            pCelulaLinha->direita = pCelula;
        }
        else{
            pCelulaLinha = pCelulaLinha->direita; //anda pelas celulas a direita
        }
        i++;
    }
    if (pCelulaLinha->direita->linha == -1){ //a celula esta mais a direita
        pCelula->direita = pCelulaLinha->direita; //aponta para a cabeca
        pCelulaLinha->direita = pCelula; //ultima celula ou cabeca, senao tiver celulas, aponta para a celula
    }
    //linha encontrada e apontada

    //Agora vamos encontrar a coluna
    //Percorre ate a coluna do elemento, percorre nas cabecas ate a informada
    for (i = 0; i < coluna-1; i++){ //-1 pq ja ta na primeira
        pCelulaColuna = pCelulaColuna->direita; //percorre a direita (colunas) ate achar a correta
    }
    i=0;
    while (i<linha && pCelulaColuna->abaixo->coluna != -1){ //anda pelas celulas ate achar a posicao
        if (pCelulaColuna->abaixo->linha > pCelula->linha){
            pCelula->abaixo = pCelulaColuna->abaixo;
            pCelulaColuna->abaixo = pCelula;
        }
        else{
            pCelulaColuna = pCelulaColuna->abaixo; //anda pelas celulas abaixo
        }
        i++;
    }
    if (pCelulaColuna->abaixo->coluna == -1){
        pCelula->abaixo = pCelulaColuna->abaixo; //aponta para a cabeca
        pCelulaColuna->abaixo = pCelula; //ultima celula ou cabeca, senao tiver celulas, aponta para a celula
    }
    return 1;
}


int imprimeMatriz(matriz_esparsa *mat){
    int i, j;
    tipo_celula *pCelula;

    if (!mat || !mat->m || !mat->n){
        return 0;
    }

    pCelula = mat->inicio->abaixo;

    printf("\n");

    for (i = 1; i <= mat->m; i++){
        for (j = 1; j <= mat->n; j++){
            if (pCelula->direita->linha == i && pCelula->direita->coluna == j){
                pCelula = pCelula->direita;
                printf("  \t%d  ", pCelula->valor);
            }
            else{
                printf("  \t%d   ", 0);
            }
        }
        printf("\n");
        pCelula = pCelula->direita->abaixo;
    }

    return 1;
}

int apagaMatriz(matriz_esparsa *mat){
    int i = 1;
    int j = 1;
    tipo_celula *pCelula, *aux;

    if (!mat || !mat->m || !mat->n){
        return 0;
    }

    pCelula = mat->inicio->abaixo;
    pCelula = pCelula->direita;

    for (i = 1; i <= mat->m; i++){
        for (j = 1; j <= mat->n; j++){
            if (pCelula->linha == i && pCelula->coluna == j){
                aux = pCelula;
                pCelula = pCelula->direita;
                free(aux);
            }
        }
        pCelula = pCelula->abaixo->direita;
    }

    //apaga cabecas
    pCelula = mat->inicio->direita;

    for (i = 0; i < mat->n; i++){ //cabecas coluna
        aux = pCelula;
        pCelula = pCelula->direita;
        free(aux);
    }

    pCelula = mat->inicio->abaixo;
    for (i = 0; i < mat->m; i++){ //cabecas linha
        aux = pCelula;
        pCelula = pCelula->abaixo;
        free(aux);
    }

    pCelula = mat->inicio;
    free(pCelula);

    mat->fimColuna = mat->fimLinha = mat->inicio = NULL;
    mat->m = mat->n = 0;
    mat = NULL;

    return 1;
}

matriz_esparsa *somaMatriz(matriz_esparsa *matA, matriz_esparsa *matB){
    int i, j;
    float soma;
    matriz_esparsa *matC;
    tipo_celula *pCelulaA, *pCelulaB;

    if (matA->m != matB->m || matA->n != matB->n){ //soma de matrizes so é possivel qndo as duas forem de mesma ordem
        printf("\nMatrizes de Ordem diferente.\nSo eh possivel somar matrizes de mesma ordem\n");
        return NULL;
    }

    if (!matA || !matB || !matA->m || !matA->n){
        return NULL;
    }

    matC = cria_matriz_esparsa(matA->m, matB->n);

    pCelulaA = matA->inicio->abaixo;
    pCelulaB = matB->inicio->abaixo;

    for (i = 1; i <= matA->m; i++){
        for (j = 1; j <= matA->n; j++){
            if (j == pCelulaA->direita->coluna && j == pCelulaB->direita->coluna){
                soma = pCelulaA->direita->valor + pCelulaB->direita->valor;
                if (soma){
                    insere(matC, i, j, soma);
                }
                pCelulaA = pCelulaA->direita;
                pCelulaB = pCelulaB->direita;
            }
            else if (j == pCelulaA->direita->coluna){
                insere(matC, i, j, pCelulaA->direita->valor);
                pCelulaA = pCelulaA->direita;
            }
            else if (j == pCelulaB->direita->coluna){
                insere(matC, i, j, pCelulaB->direita->valor);
                pCelulaB = pCelulaB->direita;
            }
        }
        pCelulaA = pCelulaA->direita->abaixo;
        pCelulaB = pCelulaB->direita->abaixo;
    }

    return matC;
}

matriz_esparsa *multiplicaMatriz(matriz_esparsa *matA, matriz_esparsa *matB){
    int i=0, j=0, k=0;
    float total;
    matriz_esparsa *matC;

    if (matA->n != matB->m){ //so pode multiplicar se o numero de colunas de A eh igual ao numero de linhas de B
        printf("\nNao foi possivel multiplicar as matrizes\nnumero de colunas de A eh diferente do numero de linhas de B\n");
        return NULL;
    }

    if (!matA || !matB || !matA->m || !matA->n || !matB->n){
        return NULL;
    }

    matC = cria_matriz_esparsa(matA->m, matB->n); //C é formada pelo numero de linhas de A e de colunas de B

    for (i = 1; i <= matA->m; i++){
        for (j = 1; j <= matB->n; j++){
            total = 0;
            for (k = 1; k <= matA->n; k++){
                total += obtemElementoPeloIndice(matA,i,k) * obtemElementoPeloIndice(matB,k,j);
            }
            if (total){
                insere(matC,i,j,total);
            }
        }
    }
    return matC;
}

int obtemElementoPeloIndice(matriz_esparsa *mat, int linha, int coluna){
    tipo_celula *pCelula;
    int i = 0;

    pCelula = mat->inicio->direita;

    for (i = 0; i < coluna-1; i++){ //chega na coluna desejada
        pCelula = pCelula->direita;
    }

    do{
        pCelula = pCelula->abaixo;
        if (pCelula->linha == linha){
            return pCelula->valor;
        }
    }while(pCelula->coluna != -1);

    return 0; //elemento nao encontrado
}

matriz_esparsa *transpostaMatriz(matriz_esparsa *mat) {
    if (!mat || mat->m <= 0 || mat->n <= 0) {
        return NULL;
    }

    // Cria uma nova matriz com dimensões transpostas
    matriz_esparsa *matT = cria_matriz_esparsa(mat->n, mat->m);

    tipo_celula *pLinha = mat->inicio->abaixo;

    for (int i = 1; i <= mat->m; i++) {
        tipo_celula *pCelula = pLinha->direita;
        
        while (pCelula != pLinha) {
            // Insere cada elemento da matriz original na posição transposta
            insere(matT, pCelula->coluna, pCelula->linha, pCelula->valor);
            pCelula = pCelula->direita;
        }
        pLinha = pLinha->abaixo;
    }

    return matT;
}



int main() {
    // Criação de duas matrizes esparsas para testes
    matriz_esparsa *matA = cria_matriz_esparsa(3, 3);
    matriz_esparsa *matB = cria_matriz_esparsa(3, 3);

    if (!matA || !matB) {
        printf("Erro ao criar matrizes.\n");
        return 1;
    }

    // Inserindo valores na matriz A
    insere(matA, 1, 1, 1);
    insere(matA, 1, 3, 2);
    insere(matA, 2, 2, 3);
    insere(matA, 3, 1, 4);

    // Inserindo valores na matriz B
    insere(matB, 1, 1, 5);
    insere(matB, 2, 2, 6);
    insere(matB, 3, 1, 7);
    insere(matB, 3, 3, 8);

    // Imprimindo matriz A
    printf("Matriz A:\n");
    imprimeMatriz(matA);

    // Imprimindo matriz B
    printf("\nMatriz B:\n");
    imprimeMatriz(matB);

    // Soma das matrizes A e B
    matriz_esparsa *matSoma = somaMatriz(matA, matB);
    if (matSoma) {
        printf("\nMatriz Soma (A + B):\n");
        imprimeMatriz(matSoma);
    } else {
        printf("\nErro: Matrizes A e B têm tamanhos diferentes.\n");
    }

    // Multiplicação das matrizes A e B
    matriz_esparsa *matMult = multiplicaMatriz(matA, matB);
    if (matMult) {
        printf("\nMatriz Multiplicação (A * B):\n");
        imprimeMatriz(matMult);
    } else {
        printf("\nErro: Número de colunas de A é diferente do número de linhas de B.\n");
    }

    // Transposta da matriz A
    matriz_esparsa *matTranspostaA = transpostaMatriz(matA);
    printf("\nMatriz Transposta de A:\n");
    imprimeMatriz(matTranspostaA);

    // Libera a memória alocada para todas as matrizes
    apagaMatriz(matA);
    apagaMatriz(matB);
    apagaMatriz(matSoma);
    apagaMatriz(matMult);
    apagaMatriz(matTranspostaA);

    return 0;
}
