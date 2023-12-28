#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define ROWS 17
#define COLS 26

#define BASE_GONDOR "GGGG"
#define BASE_MORDOR "MMMM"
#define MINES_SHIRE "SS"
#define MINES_EREBOR "EE"
#define MINES_EREBOR "EE"
#define BARRACKS_ROHAN "RR"
#define BARRACKS_ISENGARD "II"
#define STABLES_LOTHLORIEN "LL"
#define STABLES_MIRKWOOD "MK"
#define MINES_EREBOR "EE"
#define ARMOURY_GONDORIANFORGE "GF"
#define ARMOURY_DARKFORGE "DF"

#define GONDOR_INFANTARY "G"
#define GONDOR_CAVALRY "SK"
#define GONDOR_ARTILLERY "T"

#define MORDOR_INFANTARY "OW"
#define MORDOR_CAVALRY "W"
#define MORDOR_ARTILLERY "ST"

#define INFANTARY_ATTACK 5
#define CAVALRY_ATTACK 7
#define ARTILLERY_ATTACK 10

#define HEALTH_BASE 100
#define HEALTH_MINE 50
#define HEALTH_BARRACK_STABLE_ARMOURY 70
#define HEALTH_INFANTARY 30
#define HEALTH_CAVALRY 40
#define HEALTH_ARTILLERY 20

#define MAX_PLAYERS 2
#define MAX_COINS 100

struct Cell {
    struct GameEntity* gameEntity;
};

struct GameEntity {
    int health;
    char symbol[5];
    int cost;
};

struct Building {
    struct GameEntity gameEntity;
};

struct CombatCharacter {
    struct GameEntity gameEntity;
};

struct Player {
    enum PlayerType playerType;
    int coins;
};

enum PlayerType {
    Gondor,
    Mordor
};

enum GameEntityType {
    Base,
    Mine,
    Barracks,
    Stable,
    Armoury,
    Infantary,
    Cavalry,
    Artillery
};

void displayMainMenu();
void startNewGame();
void initializeGrid(struct Cell grid[ROWS][COLS]);
void printGrid(struct Cell grid[ROWS][COLS]);
void initializePlayers(struct Player players[MAX_PLAYERS]);
void chooseSide(struct Player players[MAX_PLAYERS]);
void playGame(struct Cell grid[ROWS][COLS], struct Player players[MAX_PLAYERS]);
void placeEntity(struct Cell grid[ROWS][COLS], int row, int column, GameEntity* gameEntity, Player* player);
void moveBuild(struct Cell grid[ROWS][COLS], Cell originCell, int destRow, int destColumn);
int letterToIndex(char letter);
void printBuildOptions();
void printMilitaryOptions();
GameEntity* gameEntityToPrint(struct Player player, enum GameEntityType entityType);
GameEntityType selectEntitytype(int entityType, int userSelection);
GameEntityType printEntitiesAndGetType(struct Cell grid[ROWS][COLS]);

int main() {

    int choice;
    do {
        displayMainMenu();

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            startNewGame();
            system("cls");
            break;
        case 2:
            printf("Load Game option selected (Not implemented yet).\n");
            break;
        case 3:
            printf("Settings option selected (Not implemented yet).\n");
            break;
        case 4:
            printf("Exiting the game. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
            break;
        }
    } while (choice != 4);

    return 0;
}

void displayMainMenu() {
    printf("\n*** Lord of the Rings: Strategy Game ***\n");
    printf("1. Start New Game\n");
    printf("2. Load Game\n");
    printf("3. Settings\n");
    printf("4. Exit\n");
}

void startNewGame() {
    struct Cell grid[ROWS][COLS];
    struct Player players[MAX_PLAYERS];

    initializeGrid(grid);
    initializePlayers(players);

    system("cls");
    chooseSide(players);

    system("cls");
    playGame(grid, players);
}

void initializeGrid(struct Cell grid[ROWS][COLS]) {

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            grid[i][j].gameEntity = NULL; 
        }
    }
}

