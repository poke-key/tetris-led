#include "timerISR.h"
#include "periph.h"
#include "spiAVR.h"
#include "game.h"
#include "led.h"
#include "beeper.h"
#include "irAVR.h"

const unsigned long GCD_PERIOD = 10;
decode_results results ;        // IR remote code

bool bPowerOn = false ;

int noteB3 = 8061;
int noteC4 = 7644;
int noteCs4_Db4 = 7196;
int noteD4 = 6770;
int noteDs4_Eb4 = 6385;
int noteE4 = 6023;
int noteF4 = 5686;
int noteFs4_Gb4 = 5369;
int noteG4 = 5078;
int noteGs4_Ab4 = 4798;
int noteA4 = 4544;
int noteAs4_Bb4 = 4297;
int noteB4 = 4069;
int noteC5 = 3849;

int tetristheme[36] = {noteE4, noteB3, noteC4, noteD4, noteC4, noteB3,
 noteA4, noteA4, noteC4, noteE4, noteD4, noteC4,
 noteB3, noteC4, noteD4, noteE4, 
 noteC4, noteA4, noteA4, 
 noteD4, noteF4, noteA4, noteG4, noteF4, noteE4, 
 noteC4, noteE4, noteD4, noteC4, noteB4, noteC4, noteD4, noteE4, noteC4, noteA4, noteA4};

/* D1000 F500 A1000 G500 F500 E1500 C500  E1000  D500 C500 B1500 C500 D1000 E1000 C1000 A1000 A1000 */
int maxDevices = 2;
#define B10000000 128 

gameState game_state; //instance of game

int count = 0; // counter for passive buzzer
bool on = true;

static int beep_counter = 0;
bool move_sound = false;

//Task struct for concurrent synchSMs implmentations
typedef struct _task{
	signed 	 char state; 		//Task's current state
	unsigned long period; 		//Task period
	unsigned long elapsedTime; 	//Time elapsed since last task tick
	int (*TickFct)(int); 		//Task tick function
} task;

task tasks[NUM_TASKS]; // declared task array with 5 tasks

void TimerISR() {
	for ( unsigned int i = 0; i < NUM_TASKS; i++ ) {                   // Iterate through each task in the task array
		if ( tasks[i].elapsedTime == tasks[i].period ) {           // Check if the task is ready to tick
			tasks[i].state = tasks[i].TickFct(tasks[i].state); // Tick and set the next state for this task
			tasks[i].elapsedTime = 0;                          // Reset the elapsed time for the next tick
		}
		tasks[i].elapsedTime += GCD_PERIOD;                        // Increment the elapsed time by GCD_PERIOD
	}
}

// int beep_counter =  0; //ms 0 means off.
void BeepOn()
{
    PORTD = SetBit(PORTD, 6, 1);
}
void BeepOff()
{
    PORTD = SetBit(PORTD, 6, 0);
}

/*game progress over time, piece falls from top of the board over tick function period*/
enum tetris_states {TETRIS_NEW, TETRIS_START, TETRIS_FALL, TETRIS_OVER} tetris_state;
int Tick_Tetris(int state) {
    switch (state) {
        case TETRIS_NEW:
            newGame(); 
            state = TETRIS_START; 
            break;
        case TETRIS_START:
            clearDisplay(); 
            initGame(&game_state); 
            state = TETRIS_FALL;
            break;
        case TETRIS_FALL:
            if (checkCollisionDown(&game_state)) {
                lockPiece(&game_state);
                clearCompleteLines(&game_state);
                spawnPiece(&game_state);
                if (checkCollisionDown(&game_state)) {
                    overGame(); 
                    state = TETRIS_NEW; 
                }
            } else {
                clearPiece(&game_state, &game_state.currrentPiece);
                movePieceDown(&game_state);
                drawPiece(&game_state);
            }
            break;
        default:
            break;
    }
    return state;
}




/*handles all rotation cases for UP on all possible 4 dotted LED piece(O, J, L, T, S, Z, I)*/
/*handles RIGHT, shift by one column, LEFT, shift left by one column, and DOWN, moves the block down*/
enum joystick_states {JOYSTICK_INIT, IDLE, UP, DOWN, REMAIN_UP, REMAIN_DOWN, RIGHT, REMAIN_RIGHT, LEFT, REMAIN_LEFT, BUTTON_PRESS, BUTTON_RELEASE} joystick_state;

