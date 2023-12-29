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

#define NUMBER_OF_PLAYERS 2
#define STARTING_COINS 100

struct Cell {
    struct GameEntity* gameEntity;
};

struct GameEntity {
    int health;
    const char* symbol;
    int cost;
    struct Player* player;
};

//struct Building {
//    struct GameEntity gameEntity;
//};
//
//struct CombatCharacter {
//    struct GameEntity gameEntity;
//};

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
    Artillery,
    Empty
};

void displayMainMenu();
void startNewGame();
void initializeGrid(struct Cell grid[ROWS][COLS]);
void printGrid(struct Cell grid[ROWS][COLS], Player player);
void initializePlayers(struct Player players[NUMBER_OF_PLAYERS]);
void chooseSide(struct Player players[NUMBER_OF_PLAYERS]);
void playGame(struct Cell grid[ROWS][COLS], struct Player players[NUMBER_OF_PLAYERS]);
void placeEntity(struct Cell grid[ROWS][COLS], int row, int column, GameEntity* gameEntity, Player* player);
void moveUnit(struct Cell grid[ROWS][COLS], Cell originCell, int destRow, int destColumn);
int letterToIndex(char letter);
void printBuildOptions();
void printMilitaryOptions();
GameEntity* gameEntityToPrint(struct Player* player, enum GameEntityType entityType);
GameEntityType selectEntitytype(int entityType, int userSelection);
GameEntityType printTurnOptionsAndGetEntityType(struct Cell grid[ROWS][COLS], Player player);
const char* getPlayerTypeString(enum PlayerType type);
const char* getEntityTypeString(enum GameEntityType type);
char indexToLetter(int index);

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
            system("cls");
            printf("TO DO\n");
            system("pause");
            system("cls");
            break;
        case 3:
            system("cls");
            printf("TO DO.\n");
            system("pause");
            system("cls");
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
            break;
        }
    } while (choice != 4);

    return 0;
}

void displayMainMenu() {
    printf("*** Lord of the Rings: Strategy Game ***\n\n");
    printf("1. Start New Game\n");
    printf("2. Load Game\n");
    printf("3. Settings\n");
    printf("4. Exit\n");
}

