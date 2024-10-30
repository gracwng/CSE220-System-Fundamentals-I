#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "hw3.h"

#define DEBUG(...)                              \
    fprintf(stderr, "[          ] [ DEBUG ] "); \
    fprintf(stderr, __VA_ARGS__);               \
    fprintf(stderr, " -- %s()\n", __func__)

// stack data structure
typedef struct Stack
{
    GameState *value;
    struct Stack *next;
} Stack;

Stack *stack = NULL;
int wordMatrixCreated = 0;
int publicWordHeight = 0;
char **globalWordMatrix = NULL;
// variable to keep score of the size of the stack
int stackSize = 0;

int **createIntMatrix(int row, int col)
{
    int **matrix = calloc(row, sizeof(int *));
    for (int i = 0; i < row; i++)
    {
        matrix[i] = calloc(col, sizeof(int));
    }
    return matrix;
}
char **createCharMatrix(int row, int col)
{
    char **matrix = calloc(row, sizeof(char *));
    for (int i = 0; i < row; i++)
    {
        matrix[i] = calloc(col, sizeof(char));
    }
    return matrix;
}
void initializeIntMatrix(int **matrix, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            matrix[i][j] = 0;
        }
    }
}
void initializeCharMatrix(char **matrix, int row, int col, char initializer)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            matrix[i][j] = initializer;
        }
    }
}
void initializeCharArr(char *matrix, int arrLength)
{
    for (int i = 0; i < arrLength; i++)
    {
        matrix[i] = ' ';
    }
    matrix[arrLength - 1] = '\0';
}
void freeIntMatrix(int **matrix, int row)
{
    for (int i = 0; i < row; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}
void freeCharMatrix(char **matrix, int row)
{
    for (int i = 0; i < row; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}
void printIntMatrix(int **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d", matrix[i][j]);
        }
        printf("\n");
    }
}
void printCharMatrix(char **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%c", matrix[i][j]);
            // printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}
void copyIntoCharMatrix(char **dest, char **source, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        memcpy(dest[i], source[i], sizeof(char) * col);
    }
}
void copyIntoIntMatrix(int **dest, int **source, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        memcpy(dest[i], source[i], sizeof(int) * col);
    }
}
void stringCopy(const char *source, char *destination, int start, int length)
{
    int x = 0;
    for (int i = start; i < length + start; i++)
    {
        destination[x] = source[i];
        x++;
    }
    destination[length] = '\0';
}
void freeGameState(GameState *game)
{
    char **charMatrix = game->board;
    int **intMatrix = game->heights;
    // int ** intMatrix = *currGameState->heights;
    // int ** intMatrix = *currGameState.heights;
    // QUESTION: what's the difference between the three lines above?
    int row = game->row;
    // printCharMatrix(charMatrix, row, game->col);
    freeCharMatrix(charMatrix, row);
    freeIntMatrix(intMatrix, row);
    free(game);
}
// function to determine the number of lines in the words file and length of the longest word
void dimensionsOfWordFile(char *fileName, int *numWords, int *wordLength)
{
    FILE *file = fopen(fileName, "r");
    char word[30] = {0};
    int strLength = 0;
    *numWords = 0;
    while (fscanf(file, "%s", word) != EOF)
    {
        (*numWords)++;
        strLength = strlen(word);
        *wordLength = (*wordLength > strLength) ? *wordLength : strLength;
    }
    fclose(file);
}
// function to dynamically create the words matrix and read in the words too
char **createWordsMatrix(char *fileName, int *row, int *col)
{
    dimensionsOfWordFile(fileName, row, col);
    char **wordMatrix = createCharMatrix(*row, *col + 1);
    initializeCharMatrix(wordMatrix, *row, *col, '\0');

    // read in words into word matrix
    FILE *file = fopen(fileName, "r");
    for (int i = 0; i < *row; i++)
    {
        fscanf(file, "%s", wordMatrix[i]);
        // set the words read in to uppercase
        int j = 0;
        while (wordMatrix[i][j])
        {
            wordMatrix[i][j] = toupper(wordMatrix[i][j]);
            j++;
        }
    }
    fclose(file);
    // printCharMatrix(wordMatrix, *row, *col);
    // freeCharMatrix(wordMatrix, *row);
    return wordMatrix;
}
// search for a word in words matrix using binary search. returns 1 if found
int binarySearch(char **wordMatrix, const char *word, int left, int right)
{
    if (left > right)
        return 0;
    int mid = (left + right) / 2;
    const char *currWord = wordMatrix[mid];
    if (strcmp(word, currWord) == 0)
        return 1;
    else if (strcmp(word, currWord) < 0)
        return binarySearch(wordMatrix, word, left, mid - 1);
    else
        return binarySearch(wordMatrix, word, mid + 1, right);
}