int Tick_JoyStick(int state) {
    tetrisPiece pc = game_state.currrentPiece;
    int adcval = 0;
    int adcval1 = 0;

    /* state transitions */
    switch (state) {
        case JOYSTICK_INIT:
            state = IDLE;
            break;
        case IDLE:
            if ((PINC >> 2 & 0x01) == 0) {
                state = BUTTON_PRESS;
            } else {
                adcval = ADC_read(0);
                adcval1 = ADC_read(1);
                if (adcval > 950) {
                    state = UP;
                } else if (adcval < 100) {
                    state = DOWN;
                } else if (adcval1 > 950) {
                    state = RIGHT;
                } else if (adcval1 < 100) {
                    state = LEFT;
                }
            }
            break;
        case UP:
            state = REMAIN_UP;
            break;
        case DOWN:
            state = REMAIN_DOWN;
            break;
        case REMAIN_UP:
            adcval = ADC_read(0);
            if (adcval <= 950) state = IDLE;
            break;
        case REMAIN_DOWN:
            adcval = ADC_read(0);
            if (adcval >= 100) state = IDLE;
            break;
        case RIGHT:
            state = REMAIN_RIGHT;
            break;
        case LEFT:
            state = REMAIN_LEFT;
            break;
        case REMAIN_RIGHT:
            adcval1 = ADC_read(1);
            if (adcval1 <= 950) state = IDLE;
            break;
        case REMAIN_LEFT:
            adcval1 = ADC_read(1);
            if (adcval1 >= 100) state = IDLE;
            break;
        case BUTTON_PRESS:
            if ((PINC >> 2 & 0x01) == 1)
                state = BUTTON_RELEASE;
            break;
        case BUTTON_RELEASE:
            state = IDLE;
            break;
        default:
            break;
    }

    /* state actions */
    switch (state) {
        case JOYSTICK_INIT:
            break;
        case IDLE:
            if(move_sound) {
                beep_counter++;
                if(beep_counter >= 500) {
                    move_sound = false;
                    beep_counter = 0;
                    Peep_Off();
                }
            }
            else 
            break;
        case REMAIN_UP:
            break;
        case REMAIN_DOWN:
            break;
        case BUTTON_PRESS:
            break;
        case DOWN:
            clearPiece(&game_state, &pc);
            movePieceDown(&game_state);
            drawPiece(&game_state);
            break;
        case UP:
            clearPiece(&game_state, &pc);
            rotatePiece(&game_state);
            drawPiece(&game_state);
            break;
        case RIGHT:
            clearPiece(&game_state, &pc);
            movePieceRight(&game_state);
            Peep_On(8061);
            move_sound = true;
            drawPiece(&game_state);
            break;
        case LEFT:
            clearPiece(&game_state, &pc);
            movePieceLeft(&game_state);
            Peep_On(4);
            drawPiece(&game_state);
            break;
        case REMAIN_RIGHT:
        case REMAIN_LEFT:
            break;
        case BUTTON_RELEASE:
            break;
        default:
            break;
    }
    return state;
}

enum remote_states { REMOTE_IDLE, REMOTE_RESUME, REMOTE_READ } remote_state;
int Tick_Remote(int state)
{
    // If power is on, clear the display
    if (bPowerOn) {
        clearDisplay(); 
    }

    switch (state) {
        case REMOTE_IDLE:
            if (IRdecode(&results)) {
                if (16753245 == results.value) { // power button
                    bPowerOn = !bPowerOn;
                    if (!bPowerOn) {
                        initGame(&game_state); // Initialize the game state when turning off the power
                    }
                }
            }
            state = REMOTE_RESUME;
            break;
        case REMOTE_RESUME:
            IRresume();
            state = REMOTE_IDLE;
            break;
        default:
            break;
    }

    switch (state) {
        case REMOTE_READ:
            // Handle other remote actions if any
            break;
        default:
            break;
    }
    return state;
}


