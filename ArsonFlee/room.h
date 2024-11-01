#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <thread>
#include <semaphore.h>
#include <chrono>
#include <vector>

const int SIZE_X = 5;
const int SIZE_Y = 10;
char room[SIZE_X][SIZE_Y];

struct FirePosition {
    int X;
    int Y;
};

FirePosition firePosition{-1, -1}; // Inicializa como fora da matriz

void setFire(char (&room)[SIZE_X][SIZE_Y]){
    firePosition.X = std::rand() % SIZE_X;
    firePosition.Y = std::rand() % SIZE_Y;
    room[firePosition.X][firePosition.Y] = 'F'; // Coloca o fogo na nova posição
}

void burnGround(int time, bool &gameRunning) {
    while (gameRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(time));
        {
            setFire(room);
        }
    }
}
#endif