// insert to top of stack
void push(Stack **s, GameState *g)
{
    Stack *newStack;
    newStack = malloc(sizeof(Stack));
    if (newStack == NULL)
    {
        printf("Error: malloc failed in add_to_list\n");
        exit(EXIT_FAILURE);
    }
    newStack->value = g;
    newStack->next = *s;
    *s = newStack;
    stackSize++;
}

// pop from stack
void pop()
{
    if (stack == NULL)
    {
        printf("Error: stack is empty\n");
        exit(EXIT_FAILURE);
    }
    Stack *pointerToStack = stack;
    stack = stack->next;
    Stack *pointerToNext = stack;
    (void)pointerToNext;
    // free gamestate in current stack and free stack node itself
    GameState *currGameState = pointerToStack->value;
    freeGameState(currGameState);
    free(pointerToStack);
}

void initializeGameState(GameState *g, char **board, int **height, int row, int col)
{
    // QUESTION: what's happening behind the scenes for the board and height?
    g->board = board;
    g->heights = height;
    g->row = row;
    g->col = col;
}

// flag to check if we are starting with an empty board (0 = empty board, 1 = non-empty board)
int emptyBoard = 0;
GameState *initialize_game_state(const char *filename)
{
    // do 1 scan of the entire file to determine the size of the rows and cols
    int row = 0;
    int col = 0;
    FILE *file = fopen(filename, "r");

    char c;
    int flag = 0;
    while ((c = fgetc(file)) != EOF)
    {
        if (flag == 0 && c != '\n')
        {
            col++;
        }
        if (c == '\n')
        {
            flag = 1;
            row++;
        }
    }
    fclose(file);

    // dynamically allocate memory for matrices
    char **charMatrix = createCharMatrix(row, col);
    int **intMatrix = createIntMatrix(row, col);
    // iterate through file again and now actually read in the information into the matrices
    file = fopen(filename, "r");
    // int count = 0;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            c = fgetc(file);
            if (c == '\n')
            {
                j--;
            }
            else
            {
                charMatrix[i][j] = c;
                if (c != '.')
                {
                    intMatrix[i][j] = 1;
                }
                else
                {
                    intMatrix[i][j] = 0;
                }
            }
        }
    }
    // printf("count: %d", count);
    // if (initialEmptyBoard == 0) emptyBoard = 0;
    fclose(file);
    // crete a gamestate instance and push onto stack
    GameState *initial = malloc(sizeof(GameState));
    initializeGameState(initial, charMatrix, intMatrix, row, col);
    push(&stack, initial);
    return initial;
}
// function that tells us how many characters are in our tiles
int numCharInTiles(const char *tiles)
{
    const char *pointer = tiles;
    int counter = 0;
    while (*pointer)
    {
        if (*pointer != ' ')
        {
            counter++;
        }
        pointer++;
    }
    return counter;
}

// this function gives us the updated dimensions of the row and col depending on the length of the tile
int getNewDimensions(int currStartingPoint, int currDimension, int tilesLength)
{
    return ((currStartingPoint + tilesLength > currDimension) ? currStartingPoint + tilesLength : currDimension);
}

