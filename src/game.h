#include "helper.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "stdio.h"
#include "led.h"
#ifndef GAME_H
#define GAME_H

typedef struct _tetrisPiece {
    int shape[4][4];
    int x, y;
    int type;
    int orientation;
} tetrisPiece;

typedef struct _gameState {
    int board[16][8];
    tetrisPiece currrentPiece;
    int score;
    int highscore ;
    int level;
} gameState;

const int tetrisShapes[7][4][4] = {
    // I piece
    {
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // O piece
    {
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // T piece
    {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},        
        {0, 0, 0, 0}
    },
    // S piece
    {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // Z piece
    {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // J piece
    {
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // L piece
    {
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

// prototypes
void initGame(gameState *state);
void spawnPiece(gameState *state);
void movePieceLeft(gameState *state);
void movePieceRight(gameState *state);
void movePieceDown(gameState *state);
void rotatePiece(gameState *state);
void drawPiece(gameState *state);
int isValidPosition(const gameState *state, const tetrisPiece *piece, int newX, int newY);
void clearPiece(gameState *state, const tetrisPiece *piece);
int checkCollisionDown(gameState *state);
void lockPiece(gameState *state);
void clearCompleteLines(gameState *state);
void newGame() ;
void overGame() ;


/*unused functions*/
// void dropPiece(gameState *state);
// void updateGame(gameState *state);
// void drawBoard(gameState *state);
// void clearLines(gameState *state);
// void copyPieceToBoard(gameState *state);

#endif //GAME_H