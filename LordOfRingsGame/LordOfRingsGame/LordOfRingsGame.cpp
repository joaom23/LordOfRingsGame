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
    int attack;
    bool isBuilding;
    int health;
    const char* symbol;
    int cost;
    int movementCost;
    struct Player* player;
    enum GameEntityType type;
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
void printGrid(struct Cell grid[ROWS][COLS], Player player);
void initializePlayers(struct Player players[NUMBER_OF_PLAYERS]);
void chooseSide(struct Player players[NUMBER_OF_PLAYERS]);
void playGame(struct Cell grid[ROWS][COLS], struct Player players[NUMBER_OF_PLAYERS]);
void placeEntity(struct Cell grid[ROWS][COLS], int row, int column, GameEntity* gameEntity, Player* player);
void moveUnit(struct Cell grid[ROWS][COLS], Cell* originCell, int destRow, int destColumn, int distance, Player* player);
int distanceBetweenCells(int row1, int col1, int row2, int col2);
void findCellPosition(struct Cell grid[ROWS][COLS], struct Cell* targetCell, int* foundRow, int* foundColumn);
int letterToIndex(char letter);
void printBuildOptions();
void printMilitaryOptions();
GameEntity* gameEntityToPrint(struct Player* player, enum GameEntityType entityType);
GameEntityType selectEntitytype(int entityType, int userSelection);
GameEntityType printTurnOptionsAndGetEntityType(struct Cell grid[ROWS][COLS], Player player);
const char* getPlayerTypeString(enum PlayerType type);
const char* getEntityTypeString(enum GameEntityType type);
char indexToLetter(int index);
void applyMineRewardIfAvailable(struct Cell grid[ROWS][COLS], Player* player);

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
        default:
            printf("Invalid choice. Please enter a valid option.\n");
            break;
        }
    } while (choice != 4);

    exit(0);
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
    printf("Caster coins : % d\n\n", player.coins);
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
    GameEntityType entityTypeToPlace;

    do {
        Player playerInTheTurn = players[playerIndex];
        printGrid(grid, playerInTheTurn);
        
        int choice;
        printf("1 - Place entity\n");
        printf("2 - Move unit\n");
        printf("3 - End turn\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            entityTypeToPlace = printTurnOptionsAndGetEntityType(grid, playerInTheTurn);

            GameEntity* gameEntityForBoard = gameEntityToPrint(&playerInTheTurn, entityTypeToPlace);

            printGrid(grid, playerInTheTurn);
            printf("Select cell in the grid to place %s, cost %d coins:\n", getEntityTypeString(entityTypeToPlace), gameEntityForBoard->cost);

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
            else if (playerInTheTurn.coins - gameEntityForBoard->cost < 0) {
                int test = playerInTheTurn.coins -= gameEntityForBoard->cost;
    
                printf("Insufficient funds.\n");
                system("pause");
            }
            else {
                placeEntity(grid, rowNumber, columnNumber, gameEntityForBoard, &playerInTheTurn);
            }
        }
            break;
        case 2:
        {
            int unitNumberToMove;
            Cell* originCell;
            
            printGrid(grid, playerInTheTurn);
        
            int count = 0;

            for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLS; ++j) {
                    if (grid[i][j].gameEntity != NULL) {
                        if (grid[i][j].gameEntity->player == &playerInTheTurn && grid[i][j].gameEntity->isBuilding == false) {
                            count++;
                        }
                    }                    
                }
            }

            if (count == 0) {
                printf("No available units to move.\n");
                system("pause");
                break;
            }

            printf("Available units:\n");
            printf("Select unit to move:\n");

            Cell** playerUnits = (struct Cell**)malloc(count * sizeof(struct Cell**));
            int entityIndex = 0;

            for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLS; ++j) {
                    if (grid[i][j].gameEntity != NULL) {
                        if (grid[i][j].gameEntity->player == &playerInTheTurn && grid[i][j].gameEntity->isBuilding == false) {
                            printf("%d - %s (Movement cost: %d per cell)-> Row: %d  Column: %c\n", entityIndex + 1, grid[i][j].gameEntity->symbol, grid[i][j].gameEntity->movementCost,i, indexToLetter(j));
                            playerUnits[entityIndex] = &grid[i][j];
                            entityIndex++;
                        }
                    }
                }
            }

            printf("\n");
            scanf("%d", &unitNumberToMove);

            originCell = playerUnits[unitNumberToMove - 1];

            int originCellRow;
            int originCellColumn;
            findCellPosition(grid, originCell, &originCellRow, &originCellColumn);

            printf("Select destination cell for %s:\n", originCell->gameEntity->symbol);

            int destRowNumber;
            char destColumnChar;
            int destColumnNumber;

            printf("Row: ");
            scanf("%d", &destRowNumber);

            printf("Column: ");
            scanf(" %c", &destColumnChar);
            destColumnNumber = letterToIndex(destColumnChar);

            if (grid[destRowNumber][destColumnNumber].gameEntity != NULL) {
                printf("\nCell occupied, please select another one. Press any key to contine.\n");
                system("pause");
            }
            else if (playerInTheTurn.coins - originCell->gameEntity->movementCost < 0) {
                printf("Insufficient funds.\n");
                system("pause");
            }
            else {
                int distance = distanceBetweenCells(originCellRow, originCellColumn, destRowNumber, destColumnNumber);
                moveUnit(grid, originCell, destRowNumber, destColumnNumber, distance, &playerInTheTurn);
            }        
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
            
            applyMineRewardIfAvailable(grid, &playerInTheTurn);
            
            break;
        }
        default:
            break;
        }
       
    } while (1);
}


