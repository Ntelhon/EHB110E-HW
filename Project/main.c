#include <stdio.h>
#include <stdlib.h>
#include "time.h"

void printScene(int planePos, int isBomb, int bombPosX, int bombPosY);
void play();

#define W 62
#define H 15

struct gameS
{
    int score;
} game;

int gameMode = -1;
// GameModes:   1 = Staring game
//              2 = Game Playing


int score = 0;
float planeSpeed = 0.1;
float bombSpeed = 1;

int isDrop = 0;
int bombPlace = -1;

int main(){
    srand(time(NULL));
    int input = -1;

    printf("Welcome to the \"Fastest Plane Dropping Bombs Over to the Lego City\" game!\n");
    printf("Please select what you want:\n");
    printf("1. New Game\n");
    printf("2. Load a Saved Game\n");
    printf("3. Exit\n");

    scanf("%d", &input);

    if (input == 1)
    {
        gameMode = 1;
        play();
    }

    if (input == 2)
    {
        // Do something
    }
    

    // printScene(15, 1, 5 , 2);
}

void play(){
    time_t time;
    time_t time1;
    time_t time2;
    double timeTaken = 0;
    double tempTimeTaken1 = 0;
    double tempTimeTaken2 = 0;
    int planePos = 0;
    int isBomb = 0;
    int bombPosX = 0;
    int bombPosY = 0;

    time = clock();
    time1 = clock();
    time2 = clock();

    while (1)
    {
        if(gameMode == 1){
            gameMode = 2;
            bombPosX = rand() % W;
        }

        timeTaken = (double)(clock() - time) / CLOCKS_PER_SEC;
        tempTimeTaken1 = (double)(clock() - time1) / CLOCKS_PER_SEC;
        tempTimeTaken2 = (double)(clock() - time2) / CLOCKS_PER_SEC;

        if(tempTimeTaken1 > planeSpeed){
            if(planePos == bombPosX)
                isBomb = 1;

            if(planePos == W)
                planePos = 0;
            else
                planePos++;

            time1 = clock();
            tempTimeTaken1 = 0;
        }

        if((tempTimeTaken2 > bombSpeed) && isBomb){
            bombPosY++;

            if(bombPosY == H - 2){
                isBomb = 0;
                bombPosY = 0;
                bombPosX = rand() % W;
            }

            time2 = clock();
            tempTimeTaken2 = 0;
        }

        printScene(planePos, isBomb, bombPosX , bombPosY);
    }

}

void printScene(int planePos, int isBomb, int bombPosX, int bombPosY){
    // printf("\e[1;1H\e[2J");
    system("cls");
    score++;


    int rightSpace = W - planePos;
    char planeSpace[W+1] = {""};

    for (int i = 0; i < planePos; i++)
    {
        planeSpace[i] = ' ';
    }
    

    printf("%s", planeSpace);
    printf("(|__.-. |");

    while(rightSpace > 0){
        printf(" ");
        rightSpace--;
    }

    printf("Score: %3d\n", score);

    printf("%s", planeSpace);
    printf("== ===_]+\n");
    printf("%s", planeSpace);
    printf("        |\n");

    
    if(isBomb){
        for (int i = 0; i < bombPosY; i++)
            printf("\n");

        for (int i = 0; i < bombPosX; i++)
            printf(" ");

        printf("<|>\n");
        
        for (int i = 0; i < bombPosX; i++)
            printf(" ");

        printf(" |\n");

        for (int i = 0; i < bombPosX; i++)
            printf(" ");

        printf(" *\n");

        for (int i = bombPosY; i < H-3; i++)
        {
            printf("\n");
        }
        
    }

    else{
        for (int i = 0; i < H; i++)
        {
            printf("\n");
        }
        
    }

    
    printf(" _____        _____    _________________ ________    _________\n");
    printf("|o o o|_______|    |__|               | | # # # |____|o o o o|\n");
    printf("|o o o|* * *|: ::|. . |               |o| # # # |. . |o o o o|\n");
    printf("|o o o|* * *|::  |. . | []  []  []  []|o| # # # |. . |o o o o|\n");
    printf("|o o o|**** |:  :| . .| []  []  []    |o| # # # |. . |o o o o|\n");
    printf("|_[]__|__[]_|_||_|__< |____________;;_|_|___[]__|_.|_|__[]___|\n");
}