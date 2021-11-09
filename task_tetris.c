/*
 * task_tetris.c
 */

#include <main.h>

#define TETRIS_QUEUE_LEN  2

#define SCREEN_BOUNDS_LOWER 0
#define SCREEN_BOUNDS_UPPER 131

TaskHandle_t Task_Tetris_Handle;
QueueHandle_t Queue_Tetris;

// Globals used to keep track of board, movement options and score
uint8_t SCORE;
BOARD_ELEMENT board[10][10];
bool move_left_en;
bool move_right_en;
bool move_down_en;

int testPosArrayX[] = {COL0, COL1, COL2, COL3, COL4, COL5, COL6, COL7, COL8, COL9};
int testPosArrayY[] = {ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7, ROW8, ROW9};

/******************************************************************************
 * This function will initialize Queue_TETRIS and initialize the LCD
 ******************************************************************************/
void Task_Tetris_Init(void)
{
    Queue_Tetris = xQueueCreate(TETRIS_QUEUE_LEN,sizeof(TETRIS_MSG_t));

    // initialize the LCD
    Crystalfontz128x128_Init();

    // initialize SPI and the light sensor
    i2c_init();
    opt3001_init();
}


/******************************************************************************
 * This function will resets the virtual and visual game board
 ******************************************************************************/