void printGrid(struct Cell grid[ROWS][COLS]) {
    system("cls");
    printf("   "); 

    for (char c = 'A'; c <= 'Z'; ++c) {
        printf(" %c ", c);
    }
    printf("\n");

    for (int i = 0; i < ROWS; ++i) {
        printf("%2d ", i); 

        for (int j = 0; j < COLS; ++j) {
            if (grid[i][j].gameEntity != NULL) {
                /*printf("[X]");*/
                printf("[%d: %s]", grid[i][j].gameEntity->health, grid[i][j].gameEntity->symbol);
            }
            else {
                printf("[ ]"); 
            }
        }
        printf("\n");
    }   
}

void initializePlayers(struct Player players[MAX_PLAYERS]) {

    for (int i = 0; i < MAX_PLAYERS; ++i) {
        players[i].playerType = Gondor;
        players[i].coins = MAX_COINS;
    }
}

void chooseSide(struct Player players[MAX_PLAYERS]) {
    printf("Choose your side:\n");
    printf("1 - Gondor/Rivendell\n");
    printf("2 - Mordor\n");

    int choice;
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        printf("Player %d, choose your side: ", i + 1);
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            players[i].playerType = Gondor;
            break;
        case 2:
            players[i].playerType = Mordor;
            break;
        default:
            printf("Invalid choice. Default side set for Player %d.\n", i + 1);
            break;
        }
    }
}

void playGame(struct Cell grid[ROWS][COLS], struct Player players[MAX_PLAYERS]) {
   
    int playerIndex = 0;
    
    do {
    printGrid(grid);   
    
    GameEntityType entityType = printEntitiesAndGetType(grid);
    GameEntity* gameEntityForBoard = gameEntityToPrint(players[playerIndex], entityType);

    system("clear");
    printGrid(grid);
    printf("Player coins: %d", players[playerIndex].coins);
    printf("\nSelect cell in the grid: \n");

    int rowNumber;
    char columnChar;
    int columnNumber;

    printf("Row: ");
    scanf("%d", &rowNumber);

    printf("Column: ");
    scanf(" %c", &columnChar);

    

    columnNumber = letterToIndex(columnChar);

    placeEntity(grid, rowNumber, columnNumber, gameEntityForBoard, &players[playerIndex]);
    system("clear");
    printGrid(grid);
    printf("Player coins: %d", players[playerIndex].coins);

    } while (1);


    int destRow;
    char destColumn;


    printf("Row: ");
    scanf("%d", &destRow);

    printf("Row: ");
    scanf("%d", &destColumn);

    moveBuild(grid, grid[0][0], destRow, destColumn);
    snprintf(grid[1][1].gameEntity->symbol, sizeof(grid[1][1].gameEntity->symbol), "%s", "TEST2");
    printGrid(grid);
}

void placeEntity(struct Cell grid[ROWS][COLS], int row, int column, GameEntity* gameEntity, Player* player) {
    player->coins -= gameEntity->cost;
    grid[row][column].gameEntity = gameEntity;
}

void moveBuild(struct Cell grid[ROWS][COLS], Cell originCell, int destRow, int destColumn) {
    grid[destRow][destColumn].gameEntity = originCell.gameEntity;
    originCell.gameEntity = NULL;
}

int letterToIndex(char letter) {
    letter = toupper(letter);
    return (int)(letter - 'A');
}

GameEntityType printEntitiesAndGetType(struct Cell grid[ROWS][COLS]) {
    int entityTypeSelection;
    int chooseEntity;

    system("clear");
    printGrid(grid);

    printf("\n1 - Select build");
    printf("\n2 - Select military unit\n");
    scanf("%d", &entityTypeSelection);

    system("clear");
    printGrid(grid);

    switch (entityTypeSelection)
    {
    case 1:
        printBuildOptions();
        break;
    case 2:
        printMilitaryOptions();
        break;
    default:
        break;
    }

    printf("\nSelect entity to place:\n ");
    scanf("%d", &chooseEntity);

    GameEntityType entityType = selectEntitytype(entityTypeSelection, chooseEntity);
    return entityType;
}

