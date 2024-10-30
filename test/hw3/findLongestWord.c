// everything related to freeing

// stack data structure
typedef struct Stack {
    GameState* value;
    struct Stack *next;
} Stack;


typedef struct GameState
{
    char ** board;
    int ** heights;
    int row;
    int col;
} GameState;

Stack *stack = NULL;

void freeIntMatrix (int ** matrix, int row){
    for (int i = 0; i < row; i++){
        free(matrix[i]);
    }
    free(matrix);
}
void freeCharMatrix (char ** matrix, int row){
    for (int i = 0; i < row; i++){
        free(matrix[i]);
    }
    free(matrix);
}

void freeGameState(GameState *game){
    char ** charMatrix = game->board;
    int ** intMatrix = game->heights;
    // int ** intMatrix = *currGameState->heights;
    // int ** intMatrix = *currGameState.heights;
    // QUESTION: what's the difference between the three lines above?
    int row = game->row;
    printCharMatrix(charMatrix, row, game->col);
    freeCharMatrix(charMatrix, row);
    freeIntMatrix(intMatrix, row);
    free(game);
}

//pop from stack
void pop(){
    if (stack == NULL){
        printf("Error: stack is empty\n");
        exit(EXIT_FAILURE);  
    }
    Stack *pointerToStack = stack;
    stack = stack->next;

    // free gamestate in current node and free stack node itself
    GameState *currGameState = pointerToStack->value;
    freeGameState(currGameState);
    free(pointerToStack);
    }

void free_game_state(GameState *game) {
    // call pop function
    while (stack != NULL){
    pop();
    }
    // printCharMatrix(stack->value->board, stack->value->row, stack->value->col);
    placedTile = 0;
    (void)game;
}