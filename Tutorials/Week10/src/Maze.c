#include <assert.h>   // assert()
#include <stdio.h>    // printf() and snprintf()
#include <stdlib.h>   // malloc() and free()
#include <string.h>   // memset()
#include <wchar.h>
#include "Stack.h"


#define ROWS  10
#define COLS  10

// The state of a position in the maze:
//      NOT_VISITED -> TO_RIGHT -> TO_DOWN -> TO_LEFT -> TO_UP -> FINISHED
typedef enum {
    // This position has not been visited yet
    NOT_VISITED = 0,
    // go to visit the adjacent rightward position
    TO_RIGHT = 1,
    // go to visit the adjacent downward position
    TO_DOWN = 2,
    // go to visit the adjacent leftward position
    TO_LEFT = 4,
    // go to visit the adjacent upward position
    TO_UP = 8,
    // We have explored all four directions of the current position, 
    FINISHED = 0x0F,

} PositionState;

// see https://en.wikipedia.org/wiki/Arrows_(Unicode_block)
static char *rightArrowUnicodeStr = "\u2192";
static char *downArrowUnicodeStr = "\u2193";
static char *leftArrowUnicodeStr = "\u2190";
static char *upArrowUnicodeStr = "\u2191";

// https://www.fileformat.info/info/unicode/char/1f5fc/index.htm
static char *towerUnicodeStr = "\U0001F5FC";


// The information about a position in the maze
struct PositionInfo {
    // blocked:  1
    // not blocked: 0, 
    int blocked;

    /////////// used when exploring the maze ///////
    // which row
    int r;
    // which col
    int c;
    // the state of the current position
    PositionState state;
    // points to a unicode string, e.g., upArrowUnicodeStr
    char *dirStr;
};

static int map[ROWS][COLS] = {
    {1, 1, 1, 1, 0, 1, 1, 1, 1, 1}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 1}, 
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1}, 
    {1, 0, 1, 0, 0, 1, 1, 1, 1, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 0, 1, 0, 0},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 0},
    {1, 1, 1, 1, 0, 0, 1, 0, 1, 0},
};

static struct PositionInfo maze[ROWS][COLS];

static struct PositionInfo *pStartPos = &maze[1][0];

// For PrintMaze()
static int cnt = 0;

static void InitMaze(void) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            maze[r][c].blocked = map[r][c];
            maze[r][c].r = r;
            maze[r][c].c = c;
            maze[r][c].state = NOT_VISITED;
            maze[r][c].dirStr = NULL;
        }
    }
    cnt = 0;
}

static int IsLegalPosition(int r, int c) {
    return (r >= 0) && (r < ROWS) && (c >= 0) && (c < COLS);
}

static int IsExitPosition(int r, int c) {
    if (IsLegalPosition(r, c)) {
        return (r == ROWS - 2) && (c == COLS - 1);
    } else {
        return 0;
    }    
}



static void PrintMaze(char *stepName) {
    
    printf("-------------------------  %s %d ----------------------------\n", stepName, cnt);
    cnt++;
    
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (maze[r][c].blocked) {
                printf("X   ");
            } 
            else if (IsExitPosition(r, c)) {
                printf("%s   ", towerUnicodeStr);
            }
            else {
                if (maze[r][c].dirStr != NULL) {
                    printf("%s   ", maze[r][c].dirStr);
                } else {
                    printf("    ");
                }
            }
        }
        if (r == ROWS - 1)
            printf("\n");
        else
            printf("\n\n");
    }
    printf("---------------------------------------------------------------\n\n\n");
}

/*
    If (r, c) is a legal position, it is not blocked and it has not been explored yet,
    then
        push its position information onto the data stack
 */
static void PushAdjacentPosition(struct Stack *pStack, int r, int c, PositionState initState) {
    if (IsLegalPosition(r, c)) {
        if (!maze[r][c].blocked && (maze[r][c].state == NOT_VISITED)) {
            struct PositionInfo *pPos = &maze[r][c];
            pPos->state = initState;
            StackPush(pStack, pPos);
        }
    }
}

/*
  State transition:

       TO_RIGHT -> TO_DOWN -> TO_LEFT -> TO_UP -> FINISHED
 */
