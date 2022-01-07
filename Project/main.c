#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "conio.h"

void printScene(int planePos, int isBomb, int bombPosX, int bombPosY, char word[]);
void play();
void gameLose();
void pause();
void saveGame(  int bombPosX,       int bombPosY,   float bombSpeed,    int correctTyped,
                int falseTyped,     int isBomb,     int planePos,       int tempCorrectTyped,
                int tempFalseTyped, int wordHead,   int wordL,          int wordNum,        float timeTaken);
void saveScore(int correctTyped, int falseTyped, int wordNum);
void load();
void scoreBoard();
int *findWordLength(char word[]);

#define W 62
#define H 17
#define MAXWL 15
#define PLANESPEED 0.1
#define BOMBSPEED 0.5

struct gameS
{
    int isBomb;
    int planePos;
    int bombPosX;
    int bombPosY;

    float bombSpeed;
    float timeTaken;

    int wordL;
    int wordHead;
    int wordNum;

    int score;
    int correctTyped;
    int falseTyped;
    int tempCorrectTyped;
    int tempFalseTyped;
} game;

struct scoreDataS
{
    char name[3];
    int score;
    int correctTyped;
    int falseTyped;
    int wordNum;
} scoreData;

int gameMode = -1;
// GameModes:   1 = Game Starting
//              2 = Game Playing
//              3 = Game Losed
//              4 = Game Paused
//              5 = Game Resumed
//              6 = Game Saving
//              7 = Game Loading


int score = 0;
float planeSpeed = PLANESPEED;
float bombSpeed = BOMBSPEED;
float difficulty = 0.75;

int isDrop = 0;
int bombPlace = -1;

int main(){
    srand(time(NULL));

    while(1){
        int input = -1;

        printf("Welcome to the \"Amazing Plane Dropping Bombs Over to the Lego City\" game!\t by Furkan KARABULUT\n");
        printf("Please select what you want:\n");
        printf("1. New Game\n");
        printf("2. Load a Saved Game\n");
        printf("3. ScoreBoard\n");
        printf("4. Exit\n");

        scanf("%d", &input);

        if(input == 1)
        {
            gameMode = 1;
            play();
        }

        if(input == 2)
        {
            gameMode = 7;
            load();
            play();
        }

        if(input == 3){
            scoreBoard();
        }

        if(input == 4){
            exit(0);
        }
    }

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

    char word[MAXWL];
    int wordL = -1;
    int wordHead = 0;

    int correctTyped = 0;
    int falseTyped = 0;
    int tempCorrectTyped = 0;
    int tempFalseTyped = 0;
    int wordNum = 0;

    if(gameMode == 7){
        timeTaken = game.timeTaken;
        planePos = game.planePos;
        isBomb = game.isBomb;
        bombPosX = game.bombPosX;
        bombPosY = game.bombPosY;
        correctTyped = game.correctTyped;
        falseTyped = game.falseTyped;
        tempCorrectTyped = game.tempCorrectTyped;
        tempFalseTyped = game.tempFalseTyped;
        wordNum = game.wordNum;
    }

    FILE *fileP;
    if ((fileP = fopen("words.txt", "r")) == NULL){
        printf("Error! words.txt file CAN NOT FOUND!");
        system("Pause");
        exit(1);
    }

    fscanf(fileP, "%s", word);

    int *wordData = findWordLength(word);
    wordL = wordData[0];
    wordHead = wordData[1];
    free(wordData);

    if(gameMode == 7){
        for (int i = 0; i < wordNum; i++)
        {
            fscanf(fileP, "%s", word);
        }
        wordL = game.wordL;
        wordHead = game.wordHead;
    }

    time = clock();
    time1 = clock();
    time2 = clock();

    while (1)
    {
        if(gameMode == 1){
            gameMode = 2;
            score = 0;
            bombSpeed = BOMBSPEED;
            bombPosX = rand() % W;
        }

        if(gameMode == 7){
            gameMode = 2;
            score = game.score;
            bombSpeed = game.bombSpeed;
            bombPosX = game.bombPosX;
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

                printf("\n\n\t\tGAME OVER!\n\n\t\tYou LOSE!\n\n");
                printf("Numer of bombs deactivated:%3d\n", wordNum);
                printf("Correct typed letters:%4d\n", correctTyped);
                printf("Mistyped letters:%4d\n", falseTyped);
                printf("Total Score:%3d\n", score);

                gameMode = 3;
                gameLose();

                if(gameMode == 6){
                    saveScore(correctTyped, falseTyped, wordNum);

                    gameMode = 3;
                    gameLose();
                }
                break;
            }

            time2 = clock();
            tempTimeTaken2 = 0;
        }

        printScene(planePos, isBomb, bombPosX , bombPosY, &word[wordHead]);

        if(_kbhit()){
            char c = _getch();

            if(c == word[wordHead]){
                tempCorrectTyped++;
                correctTyped++;
                wordHead++;
                wordL--;
            }

            else if(c == '\033'){
                gameMode = 4;
                pause();
                if(gameMode == 6){
                    saveGame(bombPosX, bombPosY, bombSpeed, correctTyped, falseTyped, isBomb,
                            planePos, tempCorrectTyped, tempFalseTyped, wordHead, wordL, wordNum, timeTaken);

                    gameMode = 4;
                    pause();

                }
                else if(gameMode != 5)
                    break;

                else gameMode = 2;
            }
            else{
                tempFalseTyped++;
                falseTyped++;
            }
        }

        if(wordL <= 0){
            score += tempCorrectTyped - tempFalseTyped;
            wordNum++;
            tempCorrectTyped = 0;
            tempFalseTyped = 0;
            isBomb = 0;
            bombPosY = 0;
            bombPosX = planePos;

            if(wordNum % 5 == 4)
                bombSpeed *= difficulty;

           fscanf(fileP, "%s", word);
           int *wordData = findWordLength(word);
            wordL = wordData[0];
            wordHead = wordData[1];
            free(wordData);
        }
        
    }

    fclose(fileP);
}

