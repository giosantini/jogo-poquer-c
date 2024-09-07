#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define NUM_CARTAS 52
#define NUM_VALORES 13
#define NUM_NAIPES 4
#define NUM_JOGADORES 2
#define CARTAS_POR_JOGADOR 5

// Representação de uma carta
typedef struct {
    char *valor;
    char *naipe;
} Carta;

// Representação de um jogador
typedef struct {
    Carta mao[CARTAS_POR_JOGADOR]; //Mão de cartas do jogador
    int id; // ID do jogador
    int saldo; // Saldo do jogador
    int aposta; // Aposta atual do jogador
} Jogador;

// Função para criar um baralho
void criarBaralho(Carta *baralho) {
    char *valores[NUM_VALORES] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    char *naipes[NUM_NAIPES] = {"Copas", "Ouros", "Espadas", "Paus"};

    int k = 0;
    for (int i = 0; i < NUM_VALORES; i++) {
        for (int j = 0; j < NUM_NAIPES; j++) {
            baralho[k].valor = valores[i];
            baralho[k].naipe = naipes[j];
            k++;
        }
    }
}

// Função para embaralhar o baralho
void embaralhar(Carta *baralho) {
    srand(time(NULL));
    for (int i = 0; i < NUM_CARTAS; i++) {
        int j = rand() % NUM_CARTAS;
        Carta temp = baralho[i];
        baralho[i] = baralho[j];
        baralho[j] = temp;
    }
}

// Função para distribuir cartas para os jogadores
void distribuirCartas(Carta *baralho, Jogador *jogadores) {
    int posicaoBaralho = 0; // Controle da posição atual do baralho

    for (int i = 0; i < NUM_JOGADORES; i++) {
        for (int j = 0; j < CARTAS_POR_JOGADOR; j++) {
            jogadores[i].mao[j] = baralho[posicaoBaralho++];
        }
    }
}

// Função para mostrar as cartas de cada jogador
void mostrarCartasJogador(Jogador *jogadores) {
    for (int i = 0; i < NUM_JOGADORES; i++) {
        printf("Jogador %d:\n", jogadores[i].id);
        for (int j = 0; j < CARTAS_POR_JOGADOR; j++) {
            printf("  %s de %s\n", jogadores[i].mao[j].valor, jogadores[i].mao[j].naipe);
        }
        printf("\n");
    }
}

// Função para contar o número de ocorrências de um valor na mão
int contarOcorrenciasValor(Jogador jogador, char *valor) {
    int contagem = 0;
    for (int i = 0; i < CARTAS_POR_JOGADOR; i++) {
        if (strcmp(jogador.mao[i].valor, valor) == 0) {
            contagem++;
        }
    }
    return contagem;
}

// Função para verificar se o jogador tem uma sequência (straight)
bool temSequencia(Jogador jogador) {
    int valores[CARTAS_POR_JOGADOR];

    // Converte as cartas para valores numéricos
    for (int i = 0; i < CARTAS_POR_JOGADOR; i++) {
        if (strcmp(jogador.mao[i].valor, "A") == 0) {
            valores[i] = 14; // Ás vale 14 em uma sequência
        } else if (strcmp(jogador.mao[i].valor, "K") == 0) {
            valores[i] = 13;
        } else if (strcmp(jogador.mao[i].valor, "Q") == 0) {
            valores[i] = 12;
        } else if (strcmp(jogador.mao[i].valor, "J") == 0) {
            valores[i] = 11;
        } else {
            valores[i] = atoi(jogador.mao[i].valor); // Converte strings numéricas
        }
    }

    // Ordena os valores para facilitar a verificação de sequência
    for (int i = 0; i < CARTAS_POR_JOGADOR - 1; i++) {
        for (int j = i + 1; j < CARTAS_POR_JOGADOR; j++) {
            if (valores[i] > valores[j]) {
                int temp = valores[i];
                valores[i] = valores[j];
                valores[j] = temp;
            }
        }
    }

    // Verifica se os valores são consecutivos
    for (int i = 0; i < CARTAS_POR_JOGADOR - 1; i++) {
        if (valores[i] + 1 != valores[i + 1]) {
            return false;
        }
    }
    return true;
}

// Função para verificar se o jogador tem um flush
bool temFlush(Jogador jogador) {
    for (int i = 1; i < CARTAS_POR_JOGADOR; i++) {
        if (strcmp(jogador.mao[i].naipe, jogador.mao[0].naipe) != 0) {
            return false;
        }
    }
    return true;
}

// Função para avaliar a mão do jogador (par, trinca, flush, sequência)
void avaliarMao(Jogador *jogador) {
    int par = 0;
    int trinca = 0;

    // Verifica as ocorrências de cada valor na mão
    for (int i = 0; i < CARTAS_POR_JOGADOR; i++) {
        char *valorAtual = jogador->mao[i].valor;
        int ocorrencias = contarOcorrenciasValor(*jogador, valorAtual);

        if (ocorrencias == 2) {
            par++;
        } else if (ocorrencias == 3) {
            trinca++;
        }
    }

    // Exibe a avaliação da mão
    if (temFlush(*jogador)) {
        printf("Jogador %d tem um flush!\n", jogador->id);
    } else if (temSequencia(*jogador)) {
        printf("Jogador %d tem uma sequência!\n", jogador->id);
    } else if (trinca > 0) {
        printf("Jogador %d tem uma trinca!\n", jogador->id);
    } else if (par > 0) {
        printf("Jogador %d tem um par!\n", jogador->id);
    } else {
        printf("Jogador %d não tem uma mão especial.\n", jogador->id);
    }
}

// Função para os jogadores fazerem suas apostas
void fazerApostas(Jogador *jogadores) {
    for (int i = 0; i < NUM_JOGADORES; i++) {
        printf("Jogador %d, seu saldo é de %d. Quanto deseja apostar? ", jogadores[i].id, jogadores[i].saldo);
        scanf("%d", &jogadores[i].aposta);

        // Verifica se a aposta é válida
        while (jogadores[i].aposta > jogadores[i].saldo || jogadores[i].aposta <= 0) {
            printf("Aposta inválida. Digite um valor entre 1 e %d: ", jogadores[i].saldo);
            scanf("%d", &jogadores[i].aposta);
        }

        // Subtrai a aposta do saldo
        jogadores[i].saldo -= jogadores[i].aposta;
    }
}

int main() {
    // Cria o baralho
    Carta baralho[NUM_CARTAS];
    criarBaralho(baralho);
    embaralhar(baralho);

    // Cria os jogadores
    Jogador jogadores[NUM_JOGADORES];
    for (int i = 0; i < NUM_JOGADORES; i++) {
        jogadores[i].id = i + 1; // Define o ID do jogador
        jogadores[i].saldo = 1000; // Cada jogador começa com 1000 de saldo
    }

    // Distribui as cartas
    distribuirCartas(baralho, jogadores);

    // Mostra as cartas de cada jogador
    mostrarCartasJogador(jogadores);

    // Os jogadores fazem suas apostas
    fazerApostas(jogadores);

    // Avaliar a mão de cada jogador
    for (int i = 0; i < NUM_JOGADORES; i++) {
        avaliarMao(&jogadores[i]);
    }

    return 0;
}
