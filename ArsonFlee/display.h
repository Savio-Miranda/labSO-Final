#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <thread>
#include <semaphore.h>
#include <chrono>
#include <vector>
#include "gameSemaphore.h"
#include "room.h"
#include "player.h"
// extern GameSemaphore game_semaphore;


void startRoom() {
    for (int i = 0; i < SIZE_X; ++i) {
        for (int j = 0; j < SIZE_Y; ++j) {
            room[i][j] = '.'; // Preencher a matriz com um caractere
        }
    }
    // Coloca um 'X' em uma posição aleatória da última coluna
    int xPosition = std::rand() % SIZE_X;
    room[xPosition][SIZE_Y - 1] = 'X'; // A última coluna
}



void updateRoom() {
    while (true) {
        // game_semaphore.wait_semaphore(); // Espera para entrar na seção crítica
        system("clear");
        std::cout << "Matriz do Jogo:\n";
        for (int i = 0; i < SIZE_X; ++i) {
            for (int j = 0; j < SIZE_Y; ++j) {
                std::cout << room[i][j] << " "; // imprime cada elemento da sala
            }
            std::cout << std::endl; // nova linha após cada linha da matriz
        }
        std::cout << "Vidas: " << lifePoints << std::endl;
        // game_semaphore.release_semaphore(); // Libera a seção crítica
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}


#endif
