#include <iostream>
#include <thread>
#include <semaphore.h>
#include <vector>
#include <chrono>
#include <cstdlib>
#include "display.h"
#include "player.h"
#include "room.h"

GameSemaphore game_semaphore(1);


int main() {
    Player player;
    player.position = {2, 0};

    startRoom();

    bool gameRunning = true; // Variável para controlar o estado do jogo

    std::thread playerThread(playerLives, std::ref(player), std::ref(gameRunning));
    std::thread fireThread(burnGround, 2, std::ref(gameRunning));
    std::thread displayThread(updateRoom, std::ref(gameRunning));
    
    playerThread.join();
    fireThread.join();
    displayThread.join();
    game_semaphore.destroy_semaphore();
    gameRunning = false;

    return 0;
}