/*independent of tetris and joystick tasks, plays the theme song*/
enum buzz_states{BUZZ_IDLE, BUZZ_ON, BUZZ_OFF} buzz_state;
int Tick_Buzzer(int state)
{
    
    switch (state)
    {
        case(BUZZ_IDLE):
            state = BUZZ_ON;
            if(!on)
            {
                count = 0;
                return BUZZ_IDLE;
            }
            break;

        case(BUZZ_ON):
            if(!on)
            {
                count = 0;
                OCR1A = ICR1;
                return BUZZ_ON;            
            }
            break;
        case(BUZZ_OFF):
            state = BUZZ_OFF;
            break;

        default:
            state = BUZZ_ON;
            break;
    }
    switch (state)
    {
        case(BUZZ_IDLE):
            break;
        case(BUZZ_ON):
            if(count < 10) {
            ICR1 = tetristheme[0]; 
            OCR1A = ICR1/2;
            }
            else if(count < 15) {
            ICR1 = tetristheme[1]; 
            OCR1A = ICR1/2;
            }
            else if(count < 20) {
            ICR1 = tetristheme[2]; 
            OCR1A = ICR1/2;
            }
            else if(count < 30) {
            ICR1 = tetristheme[3]; 
            OCR1A = ICR1/2;
            }
            else if(count < 35) {
            ICR1 = tetristheme[4]; 
            OCR1A = ICR1/2;
            }
            else if(count < 40) {
            ICR1 = tetristheme[5]; 
            OCR1A = ICR1/2;
            }
            else if(count < 50) {
            ICR1 = tetristheme[6]; 
            OCR1A = ICR1/2;
            }
            else if(count < 55) {
            ICR1 = tetristheme[7]; 
            OCR1A = ICR1/2;
            }
            else if(count < 60) {
            ICR1 = tetristheme[8]; 
            OCR1A = ICR1/2;
            }
            else if(count < 70) {
            ICR1 = tetristheme[9]; 
            OCR1A = ICR1/2;
            }
            else if(count < 75) {
            ICR1 = tetristheme[10]; 
            OCR1A = ICR1/2;
            }
            else if(count < 80) {
            ICR1 = tetristheme[11]; 
            OCR1A = ICR1/2;
            }
            else if(count < 95) {
            ICR1 = tetristheme[12]; 
            OCR1A = ICR1/2;
            }
            else if(count < 100) {
            ICR1 = tetristheme[13]; 
            OCR1A = ICR1/2;
            }
            else if(count < 110) {
            ICR1 = tetristheme[14]; 
            OCR1A = ICR1/2;
            }
            else if(count < 120) {
            ICR1 = tetristheme[15]; 
            OCR1A = ICR1/2;
            }
            else if(count < 130) {
            ICR1 = tetristheme[16]; 
            OCR1A = ICR1/2;
            }
            else if(count < 140) {
            ICR1 = tetristheme[17]; 
            OCR1A = ICR1/2;
            }
            else if(count < 150) {
            ICR1 = tetristheme[18]; 
            OCR1A = ICR1/2;
            }
            else if(count < 150) {
            ICR1 = tetristheme[19]; 
            OCR1A = ICR1/2;
            }
            else if(count < 160) {
            ICR1 = tetristheme[20]; 
            OCR1A = ICR1/2;
            }
            else if(count < 165) {
            ICR1 = tetristheme[21]; 
            OCR1A = ICR1/2;
            }
            else if(count < 175) {
            ICR1 = tetristheme[22]; 
            OCR1A = ICR1/2;
            }
            else if(count < 180) {
            ICR1 = tetristheme[23]; 
            OCR1A = ICR1/2;
            }
            else if(count < 185) {
            ICR1 = tetristheme[24]; 
            OCR1A = ICR1/2;
            }
            else if(count < 200) {
            ICR1 = tetristheme[25]; 
            OCR1A = ICR1/2;
            }
            else if(count < 205) {
            ICR1 = tetristheme[26]; 
            OCR1A = ICR1/2;
            }
            else if(count < 215) {
            ICR1 = tetristheme[27]; 
            OCR1A = ICR1/2;
            }
            else if(count < 220) {
            ICR1 = tetristheme[28]; 
            OCR1A = ICR1/2;
            }
            else if(count < 240) {
            ICR1 = tetristheme[29]; 
            OCR1A = ICR1/2;
            }
            else if(count < 245) {
            ICR1 = tetristheme[30]; 
            OCR1A = ICR1/2;
            }
            else if(count < 255) {
            ICR1 = tetristheme[31]; 
            OCR1A = ICR1/2;
            }
            else if(count < 265) {
            ICR1 = tetristheme[32]; 
            OCR1A = ICR1/2;
            }
            else if(count < 275) {
            ICR1 = tetristheme[33]; 
            OCR1A = ICR1/2;
            }
            else if(count < 285) {
            ICR1 = tetristheme[34]; 
            OCR1A = ICR1/2;
            }
            else if(count < 295) {
            ICR1 = tetristheme[35]; 
            OCR1A = ICR1/2;
            }
            else
            {
                OCR1A = ICR1;
                count = 295;
            }
            count++;
            break;

        case(BUZZ_OFF):
            break;
    
        default:
            break;
    }
    return state;
}

