/*
 * task_tetris.h
 *
 */

#ifndef TASK_TETRIS_H_
#define TASK_TETRIS_H_

#include <main.h>

typedef enum {
    TETRIS_CMD_RIGHT,
    TETRIS_CMD_LEFT,
    TETRIS_CMD_DROP,
    TETRIS_CMD_ROTATE,
    TETRIS_CMD_FALL
} TETRIS_CMD_t;

typedef enum {
    GAME_OVER,
    SINGLE,
    L,
    L_1,
    L_2,
    L_3,
    LINE_HORIZONTAL,
    LINE_VERTICAL,
    S_HORIZONTAL,
    SQUARE,
    T,
    T_1,
    T_2,
    T_3,
    Z_HORIZONTAL
} TETRIS_SHAPE;

typedef enum {
    WHITE = 0xFFFF,
    BLACK = 0x0000,
    RED = 0xF800,
    GREEN = 0x07E0,
    GREEN2 = 0xB723,
    BLUE = 0x001F,
    BLUE2 = 0x051D,
    YELLOW = 0xFFE0,
    ORANGE = 0xFBE4,
    CYAN = 0x07FF,
    MAGENTA = 0xA254,
    GRAY = 0x7BEF,
    BROWN = 0xBBCA
} LCD_COLOR;

typedef struct {
    uint8_t status; // 0 = empty, 1 = current falling piece, 2 = static piece, 3 = part of a full row to be scored
    LCD_COLOR color;
    TETRIS_SHAPE shape;
    uint8_t height;
    uint8_t width;
    const uint8_t * bitmap;
} BOARD_ELEMENT;

typedef struct {
    uint8_t row;
    uint8_t col;
} BOARD_POSITION;


typedef struct {
    TETRIS_CMD_t cmd;
    uint8_t     value;
} TETRIS_MSG_t;

extern TaskHandle_t Task_Tetris_Handle;
extern QueueHandle_t Queue_Tetris;

extern uint8_t SCORE;
extern bool move_right_en;
extern bool move_left_en;
extern bool move_down_en;

/* TETRIS BOARD */
// center of columns and rows in terms of pixels
#define COL0 12
#define COL1 24
#define COL2 36
#define COL3 48
#define COL4 60
#define COL5 72
#define COL6 84
#define COL7 96
#define COL8 108
#define COL9 120

#define ROW0 14
#define ROW1 26
#define ROW2 38
#define ROW3 50
#define ROW4 62
#define ROW5 74
#define ROW6 86
#define ROW7 98
#define ROW8 110
#define ROW9 122

//*****************************************************************

//*****************************************************************


/******************************************************************************
 * This function will initialize Queue_TETRIS and initialize the LCD
 ******************************************************************************/
void Task_Tetris_Init(void);

/******************************************************************************
 * This function will resets the virtual and visual game board
 ******************************************************************************/
void resetGame(void);

/******************************************************************************
 * This function checks if moving to the right/left/down is valid based
 * on current board
 *
 * PARAMS
 *  currentshape    :   Shape of board element
 *  y               :   current row of the element
 *  pos             :   position of the element on the board
 *
 * RETURNS
 *  nothing
 ******************************************************************************/
void checkValidMovement(TETRIS_SHAPE currentshape, int y, BOARD_POSITION pos);

/******************************************************************************
 * This function updates the currentImage and currentColor with the
 * new random image and color
 *
 * PARAMS
 *  none
 *
 * RETURNS
 *  new randomly generated board element
 ******************************************************************************/
BOARD_ELEMENT getRandomImage(void);

/******************************************************************************
 * This function calculates the current position of the falling image
 * in the virtual game board
 * PARAMS
 *  x_pos    :   current column of the element
 *  y_pos    :   current row of the element
 *
 * RETURNS
 *  board position data for given x-y values
 ******************************************************************************/
BOARD_POSITION getBoardPosition(uint8_t x_pos, uint8_t y_pos);

/******************************************************************************
 * This function checks if any rows have been filled, and then corrects the
 * screen accordingly
 ******************************************************************************/
void checkWins(void);

/******************************************************************************
 * This function checks if a game over condition has been met
 * PARAMS
 *  elem     :   board element in play
 *  pos      :   position data for board element
 *
 * RETURNS
 *  true if game over condition is met, false otherwise
 ******************************************************************************/
bool checkLoss(BOARD_ELEMENT elem, BOARD_POSITION pos);

/******************************************************************************
 * This function updates the board data to match the position of the Tetris
 * block on-screen
 * PARAMS
 *  elem     :   board element in play
 *  pos      :   position data for board element
 *
 * RETURNS
 *  nothing
 ******************************************************************************/
void updateBoard(BOARD_ELEMENT elem, BOARD_POSITION pos);

/******************************************************************************
 * This function clears the board data from the previous location of a board
 * element after it has moved
 * PARAMS
 *  elem     :   board element in play
 *  pos      :   position data for board element
 *  move_left_true  :   boolean indicating the element moved left
 *  move_right_true  :   boolean indicating the element moved right
 *  move_down_true  :   boolean indicating the element moved down
 *
 * RETURNS
 *  nothing
 ******************************************************************************/
void move_updateBoard(BOARD_ELEMENT elem, BOARD_POSITION pos, bool move_left_true, bool move_right_true, bool move_down_true);

/******************************************************************************
 * This function checks if an element can rotate without drawing over an
 * existing element
 *
 * PARAMS
 *  elem     :   board element in play
 *  pos      :   position data for board element
 *
 * RETURNS
 *  true if the element can rotate, false otherwise
 ******************************************************************************/
bool rotateCheck(BOARD_ELEMENT elem, BOARD_POSITION pos);

/******************************************************************************
 * This function checks what row a given element should start on
 * PARAMS
 *  elem     :   board element in play
 *
 * RETURNS
 *  true if the element should start on ROW0, false if it should start on ROW1
 ******************************************************************************/
bool checkStartingRow(BOARD_ELEMENT elem);

/******************************************************************************
 * This task manages the movement of the space ship. The joystick task or the
 * console task can send messages to the
 ******************************************************************************/
void Task_Tetris(void *pvParameters);

#endif /* TASK_TETRIS_H_ */
