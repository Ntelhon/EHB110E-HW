// Furkan Karabulut
// Electronics and Communication Engineering
// EHB110E Project
// 08/01/2021
// https://github.com/Ntelhon

#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "conio.h"

void printScene(int planePos, int isBomb, int bombPosX, int bombPosY, char word[], int wordL, int wordHead);
void play();
void gameLose();
void pause();
void saveGame(  int bombPosX,       int bombPosY,   float bombSpeed,    int correctTyped,
                int falseTyped,     int isBomb,     int planePos,       int tempCorrectTyped,
                int tempFalseTyped, int wordHead,   int wordL,          int wordNum,        float timeTaken);
void saveScore(int correctTyped, int falseTyped, int wordNum);
void load();
void scoreBoard();
void settings();
int *findWordLength(char word[]);

// Variables for change the settings
#define W 62
#define H 17
#define MAXWL 15
#define PLANESPEED 0.1
#define BOMBSPEED 0.5
#define DIFF 0.90
#define DEBUG 1

// Struct for save the game
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

// Struct for save the score
struct scoreDataS
{
    char name[3];
    int score;
    int correctTyped;
    int falseTyped;
    int wordNum;
} scoreData;

// Game'S main variable, all game controlled by that variable.
int gameMode = -1;
// GameModes:   1 = Game Starting
//              2 = Game Playing
//              3 = Game Losed
//              4 = Game Paused
//              5 = Game Resumed
//              6 = Game Saving
//              7 = Game Loading


int score = 0;
int h = H;
float planeSpeed = PLANESPEED;
float bombSpeed = BOMBSPEED;
float diff = DIFF;
int debug = DEBUG;

int isDrop = 0;
int bombPlace = -1;

int main(){
    // randomaze seed
    srand(time(NULL));

    while(1){
        int input = -1;

        // Take user input, and go corresponding function
        printf("Welcome to the \"Amazing Plane Dropping Bombs Over to the Lego City\" game! by Furkan KARABULUT\n");
        printf("Please select what you want:\n");
        printf("1. New Game\n");
        printf("2. Load a Saved Game\n");
        printf("3. ScoreBoard\n");
        printf("4. How to Play\n");
        printf("5. Settings (COMING SOON)\n");
        printf("6. Credits\n");
        printf("7. Exit\n");

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
            if(gameMode == 7)
                play();
        }

        // Will print last 10 saved score
        if(input == 3)
            scoreBoard();

        if(input == 4){
            printf("\nWelcome to How to Play menu.\nGameplay is soo easy. Bombs are falling into city and you wanna deactivate the boms. Each bomb have a deactivate code near. You have to write that deactive code with pressing letters. But be carefull is you press false letter you will take minus score.\n\n\n");
        }

        if(input == 5){
            printf("\n\tIn Progress!\n\n\n\n");
            continue;
        }

        if(input == 6){
            printf("\nGame by Furkan Karabulut\ngithub.com/ntelhon\ne-mail: ntelgon@gmail.com\n\nSpecial Thanks for playtests to:\n\tHatice Rabia Enginoglu\n\tKubra Nur Temur\n\tKubra Karabulut\n\n\n");
        }

        if(input == 7){
            exit(0);
        }
    }

}