GameEntityType selectEntitytype(int entityType, int userSelection) {

    switch (entityType)
    {
    case 1:
        switch (userSelection)
        {
        case 1:
            return Base;
        case 2:
            return Mine;
        case 3:
            return Barracks;
        case 4:
            return Stable;
        case 5:
            return Armoury;
        default:
            break;
        }
        break;
    case 2:

        switch (userSelection)
        {
        case 1:
            return Infantary;
        case 2:
            return Cavalry;
        case 3:
            return Artillery;
        default:
            break;
        }
        break;

    default:
        break;
    }
}

GameEntity* gameEntityToPrint(struct Player player, enum GameEntityType entityType) {
    
    GameEntity* gameEntity = (struct GameEntity*)malloc(sizeof(struct GameEntity));

    switch (entityType)
    {
    case Base:
        gameEntity->health = HEALTH_BASE;
        gameEntity->cost = 30;
        if (player.playerType == Gondor) {
            strcpy(gameEntity->symbol, BASE_MORDOR);
        }
        else {
            strcpy(gameEntity->symbol, BASE_MORDOR);
        }

        break;
    case Mine:
        gameEntity->cost = 20;
        gameEntity->health = HEALTH_MINE;
        if (player.playerType == Gondor) {
            strcpy(gameEntity->symbol, MINES_SHIRE);
        }
        else {
            strcpy(gameEntity->symbol, MINES_EREBOR);
        }

        break;
    case Barracks:
        gameEntity->cost = 70;
        gameEntity->health = HEALTH_BARRACK_STABLE_ARMOURY;

        if (player.playerType == Gondor) {
            strcpy(gameEntity->symbol, BARRACKS_ROHAN);
        }
        else {
            strcpy(gameEntity->symbol, BARRACKS_ISENGARD);
        }

        break;
    case Stable:
        gameEntity->cost = 30;
        gameEntity->health = HEALTH_BARRACK_STABLE_ARMOURY;

        if (player.playerType == Gondor) {
            strcpy(gameEntity->symbol, STABLES_LOTHLORIEN);
        }
        else {
            strcpy(gameEntity->symbol, STABLES_MIRKWOOD);
        }

        break;
    case Armoury:
        gameEntity->health = HEALTH_BARRACK_STABLE_ARMOURY;
        gameEntity->cost = 30;

        if (player.playerType == Gondor) {
            strcpy(gameEntity->symbol, ARMOURY_GONDORIANFORGE);
        }
        else {
            strcpy(gameEntity->symbol, ARMOURY_DARKFORGE);
        }

        break;
    case Infantary:
        gameEntity->health = HEALTH_INFANTARY;
        gameEntity->cost = 10;

        if (player.playerType == Gondor) {
            strcpy(gameEntity->symbol, GONDOR_INFANTARY);
        }
        else {
            strcpy(gameEntity->symbol, MORDOR_INFANTARY);
        }

        break;
    case Cavalry:
        gameEntity->health = HEALTH_CAVALRY;
        gameEntity->cost = 15;

        if (player.playerType == Gondor) {
            strcpy(gameEntity->symbol, GONDOR_CAVALRY);
        }
        else {
            strcpy(gameEntity->symbol, GONDOR_CAVALRY);
        }

        break;
    case Artillery:
        gameEntity->health = HEALTH_ARTILLERY;
        gameEntity->cost = 20;

        if (player.playerType == Gondor) {
            strcpy(gameEntity->symbol, GONDOR_ARTILLERY);
        }
        else {
            strcpy(gameEntity->symbol, MORDOR_ARTILLERY);
        }
        break;
    default:
        break;
    }

    return gameEntity;
}

void printBuildOptions() {
    printf("\nBuilds");
    printf("\n1 - Base");
    printf("\n2 - Mines");
    printf("\n3 - Barracks");
    printf("\n4 - Stables");
    printf("\n5 - Armoury");
}

void printMilitaryOptions() {
    printf("\Military");
    printf("\n1 - Infantry");
    printf("\n2 - Cavalry");
    printf("\n3 - Artillery");
}