// function to add tiles to board (will adjust the size too) and creates a new gamestate (could be invalid)
void createGameState(GameState *game, GameState **newGame, int row, int col, char direction, const char *tiles, int tilesLength)
{
    char **charMatrix = game->board;
    int **intMatrix = game->heights;
    int currRow = game->row;
    int currCol = game->col;
    *newGame = malloc(sizeof(GameState));
    int newRow;
    int newCol;
    // first create the size of the new board
    // then initialize to 0 or .
    // then copy values from curr game to new game
    if (tolower(direction) == 'h')
    {
        newCol = getNewDimensions(col, currCol, tilesLength);
        // newCol = (col+tilesLength > currCol) ? col + tilesLength : currCol;
        newRow = currRow;
    }
    else
    {
        newRow = getNewDimensions(row, currRow, tilesLength);
        // newRow = (row+tilesLength > currRow) ? row + tilesLength : currRow;
        newCol = currCol;
    }
    // create and initialize char and int matrices
    char **newCharMatrix = createCharMatrix(newRow, newCol);
    int **newIntMatrix = createIntMatrix(newRow, newCol);
    initializeCharMatrix(newCharMatrix, newRow, newCol, '.');
    initializeIntMatrix(newIntMatrix, newRow, newCol);
    copyIntoCharMatrix(newCharMatrix, charMatrix, currRow, currCol);
    copyIntoIntMatrix(newIntMatrix, intMatrix, currRow, currCol);

    // then add new tiles onto the board depending on the direction:
    const char *pointer = tiles;
    if (tolower(direction) == 'h')
    {
        for (int i = col; i < col + tilesLength; i++)
        {
            if (*pointer != ' ')
            {
                newCharMatrix[row][i] = *pointer;
                newIntMatrix[row][i]++;
            }
            pointer++;
        }
    }
    else
    {
        for (int i = row; i < row + tilesLength; i++)
        {
            if (*pointer != ' ')
            {
                newCharMatrix[i][col] = *pointer;
                newIntMatrix[i][col]++;
            }
            pointer++;
        }
    }
    // printCharMatrix(newCharMatrix, newRow, newCol);
    // printIntMatrix(newIntMatrix, newRow, newCol);
    initializeGameState(*newGame, newCharMatrix, newIntMatrix, newRow, newCol);
}

// function to check if the tiles we want to add are connected to other tiles
int tileConnected(GameState *game, int row, int col, char direction, int wordStart, int wordLength)
{
    char **charMatrix = game->board;
    int currRow = game->row;
    int currCol = game->col;
    int flag = 0;
    (void)wordLength;

    if (row < 0 || col < 0 || row >= currRow || col >= currCol)
    {
        return flag; // Return 0 if row or column is out of bounds
    }
    if (tolower(direction) == 'h')
    { // Horizontal direction
        int upperBound = (col + wordStart + wordLength < currCol) ? col + wordStart + wordLength : currCol;
        int initial = col + wordStart;
        for (int i = initial; i <= upperBound && flag == 0; i++)
        {
            if (i >= 0 && i < currCol && charMatrix[row][i] != '.')
                flag = 1; // Tile itself
            else if (i - 1 >= 0 && charMatrix[row][i - 1] != '.')
                flag = 1; // Left neighbor
            else if (i + 1 < currCol && charMatrix[row][i + 1] != '.')
                flag = 1; // Right neighbor
            else if (row - 1 >= 0 && i < currCol && charMatrix[row - 1][i] != '.')
                flag = 1; // Upper neighbor
            else if (row + 1 < currRow && i < currCol && charMatrix[row + 1][i] != '.')
                flag = 1; // Lower neighbor
        }
    }
    else
    { // Vertical direction
        int initial = row + wordStart;
        for (int i = initial; i <= currRow && flag == 0; i++)
        {
            if (i >= 0 && i < currRow && charMatrix[i][col] != '.')
                flag = 1; // Tile itself
            else if (i - 1 >= 0 && charMatrix[i - 1][col] != '.')
                flag = 1; // Upper neighbor
            else if (i + 1 < currRow && charMatrix[i + 1][col] != '.')
                flag = 1; // Lower neighbor
            else if (col - 1 >= 0 && i < currRow && charMatrix[i][col - 1] != '.')
                flag = 1; // Left neighbor
            else if (col + 1 < currCol && i < currRow && charMatrix[i][col + 1] != '.')
                flag = 1; // Right neighbor
        }
    }
    return flag;
}