enum states {D1, D2} state;
int Tick_Display_Score(int state){
  int digits[2] = {0,0} ;
  int i =1 ;

  int copyscore = game_state.score ;
  while ( copyscore ) {
    digits[i--] = copyscore % 10 ;
    copyscore /= 10 ;
  }


  switch(tasks[1].state){
    
    case D1:
      PORTC = SetBit(PORTC, 5 ,0x1);
      PORTC = SetBit(PORTC, 4 ,0x0);
      outNum(digits[1]); 
      tasks[1].state = D2;
      break;
    case D2:
      PORTC = SetBit(PORTC, 5 ,0x0);
      PORTC = SetBit(PORTC, 4 ,0x1);
      outNum(digits[0]) ; 
      tasks[1].state = D1;
      break;
    default:  
      break;

  }
  
  return tasks[1].state;
}


int main(void)
{

    // Init the PWM timer for 
    DDRB = 0xFE; PORTB = 0x01 ; // Pin0 is input from remote
    DDRD = 0xFF; PORTD = 0x00;  //Port D is all output.
    DDRC = 0x38; PORTC = 0xC7 ;  

    PORTB = SetBit(PORTB, PORTB2, 1); // SET INITIAL HIGH
    ADC_init();   // initializes ADC
    SPI_INIT();
    initDisplay();
    clearDisplay() ; // replaces need to clean each led panel.
    initPeeper() ;      // passive Beeper for debugging
    IRinit(&PORTB, &PINB, 0);
    IRresume() ;

    // Seed the random number generator
    srand(time(NULL));
    // gameState state;

    // Initialize the game
    // initGame(&game_state);

    /*initialize tasks*/
    tasks[0].period = TASK_TETRIS_PERIOD;
    tasks[0].state = TETRIS_NEW;
    tasks[0].elapsedTime = 0;
    tasks[0].TickFct = &Tick_Tetris;

    tasks[1].period = TASK_JOYSTICK_PERIOD;
    tasks[1].state = JOYSTICK_INIT;
    tasks[1].elapsedTime = 0;
    tasks[1].TickFct = &Tick_JoyStick;

    tasks[2].period = TASK_BUZZER_PERIOD;
    tasks[2].state = BUZZ_IDLE;
    tasks[2].elapsedTime = 0;
    tasks[2].TickFct = &Tick_Buzzer;

    tasks[3].period = TASK_REMOTE_PERIOD;
    tasks[3].state = REMOTE_IDLE;
    tasks[3].elapsedTime = 0;
    tasks[3].TickFct = &Tick_Remote;

    tasks[4].period = TASK_SCORE_PERIOD;
    tasks[4].state = D1;
    tasks[4].elapsedTime = 0;
    tasks[4].TickFct = &Tick_Display_Score;
    
    TimerSet(GCD_PERIOD);
    TimerOn();

    //Peep_Off() ;
/**/
    // setLedDisplay(8, 5, true);
    // _delay_ms(500);
    // setLedDisplay(10, 3, true);
    // return 0;
    
    // spawnPiece(&state);
    while (1) {
        // Main game loop
        // Game logic goes here

        // For demonstration, we'll spawn a new piece periodically

        // Delay to control the spawn rate
        // _delay_ms(200); // Adjust as needed
        // tetrisPiece pc = state.currrentPiece ;

        // _delay_ms(100);
        // clearPiece(&state, &pc);

        // movePieceDown(&state);
        // drawPiece(&state);
        // clearPiece(&state, &pc) ; //clear the current piece
        // movePiece( /*what goes here?*/)
        // drawPiece() // draw the piece after moving again
        // _delay_ms(200);
    }
    return 0;
}