#include <stdio.h>

// --- Constantes ---
#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define VALOR_AGUA 0
#define VALOR_NAVIO 3

// --- Coordenadas Iniciais ---
#define LINHA_N1 1 // Navio 1 Horizontal
#define COLUNA_N1 2

#define LINHA_N2 5 // Navio 2 Vertical
#define COLUNA_N2 7

/**
 * Inicializa todas as posições da matriz do tabuleiro com VALOR_AGUA (0).
 */
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = VALOR_AGUA;
        }
    }
}

/**
 * Verifica se um navio cabe dentro dos limites do tabuleiro.
 * Retorno: 1 se válido, 0 se inválido.
 */
int validarPosicionamento(int linhaInicial, int colunaInicial, int tamanho, int isHorizontal) {
    // Validação da coordenada inicial
    if (linhaInicial < 0 || linhaInicial >= TAMANHO_TABULEIRO ||
        colunaInicial < 0 || colunaInicial >= TAMANHO_TABULEIRO) {
        return 0;
    }

    // Validação de limite final
    if (isHorizontal) {
        if (colunaInicial + tamanho > TAMANHO_TABULEIRO) {
            return 0;
        }
    } else {
        if (linhaInicial + tamanho > TAMANHO_TABULEIRO) {
            return 0;
        }
    }

    return 1;
}

/**
 * Posiciona um navio no tabuleiro, checando limites e sobreposição.
 * Retorno: 1 se posicionado, 0 se falhou.
 */
int posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linhaInicial, int colunaInicial, int tamanho, int isHorizontal) {
    
    // 1. Validação de Limites
    if (!validarPosicionamento(linhaInicial, colunaInicial, tamanho, isHorizontal)) {
        printf("ERRO: Posicionamento (L%d, C%d) fora dos limites.\n", linhaInicial, colunaInicial);
        return 0;
    }

    // 2. Validação de Sobreposição
    for (int k = 0; k < tamanho; k++) {
        int linha = linhaInicial;
        int coluna = colunaInicial;

        if (isHorizontal) {
            coluna += k;
        } else {
            linha += k;
        }

        if (tabuleiro[linha][coluna] == VALOR_NAVIO) {
            printf("ERRO: Navio em (L%d, C%d) causaria sobreposição em (L%d, C%d).\n", linhaInicial, colunaInicial, linha, coluna);
            return 0;
        }
    }
    
    // 3. Posicionamento Efetivo
    for (int k = 0; k < tamanho; k++) {
        if (isHorizontal) {
            tabuleiro[linhaInicial][colunaInicial + k] = VALOR_NAVIO;
        } else {
            tabuleiro[linhaInicial + k][colunaInicial] = VALOR_NAVIO;
        }
    }

    return 1;
}

/**
 * Imprime o tabuleiro no console de forma formatada.
 */
void exibirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("\n\n=== Tabuleiro de Batalha Naval ===\n");
    
    // Imprime cabeçalho das colunas (índices 0 a 9)
    printf("   ");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf(" %d ", j);
    }
    printf("\n  --------------------------------\n");
    
    // Imprime linhas e conteúdo
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d |", i); // Índice da linha
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            printf(" %d ", tabuleiro[i][j]); 
        }
        printf("|\n");
    }
    printf("  --------------------------------\n");
    printf("\nLegenda: %d = Água, %d = Navio\n", VALOR_AGUA, VALOR_NAVIO);
}

// --- Função Principal ---
int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    
    inicializarTabuleiro(tabuleiro);
    
    // Vetores unidimensionais declarados, mas o valor (3) é usado diretamente na função posicionarNavio.
    // int navioHorizontal[TAMANHO_NAVIO] = {VALOR_NAVIO, VALOR_NAVIO, VALOR_NAVIO};
    
    // Posicionar Navio 1 (Horizontal)
    printf("Tentando posicionar Navio 1 (Horizontal) em (L%d, C%d)...\n", LINHA_N1, COLUNA_N1);
    if (posicionarNavio(tabuleiro, LINHA_N1, COLUNA_N1, TAMANHO_NAVIO, 1)) {
        printf("Navio 1 posicionado com sucesso.\n");
    } else {
        printf("Falha ao posicionar Navio 1.\n");
    }

    // Posicionar Navio 2 (Vertical)
    printf("Tentando posicionar Navio 2 (Vertical) em (L%d, C%d)...\n", LINHA_N2, COLUNA_N2);
    if (posicionarNavio(tabuleiro, LINHA_N2, COLUNA_N2, TAMANHO_NAVIO, 0)) {
        printf("Navio 2 posicionado com sucesso.\n");
    } else {
        printf("Falha ao posicionar Navio 2.\n");
    }

    exibirTabuleiro(tabuleiro);

    return 0;
}