// function to check if tiles connect but also handles case if our tiles string contains multiple substrings
int allTilesConnected(GameState *game, int row, int col, char direction, const char *tiles, int tilesLength)
{

    for (int i = 0; i < tilesLength; i++)
    {
        if (isalpha(tiles[i]))
        {
            int wordStart = i;
            while (isalpha(tiles[i]))
            {
                i++;
            }
            int wordLength = i - wordStart;
            if (wordLength > 1)
            {
                char *currWord = malloc(wordLength + 1);
                stringCopy(tiles, currWord, wordStart, wordLength);
                currWord[wordLength] = '\0';
                // now check if there are connecting words around our substring
                int isConnected = tileConnected(game, row, col, direction, wordStart, wordLength);
                if (isConnected == 0)
                {
                    free(currWord);
                    return 0;
                }
                free(currWord);
            }
        }
    }
    // check if that word is connected
    return 1;
}

// checks if our substring token is its own separate word. if it is its own word, then return 1 else 0
int isOwnWord(char **charMatrix, char *word, int wordLength, int row, int col, int currRow, int currCol, char direction)
{
    if (tolower(direction) == 'h')
    {
        // get pointer to the currentRow
        char *rowString = charMatrix[row];
        char *pointerToFirstLetter = strstr(rowString, word);
        int indexOfFirstLetter = pointerToFirstLetter - rowString;
        int indexOfLastLetter = indexOfFirstLetter + wordLength - 1;
        // edge case this function doesn't handle: what if there are 2 of the same words in the same row/col
        if (indexOfFirstLetter - 1 < 0 || ((indexOfFirstLetter - 1 >= 0 && charMatrix[row][indexOfFirstLetter - 1] == '.') && (indexOfLastLetter + 1 < currCol && charMatrix[row][indexOfLastLetter + 1] == '.')) || indexOfLastLetter + 1 >= currCol)
            return 1;
    }
    // if our word is vertical
    else
    {
        // extract entire col into a horizontal array
        char *colString = malloc(sizeof(char) * currRow);
        for (int i = 0; i < currRow; i++)
        {
            colString[i] = charMatrix[i][col];
        }
        // find our word in there. index we gather represents row index
        char *pointerToFirstLetter = strstr(colString, word);
        int indexOfFirstLetter = pointerToFirstLetter - colString;
        int indexOfLastLetter = indexOfFirstLetter + wordLength - 1;
        // if (indexOfFirstLetter - 1 < 0 || (indexOfFirstLetter - 1 >= 0 && colString[indexOfFirstLetter - 1]  == '.') ||
        // indexOfLastLetter + 1 >= currRow || (indexOfLastLetter + 1 < currRow && colString[indexOfLastLetter + 1]  == '.')){
        if ((indexOfFirstLetter - 1 >= 0 && colString[indexOfFirstLetter - 1] == '.') &&
            (indexOfLastLetter + 1 < currRow && colString[indexOfLastLetter + 1] == '.'))
        {
            free(colString);
            return 1;
        }
        free(colString);
    }
    return 0;
}