void gameLose(){
    printf("\n\n");
    printf("Select what you want:\n");
    printf("1. Play Again\n");
    printf("2. Save Last Game Scores\n");
    printf("3. Load a Saved Game\n");
    printf("4. Exit\n");

    int input = -1;
    scanf("%d", &input);

    if(input == 1){
        gameMode = 1;
        play();
        return;
    }

    if(input == 2){
        gameMode = 6;
        return;
    }

    if(input == 3){
        gameMode = 7;
        load();
        play();
    }

    if(input == 4){
        exit(0);
    }
}

void pause(){
    printf("\n\n");
    printf("\t\tGAME PAUSED!\n");
    printf("Select what you want:\n");
    printf("1. Resume\n");
    printf("2. Save Current Game\n");
    printf("3. Start a New Game\n");
    printf("4. Close That and Load a Saved Game\n");
    printf("5. Exit\n");

    int input = -1;
    scanf("%d", &input);

    if(input == 1){
        gameMode = 5;
        return;
    }

    if(input == 2){
        gameMode = 6;
        return;
    }

    if(input == 3){
        gameMode = 1;
        play();
        return;
    }

    if(input == 4){
        gameMode = 7;
        load();
        play();
    }

    if(input == 5){
        exit(0);
    }
}

void load(){
    printf("Please write the filename without their extention, example: \"mySave\":\n");
    char input[9];
    scanf("%s", input);

    input[5] = '.';
    input[6] = 'd';
    input[7] = 'a';
    input[8] = 't';
    input[9] = '\000';

    FILE *saveFileP;
    if ((saveFileP = fopen(input, "r")) == NULL){
        printf("Error! File can not found!");
        system("Pause");
        exit(1);
    }
    else{
        fread(&game, sizeof(struct gameS), 1, saveFileP);
        printf("Game Succesfully Loaded!");
        printf("WordNum: %d", game.wordNum);
    }
}

void saveGame(int bombPosX, int bombPosY, float bombSpeed, int correctTyped,
            int falseTyped, int isBomb, int planePos, int tempCorrectTyped,
            int tempFalseTyped, int wordHead, int wordL, int wordNum, float timeTaken){

    game.bombPosX = bombPosX;
    game.bombPosY = bombPosY;
    game.bombSpeed = bombSpeed;
    game.correctTyped = correctTyped;
    game.falseTyped = falseTyped;
    game.isBomb = isBomb;
    game.planePos = planePos;
    game.score = score;
    game.tempCorrectTyped = tempCorrectTyped;
    game.tempFalseTyped = tempFalseTyped;
    game.wordHead = wordHead;
    game.wordL = wordL;
    game.wordNum = wordNum;
    game.timeTaken = timeTaken;

    char input[10] = {""};
    printf("\nPlease write save name(only 5 letters): ");
    scanf("%s", input);
    input[5] = '.';
    input[6] = 'd';
    input[7] = 'a';
    input[8] = 't';
    input[9] = '\000';

    FILE *saveFileP;
    saveFileP = fopen(input, "wb");
    if (saveFileP != NULL) {
        fwrite(&game, sizeof(struct gameS), 1, saveFileP);
        fclose(saveFileP);
    }

    printf("Game Saved as: %s\n", input);
}