void resetGame(void) {
    int i;
    int j;
    // clear virtual board
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            board[i][j].status = 0;
            board[i][j].color = 0;
        }
    }
    //  clear LCD screen
    lcd_draw_rectangle(64, 64, 132, 132, BLACK);

    // reset score
    SCORE = 0;
}


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
void checkValidMovement(TETRIS_SHAPE currentshape, int y, BOARD_POSITION pos) {
    bool left_en = true;
    bool right_en = true;
    bool down_en = true;

    // for each shape, left, right and down
    // movement is checked
    switch(currentshape) {
    case L :
        // check left movement
        if(board[pos.col-2][pos.row+1].status == 2)
            left_en = false;
        else if(board[pos.col-2][pos.row].status == 2)
            left_en = false;
        else if(board[pos.col-2][pos.row-1].status == 2)
            left_en = false;
        else if(pos.col == 1)
            left_en = false;
        // check right movement
        if(board[pos.col+2][pos.row+1].status == 2)
            right_en = false;
        else if(board[pos.col][pos.row].status == 2)
            right_en = false;
        else if(board[pos.col][pos.row-1].status == 2)
            right_en = false;
        else if(pos.col == 8)
            right_en = false;
        // check down movement
        if(board[pos.col-1][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col+1][pos.row+2].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;
    case L_1 :
        // check left movement
        if(board[pos.col-2][pos.row+1].status == 2)
            left_en = false;
        else if(board[pos.col-2][pos.row].status == 2)
            left_en = false;
        else if(board[pos.col-2][pos.row-1].status == 2)
            left_en = false;
        else if(pos.col == 1)
            left_en = false;
        // check right movement
        if(board[pos.col][pos.row+1].status == 2)
            right_en = false;
        else if(board[pos.col][pos.row].status == 2)
            right_en = false;
        else if(board[pos.col+2][pos.row-1].status == 2)
            right_en = false;
        else if(pos.col == 8)
            right_en = false;
        // check down movement
        if(board[pos.col-1][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col][pos.row].status == 2)
            down_en = false;
        else if(board[pos.col+1][pos.row].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;
    case L_2 :
        // check right movement
        if(board[pos.col+2][pos.row+1].status == 2)
            right_en = false;
        else if(board[pos.col+2][pos.row].status == 2)
            right_en = false;
        else if(board[pos.col+2][pos.row-1].status == 2)
            right_en = false;
        else if(pos.col == 8)
            right_en = false;
        // check left movement
        if(board[pos.col-2][pos.row-1].status == 2)
            left_en = false;
        else if(board[pos.col][pos.row].status == 2)
            left_en = false;
        else if(board[pos.col][pos.row+1].status == 2)
            left_en = false;
        else if(pos.col == 1)
            left_en = false;
        // check down movement
        if(board[pos.col+1][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col][pos.row].status == 2)
            down_en = false;
        else if(board[pos.col-1][pos.row].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;
    case L_3 :
        // check right movement
        if(board[pos.col+2][pos.row+1].status == 2)
            right_en = false;
        else if(board[pos.col+2][pos.row].status == 2)
            right_en = false;
        else if(board[pos.col+2][pos.row-1].status == 2)
            right_en = false;
        else if(pos.col == 8)
            right_en = false;
        // check left movement
        if(board[pos.col-2][pos.row+1].status == 2)
            left_en = false;
        else if(board[pos.col][pos.row].status == 2)
            left_en = false;
        else if(board[pos.col][pos.row-1].status == 2)
            left_en = false;
        else if(pos.col == 1)
            left_en = false;
        // check down movement
        if(board[pos.col-1][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col+1][pos.row+2].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;
    case LINE_HORIZONTAL :
        // check left movement
        if(board[pos.col-2][pos.row].status == 2)
            left_en = false;
        else if(pos.col == 1)
            left_en = false;
        // check right movement
        if(board[pos.col+2][pos.row].status == 2)
            right_en = false;
        else if(pos.col == 8)
            right_en = false;
        // check down movement
        if(board[pos.col-1][pos.row+1].status == 2)
            down_en = false;
        else if(board[pos.col][pos.row+1].status == 2)
            down_en = false;
        else if(board[pos.col+1][pos.row+1].status == 2)
            down_en = false;
        else if(y == ROW9)
            down_en = false;
        break;
    case LINE_VERTICAL :
        // check left movement
        if(board[pos.col-1][pos.row+1].status == 2)
            left_en = false;
        else if(board[pos.col-1][pos.row].status == 2)
            left_en = false;
        else if(board[pos.col-1][pos.row-1].status == 2)
            left_en = false;
        else if(pos.col == 0)
            left_en = false;
        // check right movement
        if(board[pos.col+1][pos.row+1].status == 2)
            right_en = false;
        else if(board[pos.col+1][pos.row].status == 2)
            right_en = false;
        else if(board[pos.col+1][pos.row-1].status == 2)
            right_en = false;
        else if(pos.col == 9)
            right_en = false;
        // check down movement
        if(board[pos.col][pos.row+2].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;
    case SQUARE :
        // check left movement
        if(board[pos.col-2][pos.row+1].status == 2)
            left_en = false;
        else if(board[pos.col-2][pos.row].status == 2)
            left_en = false;
        else if(pos.col == 1)
            left_en = false;
        // check right movement
        if(board[pos.col+1][pos.row+1].status == 2)
            right_en = false;
        else if(board[pos.col+1][pos.row].status == 2)
            right_en = false;
        else if(pos.col == 9)
            right_en = false;
        // check down movement
        if(board[pos.col-1][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col][pos.row+2].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;
    case S_HORIZONTAL :
        // check left movement
        if(board[pos.col-2][pos.row+1].status == 2)
            left_en = false;
        else if(board[pos.col-1][pos.row].status == 2)
            left_en = false;
        else if(pos.col == 1)
            left_en = false;
        // check right movement
        if(board[pos.col+2][pos.row].status == 2)
            right_en = false;
        else if(board[pos.col+1][pos.row+2].status == 2)
            right_en = false;
        else if(pos.col == 8)
            right_en = false;
        // check down movement
        if(board[pos.col-1][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col+1][pos.row+1].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;
    case T :
        // check left movement
        if(board[pos.col-1][pos.row+1].status == 2)
            left_en = false;
        else if(board[pos.col-2][pos.row].status == 2)
            left_en = false;
        else if(pos.col == 1)
            left_en = false;
        // check right movement
        if(board[pos.col+1][pos.row+1].status == 2)
            right_en = false;
        else if(board[pos.col+2][pos.row].status == 2)
            right_en = false;
        else if(pos.col == 8)
            right_en = false;
        // check down movement
        if(board[pos.col][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col-1][pos.row+1].status == 2)
            down_en = false;
        else if(board[pos.col+1][pos.row+1].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;
    case T_1 :
        // check left movement
        if(board[pos.col-2][pos.row].status == 2)
            left_en = false;
        else if(board[pos.col-1][pos.row-1].status == 2)
            left_en = false;
        else if(board[pos.col-1][pos.row+1].status == 2)
            left_en = false;
        else if(pos.col == 1)
            left_en = false;
        // check right movement
        if(board[pos.col+1][pos.row+1].status == 2)
            right_en = false;
        else if(board[pos.col+1][pos.row].status == 2)
            right_en = false;
        else if(board[pos.col+1][pos.row-1].status == 2)
            right_en = false;
        else if(pos.col == 9)
            right_en = false;
        // check down movement
        if(board[pos.col][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col-1][pos.row+1].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;
    case T_2 :
        // check left movement
        if(board[pos.col-2][pos.row+1].status == 2)
            left_en = false;
        else if(board[pos.col-1][pos.row].status == 2)
            left_en = false;
        else if(pos.col == 1)
            left_en = false;
        // check right movement
        if(board[pos.col+2][pos.row+1].status == 2)
            right_en = false;
        else if(board[pos.col+1][pos.row].status == 2)
            right_en = false;
        else if(pos.col == 8)
            right_en = false;
        // check down movement
        if(board[pos.col][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col-1][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col+1][pos.row+2].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;
    case T_3 :
        // check left movement
        if(board[pos.col-1][pos.row+1].status == 2)
            left_en = false;
        else if(board[pos.col-1][pos.row].status == 2)
            left_en = false;
        else if(board[pos.col-1][pos.row-1].status == 2)
            left_en = false;
        else if(pos.col == 0)
            left_en = false;
        // check right movement
        if(board[pos.col+2][pos.row].status == 2)
            right_en = false;
        else if(board[pos.col+1][pos.row-1].status == 2)
            right_en = false;
        else if(board[pos.col+1][pos.row+1].status == 2)
            right_en = false;
        else if(pos.col == 8)
            right_en = false;
        // check down movement
        if(board[pos.col][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col+1][pos.row+1].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;

    case Z_HORIZONTAL :
        // check left movement
        if(board[pos.col-2][pos.row].status == 2)
            left_en = false;
        else if(board[pos.col-1][pos.row+1].status == 2)
            left_en = false;
        else if(pos.col == 1)
            left_en = false;
        // check right movement
        if(board[pos.col+1][pos.row].status == 2)
            right_en = false;
        else if(board[pos.col+2][pos.row+2].status == 2)
            right_en = false;
        else if(pos.col == 8)
            right_en = false;
        // check down movement
        if(board[pos.col-1][pos.row+1].status == 2)
            down_en = false;
        else if(board[pos.col][pos.row+2].status == 2)
            down_en = false;
        else if(board[pos.col+1][pos.row+2].status == 2)
            down_en = false;
        else if(y == ROW8)
            down_en = false;
        break;
    }
    move_down_en = down_en;
    move_left_en = left_en;
    move_right_en = right_en;
}


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
BOARD_ELEMENT getRandomImage(void) {
    BOARD_ELEMENT elem_data;
    LCD_COLOR currentColor;
    TETRIS_SHAPE currentImage;
    uint8_t currentImageHeight;
    uint8_t currentImageWidth;

    // generate random number using light sensor
    // readings as the seed
    srand((unsigned)opt3001_read_lux());

    int imageIndex = rand() % 13;
    int colorIndex =  rand() % 12;

    // select shape and image based on random number
    switch(colorIndex) {
    case 0 :
        currentColor = WHITE;
        break;
    case 1 :
        currentColor = RED;
        break;
    case 2 :
        currentColor = GREEN;
        break;
    case 3 :
        currentColor = GREEN2;
        break;
    case 4 :
        currentColor = BLUE;
        break;
    case 5 :
        currentColor = BLUE2;
        break;
    case 6 :
        currentColor = YELLOW;
        break;
    case 7 :
        currentColor = ORANGE;
        break;
    case 8 :
        currentColor = CYAN;
        break;
    case 9 :
        currentColor = MAGENTA;
        break;
    case 10 :
        currentColor = GRAY;
        break;
    case 11 :
        currentColor = BROWN;
        break;
    }

    switch(imageIndex) {
    case 0 :
        currentImage = L;
        elem_data.bitmap = lBitmaps;
        currentImageHeight = lHeightPixels;
        currentImageWidth = lWidthPixels;
        break;
    case 1 :
        currentImage = L_1;
        elem_data.bitmap = l_1Bitmaps;
        currentImageHeight = l_1HeightPixels;
        currentImageWidth = l_1WidthPixels;
        break;
    case 2 :
        currentImage = L_2;
        elem_data.bitmap = l_2Bitmaps;
        currentImageHeight = l_2HeightPixels;
        currentImageWidth = l_2WidthPixels;
        break;
    case 3 :
        currentImage =  L_3;
        elem_data.bitmap = l_3Bitmaps;
        currentImageHeight = l_3HeightPixels;
        currentImageWidth = l_3WidthPixels;
        break;
    case 4 :
        currentImage =  LINE_HORIZONTAL;
        elem_data.bitmap = line_horizontalBitmaps;
        currentImageHeight = line_horizontalHeightPixels;
        currentImageWidth = line_horizontalWidthPixels;
        break;
    case 5 :
        currentImage = LINE_VERTICAL;
        elem_data.bitmap = line_verticalBitmaps;
        currentImageHeight = line_verticalHeightPixels;
        currentImageWidth = line_verticalWidthPixels;
        break;
    case 6 :
        currentImage = S_HORIZONTAL;
        elem_data.bitmap = s_horizontalBitmaps;
        currentImageHeight = s_horizontalHeightPixels;
        currentImageWidth = s_horizontalWidthPixels;
        break;
    case 7 :
        currentImage = SQUARE;
        elem_data.bitmap = squareBitmaps;
        currentImageHeight = squareHeightPixels;
        currentImageWidth = squareWidthPixels;
        break;
    case 8 :
        currentImage = T;
        elem_data.bitmap = tBitmaps;
        currentImageHeight = tHeightPixels;
        currentImageWidth = tWidthPixels;
        break;
    case 9 :
        currentImage = T_1;
        elem_data.bitmap = t_1Bitmaps;
        currentImageHeight = t_1HeightPixels;
        currentImageWidth = t_1WidthPixels;
        break;
    case 10 :
        currentImage = T_2;
        elem_data.bitmap = t_2Bitmaps;
        currentImageHeight = t_2HeightPixels;
        currentImageWidth = t_2WidthPixels;
        break;
    case 11 :
        currentImage = T_3;
        elem_data.bitmap = t_3Bitmaps;
        currentImageHeight = t_3HeightPixels;
        currentImageWidth = t_3WidthPixels;
        break;
    case 12 :
        currentImage = Z_HORIZONTAL;
        elem_data.bitmap = z_horizontalBitmaps;
        currentImageHeight = z_horizontalHeightPixels;
        currentImageWidth = z_horizontalWidthPixels;
        break;
    }

    // set appropriate fields for newly generated element
    elem_data.color = currentColor;
    elem_data.shape = currentImage;
    elem_data.height = currentImageHeight;
    elem_data.width = currentImageWidth;

    elem_data.status = 1;

    return elem_data;
}



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
BOARD_POSITION getBoardPosition(uint8_t x_pos, uint8_t y_pos) {
    BOARD_POSITION result;

    switch(x_pos) {
    case COL0 :
        result.col = 0;
        break;
    case COL1 :
        result.col = 1;
        break;
    case COL2 :
        result.col = 2;
        break;
    case COL3 :
        result.col = 3;
        break;
    case COL4 :
        result.col = 4;
        break;
    case COL5 :
        result.col = 5;
        break;
    case COL6 :
        result.col = 6;
        break;
    case COL7 :
        result.col = 7;
        break;
    case COL8 :
        result.col = 8;
        break;
    case COL9 :
        result.col = 9;
        break;
    }

    if(y_pos <= 18) {
        result.row = 0;
    } else if((y_pos > 20) && (y_pos <= 32)) {
        result.row = 1;
    } else if((y_pos > 32) && (y_pos <= 44)) {
        result.row = 2;
    } else if((y_pos > 44) && (y_pos <= 56)) {
        result.row = 3;
    } else if((y_pos > 56) && (y_pos <= 68)) {
        result.row = 4;
    } else if((y_pos > 68) && (y_pos <= 80)) {
        result.row = 5;
    } else if((y_pos > 80) && (y_pos <= 92)) {
        result.row = 6;
    } else if((y_pos > 92) && (y_pos <= 104)) {
        result.row = 7;
    } else if((y_pos > 102) && (y_pos <= 116)) {
        result.row = 8;
    } else if((y_pos > 116) && (y_pos <= 128)) {
        result.row = 9;
    }

    return result;
}

/******************************************************************************
 * This function checks if any rows have been filled, and then corrects the
 * screen accordingly
 ******************************************************************************/
void checkWins(void) {
    CONSOLE_MSG_t msg;
    int i;
    int j;
    int k;
    uint8_t fullrows = 0;
    bool rowFull;

    // iterate through board to find full rows
    for(i = 0; i < 10; i++) {
        rowFull = true;
        for(j = 0; j < 10; j++) {
            if(board[j][i].status != 2) {
                rowFull = false;
                break;
            }
        }

        if(rowFull) {
            // mark row for clearing
            board[0][i].status = 3;
            // increment full rows count
            fullrows++;
        }
    }

    // create and send message to tast_console to display score on
    if(fullrows != 0) {
        msg.cmd = CONSOLE_CMD_CLEAR_ROW;
        msg.value = fullrows;

        xQueueSend(Queue_Console,&msg,portMAX_DELAY);
    }

    // update score
    switch(fullrows) {
    case 0 :
        // do nothing
        break;
    case 1 :
        SCORE += 40;
        break;
    case 2 :
        SCORE += 100;
        break;
    case 3 :
        SCORE += 300;
        break;
    case 4 :
        SCORE += 1200;
        break;
    default:
        SCORE += (fullrows * 500);
        break;
    }

    // clear rows
    for(i = 0; i < 10; i++) {
        if(board[0][i].status == 3) {
            for( k = i; k > 0; k--) {
                for(j = 0; j < 10; j++) {
                    board[j][k].color = board[j][k-1].color;
                    board[j][k].status = board[j][k-1].status;

                    // draw the updated board data
                    lcd_draw_image(
                            testPosArrayX[j],
                            testPosArrayY[k],
                            singleWidthPixels,
                            singleHeightPixels,
                            singleBitmaps,
                            board[j][k].color,
                            LCD_COLOR_BLACK
                    );


                }
            }
        }
    }

}

/******************************************************************************
 * This function checks if a game over condition has been met
 * PARAMS
 *  elem     :   board element in play
 *  pos      :   position data for board element
 *
 * RETURNS
 *  true if game over condition is met, false otherwise
 ******************************************************************************/
bool checkLoss(BOARD_ELEMENT elem, BOARD_POSITION pos) {

    // for each shape, check if where the shape is being drawn is the same place
    // where a shape already is - if there's no room for a new shape to get
    // generated without drawing over another shape, the game is lost
    switch(elem.shape) {
    case L :

        if (board[pos.col][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row - 1].status == 2) {
            return true;
        } else {
            return false;
        }

        break;
    case L_1 :

        if (board[pos.col][pos.row - 1].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row - 1].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row - 1].status == 2) {
            return true;
        } else {
            return false;
        }

        break;
    case L_2 :

        if (board[pos.col][pos.row - 1].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row - 1].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row - 1].status == 2) {
            return true;
        } else {
            return false;
        }



        break;
    case L_3 :

        if (board[pos.col][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row - 1].status == 2) {
            return true;
        } else {
            return false;
        }


        break;
    case LINE_HORIZONTAL :

        if (board[pos.col][pos.row].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row].status == 2) {
            return true;
        } else {
            return false;
        }


        break;
    case LINE_VERTICAL :

        if (board[pos.col][pos.row].status == 2) {
            return true;
        } else if(board[pos.col][pos.row + 1].status == 2) {
            return true;
        } else if(board[pos.col][pos.row - 1].status == 2) {
            return true;
        } else {
            return false;
        }


        break;
    case S_HORIZONTAL :

        if (board[pos.col][pos.row].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row].status == 2) {
            return true;
        } else if(board[pos.col][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row + 1].status == 2) {
            return true;
        } else {
            return false;
        }


        break;
    case SQUARE :

        if (board[pos.col][pos.row].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col][pos.row + 1].status == 2) {
            return true;
        } else {
            return false;
        }


        break;
    case T :

        if (board[pos.col][pos.row].status == 2) {
            return true;
        } else if (board[pos.col][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row].status == 2) {
            return true;
        } else {
            return false;
        }



        break;
    case T_1 :

        if (board[pos.col][pos.row].status == 2) {
            return true;
        } else if (board[pos.col][pos.row - 1].status == 2) {
            return true;
        } else if (board[pos.col][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row].status == 2) {
            return true;
        } else {
            return false;
        }


        break;
    case T_2 :

        if (board[pos.col][pos.row].status == 2) {
            return true;
        } else if (board[pos.col][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row + 1].status == 2) {
            return true;
        } else {
            return false;
        }

        break;
    case T_3 :

        if (board[pos.col][pos.row].status == 2) {
            return true;
        } else if (board[pos.col][pos.row - 1].status == 2) {
            return true;
        } else if (board[pos.col][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row].status == 2) {
            return true;
        } else {
            return false;
        }

        break;
    case Z_HORIZONTAL :

        if (board[pos.col][pos.row].status == 2) {
            return true;
        } else if (board[pos.col - 1][pos.row].status == 2) {
            return true;
        } else if (board[pos.col + 1][pos.row + 1].status == 2) {
            return true;
        } else if (board[pos.col][pos.row + 1].status == 2) {
            return true;
        } else {
            return false;
        }

        break;
    }
}

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
void updateBoard(BOARD_ELEMENT elem, BOARD_POSITION pos) {

    // for each shape, updates the board data to match what's
    // being drawn on the screen:
    // (i) the status of the blocks (a shape moving downward)
    // (ii) the color of the block
    switch(elem.shape) {
    case L :
        board[pos.col][pos.row + 1].status = elem.status;
        board[pos.col + 1][pos.row + 1].status = elem.status;
        board[pos.col - 1][pos.row].status = elem.status;
        board[pos.col - 1][pos.row + 1].status = elem.status;
        board[pos.col - 1][pos.row - 1].status = elem.status;

        board[pos.col][pos.row + 1].color = elem.color;
        board[pos.col + 1][pos.row + 1].color = elem.color;
        board[pos.col - 1][pos.row].color = elem.color;
        board[pos.col - 1][pos.row + 1].color= elem.color;
        board[pos.col - 1][pos.row - 1].color = elem.color;


        break;
    case L_1 :
        board[pos.col][pos.row - 1].status = elem.status;
        board[pos.col - 1][pos.row - 1].status = elem.status;
        board[pos.col - 1][pos.row].status = elem.status;
        board[pos.col - 1][pos.row + 1].status = elem.status;
        board[pos.col + 1][pos.row - 1].status = elem.status;

        board[pos.col][pos.row - 1].color = elem.color;
        board[pos.col - 1][pos.row - 1].color = elem.color;
        board[pos.col - 1][pos.row].color = elem.color;
        board[pos.col - 1][pos.row + 1].color = elem.color;
        board[pos.col + 1][pos.row - 1].color = elem.color;
        break;
    case L_2 :
        board[pos.col][pos.row - 1].status = elem.status;
        board[pos.col - 1][pos.row - 1].status = elem.status;
        board[pos.col + 1][pos.row].status = elem.status;
        board[pos.col + 1][pos.row + 1].status = elem.status;
        board[pos.col + 1][pos.row - 1].status = elem.status;

        board[pos.col][pos.row - 1].color = elem.color;
        board[pos.col - 1][pos.row - 1].color = elem.color;
        board[pos.col + 1][pos.row].color = elem.color;
        board[pos.col + 1][pos.row + 1].color = elem.color;
        board[pos.col + 1][pos.row - 1].color = elem.color;
        break;
    case L_3 :
        board[pos.col][pos.row + 1].status = elem.status;
        board[pos.col - 1][pos.row + 1].status = elem.status;
        board[pos.col + 1][pos.row].status = elem.status;
        board[pos.col + 1][pos.row + 1].status = elem.status;
        board[pos.col + 1][pos.row - 1].status = elem.status;

        board[pos.col][pos.row + 1].color = elem.color;
        board[pos.col - 1][pos.row + 1].color = elem.color;
        board[pos.col + 1][pos.row].color = elem.color;
        board[pos.col + 1][pos.row + 1].color = elem.color;
        board[pos.col + 1][pos.row - 1].color = elem.color;

        break;
    case LINE_HORIZONTAL :
        board[pos.col][pos.row].status = elem.status;
        board[pos.col + 1][pos.row].status = elem.status;
        board[pos.col - 1][pos.row].status = elem.status;

        board[pos.col][pos.row].color = elem.color;
        board[pos.col + 1][pos.row].color = elem.color;
        board[pos.col - 1][pos.row].color = elem.color;

        break;
    case LINE_VERTICAL :
        board[pos.col][pos.row].status = elem.status;
        board[pos.col][pos.row + 1].status = elem.status;
        board[pos.col][pos.row - 1].status = elem.status;

        board[pos.col][pos.row].color = elem.color;
        board[pos.col][pos.row + 1].color = elem.color;
        board[pos.col][pos.row - 1].color = elem.color;

        break;
    case S_HORIZONTAL :
        board[pos.col][pos.row].status = elem.status;
        board[pos.col + 1][pos.row].status = elem.status;
        board[pos.col][pos.row + 1].status = elem.status;
        board[pos.col - 1][pos.row + 1].status = elem.status;

        board[pos.col][pos.row].color = elem.color;
        board[pos.col + 1][pos.row].color = elem.color;
        board[pos.col][pos.row + 1].color = elem.color;
        board[pos.col - 1][pos.row + 1].color = elem.color;

        break;
    case SQUARE :
        board[pos.col][pos.row].status = elem.status;
        board[pos.col - 1][pos.row].status = elem.status;
        board[pos.col - 1][pos.row + 1].status = elem.status;
        board[pos.col][pos.row + 1].status = elem.status;

        board[pos.col][pos.row].color = elem.color;
        board[pos.col - 1][pos.row].color = elem.color;
        board[pos.col - 1][pos.row + 1].color = elem.color;
        board[pos.col][pos.row + 1].color = elem.color;

        break;
    case T :
        board[pos.col][pos.row].status = elem.status;
        board[pos.col][pos.row + 1].status = elem.status;
        board[pos.col - 1][pos.row].status = elem.status;
        board[pos.col + 1][pos.row].status = elem.status;

        board[pos.col][pos.row].color = elem.color;
        board[pos.col][pos.row + 1].color = elem.color;
        board[pos.col - 1][pos.row].color = elem.color;
        board[pos.col + 1][pos.row].color = elem.color;

        break;
    case T_1 :
        board[pos.col][pos.row].status = elem.status;
        board[pos.col][pos.row - 1].status = elem.status;
        board[pos.col][pos.row + 1].status = elem.status;
        board[pos.col - 1][pos.row].status = elem.status;

        board[pos.col][pos.row].color = elem.color;
        board[pos.col][pos.row - 1].color = elem.color;
        board[pos.col][pos.row + 1].color = elem.color;
        board[pos.col - 1][pos.row].color = elem.color;
        break;
    case T_2 :
        board[pos.col][pos.row].status = elem.status;
        board[pos.col][pos.row + 1].status = elem.status;
        board[pos.col - 1][pos.row + 1].status = elem.status;
        board[pos.col + 1][pos.row + 1].status = elem.status;

        board[pos.col][pos.row].color = elem.color;
        board[pos.col][pos.row + 1].color = elem.color;
        board[pos.col - 1][pos.row + 1].color = elem.color;
        board[pos.col + 1][pos.row + 1].color = elem.color;
        break;
    case T_3 :
        board[pos.col][pos.row].status = elem.status;
        board[pos.col][pos.row - 1].status = elem.status;
        board[pos.col][pos.row + 1].status = elem.status;
        board[pos.col + 1][pos.row].status = elem.status;

        board[pos.col][pos.row].color = elem.color;
        board[pos.col][pos.row - 1].color = elem.color;
        board[pos.col][pos.row + 1].color = elem.color;
        board[pos.col + 1][pos.row].color = elem.color;
        break;
    case Z_HORIZONTAL :
        board[pos.col][pos.row].status = elem.status;
        board[pos.col - 1][pos.row].status = elem.status;
        board[pos.col + 1][pos.row + 1].status = elem.status;
        board[pos.col][pos.row + 1].status = elem.status;

        board[pos.col][pos.row].color = elem.color;
        board[pos.col - 1][pos.row].color = elem.color;
        board[pos.col + 1][pos.row + 1].color = elem.color;
        board[pos.col][pos.row + 1].color = elem.color;
        break;
    }

}

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
void move_updateBoard(BOARD_ELEMENT elem, BOARD_POSITION pos, bool move_left_true, bool move_right_true, bool move_down_true) {

    // For each shape, if a movement has occured, there will be
    // some board data associated with that element at a location
    // on the board where the element no longer is - therefore
    // we need to clear the old data

    // This switch goes through each shape, clearing where old data
    // is depending on the direction of the movement (left, right, down)
    switch(elem.shape) {
    case L :
        if (move_right_true) {
            board[pos.col - 2][pos.row].status = 0;
            board[pos.col - 2][pos.row + 1].status = 0;
            board[pos.col - 2][pos.row - 1].status = 0;

            board[pos.col - 2][pos.row].color = LCD_COLOR_BLACK;
            board[pos.col - 2][pos.row + 1].color = 0;
            board[pos.col - 2][pos.row - 1].color = 0;


        } else if (move_left_true) {
            board[pos.col + 2][pos.row + 1].status = 0;
            board[pos.col][pos.row].status = 0;
            board[pos.col][pos.row - 1].status = 0;

            board[pos.col + 2][pos.row + 1].color = 0;
            board[pos.col][pos.row].color = 0;
            board[pos.col][pos.row - 1].color = 0;

        } else if (move_down_true) {
            board[pos.col - 1][pos.row - 2].status = 0;
            board[pos.col][pos.row].status = 0;
            board[pos.col + 1][pos.row].status = 0;

            board[pos.col - 1][pos.row - 2].color = 0;
            board[pos.col][pos.row].color = 0;
            board[pos.col + 1][pos.row].color = 0;
        }

        break;
    case L_1 :
        if (move_right_true) {
            board[pos.col - 2][pos.row].status = 0;
            board[pos.col - 2][pos.row + 1].status = 0;
            board[pos.col - 2][pos.row - 1].status = 0;

            board[pos.col - 2][pos.row].color = 0;
            board[pos.col - 2][pos.row + 1].color = 0;
            board[pos.col - 2][pos.row - 1].color = 0;
        } else if (move_left_true) {
            board[pos.col + 2][pos.row - 1].status = 0;
            board[pos.col][pos.row].status = 0;
            board[pos.col][pos.row + 1].status = 0;

            board[pos.col + 2][pos.row - 1].color = 0;
            board[pos.col][pos.row].color = 0;
            board[pos.col][pos.row + 1].color = 0;

        } else if (move_down_true) {
            board[pos.col][pos.row - 2].status = 0;
            board[pos.col - 1][pos.row - 2].status = 0;
            board[pos.col + 1][pos.row - 2].status = 0;

            board[pos.col][pos.row - 2].color = 0;
            board[pos.col - 1][pos.row - 2].color = 0;
            board[pos.col + 1][pos.row - 2].color = 0;
        }
        break;
    case L_2 :
        if (move_right_true) {
            board[pos.col - 2][pos.row - 1].status = 0;
            board[pos.col][pos.row].status = 0;
            board[pos.col][pos.row + 1].status = 0;

            board[pos.col - 2][pos.row - 1].color = 0;
            board[pos.col][pos.row].color = 0;
            board[pos.col][pos.row + 1].color = 0;

        } else if (move_left_true) {
            board[pos.col + 2][pos.row].status = 0;
            board[pos.col + 2][pos.row + 1].status = 0;
            board[pos.col + 2][pos.row - 1].status = 0;

            board[pos.col + 2][pos.row].color = 0;
            board[pos.col + 2][pos.row + 1].color = 0;
            board[pos.col + 2][pos.row - 1].color = 0;

        } else if (move_down_true) {
            board[pos.col][pos.row - 2].status = 0;
            board[pos.col - 1][pos.row - 2].status = 0;
            board[pos.col + 1][pos.row - 2].status = 0;

            board[pos.col][pos.row - 2].color = 0;
            board[pos.col - 1][pos.row - 2].color = 0;
            board[pos.col + 1][pos.row - 2].color = 0;
        }
        break;
    case L_3 :
        if (move_right_true) {
            board[pos.col - 2][pos.row + 1].status = 0;
            board[pos.col][pos.row].status = 0;
            board[pos.col][pos.row - 1].status = 0;

            board[pos.col - 2][pos.row + 1].color = 0;
            board[pos.col][pos.row].color = 0;
            board[pos.col][pos.row - 1].color = 0;

        } else if (move_left_true) {
            board[pos.col + 2][pos.row].status = 0;
            board[pos.col + 2][pos.row + 1].status = 0;
            board[pos.col + 2][pos.row - 1].status = 0;

            board[pos.col + 2][pos.row].color = 0;
            board[pos.col + 2][pos.row + 1].color = 0;
            board[pos.col + 2][pos.row - 1].color = 0;

        } else if (move_down_true) {
            board[pos.col + 1][pos.row - 2].status = 0;
            board[pos.col][pos.row].status = 0;
            board[pos.col - 1][pos.row].status = 0;

            board[pos.col + 1][pos.row - 2].color = 0;
            board[pos.col][pos.row].color = 0;
            board[pos.col - 1][pos.row].color = 0;
        }

        break;
    case LINE_HORIZONTAL :
        if (move_right_true) {
            board[pos.col - 2][pos.row].status = 0;

            board[pos.col - 2][pos.row].color = 0;
        } else if (move_left_true) {
            board[pos.col + 2][pos.row].status = 0;

            board[pos.col + 2][pos.row].color = 0;
        } else if (move_down_true) {
            board[pos.col][pos.row - 1].status = 0;
            board[pos.col + 1][pos.row - 1].status = 0;
            board[pos.col - 1][pos.row - 1].status = 0;

            board[pos.col][pos.row - 1].color = 0;
            board[pos.col + 1][pos.row - 1].color = 0;
            board[pos.col - 1][pos.row - 1].color = 0;
        }
        break;
    case LINE_VERTICAL :
        if (move_right_true) {
            board[pos.col - 1][pos.row].status = 0;
            board[pos.col - 1][pos.row + 1].status = 0;
            board[pos.col - 1][pos.row - 1].status = 0;

            board[pos.col - 1][pos.row].color = 0;
            board[pos.col - 1][pos.row + 1].color = 0;
            board[pos.col - 1][pos.row - 1].color = 0;
        } else if (move_left_true) {
            board[pos.col + 1][pos.row].status = 0;
            board[pos.col + 1][pos.row + 1].status = 0;
            board[pos.col + 1][pos.row - 1].status = 0;

            board[pos.col + 1][pos.row].color = 0;
            board[pos.col + 1][pos.row + 1].color = 0;
            board[pos.col + 1][pos.row - 1].color = 0;
        } else if (move_down_true) {
            board[pos.col][pos.row - 2].status = 0;

            board[pos.col][pos.row - 2].color = 0;
        }

        break;
    case S_HORIZONTAL :
        if (move_right_true) {
            board[pos.col - 1][pos.row].status = 0;
            board[pos.col - 2][pos.row + 1].status = 0;

            board[pos.col - 1][pos.row].color = 0;
            board[pos.col - 2][pos.row + 1].color = 0;

        } else if (move_left_true) {
            board[pos.col + 1][pos.row + 1].status = 0;
            board[pos.col + 2][pos.row].status = 0;

            board[pos.col + 1][pos.row + 1].color = 0;
            board[pos.col + 2][pos.row].color = 0;

        } else if (move_down_true) {
            board[pos.col][pos.row - 1].status = 0;
            board[pos.col + 1][pos.row - 1].status = 0;
            board[pos.col - 1][pos.row].status = 0;

            board[pos.col][pos.row - 1].color = 0;
            board[pos.col + 1][pos.row - 1].color = 0;
            board[pos.col - 1][pos.row].color = 0;
        }

        break;
    case SQUARE :
        if (move_right_true) {
            board[pos.col - 2][pos.row].status = 0;
            board[pos.col - 2][pos.row + 1].status = 0;

            board[pos.col - 2][pos.row].color = 0;
            board[pos.col - 2][pos.row + 1].color = 0;
        } else if (move_left_true) {
            board[pos.col + 1][pos.row].status = 0;
            board[pos.col + 1][pos.row + 1].status = 0;

            board[pos.col + 1][pos.row].color = 0;
            board[pos.col + 1][pos.row + 1].color = 0;
        } else if (move_down_true) {
            board[pos.col][pos.row - 1].status = 0;
            board[pos.col - 1][pos.row - 1].status = 0;

            board[pos.col][pos.row - 1].color = 0;
            board[pos.col - 1][pos.row - 1].color = 0;
        }

        break;
    case T :
        if (move_right_true) {
            board[pos.col - 2][pos.row].status = 0;
            board[pos.col - 1][pos.row + 1].status = 0;

            board[pos.col - 2][pos.row].color = 0;
            board[pos.col - 1][pos.row + 1].color = 0;

        } else if (move_left_true) {
            board[pos.col + 2][pos.row].status = 0;
            board[pos.col + 1][pos.row + 1].status = 0;

            board[pos.col + 2][pos.row].color = 0;
            board[pos.col + 1][pos.row + 1].color = 0;
        } else if (move_down_true) {
            board[pos.col][pos.row - 1].status = 0;
            board[pos.col - 1][pos.row - 1].status = 0;
            board[pos.col + 1][pos.row - 1].status = 0;

            board[pos.col][pos.row - 1].color = 0;
            board[pos.col - 1][pos.row - 1].color = 0;
            board[pos.col + 1][pos.row - 1].color = 0;
        }

        break;
    case T_1 :
        if (move_right_true) {
            board[pos.col - 2][pos.row].status = 0;
            board[pos.col - 1][pos.row + 1].status = 0;
            board[pos.col - 1][pos.row - 1].status = 0;

            board[pos.col - 2][pos.row].color = 0;
            board[pos.col - 1][pos.row + 1].color = 0;
            board[pos.col - 1][pos.row - 1].color = 0;
        } else if (move_left_true) {
            board[pos.col + 1][pos.row].status = 0;
            board[pos.col + 1][pos.row - 1].status = 0;
            board[pos.col + 1][pos.row + 1].status = 0;

            board[pos.col + 1][pos.row].color = 0;
            board[pos.col + 1][pos.row - 1].color = 0;
            board[pos.col + 1][pos.row + 1].color = 0;
        } else if (move_down_true) {
            board[pos.col][pos.row - 2].status = 0;
            board[pos.col - 1][pos.row - 1].status = 0;

            board[pos.col][pos.row - 2].color = 0;
            board[pos.col - 1][pos.row - 1].color = 0;
        }

        break;
    case T_2 :
        if (move_right_true) {
            board[pos.col - 1][pos.row].status = 0;
            board[pos.col - 2][pos.row + 1].status = 0;

            board[pos.col - 1][pos.row].color = 0;
            board[pos.col - 2][pos.row + 1].color = 0;
        } else if (move_left_true) {
            board[pos.col + 1][pos.row].status = 0;
            board[pos.col + 2][pos.row + 1].status = 0;

            board[pos.col + 1][pos.row].color = 0;
            board[pos.col + 2][pos.row + 1].color = 0;
        } else if (move_down_true) {
            board[pos.col][pos.row - 1].status = 0;
            board[pos.col - 1][pos.row].status = 0;
            board[pos.col + 1][pos.row].status = 0;

            board[pos.col][pos.row - 1].color = 0;
            board[pos.col - 1][pos.row].color = 0;
            board[pos.col + 1][pos.row].color = 0;
        }

        break;
    case T_3 :
        if (move_right_true) {
            board[pos.col - 1][pos.row].status = 0;
            board[pos.col - 1][pos.row - 1].status = 0;
            board[pos.col - 1][pos.row + 1].status = 0;

            board[pos.col - 1][pos.row].color = 0;
            board[pos.col - 1][pos.row - 1].color = 0;
            board[pos.col - 1][pos.row + 1].color = 0;
        } else if (move_left_true) {
            board[pos.col + 2][pos.row].status = 0;
            board[pos.col + 1][pos.row + 1].status = 0;
            board[pos.col + 1][pos.row - 1].status = 0;

            board[pos.col + 2][pos.row].color = 0;
            board[pos.col + 1][pos.row + 1].color = 0;
            board[pos.col + 1][pos.row - 1].color = 0;
        } else if (move_down_true) {
            board[pos.col][pos.row - 2].status = 0;
            board[pos.col + 1][pos.row - 1].status = 0;

            board[pos.col][pos.row - 2].color = 0;
            board[pos.col + 1][pos.row - 1].color = 0;
        }

        break;
    case Z_HORIZONTAL :
        if (move_right_true) {
            board[pos.col - 1][pos.row + 1].status = 0;
            board[pos.col - 2][pos.row].status = 0;

            board[pos.col - 1][pos.row + 1].color = 0;
            board[pos.col - 2][pos.row].color = 0;
        } else if (move_left_true) {
            board[pos.col + 1][pos.row].status = 0;
            board[pos.col + 2][pos.row + 1].status = 0;

            board[pos.col + 1][pos.row].color = 0;
            board[pos.col + 2][pos.row + 1].color = 0;
        } else if (move_down_true) {
            board[pos.col][pos.row - 1].status = 0;
            board[pos.col - 1][pos.row - 1].status = 0;
            board[pos.col + 1][pos.row].status = 0;

            board[pos.col][pos.row - 1].color = 0;
            board[pos.col - 1][pos.row - 1].color = 0;
            board[pos.col + 1][pos.row].color = 0;
        }

        break;
    }

}

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
bool rotateCheck(BOARD_ELEMENT elem, BOARD_POSITION pos) {

    // This switch checks if another element would be drawn over
    // if a rotation occured, or if a rotation would place an
    // element out of bounds (this is checked by the nested conditional
    // for elements where this is an issue)
    switch(elem.shape) {
    case L :
        if ((board[pos.col][pos.row - 1].status == 0) && (board[pos.col + 1][pos.row - 1].status == 0)) {
            return true;
        } else {
            return false;
        }

        break;
    case L_1 :
        if ((board[pos.col + 1][pos.row + 1].status == 0) && (board[pos.col][pos.row + 1].status == 0)) {
            return true;
        } else {
            return false;
        }
        break;
    case L_2 :
        if ((board[pos.col][pos.row + 1].status == 0) && (board[pos.col - 1][pos.row + 1].status == 0)) {
            return true;
        } else {
            return false;
        }
        break;
    case L_3 :
        if ((board[pos.col - 1][pos.row].status == 0) && (board[pos.col - 1][pos.row - 1].status == 0)) {
            return true;
        } else {
            return false;
        }
        break;
    case LINE_HORIZONTAL :
        if ((board[pos.col][pos.row - 1].status == 0) && (board[pos.col][pos.row + 1].status == 0)) {
            if (pos.row != 0) {
                return true;
            } else {
                return false;
            }
        }

        return false;

        break;
    case LINE_VERTICAL :
        if ((board[pos.col + 1][pos.row].status == 0) && (board[pos.col - 1][pos.row].status == 0)) {
            if ((pos.col != 0) && (pos.col != 9)) {
                return true;
            } else {
                return false;
            }
        }

        return false;

        break;
    case S_HORIZONTAL :
        if ((board[pos.col - 1][pos.row].status == 0) && (board[pos.col + 1][pos.row + 1].status == 0)) {
            return true;
        } else {
            return false;
        }
        break;
    case T :
        if (board[pos.col][pos.row - 1].status == 0) {
            if (pos.row != 0) {
                return true;
            } else {
                return false;
            }
        }

        return false;

        break;
    case T_1 :
        if ((board[pos.col - 1][pos.row + 1].status == 0) && (board[pos.col + 1][pos.row + 1].status == 0)) {
            if (pos.col != 9) {
                return true;
            } else {
                return false;
            }
        }

        return false;

        break;
    case T_2 :
        if ((board[pos.col][pos.row - 1].status == 0) && (board[pos.col + 1][pos.row].status == 0)) {
            if (pos.row != 0) {
                return true;
            } else {
                return false;
            }
        }

        return false;

        break;
    case T_3 :
        if (board[pos.col - 1][pos.row].status == 0) {
            if (pos.col != 0) {
                return true;
            } else {
                return false;
            }
        }

        return false;

        break;
    case Z_HORIZONTAL :
        if ((board[pos.col + 1][pos.row].status == 0) && (board[pos.col - 1][pos.row + 1].status == 0)) {
            return true;
        } else {
            return false;
        }
        break;
    }

}

/******************************************************************************
 * This function checks what row a given element should start on
 * PARAMS
 *  elem     :   board element in play
 *
 * RETURNS
 *  true if the element should start on ROW0, false if it should start on ROW1
 ******************************************************************************/
bool checkStartingRow(BOARD_ELEMENT elem) {

    // This simple switch checks, given an element, if
    // it should be drawn on ROW0 or ROW1 for it to display
    // correctly
    switch(elem.shape) {
    case LINE_HORIZONTAL :
        return true;
        break;
    case S_HORIZONTAL :
        return true;
        break;
    case SQUARE :
        return true;
        break;
    case T :
        return true;
        break;
    case T_2 :
        return true;
        break;
    case Z_HORIZONTAL :
        return true;
        break;
    default :
        return false;
    }
}

/******************************************************************************
 * This task manages the movement of the space TETRIS. The joystick task or the
 * console task can send messages to TETRIS_QUEUE_LEN
 ******************************************************************************/
void Task_Tetris(void *pvParameters)
{
    TETRIS_MSG_t msg;
    BaseType_t status;

    bool gameOver = false;
    CONSOLE_MSG_t c_msg;

    // Set up board data to keep track of static blocks
    int i;
    int j;
    for(i = 0; i < 10; i++) {
        for(j = 0; j < 10; j++) {
            board[i][j].status = 0;
        }
    }
    move_down_en = true;
    move_right_en = false;
    move_left_en = false;

    BOARD_ELEMENT current_block = getRandomImage();

    // Set up variables to keep track of location
    int x = COL4;
    int y = 0;

    if (checkStartingRow(current_block))
        y = ROW0;
    else
        y = ROW1;

    // Set up default speed value
    int speed = 2;

    // Set up screen edge values that the TETRIS can't go past
    int MAX_X = COL9;
    int MIN_X = COL0;
    int MAX_Y = ROW8;

    // Update board to represent the initial element being spawned
    // in at the top
    BOARD_POSITION current_pos = getBoardPosition(x, y);
    updateBoard(current_block, current_pos);



    // Draw the initial starting image of the spaceTETRIS.
    lcd_draw_image(
            x,
            y,
            current_block.width,
            current_block.height,
            current_block.bitmap,
            current_block.color,
            LCD_COLOR_BLACK
    );

    // set the timer to wait 1000 mS
    timer1_task_init(1000);

    while(1)
    {
        /*
         * Wait until we receive a message from the Queue_Space_TETRIS
         */
        status = xQueueReceive(Queue_Tetris, &msg, portMAX_DELAY);

        // Check for available board movements
        checkValidMovement(current_block.shape,y,getBoardPosition(x,y));

        // Check what type of message is in the queue (up, down, left, right, speed)

        if (msg.cmd == TETRIS_CMD_FALL) {

            // if the game is over, use this message (associated with S1)
            // to restart the game
            if (gameOver) {
                // reset game values
                resetGame();

                // generate a new element
                current_block = getRandomImage();

                // get its initial position
                x = COL4;

                if (checkStartingRow(current_block))
                    y = ROW0;
                else
                    y = ROW1;

                // update the board to represent new element
                // being spawned
                current_pos = getBoardPosition(x, y);
                updateBoard(current_block, current_pos);

                // reset score
                SCORE = 0;

                // draw new initial element
                lcd_draw_image(
                        x,
                        y,
                        current_block.width,
                        current_block.height,
                        current_block.bitmap,
                        current_block.color,
                        LCD_COLOR_BLACK
                );

                gameOver = false;

            } else {

                // While the element can still move downwards
                // make it move downwards
                while (move_down_en) {

                    // get new values for y position
                    int value = y + msg.value;

                    // redrawn element at new position
                    lcd_draw_image(
                            x,
                            y,
                            current_block.width,
                            current_block.height,
                            current_block.bitmap,
                            LCD_COLOR_BLACK,
                            LCD_COLOR_BLACK
                    );

                    lcd_draw_image(
                            x,
                            value,
                            current_block.width,
                            current_block.height,
                            current_block.bitmap,
                            current_block.color,
                            LCD_COLOR_BLACK
                    );

                    // Update location variable
                    y = value;

                    // update board data so its represents element at
                    // new position
                    current_pos = getBoardPosition(x, y);
                    updateBoard(current_block, current_pos);
                    move_updateBoard(current_block, current_pos, false, false, true);

                    // check if the element can still move downwards
                    checkValidMovement(current_block.shape,y,getBoardPosition(x,y));

                    // Delay the next movement
                    vTaskDelay(pdMS_TO_TICKS(15));
                }

                // if the block can't move downwards anymore, change
                // its status to being static
                current_block.status = 2;

                // update board data and check if any rows are full
                current_pos = getBoardPosition(x, y);
                updateBoard(current_block, current_pos);
                checkWins();

                // generate new element
                current_block = getRandomImage();

                // get new element's starting position
                x = COL4;

                if (checkStartingRow(current_block))
                    y = ROW0;
                else
                    y = ROW1;

                current_pos = getBoardPosition(x, y);

                // check is a game over condition is met
                if (checkLoss(current_block, current_pos)) {
                    // game over

                    // draw the board, demonstrating the game over
                    // position

                    lcd_draw_image(
                            x,
                            y,
                            current_block.width,
                            current_block.height,
                            current_block.bitmap,
                            current_block.color,
                            LCD_COLOR_BLACK
                    );

                    // draw the game over message over the TETRIS board
                    lcd_draw_image(
                            COL4,
                            ROW6,
                            game_OverWidthPixels,
                            game_OverHeightPixels,
                            game_OverBitmaps,
                            LCD_COLOR_WHITE,
                            LCD_COLOR_BLACK
                    );

                    // send a message to the console w/ game over message
                    // and scoring data
                    c_msg.cmd = CONSOLE_CMD_GAME_OVER;
                    c_msg.value = SCORE;
                    xQueueSend(Queue_Console,&c_msg,portMAX_DELAY);

                    gameOver = true;


                } else {
                    // otherwise draw the new element and update board data
                    updateBoard(current_block, current_pos);

                    lcd_draw_image(
                            x,
                            y,
                            current_block.width,
                            current_block.height,
                            current_block.bitmap,
                            current_block.color,
                            LCD_COLOR_BLACK
                    );
                }

            }
        }
        else if ((msg.cmd == TETRIS_CMD_DROP) && (!gameOver)) {

            // Check if the element can move downwards
            if (move_down_en) {
                // if so, redraw element
                lcd_draw_image(
                        x,
                        y,
                        current_block.width,
                        current_block.height,
                        current_block.bitmap,
                        LCD_COLOR_BLACK,
                        LCD_COLOR_BLACK
                );

                // get new value for y position
                int value = y + msg.value;

                lcd_draw_image(
                        x,
                        value,
                        current_block.width,
                        current_block.height,
                        current_block.bitmap,
                        current_block.color,
                        LCD_COLOR_BLACK
                );

                // Update location variable
                y = value;

                // Update board data with element's new position
                current_pos = getBoardPosition(x, y);
                updateBoard(current_block, current_pos);
                move_updateBoard(current_block, current_pos, false, false, true);

                // Delay the next movement
                vTaskDelay(pdMS_TO_TICKS(speed));
            } else {

                // otherwise set element's status to static
                current_block.status = 2;

                // update board with new data and check for any
                // full rows
                current_pos = getBoardPosition(x, y);
                updateBoard(current_block, current_pos);
                checkWins();

                // generate new element
                current_block = getRandomImage();

                // get new element's starting position
                x = COL4;

                if (checkStartingRow(current_block))
                    y = ROW0;
                else
                    y = ROW1;

                current_pos = getBoardPosition(x, y);

                // check if a game over condition is met
                if (checkLoss(current_block, current_pos)) {
                    // game over

                    // draw image, demonstrating game over condition


                    lcd_draw_image(
                            x,
                            y,
                            current_block.width,
                            current_block.height,
                            current_block.bitmap,
                            current_block.color,
                            LCD_COLOR_BLACK
                    );

                    // draw game over message
                    lcd_draw_image(
                            COL4,
                            ROW6,
                            game_OverWidthPixels,
                            game_OverHeightPixels,
                            game_OverBitmaps,
                            LCD_COLOR_WHITE,
                            LCD_COLOR_BLACK
                    );

                    // send game over message and scoring data
                    // to console
                    c_msg.cmd = CONSOLE_CMD_GAME_OVER;
                    c_msg.value = SCORE;
                    xQueueSend(Queue_Console,&c_msg,portMAX_DELAY);

                    gameOver = true;

                } else {
                    // otherwise update the board with element's
                    // new position and draw element
                    updateBoard(current_block, current_pos);

                    lcd_draw_image(
                            x,
                            y,
                            current_block.width,
                            current_block.height,
                            current_block.bitmap,
                            current_block.color,
                            LCD_COLOR_BLACK
                    );
                }

            }
        } else if ((msg.cmd == TETRIS_CMD_LEFT) && (!gameOver)) {
            // Update location if edge of screen hasn't been
            // reached
            if (move_left_en) {
                // Redraw space TETRIS image

                lcd_draw_image(
                        x,
                        y,
                        current_block.width,
                        current_block.height,
                        current_block.bitmap,
                        LCD_COLOR_BLACK,
                        LCD_COLOR_BLACK
                );

                // Get new value for x position
                int value = (x - msg.value >= MIN_X) ? (x - msg.value) : MIN_X;

                lcd_draw_image(
                        value,
                        y,
                        current_block.width,
                        current_block.height,
                        current_block.bitmap,
                        current_block.color,
                        LCD_COLOR_BLACK
                );

                // Update location variable
                x = value;

                // Update board data with element's new position
                current_pos = getBoardPosition(x, y);
                updateBoard(current_block, current_pos);
                move_updateBoard(current_block, current_pos, true, false, false);


                // Delay the next movement
                vTaskDelay(pdMS_TO_TICKS(speed));
            }
        } else if ((msg.cmd == TETRIS_CMD_RIGHT) && (!gameOver)) {
            // Update location if edge of screen hasn't been
            // reached
            if (move_right_en) {
                // Redraw space TETRIS image

                lcd_draw_image(
                        x,
                        y,
                        current_block.width,
                        current_block.height,
                        current_block.bitmap,
                        LCD_COLOR_BLACK,
                        LCD_COLOR_BLACK
                );

                // Get new value for x position
                int value = (x + msg.value <= MAX_X) ? (x + msg.value) : MAX_X;

                lcd_draw_image(
                        value,
                        y,
                        current_block.width,
                        current_block.height,
                        current_block.bitmap,
                        current_block.color,
                        LCD_COLOR_BLACK
                );

                // Update location variable
                x = value;

                // Update board data with element's new position
                current_pos = getBoardPosition(x, y);
                updateBoard(current_block, current_pos);
                move_updateBoard(current_block, current_pos, false, true, false);

                // Delay the next movement
                vTaskDelay(pdMS_TO_TICKS(speed));
            }

        } else if ((msg.cmd == TETRIS_CMD_ROTATE) && (!gameOver)) {

            // check if the block can rotate without drawing over
            // another element
            if (rotateCheck(current_block, current_pos)) {

                // if so, clear old image
                lcd_draw_image(
                        x,
                        y,
                        current_block.width,
                        current_block.height,
                        current_block.bitmap,
                        LCD_COLOR_BLACK,
                        LCD_COLOR_BLACK
                );

                // save color data
                int oldColor = current_block.color;

                // clear old element's data and update board data
                current_block.status = 0;
                current_block.color = 0;
                current_pos = getBoardPosition(x, y);
                updateBoard(current_block, current_pos);

                // determine which element will be generated for the rotation
                switch(current_block.shape) {
                case LINE_HORIZONTAL :
                    current_block.shape = LINE_VERTICAL;
                    current_block.bitmap = line_verticalBitmaps;
                    current_block.height = line_verticalHeightPixels;
                    current_block.width = line_verticalWidthPixels;
                    break;
                case LINE_VERTICAL :
                    current_block.shape = LINE_HORIZONTAL;
                    current_block.bitmap = line_horizontalBitmaps;
                    current_block.height = line_horizontalHeightPixels;
                    current_block.width = line_horizontalWidthPixels;
                    break;
                case L :
                    current_block.shape = L_1;
                    current_block.bitmap = l_1Bitmaps;
                    current_block.height = l_1HeightPixels;
                    current_block.width = l_1WidthPixels;
                    break;
                case L_1 :
                    current_block.shape = L_2;
                    current_block.bitmap = l_2Bitmaps;
                    current_block.height = l_2HeightPixels;
                    current_block.width = l_2WidthPixels;
                    break;
                case L_2 :
                    current_block.shape = L_3;
                    current_block.bitmap = l_3Bitmaps;
                    current_block.height = l_3HeightPixels;
                    current_block.width = l_3WidthPixels;
                    break;
                case L_3 :
                    current_block.shape = L;
                    current_block.bitmap = lBitmaps;
                    current_block.height = lHeightPixels;
                    current_block.width = lWidthPixels;
                    break;
                case S_HORIZONTAL :
                    current_block.shape = Z_HORIZONTAL;
                    current_block.bitmap = z_horizontalBitmaps;
                    current_block.height = z_horizontalHeightPixels;
                    current_block.width = z_horizontalWidthPixels;
                    break;
                case T :
                    current_block.shape = T_1;
                    current_block.bitmap = t_1Bitmaps;
                    current_block.height = t_1HeightPixels;
                    current_block.width = t_1WidthPixels;
                    break;
                case T_1 :
                    current_block.shape = T_2;
                    current_block.bitmap = t_2Bitmaps;
                    current_block.height = t_2HeightPixels;
                    current_block.width = t_2WidthPixels;
                    break;
                case T_2 :
                    current_block.shape = T_3;
                    current_block.bitmap = t_3Bitmaps;
                    current_block.height = t_3HeightPixels;
                    current_block.width = t_3WidthPixels;
                    break;
                case T_3 :
                    current_block.shape = T;
                    current_block.bitmap = tBitmaps;
                    current_block.height = tHeightPixels;
                    current_block.width = tWidthPixels;
                    break;
                case Z_HORIZONTAL :
                    current_block.shape = S_HORIZONTAL;
                    current_block.bitmap = s_horizontalBitmaps;
                    current_block.height = s_horizontalHeightPixels;
                    current_block.width = s_horizontalWidthPixels;
                    break;
                default :
                    current_block.shape = current_block.shape;
                    current_block.bitmap = current_block.bitmap;
                    current_block.height = current_block.height;
                    current_block.width = current_block.width;
                    break;
                }

                // set rest of element's values (color and status)
                current_block.status = 1;
                current_block.color = oldColor;

                // draw the newly rotated element
                lcd_draw_image(
                        x,
                        y,
                        current_block.width,
                        current_block.height,
                        current_block.bitmap,
                        current_block.color,
                        LCD_COLOR_BLACK
                );

                // update board data
                current_pos = getBoardPosition(x, y);
                updateBoard(current_block, current_pos);

            }

        }


        // if the game isn't over, redraw the screen based on board data
        if (!gameOver) {
            for (i = 0; i < 10; i++){
                for (j = 0; j < 10; j++) {

                    lcd_draw_image(
                            testPosArrayX[i],
                            testPosArrayY[j],
                            singleWidthPixels,
                            singleHeightPixels,
                            singleBitmaps,
                            board[i][j].color,
                            LCD_COLOR_BLACK
                    );
                }
            }
        }

    }


}