// function to check if our tiles cover words
int wordCovered(GameState *game, char **wordMatrix, int wordHeight, int row, int col, char direction, const char *tiles, int tilesLength)
{
    char **charMatrix = game->board;
    int currRow = game->row;
    int currCol = game->col;
    char *overlappedTiles = malloc(sizeof(char) * (tilesLength + 1));
    char *pointertoOverLapped = overlappedTiles;
    // feel like something weirds happening here bc when i initialize the array its bigger than the given size
    initializeCharArr(overlappedTiles, tilesLength + 1);
    // printf("%ld\n", strlen(overlappedTiles));
    if (tolower(direction) == 'h')
    {
        int upperBound = (col + tilesLength > currCol) ? currCol : col + tilesLength;
        // extract and copy letters that will be overlapped by our tiles
        for (int i = col; i < upperBound; i++)
        {
            if (charMatrix[row][i] != '.')
            {
                *pointertoOverLapped = charMatrix[row][i];
            }
            pointertoOverLapped++;
        }
        // printCharArr(overlappedTiles, tilesLength);
    }
    else
    {
        int upperBound = (row + tilesLength > currRow) ? currRow : row + tilesLength;
        // extract and copy letters that will be overlapped by our tiles
        for (int i = row; i < upperBound; i++)
        {
            if (charMatrix[i][col] != '.')
            {
                *pointertoOverLapped = charMatrix[i][col];
            }
            pointertoOverLapped++;
        }
        // printCharArr(overlappedTiles, tilesLength);
    }
    // check if words exist in the overlappedTiles array
    for (int i = 0; i < tilesLength; i++)
    {
        if (isalpha(overlappedTiles[i]))
        {
            int wordStart = i;
            while (isalpha(overlappedTiles[i]))
            {
                i++;
            }
            int wordLength = i - wordStart;
            if (wordLength > 1)
            {
                char *currWord = malloc(wordLength + 1);
                stringCopy(overlappedTiles, currWord, wordStart, wordLength);
                currWord[wordLength] = '\0';
                int isValidWord = binarySearch(wordMatrix, currWord, 0, wordHeight);
                if (isValidWord == 1)
                {
                    int ownWord = isOwnWord(charMatrix, currWord, strlen(currWord), row, col, currRow, currCol, direction);
                    if (ownWord == 1)
                    {
                        free(overlappedTiles);
                        free(currWord);
                        return 1;
                    }
                }
                free(currWord);
            }
        }
    }

    free(overlappedTiles);
    (void)tiles;
    return 0;
}

// handles cases if our tiles string contains spaces. break up the tiles string to check if any substrings of tiles is covering entire words. if yes, return 1
int wordCoveredForAllTiles(GameState *game, char **wordMatrix, int wordHeight, int row, int col, char direction, const char *tiles, int tilesLength)
{
    // char * copyOfTiles = malloc(sizeof(char) * (tilesLength + 1));
    char *copyOfTiles = malloc((tilesLength + 1));
    stringCopy(tiles, copyOfTiles, 0, tilesLength);
    int updatedRow = row;
    int updatedCol = col;
    char *token = strtok(copyOfTiles, " ");
    while (token != NULL && updatedCol <= col && updatedRow <= row)
    {
        if (strlen(token) > 1)
        {
            int tokenStartIndex = token - copyOfTiles;
            // if direction is horizontal, update the starting column, else update the starting row
            if (tolower(direction) == 'h')
                updatedCol += tokenStartIndex;
            if (tolower(direction) == 'v')
                updatedRow += tokenStartIndex;
            int wordCover = wordCovered(game, wordMatrix, wordHeight, updatedRow, updatedCol, direction, token, strlen(token));
            if (wordCover == 1)
            {
                free(copyOfTiles);
                return 1;
            }
        }
        token = strtok(NULL, " ");
    }

    free(copyOfTiles);
    return 0;
}
// checks if a given row/column contains valid words. returns 0 if its not valid, returns 1 if it is valid
int isValidRowOrColumn(char *string, int strLength, char **wordMatrix, int wordHeight)
{
    // makes a copy of entire string then tokenizes it based on .
    // if string is greater than size of 1, check if its valid. if its not, return 0
    int flag = 1;
    for (int i = 0; i < strLength; i++)
    {
        if (isalpha(string[i]))
        {
            int wordStart = i;
            while (isalpha(string[i]))
            {
                i++;
            }
            int wordLength = i - wordStart;
            if (wordLength > 1)
            {
                // allocate memory for the word and copy it to the output string
                char *currWord = malloc(wordLength + 1);
                stringCopy(string, currWord, wordStart, wordLength);
                currWord[wordLength] = '\0';
                int isValidWord = binarySearch(wordMatrix, currWord, 0, wordHeight);
                int potentialPluralValidWord = 0;
                char *potentialPlural = NULL;
                if (isValidWord == 0)
                {
                    // HAVE TO HANDLE POTENTIAL PLURALS
                    if (currWord[wordLength - 1] == 'S')
                    {
                        potentialPlural = malloc(wordLength);
                        stringCopy(string, potentialPlural, wordStart, wordLength - 1);
                        currWord[wordLength] = '\0';
                        potentialPluralValidWord = binarySearch(wordMatrix, potentialPlural, 0, wordHeight);
                    }
                }
                if (isValidWord == 0 && potentialPluralValidWord == 0)
                {
                    flag = 0;
                }
                free(currWord);
                if (potentialPlural != NULL)
                {
                    free(potentialPlural);
                }
            }
        }
    }

    return flag;
}

