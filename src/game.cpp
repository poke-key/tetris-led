#include "periph.h"
#include "spiAVR.h"
#include "game.h"
#include "beeper.h"

void newGame()
{
     int ng1[] =
     {
        0b00000000,
        0b01000010,
        0b01100010,
        0b01010010,
        0b01001010,
        0b01000110,
        0b01000010,
        0b00000000
     };
 
     int ng2[] =
     {
        0b00000000,
        0b00111100,
        0b00100000,
        0b00100000,
        0b00101100,
        0b00100100,
        0b00111100,
        0b00000000
     };

     for(int rownum = 0; rownum < 8; rownum++)
     {
         setRow(0,rownum,ng1[rownum]);
         setRow(1,rownum,ng2[rownum]);
     }
}

void overGame()
{
     int go1[] =
     {
        0b00000000,
        0b00111100,
        0b00100000,
        0b00100000,
        0b00101100,
        0b00100100,
        0b00111100,
        0b00000000
     };
 
    int go2[] =
    {
        0b00000000,
        0b00111100,
        0b01000010,
        0b01000010,
        0b01000010,
        0b01000010,
        0b00111100,
        0b00000000
    };

     for(int rownum = 0; rownum < 8; rownum++)
     {
         setRow(0,rownum,go1[rownum]);
         setRow(1,rownum,go2[rownum]);
         
     }
 
}

void initGame(gameState *state)
 {
    // set the game state
    memset(state->board, 0, sizeof(state->board));
    state->score = 0;
    state->highscore = 0 ;
    state->level = 1;
    spawnPiece(state);
}

void spawnPiece(gameState *state) {
    
    int type = rand() % 7;

    // Initialize the new piece at the top of the matrix
    tetrisPiece *piece = &state->currrentPiece;
    piece->type = type;
    piece->orientation = 0;
    piece->x = 2; // Center horizontally
    piece->y = 0; // Top of the matrix

    // copy piece shape
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            piece->shape[i][j] = tetrisShapes[type][i][j];
        }
    }

    // clear display before displaying new one
    //clearScreen(0);

    // draw piece into matrix
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (piece->shape[y][x]) {
                    setLed(0, piece->y + y, piece->x + x, true);
                }
            }
        }
}


/*drawing piece logic*/
void drawPiece(gameState *state) {
    tetrisPiece *piece = &state->currrentPiece;
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            if(piece->shape[y][x]) {
                int ledY = piece->y + y;
                setLedDisplay(ledY, piece->x + x, true);
            }
        }
    }
}

/*check if the position is valid while moving piece*/
int isValidPosition(const gameState *state, const tetrisPiece *piece, int newX, int newY) {

    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            if(piece->shape[y][x]) {

                int boardX = newX + x;
                int boardY = newY + y;

                if (boardX < 0 || boardX >= 8 || boardY < 0 || boardY >= 16 || state->board[boardY][boardX])
                    return 0; //invalid position

            }

        }
    }
    return 1; //valid position

}

void clearPiece(gameState *state, const tetrisPiece *piece) {
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            if(piece->shape[y][x]) {
                setLedDisplay(piece->y + y, piece->x +x, false);
            }
        }
    }
}

/* Move the current piece down by one row */
void movePieceDown(gameState *state) {
    int newY = state->currrentPiece.y + 1; // Calculate the new Y coordinate

    // Check if the new position is valid
    if (isValidPosition(state, &state->currrentPiece, state->currrentPiece.x, newY)) {
        // Clear the current position of the piece
        clearPiece(state, &state->currrentPiece);
        
        // Move the piece down
        state->currrentPiece.y = newY;
        
        // Draw the piece in its new position
         drawPiece(state);
    }
}

/* Move the current piece left by one column */
void movePieceLeft(gameState *state) {
    int newX = state->currrentPiece.x - 1; // Calculate the new X coordinate

    // Check if the new position is valid
    if (isValidPosition(state, &state->currrentPiece, newX, state->currrentPiece.y)) {
        // Clear the current position of the piece
        clearPiece(state, &state->currrentPiece);
        
        // Move the piece left
        state->currrentPiece.x = newX;
        
        // Draw the piece in its new position
         drawPiece(state);
    }
}

/* Move the current piece right by one column */
void movePieceRight(gameState *state) {
    int newX = state->currrentPiece.x + 1; // Calculate the new X coordinate

    // Check if the new position is valid
    if (isValidPosition(state, &state->currrentPiece, newX, state->currrentPiece.y)) {
        // Clear the current position of the piece
        clearPiece(state, &state->currrentPiece);
        
        // Move the piece right
        state->currrentPiece.x = newX;
        
        // Draw the piece in its new position
         drawPiece(state);
    }
}

/*rotate the piece*/
void rotatePiece(gameState *state) {

    tetrisPiece *piece = &state->currrentPiece;
    tetrisPiece rotatedPiece = * piece;

    //perform rotation
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            rotatedPiece.shape[j][3-i] = piece->shape[i][j];
        }
    }

    //check if valid position
    if(isValidPosition(state, &rotatedPiece, piece->x, piece->y)) {

        clearPiece(state, piece);
        *piece = rotatedPiece;
        drawPiece(state);
    }

}

int checkCollisionDown(gameState *state) {
    tetrisPiece *piece = &state->currrentPiece;
    int newY = piece->y + 1;
    return !isValidPosition(state, piece, piece->x, newY);
}

/* Locks the current piece into the board */
void lockPiece(gameState *state) {
    tetrisPiece *piece = &state->currrentPiece;
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            if(piece->shape[y][x]) {
                state->board[piece->y + y][piece->x + x] = 1;
            }
        }
    }
}

void clearCompleteLines(gameState *state) {
    int row, column, i;

    for (row = 0; row < 16; row++) {
        int complete = 1; // flag for a complete row

        // Check if the row is complete
        for (column = 0; column < 8; column++) {
            if (state->board[row][column] == 0) {
                complete = 0;
                break;
            }
        }

        // If the row is complete, clear it and shift all above rows down
        if (complete) {
            state->score++ ;
            for (i = row; i > 0; i--) {
                for (column = 0; column < 8; column++) {
                    state->board[i][column] = state->board[i - 1][column]; // Shift rows down
                    setLedDisplay(i, column, state->board[i][column]); // Update LEDs accordingly
                }
            }

            // Clear the topmost row as it has no row above it
            for (column = 0; column < 8; column++) {
                state->board[0][column] = 0;
                setLedDisplay(0, column, false); // Turn off LEDs in the top row
            }

            row--; // Recheck the current row in case multiple rows were cleared at once
        }
    }
}