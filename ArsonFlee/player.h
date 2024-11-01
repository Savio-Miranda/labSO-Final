#ifndef PLAYERTESTE_H
#define PLAYERTESTE_H

#include <iostream>
#include <vector>

#include "gameSemaphore.h"
#include "room.h"

using namespace std;

extern GameSemaphore game_semaphore;


int lifePoints = 3;

struct PlayerPosition {
    int X;
    int Y;
};

struct Player {
    PlayerPosition position;
};

bool checkForDamage(Player &player) {
    return room[player.position.X][player.position.Y] == 'F';
    //return player.position.X == firePosition.X && player.position.Y == firePosition.Y;
}


void playerWalks(Player &player, char whichWay, bool &gameRunning) {
    game_semaphore.wait_semaphore(); // Espera para entrar na seção crítica.
    PlayerPosition newPosition = player.position; // Posição temporária

    switch (whichWay) {
        case 'w':
            if (newPosition.X > 0) newPosition.X--;
            break;
        case 's':
            if (newPosition.X < SIZE_X - 1) newPosition.X++;
            break;
        case 'a':
            if (newPosition.Y > 0) newPosition.Y--;
            break;
        case 'd':
            if (newPosition.Y < SIZE_Y - 1) newPosition.Y++;
            break;
        default:
            std::cout << "Direção inválida!" << std::endl;
            game_semaphore.release_semaphore(); // Libera antes de retornar
            return;
    }

    // Verifica se o novo movimento vai para uma posição com fogo
    Player tempPlayer = player; // Cria uma cópia do jogador
    tempPlayer.position = newPosition; // Atualiza a posição temporária
    if (checkForDamage(tempPlayer)) {
        lifePoints -= 1;
        std::cout << "Você tomou dano! Vidas restantes: " << lifePoints << std::endl;
    } else {
        // Atualiza a matriz se não tomou dano
        room[player.position.X][player.position.Y] = '.';
        player.position = newPosition; // Atualiza a posição do jogador
        room[player.position.X][player.position.Y] = 'O'; // Marca a nova posição
    }

    // Verifica se o jogador alcançou o 'X'
    if (room[player.position.X][player.position.Y] == 'X') {
        std::cout << "Você chegou ao 'X'! Fim do jogo." << std::endl;
        gameRunning = false; // Define que o jogo não está mais rodando
    }

    game_semaphore.release_semaphore(); // Libera a seção crítica
}

void playerLives(Player &player, bool &gameRunning) {
    while (gameRunning) {
        char direction;
        std::cin >> direction;
        playerWalks(player, direction, gameRunning);

        if (lifePoints <= 0) {
            std::cout << "Você morreu! Fim do jogo." << std::endl;
            gameRunning = false; // Define que o jogo não está mais rodando
        }
    }
}

#endif