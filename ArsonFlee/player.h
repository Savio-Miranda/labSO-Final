#ifndef PLAYERTESTE_H
#define PLAYERTESTE_H

#include <iostream>
#include <vector>
#include <chrono>

#include "gameSemaphore.h"
#include "room.h"

using namespace std;

extern GameSemaphore game_semaphore;

int lifePoints = 3;
std::string endGameMessage;

struct PlayerPosition {
    int X;
    int Y;
};

struct Player {
    PlayerPosition position;
};

bool checkForDamage(Player &player){
    return room[player.position.X][player.position.Y] == 'F';
}

bool checkForFinish(Player &player){
    return room[player.position.X][player.position.Y] == 'X';
}

void playerWalks(Player &player, char whichWay, bool &gameRunning) {
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
            return;
    }

    // Verifica se o novo movimento vai para uma posição com fogo
    Player tempPlayer = player; // Cria uma cópia do jogador
    tempPlayer.position = newPosition; // Atualiza a posição temporária
    if (checkForDamage(tempPlayer)) {
        lifePoints -= 1;
        std::cout << "Você tomou dano! Vidas restantes: " << lifePoints << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));

    
    } else if (checkForFinish(tempPlayer)) {
        std::cout << "Você chegou ao 'X'! Fim do jogo." << std::endl;
        endGameMessage = "Você chegou na saída! Parabéns!\n";
        gameRunning = false; // Define que o jogo não está mais rodando
    } else {
        // Atualiza a matriz se não tomou dano
        room[player.position.X][player.position.Y] = '.';
        player.position = newPosition; // Atualiza a posição do jogador
        room[player.position.X][player.position.Y] = 'O'; // Marca a nova posição
    }
    
}

void playerLives(Player &player, bool &gameRunning) {
    while (gameRunning) {
        char direction;
        std::cin >> direction;
        game_semaphore.wait_semaphore(); // Espera para entrar na seção crítica.
        {
            playerWalks(player, direction, gameRunning);

            if (lifePoints <= 0) {
                endGameMessage = "Você morreu! Fim do jogo.\n";
                gameRunning = false; // Define que o jogo não está mais rodando
            }
        }
        game_semaphore.release_semaphore(); // Libera a seção crítica
    }
}

#endif