void ExploreMaze(void) {    
    struct Stack *pStack = CreateStack();    
    // Initialize the maze
    InitMaze();
    char *stepName = "Step";
    PrintMaze(stepName);
    // Push the start position
    pStartPos->state = TO_RIGHT;
    StackPush(pStack, pStartPos);

    while (!StackIsEmpty(pStack)) {
        struct PositionInfo *pCurPos = StackPeek(pStack);        
        if (IsExitPosition(pCurPos->r, pCurPos->c)) {
            break;
        }        
        switch(pCurPos->state) {
            case TO_RIGHT:
                pCurPos->dirStr = rightArrowUnicodeStr;              
                PushAdjacentPosition(pStack, pCurPos->r, pCurPos->c + 1, TO_RIGHT);
                // When this position becomes the top element on the stack,  
                // we need to go downward.
                pCurPos->state = TO_DOWN;
                break;
            case TO_DOWN:                
                pCurPos->dirStr = downArrowUnicodeStr;
                PushAdjacentPosition(pStack, pCurPos->r + 1, pCurPos->c, TO_RIGHT);
                // When this position becomes the top element on the stack,  
                // we need to go leftward.                
                pCurPos->state = TO_LEFT;
                break;
            case TO_LEFT:
                pCurPos->dirStr = leftArrowUnicodeStr;                
                PushAdjacentPosition(pStack, pCurPos->r, pCurPos->c - 1, TO_RIGHT);                
                // When this position becomes the top element on the stack,  
                // we need to go upward.                 
                pCurPos->state = TO_UP;
                break;
            case TO_UP:
                pCurPos->dirStr = upArrowUnicodeStr;
                PushAdjacentPosition(pStack, pCurPos->r - 1, pCurPos->c, TO_RIGHT);
                // When this position becomes the top element on the stack,  
                // we have finished all directions.                  
                pCurPos->state = FINISHED;
                break;
            case FINISHED:
                pCurPos->dirStr = NULL;
                StackPop(pStack);                
                break;
            default:
                break;                                
        }
        PrintMaze(stepName);
    }

    ReleaseStack(pStack);
    PrintMaze(stepName);
}

#if 0
/*
    This function returns the next unexplored state randomly and also updates pCurPos->state.
 */
static PositionState NextUnexploredState(struct PositionInfo *pCurPos) {
    long x;
    long state = (long) (pCurPos->state);
    assert(pCurPos->state != FINISHED);
    // randomly generate an integer x in {0, 1, 2, 3},
    // such that (1 << x) represents a state which has not been explored yet.
    do {
        x = random();
        x %= 4;
    } while (((1 << x) & state) != 0);

    PositionState nextState = (PositionState)(1 << x);
    pCurPos->state |= nextState;
    return nextState;
}


void ExploreMazeRandomly(void) {
    struct Stack *pStack = CreateStack();
    // Initialize the maze
    InitMaze();
    char *stepName = "Random Step";
    PrintMaze(stepName);

    // pStartPos->state = NOT_VISITED;
    StackPush(pStack, pStartPos);

    while (!StackIsEmpty(pStack)) {
        struct PositionInfo *pCurPos = StackPeek(pStack);
        if (IsExitPosition(pCurPos->r, pCurPos->c)) {
            break;
        }        
        PositionState nextState;
        if (pCurPos->state != FINISHED) {
            nextState = ______Q1______;
        } else {
            nextState = FINISHED;
        }
        //      
        switch(nextState) {
            case TO_RIGHT:
                pCurPos->dirStr = rightArrowUnicodeStr;
                ______Q2______;
                break;
            case TO_DOWN:
                pCurPos->dirStr = downArrowUnicodeStr;
                ______Q3______;
                break;
            case TO_LEFT:
                pCurPos->dirStr = leftArrowUnicodeStr;
                ______Q4______;
                break;
            case TO_UP:
                pCurPos->dirStr = upArrowUnicodeStr;
                ______Q5______;
                break;
            case FINISHED:
                pCurPos->dirStr = NULL;
                StackPop(pStack);
                break;
            default:
                break;
        }
        PrintMaze(stepName);
    }

    ReleaseStack(pStack);
    PrintMaze(stepName);
}
#endif