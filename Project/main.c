#include <stdio.h>
#include <stdlib.h>
#include "time.h"

void printScene();

#define W 62

char planeSpace[W-8] = {""};
int leftSpace = 0;
int score = 0;
float planeSpeed = 0.05;
int height = 5;

int main(){
    time_t time;
    double timeTaken = 0;

    time = clock();

    while (planeSpace[W-9] == '\000')
    {
        timeTaken = (double)(clock() - time) / CLOCKS_PER_SEC;

        if(timeTaken > planeSpeed){
            time = clock();
            timeTaken = 0;
            printScene();
            planeSpace[leftSpace] = ' ';
            leftSpace++;
        }
    }
    

}

void printScene(){
    printf("\e[1;1H\e[2J");
    

    printf("%s", planeSpace);
    printf("(|__.-. |");

    int rightSpace = W - 9 - leftSpace;
    while(rightSpace > 0){
        printf(" ");
        rightSpace--;
    }

    printf("Score: %3d\n", score);

    printf("%s", planeSpace);
    printf("== ===_]+\n");
    printf("%s", planeSpace);
    printf("        |\n");

    int h = height;
    while (h > 0)
    {
        printf("\n");
        h--;
    }

    printf("<|>\n");
    printf(" |\n");
    printf(" *\n");
    
    printf(" _____        _____    _________________ ________    _________\n");
    printf("|o o o|_______|    |__|               | | # # # |____|o o o o|\n");
    printf("|o o o|* * *|: ::|. . |               |o| # # # |. . |o o o o|\n");
    printf("|o o o|* * *|::  |. . | []  []  []  []|o| # # # |. . |o o o o|\n");
    printf("|o o o|**** |:  :| . .| []  []  []    |o| # # # |. . |o o o o|\n");
    printf("|_[]__|__[]_|_||_|__< |____________;;_|_|___[]__|_.|_|__[]___|\n");
}