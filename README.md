# Red-Dotted Tetris

**Developer:** Kunal Shrivastav  
**Date:** June 13th, 2024

## Introduction

Red-Dotted Tetris is a single-player game where the objective is to prevent randomly generated 4-dotted blocks from reaching the top of the display. The game features the classic Tetris shapes: J, T, L, I, T, S, and Z, which can be rotated in four different directions to fit perfectly within the grid. The game progresses as players form and clear perfect lines of red dots, earning points for each line cleared. Achieving a score of 99 results in victory. The game ends when blocks stack to the top, displaying "G" and "O" for Game Over. A new game can be initiated by restarting, indicated by "N" and "G" on the display.

## Features

- **8x8 LED Matrix Interface:** The primary display for Tetris gameplay.
- **Tetris Theme Song:** Played at the start of the game using a passive buzzer.
- **Remote Control:** Allows the game to be turned on/off and restarted using a TV remote.
- **Joystick Controls:**
  - **Left/Right:** Shift the block left or right by one column.
  - **Down:** Accelerate the block's downward movement.
  - **Up:** Rotate the block.

## User Guide

1. **Starting the Game:**
   - The game begins with the Tetris theme song playing and "N" and "G" displayed on the LED matrix.
   - Randomly generated Tetris blocks will start falling on the matrix.

2. **Controlling the Blocks:**
   - Use the joystick to move the block left, right, or down.
   - Rotate the block by moving the joystick up.

3. **Scoring and Clearing Lines:**
   - Complete a row with dots to clear it and shift all lines above downwards.
   - Your score increases by 1 for each line cleared, displayed on the 7-segment display.
   - The maximum achievable score is 99.

4. **Ending and Restarting the Game:**
   - Press the on/off button on the TV remote to clear the display and stop the game.
   - Press it again to restart the game.

## Hardware Components

- **Breadboard**
- **Wires**
- **Joystick**
- **4-digit 7-segment display**
- **8x8 LED Matrix:** Displays dotted blocks and serves as the interface.
- **Elegoo Uno R3 Microcontroller**
- **IR Remote:** Used to turn the game on and off.
- **Passive Buzzer**

## Software Libraries

- **stdlib.h, string.h, time.h, stdio.h** - Used for random number generation and memory management.
- **SPI and TV Remote Libraries** - Provided by TAs.
- **Custom LED Matrix Functions** - Created with help from SPI_SEND function provided by TAs.

## Bugs and Future Improvements

- **Current Issues:**
  - TV remote signal might not be sensed immediately or consistently.
  - Sound effects may not work as intended.

- **Future Enhancements:**
  - Improve TV remote responsiveness and sound effects.
  - Implement score storage in EEPROM to track high scores.

## Acknowledgments

Special thanks to the TAs for providing essential libraries and guidance throughout the project.

