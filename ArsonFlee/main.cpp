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
    std::thread displayThread(updateRoom);
    std::thread fireThread(burnGround, 2, std::ref(gameRunning));
    // Espera que o jogador termine
    playerThread.join();

    // Pausa de 2 segundos se o jogador morreu ou venceu
    if (lifePoints <= 0) {
        std::cout << "Você morreu! Fim do jogo." << std::endl;
    } else {
        std::cout << "Você chegou ao 'X'! Fim do jogo." << std::endl;
    }
    
    // Finaliza as outras threads
    gameRunning = false; // Define que o jogo não está mais rodando
    displayThread.join();
    fireThread.join();

    return 0;
}