void startNewGame() {
    struct Cell grid[ROWS][COLS];
    struct Player players[NUMBER_OF_PLAYERS];

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

void printGrid(struct Cell grid[ROWS][COLS], Player player) {
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
    const char* playerTypeStrig = getPlayerTypeString(player.playerType);

    printf("\n--- Player %s turn --- \n", playerTypeStrig);
    printf("Caster coins : % d\n", player.coins);
}

void initializePlayers(struct Player players[NUMBER_OF_PLAYERS]) {

    for (int i = 0; i < NUMBER_OF_PLAYERS; ++i) {
        players[i].playerType = Gondor;
        players[i].coins = STARTING_COINS;
    }
}

void chooseSide(struct Player players[NUMBER_OF_PLAYERS]) {
    printf("Choose your side:\n");
    printf("1 - Gondor/Rivendell\n");
    printf("2 - Mordor\n");

    int choice;

    scanf("%d", &choice);

    switch (choice) {
    case 1:
        players[0].playerType = Gondor;
        players[1].playerType = Mordor;
        break;
    case 2:
        players[0].playerType = Mordor;
        players[1].playerType = Gondor;
        break;
    default:
        break;
    }

    printf("Player 1 will be playing as %s\n", getPlayerTypeString(players[0].playerType));
    printf("Player 2 will be playing as %s\n", getPlayerTypeString(players[1].playerType));
    printf("\n\n");
    system("pause");
}

void playGame(struct Cell grid[ROWS][COLS], struct Player players[NUMBER_OF_PLAYERS]) {

    int playerIndex = 0;
    GameEntityType entityTypeToPlace = Empty;

    do {
        system("clear");
        printGrid(grid, players[playerIndex]);

        int choice;
        printf("1 - Place entity\n");
        printf("2 - Move unit\n");
        printf("3 - End turn\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            entityTypeToPlace = printTurnOptionsAndGetEntityType(grid, players[playerIndex]);

            GameEntity* gameEntityForBoard = gameEntityToPrint(&players[playerIndex], entityTypeToPlace);

            system("clear");
            printGrid(grid, players[playerIndex]);
            printf("\nSelect cell in the grid to place %s, cost %d coins:\n", getEntityTypeString(entityTypeToPlace), gameEntityForBoard->cost);

            int rowNumber;
            char columnChar;
            int columnNumber;

            printf("Row: ");
            scanf("%d", &rowNumber);

            printf("Column: ");
            scanf(" %c", &columnChar);

            columnNumber = letterToIndex(columnChar);

            if (grid[rowNumber][columnNumber].gameEntity != NULL) {
                printf("\nCell occupied, please select another one. Press any key to contine.\n");
                system("pause");
            }
            else {
                placeEntity(grid, rowNumber, columnNumber, gameEntityForBoard, &players[playerIndex]);
            }
        }
            break;
        case 2:
        {
            int unitNumberToMove;
            int count = 1;
            GameEntityType entityType = Empty;

            system("clear");
            printGrid(grid, players[playerIndex]);

            printf("Available units:\n");

            for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLS; ++j) {
                    if (grid[i][j].gameEntity != NULL) {
                        if (grid[i][j].gameEntity->player == &players[playerIndex]) {
                            printf("%d - %s -> Row: %d  Column: %c", count, grid[i][j].gameEntity->symbol, i, indexToLetter(j));
                        }
                    }                    
                }
            }

            scanf("%d", &unitNumberToMove);

            /*system("clear");
            printGrid(grid, players[playerIndex]);*/
        }
            break;
        case 3:
        {
            if (playerIndex == 1) {
                playerIndex = 0;
            }
            else if (playerIndex == 0) {
                playerIndex = 1;
            }
            break;
        }
        default:
            break;
        }

        //entityType = printTurnOptionsAndGetEntityType(grid, players[playerIndex]); //entity type empty -> turn ended

        //if (entityType != Empty) {

        //    GameEntity* gameEntityForBoard = gameEntityToPrint(players[playerIndex], entityType);

        //    system("clear");
        //    printGrid(grid, players[playerIndex]);
        //    printf("\nSelect cell in the grid to place %s, cost %d coins:\n", getEntityTypeString(entityType), gameEntityForBoard->cost);

        //    int rowNumber;
        //    char columnChar;
        //    int columnNumber;

        //    printf("Row: ");
        //    scanf("%d", &rowNumber);

        //    printf("Column: ");
        //    scanf(" %c", &columnChar);

        //    columnNumber = letterToIndex(columnChar);

        //    if (grid[rowNumber][columnNumber].gameEntity != NULL) {
        //        printf("\nCell occupied, please select another one. Press any key to contine.\n");
        //        system("pause");
        //    }
        //    else {
        //        placeEntity(grid, rowNumber, columnNumber, gameEntityForBoard, &players[playerIndex]);
        //    }

        //    system("clear");
        //    printGrid(grid, players[playerIndex]);
        //}

    } while (1);


    int destRow;
    char destColumn;


    printf("Row: ");
    scanf("%d", &destRow);

    printf("Row: ");
    scanf("%d", &destColumn);

    moveUnit(grid, grid[0][0], destRow, destColumn);

}


void placeEntity(struct Cell grid[ROWS][COLS], int row, int column, GameEntity* gameEntity, Player* player) {   
    player->coins -= gameEntity->cost;

    int entitySize = strlen(gameEntity->symbol);

    grid[row][column].gameEntity = gameEntity;
}

void moveUnit(struct Cell grid[ROWS][COLS], Cell originCell, int destRow, int destColumn) {
    grid[destRow][destColumn].gameEntity = originCell.gameEntity;
    originCell.gameEntity = NULL;
}

int letterToIndex(char letter) {
    letter = toupper(letter);
    return (int)(letter - 'A');
}

