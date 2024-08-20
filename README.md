watch video demo here: https://youtu.be/I9aAsP0D2mg

 Red-Dotted Tetris
 Kunal Shrivastav
 June 13th, 2024
 
 Introduction:
 Tetris is a 1 player game where the player can’t let a random 4-dotted block reach the top of the display.
 The shapes of the seven blocks include J, T, L, I, T, S, and Z. The player can flip the block in four
 different directions to try to fit it in the best way possible, which would ideally be to form a perfect line. If
 a perfect line of red dots is formed, that line is removed from the matrix and the player has more space to
 work with. The score of the player increases each time a perfect line is formed and cleared. Reaching a
 score of 99 indicates victory. When you lose, G and O are displayed to indicate Game Over, when a new
 game begins, N and G are displayed to signal a new game is starting.
 Some bugs included tv remote not being sensed immediately or everytime and my sound effects not
 working. If I had more time I would work to improve those and find a way to store scores in EEPROM so
 that the user knows his highest score and so that it could be bigger.

Build-upons:
 1. Useof the 8x8 LED matrix for the interface of the tetris gameplay
 2. Play the gameplay theme song when the game start using a passive buzzer
 3. Allow the user to turn turn the game off, and restart the game using the tv remote

 User Guide:
 Describe how a user would interact with the system. For example, if the project is a video game,
 what are the controls, what are the visual outputs, is there a sound mechanism, etc.?
 The game starts when the buzzer plays the Tetris theme song and the letters N and G are
 displayed on the 2 led devices. After that, randomly 7 blocks will periodically fall to the bottom
 of the 2nd matrix. The user can use the joystick to control the block. Moving it left will shift the
 block left 1 column. Moving the joystick right will shift the block right one column. Moving the
 joystick down will shift the block down one row even as the block is periodically falling down,
but say you want to move it down faster, then you can move it down with down on joystick.
 Lastly, you can rotate the block consistently by moving the joystick up.
 Each time the user clears a line by completing a row with dots, the game shifts down by basically
 deleting the completed lines and shifting anything above down to take its place. Each time this
 happens, the user's score goes up by 1, which is displayed in the digit display. You can get a max
 score of 99 as that is the most two digits holds. If the user wants to close the game during
 gameplay, he can press the on/off button in the tv remote once to clear the display. Pressing it
 again will reinitialize the game so he can start again.
 
 Hardware Components Used:--------
 Breadboard
 Wires
 Joystick
 4 digit 7 segment display
 8x8 LEDmatrix- displays dotted blocks and interface for game
 Elegoo Uno R3 Microcontroller
 IR Remote- used to turn game on and off
 Passive Buzzer
 Software Libraries Used:
 I used the stdlib.h, string.h, time.h, and stdio.h libraries from C for random number generation
 and using memset(). Other header files were given to us by TA’s in like for tv remote and spi, or
 helper.h from previous labs. Functions for modifying the led matrix were created by myself with
 help from SPI_SEND function provided by TA