// this function checks if the board we are given is valid based on the new word we added. 1 means it's not valid, 0 means it is
int boardIsValid(GameState *game, char **wordMatrix, int wordHeight, int row, int col, char direction, const char *tiles, int tilesLength)
{
    char **charMatrix = game->board;
    int currRow = game->row;
    int currCol = game->col;
    // based on direction, check if the row/col we added our new word to is valid
    if (tolower(direction) == 'h')
    {
        // extract the entire row of where our new word is
        char *entireRow = malloc(sizeof(char) * (currCol + 1));
        entireRow[currCol] = '\0';
        // strncpy(entireRow, charMatrix[row], currRow);
        stringCopy(charMatrix[row], entireRow, 0, currCol);
        // printf("%s", entireRow);
        // now check if there are valid words inside. returns 0 if its not valid, returns 1 if it is valid
        int initialValid = isValidRowOrColumn(entireRow, currCol, wordMatrix, wordHeight);
        if (initialValid == 0)
        {
            free(entireRow);
            return 1;
        }
        free(entireRow);
        // now check all the columns that were affected
        char *entireCol = malloc(sizeof(char) * (currRow + 1));
        entireCol[currRow] = '\0';
        int column = col;
        for (int i = 0; i < tilesLength; i++)
        {
            if (tiles[i] != ' ')
            {
                // check validity of column corresponding to the letter of tiles
                // extract the column
                for (int j = 0; j < currRow; j++)
                {
                    entireCol[j] = charMatrix[j][column];
                }
                int isValid = isValidRowOrColumn(entireCol, currRow, wordMatrix, wordHeight);
                if (isValid == 0)
                {
                    free(entireCol);
                    return 1;
                }
            }
            column++;
        }
        free(entireCol);
    }
    else
    {
        char *entireCol = malloc(sizeof(char) * (currRow + 1));
        entireCol[currRow] = '\0';
        // copy column from matrix into array
        for (int i = 0; i < currRow; i++)
        {
            entireCol[i] = charMatrix[i][col];
        }
        // now check if there are valid words inside. returns 0 if its not valid, returns 1 if it is valid
        int initialValid = isValidRowOrColumn(entireCol, currRow, wordMatrix, wordHeight);
        if (initialValid == 0)
        {
            free(entireCol);
            return 1;
        }
        free(entireCol);
        // now check all the rows that were affected
        char *entireRow = malloc(sizeof(char) * (currCol + 1));
        entireRow[currCol] = '\0';
        int row1 = row;
        for (int i = 0; i < tilesLength; i++)
        {
            if (tiles[i] != ' ')
            {
                // check validity of column corresponding to the letter of tiles
                // extract the column
                for (int j = 0; j < currCol; j++)
                {
                    entireRow[j] = charMatrix[row1][j];
                }
                int isValid = isValidRowOrColumn(entireRow, currCol, wordMatrix, wordHeight);
                if (isValid == 0)
                {
                    free(entireRow);
                    return 1;
                }
            }
            row1++;
        }
        free(entireRow);
    }
    return 0;
}
// function to check if stack exceeds 5 tiles. returns 1 = stack does exceed
int stackExceeds(GameState *game, int row, int col, char direction, const char *tiles)
{
    // iterate through the characters that tiles would overlap and check if the height is already 5
    int **intMatrix = game->heights;
    int currRow = game->row;
    int currCol = game->col;
    const char *pointer = tiles;
    // flag to signify whether or not the height is over 5
    int flag = 0;

    // for each character that we iterate through, check surrounding characters depending on the direction
    if (tolower(direction) == 'h')
    {

        for (int i = col; i < currCol && flag == 0 && *pointer; i++)
        {
            if (*pointer != ' ' && intMatrix[row][i] > 4)
                flag = 1;
            pointer++;
        }
    }
    else
    {
        for (int i = row; i < currRow && flag == 0 && *pointer; i++)
        {
            if (*pointer != ' ' && intMatrix[i][col] > 4)
                flag = 1;
            pointer++;
        }
    }
    return flag;
}
// this function checks if we are covering the same letters. 1 if yes, 0 if no
int isCoveringSameLetter(GameState *game, int row, int col, char direction, const char *tiles, int tilesLength)
{
    char **charMatrix = game->board;
    int currRow = game->row;
    int currCol = game->col;
    const char *pointer = tiles;
    if (tolower(direction) == 'h')
    {
        int colLength = (col + tilesLength > currCol) ? currCol : col + tilesLength;
        for (int i = col; i < colLength; i++)
        {
            if (*pointer++ == charMatrix[row][i])
                return 1;
        }
    }
    else
    {
        int rowLength = (row + tilesLength > currRow) ? currRow : row + tilesLength;
        for (int i = row; i < rowLength; i++)
        {
            if (*pointer++ == charMatrix[i][col])
                return 1;
        }
    }
    return 0;
}

