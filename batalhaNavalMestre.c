#include <stdio.h>
#include <stdlib.h> // Para a função abs()

// --- Constantes Globais ---
#define TAMANHO_TABULEIRO 10 
#define TAMANHO_NAVIO 3       
#define RAIO_HABILIDADE 2     // Raio do efeito das habilidades (resulta em matriz de efeito 5x5)
#define VALOR_AGUA 0          
#define VALOR_NAVIO 3         
#define VALOR_HABILIDADE 5    // Valor para áreas afetadas pela habilidade

// --- Códigos de Orientação para Navios ---
#define HORIZONTAL 0
#define VERTICAL 1
#define DIAGONAL_PRINCIPAL 2
#define DIAGONAL_SECUNDARIA 3

// --- Códigos de Tipo de Habilidade ---
#define HABILIDADE_CRUZ 0
#define HABILIDADE_CONE 1
#define HABILIDADE_OCTAEDRO 2

// --- Definição das Habilidades no Tabuleiro (Centro) ---
#define CENTRO_L_CRUZ 2
#define CENTRO_C_CRUZ 2

#define CENTRO_L_CONE 5
#define CENTRO_C_CONE 5

#define CENTRO_L_OCTAEDRO 8
#define CENTRO_C_OCTAEDRO 8

// --- Protótipos das Funções ---
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void calcularCoordenadas(int linhaInicial, int colunaInicial, int k, int orientacao, int *linha, int *coluna);
int validarPosicionamento(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linhaInicial, int colunaInicial, int tamanho, int orientacao);
int posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linhaInicial, int colunaInicial, int tamanho, int orientacao);
// PROTÓTIPO CORRIGIDO: O tamanho deve vir antes da matriz VLA
void criarMatrizEfeito(int tamanho, int matrizEfeito[tamanho][tamanho], int tipoHabilidade);
void aplicarHabilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int centroL, int centroC, int tipoHabilidade);
void exibirTabuleiroComLegenda(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);

// -----------------------------------------------------------------------------
// FUNÇÕES DE UTILIDADE (Níveis Novato/Aventureiro)
// -----------------------------------------------------------------------------

void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = VALOR_AGUA;
        }
    }
}

void calcularCoordenadas(int linhaInicial, int colunaInicial, int k, int orientacao, int *linha, int *coluna) {
    *linha = linhaInicial;
    *coluna = colunaInicial;
    switch (orientacao) {
        case HORIZONTAL: *coluna += k; break;
        case VERTICAL: *linha += k; break;
        case DIAGONAL_PRINCIPAL: *linha += k; *coluna += k; break;
        case DIAGONAL_SECUNDARIA: *linha += k; *coluna -= k; break;
    }
}

int validarPosicionamento(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linhaInicial, int colunaInicial, int tamanho, int orientacao) {
    int l, c;
    for (int k = 0; k < tamanho; k++) {
        calcularCoordenadas(linhaInicial, colunaInicial, k, orientacao, &l, &c);
        if (l < 0 || l >= TAMANHO_TABULEIRO || c < 0 || c >= TAMANHO_TABULEIRO) return 0;
        if (tabuleiro[l][c] == VALOR_NAVIO) return 0;
    }
    return 1;
}

int posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linhaInicial, int colunaInicial, int tamanho, int orientacao) {
    if (!validarPosicionamento(tabuleiro, linhaInicial, colunaInicial, tamanho, orientacao)) return 0;
    int l, c;
    for (int k = 0; k < tamanho; k++) {
        calcularCoordenadas(linhaInicial, colunaInicial, k, orientacao, &l, &c);
        tabuleiro[l][c] = VALOR_NAVIO;
    }
    return 1;
}

// -----------------------------------------------------------------------------
// FUNÇÕES DO NÍVEL MESTRE: Criação e Aplicação de Habilidades
// -----------------------------------------------------------------------------

/**
 * Cria a matriz de efeito da habilidade (tamanho = 2*RAIO + 1).
 * Usa a Distância de Manhattan para definir as formas (Cruz, Cone, Octaedro).
 */
void criarMatrizEfeito(int tamanho, int matrizEfeito[tamanho][tamanho], int tipoHabilidade) {
    int centro = RAIO_HABILIDADE; // Centro da matriz de habilidade 
    
    // Loop aninhado para preencher a matriz de efeito (0 ou 1)
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            int dist_l = abs(i - centro);
            int dist_c = abs(j - centro);
            
            // Lógica condicional para construir a forma
            switch (tipoHabilidade) {
                case HABILIDADE_CRUZ:
                    // Se estiver na linha central OU coluna central (dentro do raio)
                    if (dist_l == 0 || dist_c == 0) {
                        matrizEfeito[i][j] = 1;
                    } else {
                        matrizEfeito[i][j] = 0;
                    }
                    break;
                case HABILIDADE_CONE:
                    // Forma de losango (dist_l + dist_c <= RAIO) limitada ao semicírculo inferior (i >= centro)
                    if ((dist_l + dist_c <= RAIO_HABILIDADE) && (i >= centro)) {
                        matrizEfeito[i][j] = 1;
                    } else {
                        matrizEfeito[i][j] = 0;
                    }
                    break;
                case HABILIDADE_OCTAEDRO:
                    // Losango/Octaedro: Distância de Manhattan menor ou igual ao raio
                    if (dist_l + dist_c <= RAIO_HABILIDADE) {
                        matrizEfeito[i][j] = 1;
                    } else {
                        matrizEfeito[i][j] = 0;
                    }
                    break;
            }
        }
    }
}

/**
 * Aplica a matriz de efeito sobre o tabuleiro principal, marcando as áreas afetadas com VALOR_HABILIDADE (5).
 */
void aplicarHabilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int centroL, int centroC, int tipoHabilidade) {
    int tamanho_efeito = 2 * RAIO_HABILIDADE + 1;
    // Declaração do array de tamanho variável (VLA)
    int matrizEfeito[tamanho_efeito][tamanho_efeito];
    
    // 1. Cria a matriz de efeito (0 ou 1) - CORRIGIDO: Passando o tamanho como primeiro argumento
    criarMatrizEfeito(tamanho_efeito, matrizEfeito, tipoHabilidade);

    // 2. Itera sobre a matriz de efeito e sobrepõe ao tabuleiro
    for (int el = 0; el < tamanho_efeito; el++) {       
        for (int ec = 0; ec < tamanho_efeito; ec++) {   
            
            // Calcula a coordenada real no tabuleiro 10x10 (tl: linha do tabuleiro, tc: coluna do tabuleiro)
            int tl = centroL + (el - RAIO_HABILIDADE); 
            int tc = centroC + (ec - RAIO_HABILIDADE);
            
            // Condicional para garantir que o efeito está dentro dos limites do Tabuleiro
            if (tl >= 0 && tl < TAMANHO_TABULEIRO && tc >= 0 && tc < TAMANHO_TABULEIRO) {
                
                // Se a posição da matriz de efeito deve ser ativada (valor 1)
                if (matrizEfeito[el][ec] == 1) {
                    // Marca a posição no tabuleiro com 5
                    tabuleiro[tl][tc] = VALOR_HABILIDADE;
                }
            }
        }
    }
}

/**
 * Imprime o tabuleiro com a legenda clara para 0, 3 e 5.
 */
void exibirTabuleiroComLegenda(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("\n   ");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf(" %d ", j);
    }
    printf("\n  --------------------------------\n"); 
    
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d |", i);
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            printf(" %d ", tabuleiro[i][j]); 
        }
        printf("|\n");
    }
    printf("  --------------------------------\n");
    printf("\nLegenda: %d = Água, %d = Navio, %d = Área de Habilidade\n", VALOR_AGUA, VALOR_NAVIO, VALOR_HABILIDADE);
}

// -----------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// -----------------------------------------------------------------------------
int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    
    // --- Posicionamento dos Navios (Base do Nível Aventureiro) ---
    inicializarTabuleiro(tabuleiro);
    posicionarNavio(tabuleiro, 1, 1, TAMANHO_NAVIO, HORIZONTAL);
    posicionarNavio(tabuleiro, 5, 8, TAMANHO_NAVIO, VERTICAL);
    posicionarNavio(tabuleiro, 1, 5, TAMANHO_NAVIO, DIAGONAL_PRINCIPAL);
    posicionarNavio(tabuleiro, 6, 3, TAMANHO_NAVIO, DIAGONAL_SECUNDARIA);

    printf("============================================\n");
    printf("         NÍVEL MESTRE: HABILIDADES\n");
    printf("============================================\n");

    // --- APLICAÇÃO DA HABILIDADE 1: CRUZ ---
    printf("\n--- HABILIDADE: CRUZ (Centro L%d, C%d) ---\n", CENTRO_L_CRUZ, CENTRO_C_CRUZ);
    aplicarHabilidade(tabuleiro, CENTRO_L_CRUZ, CENTRO_C_CRUZ, HABILIDADE_CRUZ);
    exibirTabuleiroComLegenda(tabuleiro);
    
    // Reinicia o tabuleiro para a próxima habilidade (mantendo os navios)
    inicializarTabuleiro(tabuleiro);
    posicionarNavio(tabuleiro, 1, 1, TAMANHO_NAVIO, HORIZONTAL);
    posicionarNavio(tabuleiro, 5, 8, TAMANHO_NAVIO, VERTICAL);
    posicionarNavio(tabuleiro, 1, 5, TAMANHO_NAVIO, DIAGONAL_PRINCIPAL);
    posicionarNavio(tabuleiro, 6, 3, TAMANHO_NAVIO, DIAGONAL_SECUNDARIA);

    // --- APLICAÇÃO DA HABILIDADE 2: CONE ---
    printf("\n--- HABILIDADE: CONE (Centro L%d, C%d) ---\n", CENTRO_L_CONE, CENTRO_C_CONE);
    aplicarHabilidade(tabuleiro, CENTRO_L_CONE, CENTRO_C_CONE, HABILIDADE_CONE);
    exibirTabuleiroComLegenda(tabuleiro);
    
    // Reinicia o tabuleiro
    inicializarTabuleiro(tabuleiro);
    posicionarNavio(tabuleiro, 1, 1, TAMANHO_NAVIO, HORIZONTAL);
    posicionarNavio(tabuleiro, 5, 8, TAMANHO_NAVIO, VERTICAL);
    posicionarNavio(tabuleiro, 1, 5, TAMANHO_NAVIO, DIAGONAL_PRINCIPAL);
    posicionarNavio(tabuleiro, 6, 3, TAMANHO_NAVIO, DIAGONAL_SECUNDARIA);

    // --- APLICAÇÃO DA HABILIDADE 3: OCTAEDRO (LOSANGO) ---
    printf("\n--- HABILIDADE: OCTAEDRO/LOSANGO (Centro L%d, C%d) ---\n", CENTRO_L_OCTAEDRO, CENTRO_C_OCTAEDRO);
    aplicarHabilidade(tabuleiro, CENTRO_L_OCTAEDRO, CENTRO_C_OCTAEDRO, HABILIDADE_OCTAEDRO);
    exibirTabuleiroComLegenda(tabuleiro);

    return 0;
}