#include <stdio.h>

// --- Constantes Globais ---
#define TAMANHO_TABULEIRO 10  // Tabuleiro 10x10
#define TAMANHO_NAVIO 3       // Navios têm tamanho fixo de 3
#define VALOR_AGUA 0          // Representa a água
#define VALOR_NAVIO 3         // Representa uma parte do navio

// --- Códigos de Orientação ---
#define HORIZONTAL 0
#define VERTICAL 1
#define DIAGONAL_PRINCIPAL 2  // Linha e Coluna aumentam (i++, j++)
#define DIAGONAL_SECUNDARIA 3 // Linha aumenta, Coluna diminui (i++, j--)

// --- Definição das Coordenadas e Orientação (Nível Aventureiro) ---
// Navio 1: Horizontal (L1, C1)
#define L1 1
#define C1 1
#define ORIENTACAO_N1 HORIZONTAL

// Navio 2: Vertical (L5, C8)
#define L2 5
#define C2 8
#define ORIENTACAO_N2 VERTICAL

// Navio 3: Diagonal Principal (L1, C5) - Começa em (1, 5), termina em (3, 7)
#define L3 1
#define C3 5
#define ORIENTACAO_N3 DIAGONAL_PRINCIPAL

// Navio 4: Diagonal Secundária (L6, C3) - Começa em (6, 3), termina em (8, 1)
#define L4 6
#define C4 3
#define ORIENTACAO_N4 DIAGONAL_SECUNDARIA

// -----------------------------------------------------------------------------
// Função: inicializarTabuleiro
// Descrição: Preenche todas as posições da matriz do tabuleiro com o valor da água (0).
// -----------------------------------------------------------------------------
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = VALOR_AGUA;
        }
    }
}

// -----------------------------------------------------------------------------
// Função: calcularCoordenadas
// Descrição: Calcula as coordenadas da k-ésima parte do navio com base na orientação.
// -----------------------------------------------------------------------------
void calcularCoordenadas(int linhaInicial, int colunaInicial, int k, int orientacao, int *linha, int *coluna) {
    *linha = linhaInicial;
    *coluna = colunaInicial;

    switch (orientacao) {
        case HORIZONTAL:
            *coluna += k;
            break;
        case VERTICAL:
            *linha += k;
            break;
        case DIAGONAL_PRINCIPAL: // Linha++ e Coluna++
            *linha += k;
            *coluna += k;
            break;
        case DIAGONAL_SECUNDARIA: // Linha++ e Coluna--
            *linha += k;
            *coluna -= k;
            break;
    }
}

// -----------------------------------------------------------------------------
// Função: validarPosicionamento
// Descrição: Verifica se o navio cabe dentro dos limites e se há sobreposição.
// Retorno: 1 se a posição for válida, 0 caso contrário.
// -----------------------------------------------------------------------------
int validarPosicionamento(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linhaInicial, int colunaInicial, int tamanho, int orientacao) {
    int l, c;

    for (int k = 0; k < tamanho; k++) {
        // Calcula a coordenada da k-ésima parte do navio
        calcularCoordenadas(linhaInicial, colunaInicial, k, orientacao, &l, &c);

        // 1. Validação de Limites (Checa se L e C estão dentro de [0, 9])
        if (l < 0 || l >= TAMANHO_TABULEIRO || c < 0 || c >= TAMANHO_TABULEIRO) {
            printf("ERRO de Limite: Posição (L%d, C%d) fora do tabuleiro.\n", l, c);
            return 0; // Fora dos limites
        }

        // 2. Validação de Sobreposição (Checa se a posição já está ocupada por VALOR_NAVIO)
        if (tabuleiro[l][c] == VALOR_NAVIO) {
            printf("ERRO de Sobreposição: Posição (L%d, C%d) já ocupada.\n", l, c);
            return 0; // Sobreposição
        }
    }

    return 1; // Posição válida e sem sobreposição
}