char indexToLetter(int index) {
    if (index < 0 || index > 25) {
        return '\0'; 
    }
    return (char)('A' + index);
}

GameEntityType printTurnOptionsAndGetEntityType(struct Cell grid[ROWS][COLS], Player player) {
    int entityTypeSelection;
    int chooseEntity;
    GameEntityType entityType = Empty;

    system("clear");
    printGrid(grid, player);

    printf("\n1 - Select build");
    printf("\n2 - Select military unit\n");
    scanf("%d", &entityTypeSelection);

    system("clear");
    printGrid(grid, player);

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

    printf("\nSelect entity to place: ");
    scanf("%d", &chooseEntity);

    entityType = selectEntitytype(entityTypeSelection, chooseEntity);
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

GameEntity* gameEntityToPrint(struct Player* player, enum GameEntityType entityType) {

    GameEntity* gameEntity = (struct GameEntity*)malloc(sizeof(struct GameEntity));
    gameEntity->player = player;
    switch (entityType)
    {
    case Base:
        gameEntity->health = HEALTH_BASE;
        gameEntity->cost = 30;
        if (player->playerType == Gondor) {
            gameEntity->symbol = BASE_GONDOR;
        }
        else {
            gameEntity->symbol = BASE_MORDOR;
        }

        break;
    case Mine:
        gameEntity->cost = 20;
        gameEntity->health = HEALTH_MINE;
        if (player->playerType == Gondor) {
            gameEntity->symbol = MINES_SHIRE;
        }
        else {
            gameEntity->symbol = MINES_EREBOR;
        }

        break;
    case Barracks:
        gameEntity->cost = 70;
        gameEntity->health = HEALTH_BARRACK_STABLE_ARMOURY;

        if (player->playerType == Gondor) {
            gameEntity->symbol = BARRACKS_ROHAN;
        }
        else {
            gameEntity->symbol = BARRACKS_ISENGARD;
        }

        break;
    case Stable:
        gameEntity->cost = 30;
        gameEntity->health = HEALTH_BARRACK_STABLE_ARMOURY;

        if (player->playerType == Gondor) {
            gameEntity->symbol = STABLES_LOTHLORIEN;
        }
        else {
            gameEntity->symbol = STABLES_MIRKWOOD;
        }

        break;
    case Armoury:
        gameEntity->health = HEALTH_BARRACK_STABLE_ARMOURY;
        gameEntity->cost = 30;

        if (player->playerType == Gondor) {
            gameEntity->symbol = ARMOURY_GONDORIANFORGE;
        }
        else {
            gameEntity->symbol = ARMOURY_DARKFORGE;
        }

        break;
    case Infantary:
        gameEntity->health = HEALTH_INFANTARY;
        gameEntity->cost = 10;

        if (player->playerType == Gondor) {
            gameEntity->symbol = GONDOR_INFANTARY;
        }
        else {
            gameEntity->symbol = MORDOR_INFANTARY;
        }

        break;
    case Cavalry:
        gameEntity->health = HEALTH_CAVALRY;
        gameEntity->cost = 15;

        if (player->playerType == Gondor) {
            gameEntity->symbol = GONDOR_CAVALRY;
        }
        else {
            gameEntity->symbol = GONDOR_CAVALRY;
        }

        break;
    case Artillery:
        gameEntity->health = HEALTH_ARTILLERY;
        gameEntity->cost = 20;

        if (player->playerType == Gondor) {
            gameEntity->symbol = GONDOR_ARTILLERY;
        }
        else {
            gameEntity->symbol = MORDOR_ARTILLERY;
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

const char* getPlayerTypeString(enum PlayerType type) {
    static const char* typeStrings[] = {
        "Gondor",
        "Mordor"
    };

    return typeStrings[type];
}

const char* getEntityTypeString(enum GameEntityType type) {
    static const char* typeStrings[] = {
        "Base",
        "Mine",
        "Barracks",
        "Stable",
        "Armoury",
        "Infantry",
        "Cavalry",
        "Artillery"
    };
    
    return typeStrings[type];
}