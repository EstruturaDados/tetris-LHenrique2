#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Definição da Estrutura de Dados (Struct) ---

// Define a estrutura de uma peça do Tetris Stack
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L', etc.)
    int id;    // Identificador único da peça (ordem de criação)
} Peca;

// --- Configuração da Fila Circular ---

// Define o tamanho máximo da fila de peças futuras
#define TAMANHO_MAXIMO 5 

// Array estático para armazenar as peças da fila
Peca filaDePecas[TAMANHO_MAXIMO]; 

// Variáveis para controlar a fila circular
int frente = 0; // Índice do elemento da frente (próxima peça a ser jogada)
int tras = 0;   // Índice do elemento de trás (próximo local para inserção)
int contador = 0; // Número atual de peças na fila
int proximo_id = 0; // Contador global para garantir IDs únicos para novas peças

// --- Funções de Manipulação da Fila ---

/**
 * @brief Verifica se a fila está vazia.
 * @return 1 se a fila estiver vazia, 0 caso contrário.
 */
int filaVazia() {
    return contador == 0;
}

/**
 * @brief Verifica se a fila está cheia.
 * @return 1 se a fila estiver cheia, 0 caso contrário.
 */
int filaCheia() {
    return contador == TAMANHO_MAXIMO;
}

/**
 * @brief Gera uma nova peça com um tipo aleatório e um ID único.
 * @return A nova peça gerada.
 */
Peca gerarPeca() {
    // Array de tipos de peças disponíveis
    char tipos[] = {'I', 'O', 'T', 'L'}; 
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Sorteia um tipo de peça
    char tipo_aleatorio = tipos[rand() % num_tipos];

    // Cria e retorna a nova peça
    Peca novaPeca;
    novaPeca.nome = tipo_aleatorio;
    novaPeca.id = proximo_id++; // Atribui o ID atual e incrementa para o próximo
    
    return novaPeca;
}

/**
 * @brief Insere uma nova peça no final da fila (enqueue).
 * @param novaPeca A peça a ser inserida.
 * @return 1 em caso de sucesso, 0 se a fila estiver cheia.
 */
int inserirPeca(Peca novaPeca) {
    if (filaCheia()) {
        printf("\n🚫 ERRO: A fila de peças futuras está cheia! Não é possível adicionar mais peças.\n");
        return 0; // Falha na inserção
    }

    // Adiciona a peça na posição 'tras'
    filaDePecas[tras] = novaPeca;
    
    // Move o índice 'tras' para a próxima posição (circular)
    tras = (tras + 1) % TAMANHO_MAXIMO;
    
    // Incrementa o contador de elementos
    contador++; 
    
    printf("\n✅ PEÇA INSERIDA: [%c %d] adicionada ao final da fila.\n", novaPeca.nome, novaPeca.id);
    return 1; // Sucesso
}

/**
 * @brief Remove a peça da frente da fila (dequeue), simulando 'jogar' a peça.
 * @param pecaJogada Ponteiro para a variável onde a peça removida será armazenada.
 * @return 1 em caso de sucesso, 0 se a fila estiver vazia.
 */
int jogarPeca(Peca *pecaJogada) {
    if (filaVazia()) {
        printf("\n🚫 ERRO: A fila de peças futuras está vazia! Nenhuma peça para jogar.\n");
        return 0; // Falha na remoção
    }

    // Pega a peça da frente
    *pecaJogada = filaDePecas[frente];
    
    // Move o índice 'frente' para a próxima posição (circular)
    frente = (frente + 1) % TAMANHO_MAXIMO;
    
    // Decrementa o contador de elementos
    contador--; 
    
    printf("\n🕹️ PEÇA JOGADA: [%c %d] removida da frente da fila.\n", pecaJogada->nome, pecaJogada->id);
    return 1; // Sucesso
}

/**
 * @brief Exibe o estado atual da fila de peças futuras.
 */
void exibirFila() {
    printf("\n--- 🧱 FILA DE PEÇAS FUTURAS (%d/%d) 🧱 ---\n", contador, TAMANHO_MAXIMO);

    if (filaVazia()) {
        printf("Fila de peças está vazia.\n");
        return;
    }

    printf("Fila: ");

    // Índice de início (frente)
    int i = frente; 
    
    // Itera pelo número de elementos atuais (contador)
    for (int count = 0; count < contador; count++) {
        // Exibe a peça atual
        printf("[%c %d] ", filaDePecas[i].nome, filaDePecas[i].id);
        
        // Move para a próxima posição de forma circular
        i = (i + 1) % TAMANHO_MAXIMO; 
    }
    printf("\n");
    // 
}

/**
 * @brief Inicializa a fila preenchendo-a com um número fixo de peças.
 * @param num_inicial O número de peças a serem geradas e inseridas.
 */
void inicializarFila(int num_inicial) {
    printf("🛠️ Inicializando a fila de peças com %d elementos iniciais...\n", num_inicial);
    for (int i = 0; i < num_inicial; i++) {
        Peca nova = gerarPeca();
        // A inserção inicial não verifica 'filaCheia' assumindo num_inicial <= TAMANHO_MAXIMO
        filaDePecas[tras] = nova;
        tras = (tras + 1) % TAMANHO_MAXIMO;
        contador++;
    }
    printf("✅ Inicialização concluída. ID da próxima peça: %d\n", proximo_id);
}

// --- Função Principal e Menu ---

int main() {
    // Inicializa a semente para a função rand() para gerar peças aleatórias
    srand(time(NULL)); 
    
    // Inicializa a fila com 5 peças
    inicializarFila(5); 

    int opcao;
    Peca pecaRemovida; // Variável para receber a peça que foi jogada

    do {
        // Exibe o estado atual da fila
        exibirFila(); 

        printf("\n--- ⚙️ Opções de Ação ---\n");
        printf("Código | Ação\n");
        printf("-------|------------------------------\n");
        printf("1      | Jogar peça (dequeue)\n");
        printf("2      | Inserir nova peça (enqueue)\n");
        printf("0      | Sair\n");
        printf("--------------------------------------\n");
        printf("Escolha uma opção: ");

        // Leitura da opção do jogador
        if (scanf("%d", &opcao) != 1) {
            // Limpa o buffer de entrada em caso de falha (não-número)
            while (getchar() != '\n');
            printf("\n🚫 ENTRADA INVÁLIDA: Por favor, digite um número.\n");
            opcao = -1; // Garante que o loop continue
            continue;
        }

        switch (opcao) {
            case 1:
                // Tenta jogar/remover a peça da frente
                jogarPeca(&pecaRemovida);
                break;
            case 2:
                // Gera uma nova peça e tenta inseri-la no final
                inserirPeca(gerarPeca());
                break;
            case 0:
                printf("\n👋 Saindo do Tetris Stack... Até logo!\n");
                break;
            default:
                printf("\n🚫 OPÇÃO INVÁLIDA: Por favor, escolha 1, 2 ou 0.\n");
                break;
        }

    } while (opcao != 0); // Continua até o jogador escolher 'Sair'

    return 0;
}