void placeEntity(struct Cell grid[ROWS][COLS], int row, int column, GameEntity* gameEntity, Player* player) {      
    player->coins -= gameEntity->cost;

    int entitySize = strlen(gameEntity->symbol);

    grid[row][column].gameEntity = gameEntity;
}

void findCellPosition(struct Cell grid[ROWS][COLS], struct Cell* targetCell, int* foundRow, int* foundColumn) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (&grid[i][j] == targetCell) {
                *foundRow = i;
                *foundColumn = j;
                return; 
            }
        }
    }

    *foundRow = -1;
    *foundColumn = -1;
}

int distanceBetweenCells(int row1, int col1, int row2, int col2) {
    return abs(row1 - row2) + abs(col1 - col2);
}

void moveUnit(struct Cell grid[ROWS][COLS], Cell* originCell, int destRow, int destColumn, int distance, Player* player) {
    player->coins -= originCell->gameEntity->movementCost * distance;
    grid[destRow][destColumn].gameEntity = originCell->gameEntity;
    originCell->gameEntity = NULL;
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
    GameEntityType entityType;

    printGrid(grid, player);

    printf("1 - Select build\n");
    printf("2 - Select military unit\n");
    scanf("%d", &entityTypeSelection);

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
        gameEntity->isBuilding = true;
        gameEntity->type = Base;

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
        gameEntity->isBuilding = true;
        gameEntity->type = Mine;

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
        gameEntity->isBuilding = true;
        gameEntity->type = Barracks;

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
        gameEntity->isBuilding = true;
        gameEntity->type = Stable;

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
        gameEntity->isBuilding = true;
        gameEntity->type = Armoury;

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
        gameEntity->attack = 5;
        gameEntity->isBuilding = false;
        gameEntity->movementCost = 2;
        gameEntity->type = Infantary;

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
        gameEntity->attack = 7;
        gameEntity->isBuilding = false;
        gameEntity->movementCost = 1;
        gameEntity->type = Cavalry;

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
        gameEntity->attack = 10;
        gameEntity->isBuilding = false;
        gameEntity->movementCost = 3;
        gameEntity->type = Artillery;

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
    printf("Builds\n");
    printf("\n1 - Base");
    printf("\n2 - Mines");
    printf("\n3 - Barracks");
    printf("\n4 - Stables");
    printf("\n5 - Armoury");
}

void printMilitaryOptions() {
    printf("Military\n");
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

void applyMineRewardIfAvailable(struct Cell grid[ROWS][COLS], Player* player) {
    
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (grid[i][j].gameEntity != NULL) {
                if (grid[i][j].gameEntity->type == Mine && grid[i][j].gameEntity->player == player) {
                    player->coins += 5;
                }
            }
        }
    }
}