// Main fuction of program
void play(){

    // Initilaze all variables with 0.
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

    // If you plays a loaded game set variables as saved
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

    // Check the words.txt file and read first word
    FILE *fileP;
    if ((fileP = fopen("words.txt", "r")) == NULL){
        printf("Error! words.txt file CAN NOT FOUND!");
        system("Pause");
        exit(1);
    }

    fscanf(fileP, "%s", word);

    // Find lengt of first word
    int *wordData = findWordLength(word);
    wordL = wordData[0];
    wordHead = wordData[1];
    free(wordData);

    // If you plays a loaded game, set word valiable as saved
    if(gameMode == 7){
        for (int i = 0; i < wordNum; i++)
        {
            fscanf(fileP, "%s", word);
        }
        wordL = game.wordL;
        wordHead = game.wordHead;
    }

    // Start clocks
    time = clock();
    time1 = clock();
    time2 = clock();

    while (1)
    {
        // Set a random bomb falling place
        if(gameMode == 1){
            gameMode = 2;
            score = 0;
            bombSpeed = BOMBSPEED;
            bombPosX = rand() % (int)(W * 0.6);
        }

        // If you plays a loaded game, set variables as saved
        if(gameMode == 7){
            gameMode = 2;
            score = game.score;
            bombSpeed = game.bombSpeed;
            bombPosX = game.bombPosX;
        }

        timeTaken = (double)(clock() - time) / CLOCKS_PER_SEC;
        tempTimeTaken1 = (double)(clock() - time1) / CLOCKS_PER_SEC;
        tempTimeTaken2 = (double)(clock() - time2) / CLOCKS_PER_SEC;

        // If required time pass, move the plane a block
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

        // If required time pass, fall bomb a block.
        if((tempTimeTaken2 > bombSpeed) && isBomb){
            bombPosY++;

            // If bomb touch the city, call gameLose()
            if(bombPosY == h - 2){
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

                // If user select to saveScore, call saveScore function
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

        // Print the scene with corresponding variables
        printScene(planePos, isBomb, bombPosX , bombPosY, &word[wordHead], wordL, wordHead);

        // Check user inputs
        if(_kbhit()){
            char c = _getch();

            // If user push correct letter, modify word: book -> ook -> ok -> k -> NewWord
            if(c == word[wordHead]){
                tempCorrectTyped++;
                correctTyped++;
                wordHead++;
                wordL--;
            }

            // Check for ESC input, and call pause()
            else if(c == '\033'){
                gameMode = 4;
                pause();

                // If user want to save game, call saveGame() func
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

        // If all letters types correctly, update the word and score and difficulty
        if(wordL <= 0){
            score += tempCorrectTyped - tempFalseTyped;
            wordNum++;
            tempCorrectTyped = 0;
            tempFalseTyped = 0;
            isBomb = 0;
            bombPosY = 0;
            bombPosX = planePos;

            if(wordNum % 5 == 4)
                bombSpeed *= diff;

           fscanf(fileP, "%s", word);
           int *wordData = findWordLength(word);
            wordL = wordData[0];
            wordHead = wordData[1];
            free(wordData);
        }
        
    }

    fclose(fileP);
}

// Lose Screen
void gameLose(){
    printf("\n\n");
    printf("Select what you want:\n");
    printf("1. Play Again\n");
    printf("2. Save Last Game Scores\n");
    printf("3. Load a Saved Game\n");
    printf("4. Main Menu\n");
    printf("5. Exit\n");

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
        if(gameMode == 7)
            play();
    }

    if(input == 4){
        printf("\n\n\n\n\n");
        return;
    }

    if(input == 5){
        exit(0);
    }
}

// Pause Screen
void pause(){
    printf("\n\n");
    printf("\t\tGAME PAUSED!\n");
    printf("Select what you want:\n");
    printf("1. Resume\n");
    printf("2. Save Current Game\n");
    printf("3. Start a New Game\n");
    printf("4. Close That and Load a Saved Game\n");
    printf("5. Main Menu\n");
    printf("6. Exit\n");

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
        if(gameMode == 7)
            play();
    }

    if(input == 5){
        printf("\n\n\n\n\n");
        return;
    }

    if(input == 6){
        exit(0);
    }
}

// Load function for load a saved game
void load(){

    // Take save name from the user
    printf("\nPlease write the filename without their extention, example: \"mySave\":\n");
    char input[9];
    scanf("%s", input);

    input[5] = '.';
    input[6] = 'd';
    input[7] = 'a';
    input[8] = 't';
    input[9] = '\000';

    // If file can not find print an error message
    FILE *saveFileP;
    if ((saveFileP = fopen(input, "r")) == NULL){
        printf("Error! File can not found!\n\n\n\n");
        gameMode = -1;
        return;
    }

    // else update game datas with saved ones
    else{
        fread(&game, sizeof(struct gameS), 1, saveFileP);
        printf("Game Succesfully Loaded!");
        printf("WordNum: %d", game.wordNum);
    }
}

// Function for save the current game
void saveGame(int bombPosX, int bombPosY, float bombSpeed, int correctTyped,
            int falseTyped, int isBomb, int planePos, int tempCorrectTyped,
            int tempFalseTyped, int wordHead, int wordL, int wordNum, float timeTaken){

    // Save all data in a game struct
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

    // Take a save name from the user
    char input[10] = {""};
    printf("\nPlease write save name(only 5 letters): ");
    scanf("%s", input);
    input[5] = '.';
    input[6] = 'd';
    input[7] = 'a';
    input[8] = 't';
    input[9] = '\000';

    // Write all data to the save file
    FILE *saveFileP;
    saveFileP = fopen(input, "wb");
    if (saveFileP != NULL) {
        fwrite(&game, sizeof(struct gameS), 1, saveFileP);
        fclose(saveFileP);
    }

    printf("Game Saved as: %s\n", input);
}

// Function for save to current score
void saveScore(int correctTyped, int falseTyped, int wordNum){
    scoreData.score = score;
    scoreData.correctTyped = correctTyped;
    scoreData.falseTyped = falseTyped;
    scoreData.wordNum = wordNum;

    // Take username for save
    char input[4] = {""};
    printf("\nPlease write save name(only 3 letters): ");
    scanf("%s", input);

    scoreData.name[0] = input[0];
    scoreData.name[1] = input[1];
    scoreData.name[2] = input[2];

    // Save score in scores.dat file
    FILE *scoreFileP;
    scoreFileP = fopen("scores.dat", "ab");
    if (scoreFileP != NULL) {
        fwrite(&scoreData, sizeof(struct scoreDataS), 1, scoreFileP);
        fclose(scoreFileP);
    }

    printf("Score Saved in: scores.dat\n");
}

// Functiion for read the saved scores
void scoreBoard(){
    FILE *scoreFileP;
    struct scoreDataS arrScore[10];

    // Check scores.dat file is exist or not
    if ((scoreFileP = fopen("scores.dat", "rb")) == NULL){
        printf("Error! opening file\n\n\n");
    }

    // If exist print first 10 score from file
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

    printf("\n\n");
}

// This stage is on development, it is working but not compatible with LoadGame system. So now inactive.
void settings(){
    while (1)
    {
        printf("\nCurrent settings are:");
        printf("\n\t1. Height: %d", H);
        printf("\n\t2. MAX word length: %d", MAXWL);
        printf("\n\t3. Plane Speed: %f", PLANESPEED);
        printf("\n\t4. Bombs Starting Speed: %f", BOMBSPEED);
        printf("\n\t5. Difficulty: %f", DIFF);
        printf("\n\t6. Debug Mode: %d", DEBUG);
        printf("\n7. Main Menu\n");
        printf("\nSelect one which you wanna change: ");

        int input = -1;
        scanf("%d", &input);

        int intVal = 0;
        int floatVal = 0.0;

        printf("New Value: ");

        if((input == 1) || (input == 2) || (input == 6)){
            scanf("%d", &intVal);

            if(input == 1)
                h = intVal;
            if(input == 2)
                //maxWL = intVal;
            if(input == 6)
                debug = intVal;
        }

        else if((input == 3) || (input == 4) || (input == 5)){
            scanf("%f", &floatVal);

            if(input == 3)
                planeSpeed = floatVal;
            if(input == 4)
                bombSpeed = floatVal;
            if(input == 5)
                diff = floatVal;
        }
        else{
            break;
        }
    }

    printf("\n\n");
}

// Main print function
void printScene(int planePos, int isBomb, int bombPosX, int bombPosY, char word[], int wordL, int wordHead){
    
    // Firstly clear all screen
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

        for (int i = bombPosY; i < h-3; i++)
        {
            printf("\n");
        }
        
    }

    else{
        for (int i = 0; i < h; i++)
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
    if(debug) printf("Word data: %d / %d / %s\n", wordL, wordHead, word);
}

// Function for determine to lengt and head of word
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

        if(wordStart && (word[i] == '\000'))
            break;
            
    }

    // There i used a malloc data soo i have to free that after.
    int* data;
    data = malloc(sizeof(int) * 2);
    data[0] = length;
    data[1] = head;

    return data;
}