void saveScore(int correctTyped, int falseTyped, int wordNum){
    scoreData.score = score;
    scoreData.correctTyped = correctTyped;
    scoreData.falseTyped = falseTyped;
    scoreData.wordNum = wordNum;

    char input[4] = {""};
    printf("\nPlease write save name(only 3 letters): ");
    scanf("%s", input);

    scoreData.name[0] = input[0];
    scoreData.name[1] = input[1];
    scoreData.name[2] = input[2];

    FILE *scoreFileP;
    scoreFileP = fopen("scores.dat", "ab");
    if (scoreFileP != NULL) {
        fwrite(&scoreData, sizeof(struct scoreDataS), 1, scoreFileP);
        fclose(scoreFileP);
    }

    printf("Score Saved in: scores.dat\n");
}

void scoreBoard(){
    FILE *scoreFileP;
    struct scoreDataS arrScore[10];

    if ((scoreFileP = fopen("scores.dat", "rb")) == NULL){
        printf("Error! opening file\n\n\n");
    // Program exits if the file pointer returns NULL.
    }
    else{
        fseek(scoreFileP, 0L, SEEK_END);       
        int size = (int)(ftell(scoreFileP) / sizeof(struct scoreDataS));
        fseek(scoreFileP, 0L, SEEK_SET);

        fread(&arrScore, sizeof(struct scoreDataS), size, scoreFileP);

        for (int i = 0; i < size; i++)
        {
            if(arrScore[i].name == NULL) break;
            printf("\nName: %s\n", arrScore[i].name);
            printf("Score: %d\n", arrScore[i].score);
            printf("Deactivated: %d\n", arrScore[i].wordNum);
            printf("Correct Typed: %d\n", arrScore[i].correctTyped);
            printf("False Typed: %d\n\n", arrScore[i].falseTyped);
        }
        
    }
}

void printScene(int planePos, int isBomb, int bombPosX, int bombPosY, char word[]){
    // printf("\e[1;1H\e[2J");
    system("cls");

    // set the place of plane
    int rightSpace = W - planePos;
    char planeSpace[W+1] = {""};

    for (int i = 0; i < planePos; i++)
    {
        planeSpace[i] = ' ';
    }
    

    // draw to plane
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

    // draw to bomb
    if(isBomb){
        for (int i = 0; i < bombPosY; i++)
            printf("\n");

        for (int i = 0; i < bombPosX; i++)
            printf(" ");

        printf("<|>\n");
        
        for (int i = 0; i < bombPosX; i++)
            printf(" ");

        printf(" | ");
        printf("%s\n", word);

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

    // draw to city
    printf(" _____        _____    _________________ ________    _________\n");
    printf("|o o o|_______|    |__|               | | # # # |____|o o o o|\n");
    printf("|o o o|* * *|: ::|. . |               |o| # # # |. . |o o o o|\n");
    printf("|o o o|* * *|::  |. . | []  []  []  []|o| # # # |. . |o o o o|\n");
    printf("|o o o|**** |:  :| . .| []  []  []    |o| # # # |. . |o o o o|\n");
    printf("|_[]__|__[]_|_||_|__< |____________;;_|_|___[]__|_.|_|__[]___|\n");
}

int *findWordLength(char word[]){
    int length = 0;
    int head = 0;
    int wordStart = 0;

    for (int i = 0; i < MAXWL; i++)
    {
        if((word[i] == '\000') && !wordStart)
            head++;

        if(word[i] != '\000'){
            length++;
            wordStart = 1;
        }
            
    }

    int* data;
    data = malloc(sizeof(int) * 2);
    data[0] = length;
    data[1] = head;

    return data;
}