// -----------------------------------------------------------------------------
// Função: posicionarNavio
// Descrição: Posiciona o navio no tabuleiro, apenas se a validação for bem-sucedida.
// Retorno: 1 se posicionado, 0 se falhou.
// -----------------------------------------------------------------------------
int posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linhaInicial, int colunaInicial, int tamanho, int orientacao) {
    
    // Validação completa de Limites e Sobreposição
    if (!validarPosicionamento(tabuleiro, linhaInicial, colunaInicial, tamanho, orientacao)) {
        printf("Falha no posicionamento do navio em (L%d, C%d).\n", linhaInicial, colunaInicial);
        return 0;
    }
    
    // Posicionamento Efetivo
    int l, c;
    for (int k = 0; k < tamanho; k++) {
        // Recalcula as coordenadas para posicionar
        calcularCoordenadas(linhaInicial, colunaInicial, k, orientacao, &l, &c);
        tabuleiro[l][c] = VALOR_NAVIO;
    }

    return 1;
}

// -----------------------------------------------------------------------------
// Função: exibirTabuleiro
// Descrição: Imprime o tabuleiro 10x10 no console de forma formatada.
// -----------------------------------------------------------------------------
void exibirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("\n\n=== Tabuleiro 10x10: Nível Aventureiro ===\n");
    
    // Imprime o cabeçalho das colunas (0 a 9)
    printf("   ");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf(" %d ", j);
    }
    printf("\n  --------------------------------\n"); 
    
    // Loop aninhado para percorrer e imprimir a matriz
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d |", i); // Imprime o índice da linha
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            // Imprime o valor: 0=Água, 3=Navio.
            printf(" %d ", tabuleiro[i][j]); 
        }
        printf("|\n");
    }
    printf("  --------------------------------\n");
    printf("\nLegenda: %d = Água, %d = Navio\n", VALOR_AGUA, VALOR_NAVIO);
}

// -----------------------------------------------------------------------------
// Função Principal (main)
// -----------------------------------------------------------------------------
int main() {
    // 1. Criar e Inicializar o Tabuleiro 10x10
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    inicializarTabuleiro(tabuleiro);
    
    // Variável para rastrear o sucesso
    int sucesso = 1;

    printf("--- Posicionamento dos Navios (Tamanho %d) ---\n", TAMANHO_NAVIO);

    // 2. Posicionar Navio 1 (Horizontal)
    printf("Navio 1 (H) em (L%d, C%d): ", L1, C1);
    if (!posicionarNavio(tabuleiro, L1, C1, TAMANHO_NAVIO, ORIENTACAO_N1)) sucesso = 0;
    else printf("OK.\n");

    // 3. Posicionar Navio 2 (Vertical)
    printf("Navio 2 (V) em (L%d, C%d): ", L2, C2);
    if (!posicionarNavio(tabuleiro, L2, C2, TAMANHO_NAVIO, ORIENTACAO_N2)) sucesso = 0;
    else printf("OK.\n");

    // 4. Posicionar Navio 3 (Diagonal Principal /)
    printf("Navio 3 (D /) em (L%d, C%d): ", L3, C3);
    if (!posicionarNavio(tabuleiro, L3, C3, TAMANHO_NAVIO, ORIENTACAO_N3)) sucesso = 0;
    else printf("OK.\n");

    // 5. Posicionar Navio 4 (Diagonal Secundária \)
    printf("Navio 4 (D \\) em (L%d, C%d): ", L4, C4);
    if (!posicionarNavio(tabuleiro, L4, C4, TAMANHO_NAVIO, ORIENTACAO_N4)) sucesso = 0;
    else printf("OK.\n");
    
    // 6. Exibir o Tabuleiro
    if (sucesso) {
        exibirTabuleiro(tabuleiro);
    } else {
        printf("\nO tabuleiro foi exibido, mas com erros de posicionamento (verifique logs acima).\n");
        exibirTabuleiro(tabuleiro); // Exibe mesmo com erro para mostrar o que foi posicionado
    }

    return 0;
}