int containsOnlyDots(char **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (matrix[i][j] != '.')
            {
                return 0; // If any character is not '.', return 0 (false)
            }
        }
    }
    return 1; // If all characters are '.', return 1 (true)
}
int placedTile = 0;
GameState *place_tiles(GameState *game, int row, int col, char direction, const char *tiles, int *num_tiles_placed)
{
    char **charMatrix = game->board;
    // int ** intMatrix = game->heights;
    int currRow = game->row;
    int currCol = game->col;
    int numCharsInTiles = numCharInTiles(tiles);
    int tilesLength = strlen(tiles);
    *num_tiles_placed = numCharInTiles(tiles);
    int invalidity = 0;
    GameState *newGame = NULL;
    int wordLength = 0;
    // int wordHeight = 0;
    // char **wordMatrix = NULL;
    // int freedWordMatrixFlag = 0;
    int initialEmptyBoard = 1;
    // check invalidity:
    // row or col are invalid (i.e., row and/or col are less than 0; row ≥ the number of rows; col ≥ the number of columns)
    // direction is not one of 'H' or 'V'
    if (containsOnlyDots(charMatrix, currRow, currCol) == 1)
    {
        initialEmptyBoard = 0;
    }
    if (row < 0 || col < 0 || row >= currRow || col >= currCol || (tolower(direction) != 'h' && tolower(direction) != 'v') || (strlen(tiles) <= 1 && initialEmptyBoard == 0))
    {
        invalidity = 1;
    }
    // make sure we aren't covering the same letters
    if (isCoveringSameLetter(game, row, col, direction, tiles, tilesLength) == 1)
    {
        invalidity = 1;
    }

    // handle case if we are dealing with an initially empty board
    if (invalidity == 0 && initialEmptyBoard == 0 && stackSize == 1 && numCharsInTiles < 2)
    {
        invalidity = 1;
    }

    // create game state if we have a valid empty board
    if (invalidity == 0 && initialEmptyBoard == 0)
    {
        // call function to add to board. should be able to handle bigger sizes too
        createGameState(game, &newGame, row, col, direction, tiles, tilesLength);
        push(&stack, newGame);
    }
    // all other cases of adding to an existing board
    if (invalidity == 0 && initialEmptyBoard == 1)
    {
        // make sure the tiles placed are connected to an existing word. HANDLE CASE IF OUR TILES HAS SPACES
        int tilesAreConnected = allTilesConnected(game, row, col, direction, tiles, tilesLength);
        if (tilesAreConnected == 0)
            invalidity = 1;

        // make sure I'm not covering all the tiles of an existing word
        if (invalidity == 0)
        {
            // fill in the words array here
            if (globalWordMatrix == NULL || wordMatrixCreated == 0)
            {
                globalWordMatrix = createWordsMatrix("tests/words.txt", &publicWordHeight, &wordLength);
                wordMatrixCreated = 1;
            }
            int wordsCovered = wordCoveredForAllTiles(game, globalWordMatrix, publicWordHeight, row, col, direction, tiles, tilesLength);
            // if words are covered, free word matrix
            if (wordsCovered == 1)
            {
                invalidity = 1;
            }
        }
        if (invalidity == 0)
        {
            int stackExceed = stackExceeds(game, row, col, direction, tiles);
            if (stackExceed)
                invalidity = 1;
        }
        if (invalidity == 0)
        {
            createGameState(game, &newGame, row, col, direction, tiles, tilesLength);
            // check if the newly created word is legal and check surrounding row/tiles to check validity. 1 means it's not valid, 0 means it is
            int validWord = boardIsValid(newGame, globalWordMatrix, publicWordHeight, row, col, direction, tiles, tilesLength);
            if (validWord == 1)
            {
                invalidity = 1;
            }
            // if our board is still valid, then add board onto the stack
            if (invalidity == 0)
            {
                push(&stack, newGame);
                // freeCharMatrix(wordMatrix, wordHeight);
                // freedWordMatrixFlag = 1;
            }
        }
        if (invalidity == 1 && newGame != NULL)
        {
            freeGameState(newGame);
        }
    }
    // if we have an invalid case:
    if (invalidity == 1)
    {
        *num_tiles_placed = 0;
        return game;
    }
    placedTile = 1;

    return newGame;
}

GameState *undo_place_tiles(GameState *game)
{
    // call pop function
    if (placedTile == 1 && stackSize > 1)
    {
        pop();
        stackSize--;
    }
    (void)game;
    return stack->value;
}

void free_game_state(GameState *game)
{
    // call pop function
    while (stack != NULL)
    {
        pop();
    }
    placedTile = 0;
    // slight change: free the words matrix at the end of every game. makes it more efficient that freeing it every move
    if (wordMatrixCreated == 1 || globalWordMatrix != NULL)
    {
        freeCharMatrix(globalWordMatrix, publicWordHeight);
        globalWordMatrix = NULL;
        wordMatrixCreated = 0;
    }
    (void)game;
}

void save_game_state(GameState *game, const char *filename)
{
    char **charMatrix = game->board;
    int **intMatrix = game->heights;
    int row = game->row;
    int col = game->col;
    FILE *file = fopen(filename, "w");
    // print character matrix
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            fprintf(file, "%c", charMatrix[i][j]);
        }
        fprintf(file, "%c", '\n');
    }
    // print height matrix
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            fprintf(file, "%d", intMatrix[i][j]);
        }
        fprintf(file, "%c", '\n');
    }
    fclose(file);
}
