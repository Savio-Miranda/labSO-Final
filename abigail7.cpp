#include <iostream>
#include <thread>
#include <semaphore.h>
#include <vector>
#include <chrono>
#include <cstdlib>

const int SIZEX = 5;
const int SIZEY = 10;
char room[SIZEX][SIZEY];

sem_t semaphore; // Semáforo global
int lifePoints = 3;

struct FirePosition {
    int X;
    int Y;
};
FirePosition firePosition{-1, -1}; // Inicializa como fora da matriz

struct PlayerPosition {
    int X;
    int Y;
};

struct Player {
    PlayerPosition position;
};

void startRoom() {
    for (int i = 0; i < SIZEX; ++i) {
        for (int j = 0; j < SIZEY; ++j) {
            room[i][j] = '.'; // Preencher a matriz com um caractere
        }
    }
    // Coloca um 'X' em uma posição aleatória da última coluna
    int xPosition = std::rand() % SIZEX;
    room[xPosition][SIZEY - 1] = 'X'; // A última coluna
}

bool checkForDamage(Player &player) {
    return player.position.X == firePosition.X && player.position.Y == firePosition.Y;
}

void updateRoom() {
    while (true) {
        sem_wait(&semaphore); // Espera para entrar na seção crítica
        system("clear");
        std::cout << "Matriz do Jogo:\n";
        for (int i = 0; i < SIZEX; ++i) {
            for (int j = 0; j < SIZEY; ++j) {
                std::cout << room[i][j] << " "; // imprime cada elemento da sala
            }
            std::cout << std::endl; // nova linha após cada linha da matriz
        }
        std::cout << "Vidas: " << lifePoints << std::endl;
        sem_post(&semaphore); // Libera a seção crítica
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void burnGround(int time, bool &gameRunning) {
    while (gameRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(time));
        sem_wait(&semaphore); // Espera para entrar na seção crítica
        firePosition.X = std::rand() % SIZEX;
        firePosition.Y = std::rand() % SIZEY;
        room[firePosition.X][firePosition.Y] = 'F'; // Coloca o fogo na nova posição
        sem_post(&semaphore); // Libera a seção crítica
    }
}

void playerWalks(Player &player, char whichWay, bool &gameRunning) {
    sem_wait(&semaphore); // Espera para entrar na seção crítica
    PlayerPosition newPosition = player.position; // Posição temporária

    switch (whichWay) {
        case 'w':
            if (newPosition.X > 0) newPosition.X--;
            break;
        case 's':
            if (newPosition.X < SIZEX - 1) newPosition.X++;
            break;
        case 'a':
            if (newPosition.Y > 0) newPosition.Y--;
            break;
        case 'd':
            if (newPosition.Y < SIZEY - 1) newPosition.Y++;
            break;
        default:
            std::cout << "Direção inválida!" << std::endl;
            sem_post(&semaphore); // Libera antes de retornar
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

    sem_post(&semaphore); // Libera a seção crítica
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

void pauseGame(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

int main() {
    Player player;
    player.position = {2, 0};

    startRoom();

    sem_init(&semaphore, 0, 1); // Inicializa o semáforo
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
    pauseGame(2); // Pausa de 2 segundos antes de encerrar

    // Finaliza as outras threads
    gameRunning = false; // Define que o jogo não está mais rodando
    displayThread.join();
    fireThread.join();

    sem_destroy(&semaphore); // Destrói o semáforo

    return 0;
}
