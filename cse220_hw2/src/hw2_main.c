#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

// Part 1: read in command line input and check for errors
// we need pointers to input and output files, copied parameters (stored in unsigned long array), pasted parameters
// (stored in unsigned long array), printedMessage(string), font file, printed parameters (stored in unsigned long array)
void validateArgument(int argc, char **argv, char **inputFilePathFinal, char **outputFilePathFinal,
                      char **fontFilePathFinal, char **messageFinal, unsigned long *copiedParameters, unsigned long *pastedParameters,
                      unsigned long *printedIntParameters)
// void validateArgument(int argc, char **argv, FILE*inputFile, FILE*output, FILE*fontFile, unsigned long *copiedParameters, unsigned long *pastedParameters, unsigned long *printedIntParameters)
{
    // Each argument below must appear exactly once
    // -i: input file (string)
    // -o: output file (string)
    // -c: row (int), col(int) , width(int) , height (int)
    // -p: row (int), col (int)
    // -r: message(string), path to font (string), font size(int), row(int), col(int)

    extern char *optarg;
    // extern int optind;
    char *inputFilePath = NULL;
    char *outputFilePath = NULL;
    char *fontFilePath = NULL;
    char *message = NULL;
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;
    FILE *fontFile = NULL;

    // keeps entire string of of -c parameters
    char *copiedArg = NULL;
    // keeps entire string of of -p parameters
    char *pastedArg = NULL;
    // keeps entire string of of -r parameters
    char *printedArg = NULL;
    // 1 because we don't want to read the name of the command
    int c = 1;
    // unsigned long cRow, cCol, cWidth, cHeight, pRow, pCol, fontSize, rRow, rCol = 0;

    // char *message = NULL;
    // char *pathToFont = NULL;
    int invalidArg = 0;
    int unrecognizedArg = 0;

    int iFlag = 0;
    int oFlag = 0;
    int cFlag = 0;
    int pFlag = 0;
    int rFlag = 0;
    if (*argv[argc - 1] == '-')
        invalidArg = 1;
    while ((c = getopt(argc, argv, "i:o:c:p:r:")) != -1)
    {
        switch (c)
        {
        case ('i'):
            // read in the parameter for the input
            iFlag += 1;
            // read in the input file path
            inputFilePath = optarg;
            break;
        case ('o'):
            oFlag += 1;
            outputFilePath = optarg;
            break;
        case ('c'):
            cFlag += 1;
            copiedArg = optarg;
            // cRow = strtoul(optarg, NULL, 10);
            // cCol = strtoul(optarg, NULL, 10);
            // cWidth = strtoul(optarg, NULL, 10);
            // cHeight = strtoul(optarg, NULL, 10);
            break;
        case ('p'):
            pFlag += 1;
            pastedArg = optarg;
            // pRow = strtoul(optarg, NULL, 10);
            // pCol = strtoul(optarg, NULL, 10);
            break;
        case ('r'):
            rFlag += 1;
            printedArg = optarg;
            // fontSize = strtoul(optarg, NULL, 10);
            // rRow = strtoul(optarg, NULL, 10);
            // rCol = strtoul(optarg, NULL, 10);
            break;
        case ('?'):
            unrecognizedArg = 1;
            // c = -1;
            break;
        case ('-'):
            invalidArg = 1;
            // c = -1;
            break;
        case (':'):
            invalidArg = 1;
            c = -1;
            break;
        }
    }
    // missing argument 1: check if at least one of the -i and -o arguments is missing
    if (iFlag == 0 || oFlag == 0)
    {
        exit(MISSING_ARGUMENT);
    }
    // missing argument 2: check if the parameter is missing
    // FIX THIS: there could be the case where the command was given but nothing was inputted. this means output of getopt = ? so also check if unrecognizedArg = 1
    if (inputFilePath == NULL || *inputFilePath == '-' ||
        outputFilePath == NULL || *outputFilePath == '-' ||
        (copiedArg != NULL && *copiedArg == '-') ||
        (pastedArg != NULL && *pastedArg == '-') ||
        (printedArg != NULL && *printedArg == '-') ||
        invalidArg == 1)
    {
        exit(MISSING_ARGUMENT);
    }
    // An unrecognized argument was provided on the command line.
    if (unrecognizedArg == 1)
    {
        exit(UNRECOGNIZED_ARGUMENT);
    }
    // A recognized argument is provided two or more times.
    if (iFlag > 1 || oFlag > 1 || cFlag > 1 || pFlag > 1 || rFlag > 1)
    {
        exit(DUPLICATE_ARGUMENT);
    }
    // The named input file is missing or cannot be opened for reading for some reason.
    // first read in files
    inputFile = fopen(inputFilePath, "r");
    if (inputFile == NULL)
    {
        exit(INPUT_FILE_MISSING);
    }
    fclose(inputFile);
    // We cannot create the output file for writing for some reason.
    outputFile = fopen(outputFilePath, "w");
    if (outputFile == NULL)
    {
        // printf("%s\n", strerror(errno));
        exit(OUTPUT_FILE_UNWRITABLE);
    }
    fclose(outputFile);
    // The -p argument has been provided at least once, but the -c argument was not provided.
    if (pFlag > 0 && cFlag == 0)
    {
        exit(C_ARGUMENT_MISSING);
    }
    // Too many or too few parameters have been provided to the -c argument.
    // this condition checks if there even exists a -c command and that the parameters aren't empty
    if (cFlag == 1 && copiedArg != NULL && *copiedArg != '-')
    {
        // tokenize copiedArg first so that we can extract the individual fields from the single string
        // unsigned long copiedParameters[4] = {-1};
        // char printedParameters[5] = {'@'};
        char *token = copiedArg;
        token = strtok(copiedArg, ",");
        int countOfCopiedParameters = 0;
        // int countOfPastedParameters, countOfPrintedParameters = 0;

        while (token != NULL)
        {
            if (countOfCopiedParameters < 4)
                copiedParameters[countOfCopiedParameters] = strtoul(token, NULL, 10);
            ++countOfCopiedParameters;
            token = strtok(NULL, ",");
        }

        // printf("MY OWN TEST, %d\n", countOfCopiedParameters);
        // for (int i = 0; i < 4; i++)
        // {
        //     printf("%d) %lu\n", i, copiedParameters[i]);
        // }
        if (countOfCopiedParameters != 4)
        {
            exit(C_ARGUMENT_INVALID);
        }
    }

    // Too many or too few parameters have been provided to the -p argument.
    if (pFlag == 1 && pastedArg != NULL && *pastedArg != '-')
    {
        // unsigned long pastedParameters[2] = {-1};
        // tokenize copiedArg first so that we can extract the individual fields from the single string
        // char printedParameters[5] = {'@'};
        char *token = pastedArg;
        token = strtok(pastedArg, ",");
        int countOfPastedParameters = 0;
        // printf("garbage %lu", pastedParameters[0]);

        // int countOfPastedParameters, countOfPrintedParameters = 0;
        pastedParameters[0] = 0;
        while (token != NULL)
        {
            if (countOfPastedParameters < 2)
                pastedParameters[countOfPastedParameters] = strtoul(token, NULL, 10);
            ++countOfPastedParameters;
            token = strtok(NULL, ",");
        }
        if (countOfPastedParameters != 2)
        {
            exit(P_ARGUMENT_INVALID);
        }
    }
    // Too many or too few parameters have been provided to the -r argument, or the indicated font file could not be opened.
    // Assume that if the font file exists, then its contents are valid.
    if (rFlag == 1 && printedArg != NULL && *printedArg != '-')
    {
        // unsigned long printedIntParameters[3] = {0};

        char *token = printedArg;
        token = strtok(printedArg, ",");
        int countOfPrintedParameters = 0;
        // int countOfPastedParameters, countOfPrintedParameters = 0;
        int i = 0;
        while (token != NULL)
        {

            if (countOfPrintedParameters == 0)
            {
                message = token;
            }
            if (countOfPrintedParameters == 1)
            {
                fontFilePath = token;
            }
            if (1 < countOfPrintedParameters && countOfPrintedParameters < 5)
            {
                printedIntParameters[i++] = strtoul(token, NULL, 10);
            }
            ++countOfPrintedParameters;
            token = strtok(NULL, ",");
        }
        // printf("garbage %s", message);
        fontFile = fopen(fontFilePath, "r");

        if (countOfPrintedParameters != 5 || fontFile == NULL)
        {
            exit(R_ARGUMENT_INVALID);
        }
        fclose(fontFile);
    }
    (void)argc;                 // Marking argc as unused
    (void)argv;                 // Marking argv as unused
    (void)inputFilePathFinal;   // Marking inputFilePathFinal as unused
    (void)outputFilePathFinal;  // Marking outputFilePathFinal as unused
    (void)fontFilePathFinal;    // Marking fontFilePathFinal as unused
    (void)messageFinal;         // Marking messageFinal as unused
    (void)copiedParameters;     // Marking copiedParameters as unused
    (void)pastedParameters;     // Marking pastedParameters as unused
    (void)printedIntParameters; // Marking printedIntParameters as unused

    // now link pointers to filepaths
    *inputFilePathFinal = inputFilePath;
    *outputFilePathFinal = outputFilePath;
    *fontFilePathFinal = fontFilePath;
    *messageFinal = message;
}

// function to print a matrix of type int
void printMatrix(int **matrix, int rows, int cols)
{
    char a = 'a';
    for (int i = 0; i < rows; i++)
    {
        printf("%c %d: ", a + i, i);
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// print character matrix
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
        // printf("\n");
        // printf("%c", ' ');
    }
}
// function to check which file type our input file is, and function to check which file type our output file is
int fileType(char *filePath)
{
    // tokenize file path string and check if the last token starts with s or p. return 1 if p, return 0 if s
    // KEEP IN MIND THAT STRTOK CHANGES THE ACTUAL STRUCTURE OF FILEPATH
    int flag = 0;
    char *p1 = strtok(filePath, ".");
    char *p2 = strtok(NULL, ".");
    if (p2 != NULL && p2[0] == 'p')
    {
        flag = 1;
    }
    // printf("%s", p2);

    (void)p1;
    return flag;
}
// This function reads in the PPM and SBU files and store color table into 1D matrix
// parameters: takes in flag and filepath string, and a pointer to the matrix

// function to dynamically allocate memory for a 2D matrix
int **createMatrix(int width, int height)
{
    // dynamically allocate memory to fullTable

    // creates rows for our 2D array by storing a pointer to each row in the array
    int **matrix = (int **)malloc(sizeof(int *) * height);
    // allocate space for columns. for each pointer in the array, we will have it point to an array of ints
    for (int i = 0; i < height; i++)
    {
        // this allocates space for each row in our array
        matrix[i] = (int *)malloc(sizeof(int) * width);
    }

    return matrix;
}

// function to dynamically allocate memory for a 2D character matrix
char **createCharMatrix(int width, int height)
{
    // dynamically allocate memory to fullTable
    char **matrix;
    // creates rows for our 2D array by storing a pointer to each row in the array
    matrix = malloc(sizeof(char *) * height);
    // allocate space for columns. for each pointer in the array, we will have it point to an array of ints
    for (int i = 0; i < height; i++)
    {
        // this allocates space for each row in our array
        matrix[i] = malloc(sizeof(char) * width);
    }
    return matrix;
}

// function to free memory of 2D matrix
void freeMatrix(int **matrix, int height)
{
    for (int i = 0; i < height; i++)
    {
        free(matrix[i]);
        matrix[i] = NULL;
    }
    free(matrix);
    matrix = NULL;
}

// function to free memory of 2D matrix (character matrix)
void freeCharMatrix(char **matrix, int height)
{

    char c;
    for (int i = 0; i < height; i++)
    {
        // c = matrix[1][0];
        // printf("%c ", c);
        free(matrix[i]);
    }
    free(matrix);
    (void)c;
}

// function returns a substring based on a string. Used to handle special SBU pixel data
void returnSubstring(char *arrayOfAllCharacters, char **substring)
{
    int SIZEOFARRAY = 5;
    // traverse through array of all characters until we reach one named 'z' and keep track of a count variable
    long int count = 0;
    int flag = 0;
    int i = 0;
    if (*arrayOfAllCharacters == '*')
        flag = 1;
    for (i = 0; i < SIZEOFARRAY; i++)
    {
        if (arrayOfAllCharacters[i] != '\0' && arrayOfAllCharacters[i] != 'z' && arrayOfAllCharacters[i] != ' ' && arrayOfAllCharacters[i] != '*')
            // if (isdigit(arrayOfAllCharacters[i]))
            count++;
    }
    // then initalize a substring
    // THIS HAS TO BE DONE DYNAMICALLY
    *substring = malloc(sizeof(int) * (count + 1));
    // char sub[count];
    // traverse through the array count times and add elements into substring.
    i = (flag == 0) ? 0 : 1;
    count += (flag == 0) ? 0 : 1;
    int j = 0;
    while (i < count)
    {
        (*substring)[j++] = arrayOfAllCharacters[i++];
    }
    // DONT FORGET NULL TERMINATOR!
    (*substring)[j] = '\0'; // Null-terminate the string

    // reset long array to 'z'
    for (i = 0; i < SIZEOFARRAY; i++)
    {
        arrayOfAllCharacters[i] = 'z';
    }
}

// function to add colors from color table to matrix
int *addColorToMatrix(int count, int **colorTable, int *matrix, int table_index)
{
    int *p = matrix;
    for (int i = 0; i < count; i++)
    {
        int *rowPtr = colorTable[table_index];
        for (int j = 0; j < 3; j++)
        {
            // int x = *rowPtr++;
            // *p++ = x;

            *p++ = rowPtr[j];
        }
    }
    return p;
}
// function to read input files meaning storing pixel/RGB data into a matrix
int *readInputFiles(int flag, char *filePath, int *w, int *h)
{

    // open file
    FILE *fp = fopen(filePath, "r");
    // read in first 4 tokens of the file no matter the type
    int width;
    int height;
    int numOfEntries;
    // have to dynamically allocate memory to matrix
    // int colorTable[numOfEntries][3];
    // read in the width height and numOfEntries
    fscanf(fp, "%*s %d %d %d", &width, &height, &numOfEntries);
    *w = width;
    *h = height;

    // dynamically allocate memory to fullTable
    // int **fullTable = createMatrix(width*3, height);
    int *fullTable = malloc(sizeof(int) * width * height * 3);
    int **colorTable = createMatrix(3, numOfEntries);

    // int **colorTable = (int **)malloc(sizeof(int *) * numOfEntries);
    // // allocate space for columns. for each pointer in the array, we will have it point to an array of ints
    // for (int i = 0; i < numOfEntries; i++)
    // {
    //     // this allocates space for each row in our array
    //     colorTable[i] = (int *)malloc(sizeof(int) * 3);
    // }

    // point to first row of color table
    // int **p = colorTable;
    int *fullTablePointer = fullTable; // int fullTable[height][width * 3];
    // int lengthOfColorTable = numOfEntries * 3;
    // if flag is 0, then handle case of SBU file
    switch (flag)
    {
        // if flag is 0, handle case of SBU file
    case 0:
        // read in color table
        for (int a = 0; a < numOfEntries; a++)
        {
            for (int b = 0; b < 3; b++)
            {

                fscanf(fp, "%d", &colorTable[a][b]);
            }
        }

        // char s[20] = "zzzzzzzzzzzzzzzzzzzz";
        char s[5] = "zzzzz";
        // int iteration = 0;
        int table_index;
        while (fscanf(fp, "%s", s) != EOF)
        // while (fgets(s, sizeof(s), fp) != NULL)
        {
            {

                char *substring = NULL;
                // check case if we reach a * character
                if (*s == '*')
                {
                    // this function returns the integer portion of the string if there's a star at the front
                    returnSubstring(s, &substring);
                    // printf("%s\n", substring);
                    // this converts the substring into an integer
                    int count = strtol(substring, NULL, 10);
                    free(substring);
                    // count = leftmostDigit(count);
                    fscanf(fp, "%d", &table_index);
                    // printf("count: %d, table_index: %d\n", count, table_index); // Print count and table_index
                    fullTablePointer = addColorToMatrix(count, colorTable, fullTablePointer, table_index);
                }
                // this means we reached a regular number
                else
                {
                    char *temp = s;
                    returnSubstring(temp, &substring);

                    int count = 1;
                    // int length = strlen(substring);
                    table_index = (int)strtoul(substring, NULL, 10);
                    fullTablePointer = addColorToMatrix(count, colorTable, fullTablePointer, table_index);
                    free(substring);
                }

                // s = NULL;
            }
        }
        fullTablePointer = NULL;
        break;
    // if flag is 1, handle case of PPM file
    case 1:
        // read values into full table directly
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width * 3; j++)
            {
                // fscanf(fp, "%d", &fullTable[i][j]);
                fscanf(fp, "%d", fullTablePointer++);
            }
        }
        break;
    default:
        printf("There's an error in reading in input files");
        break;
    }

    fclose(fp);
    freeMatrix(colorTable, numOfEntries);

    return fullTable;
}

// function to handle copying strings. returns the copied string
// STRDUP does this for me
char *copiedString(char *inputString)
{
    int length = strlen(inputString);
    // dynamically allocats memory to copied string
    char *copied = malloc((length + 1) * sizeof(char));
    if (copied == NULL)
    {
        printf("copied memory is null");
        return NULL;
    }
    strncpy(copied, inputString, length);
    copied[length] = '\0';
    return copied;
}
// copy pixels function

// returns the starting index of where to start copying (upper left)
// remember m is a single dimensional array
int getDimensionsOfValidMatrix(int cRow, int cCol, int cWidth, int cHeight, int eWidth, int eHeight, int *copyMatrix, int *flag)
{
    // printf("%d %d", cRow, cCol);
    int copyA, copyB, copyC, copyD, validA, validB, validC;
    // store upper left value index
    // copyA = m[(cRow * eWidth * 3) + (cCol * 3)];
    copyA = (cRow * eWidth * 3) + (cCol * 3);
    validA = copyA;
    // store upper right value index (includes right border index)
    copyB = copyA + (cWidth * 3);
    int startingIndexRowOfA = (int)((int)((copyA / (eWidth * 3))) * (eWidth * 3));
    validB = startingIndexRowOfA + (eWidth * 3);
    // simple check to see if the upper right edge will be changed
    if (copyB > validB)
        *flag = 1;
    copyB = (copyB >= validB) ? validB : copyB;
    // copyB = copyA + m[(cWidth * 3)];
    // store lower left value index
    // copyC = copyA + ((eHeight - 1) * eWidth * 3);
    copyC = copyA + ((cHeight - 1) * eWidth * 3);
    int startingIndexRowOfCopyC = (int)(copyC / (eWidth * 3)) * (eWidth * 3);
    validC = ((eHeight - 1) * eWidth * 3) + (cCol * 3);
    // validC = ((eHeight - 1) * eWidth * 3) + (cCol * 3);
    int startingIndexRowOfValidC = (int)(validC / (eWidth * 3)) * (eWidth * 3);
    copyC = (startingIndexRowOfCopyC >= startingIndexRowOfValidC) ? validC : copyC;
    // copyC = copyA + m[((cHeight - 1) * eWidth * 3)];
    // store lower right value index
    // distance gives us the number of columns in our matrix
    int distBtwnAB = copyB - copyA;
    copyD = copyC + distBtwnAB;
    // validD = copyC + distBtwnAB;
    // copyD = copyC + m[(cWidth * 3)];

    int rowOfA = (int)(copyA / (eWidth * 3));
    int rowOfC = (int)(copyC / (eWidth * 3));
    int copyRow = (rowOfC - rowOfA) + 1;
    int copyCol = (copyB - copyA);

    // this is where I store the dimensions
    copyMatrix[0] = copyRow;
    // 1 pixel = 3 numbers
    copyMatrix[1] = copyCol;

    // now check the bounds of the upper right and lower left. don't need to check lower right

    // (void)copyA;
    // (void)copyB;
    // (void)copyC;
    (void)copyD;
    (void)validA;
    // (void)validB;
    // (void)validC;
    // (void)validD;
    // (void)eHeight;
    // (void)distBtwnAB;
    // (void)cHeight;
    return copyA;
}

// MEMCPY/STRCOPY : MEMCPY tells the size of the elments of the array
// this function copies pixels into a matrix and returns the pointer to the dynamically created matrix
int **copyPixels(int cRow, int cCol, int cWidth, int cHeight, int *m, int eWidth, int eHeight, int *copiedHeight, int *dimensions)
{
    int dummy = 0;
    int *matrix = m;
    // 0: row/height 1: col/width (includes 3 integers for every pixel)
    // int dimensions[2] = {-1};
    // save starting index of where to start copying
    int startingIndex = getDimensionsOfValidMatrix(cRow, cCol, cWidth, cHeight, eWidth, eHeight, dimensions, &dummy);
    // printf("copyPixel Dimensions: %d %d", dimensions[0], dimensions[1]);

    int **copiedMatrix = createMatrix(dimensions[1], dimensions[0]);
    *copiedHeight = dimensions[0];

    int copyRow = dimensions[0];
    // width represents 3 elements per pixel
    int copyCol = dimensions[1];
    // iterate through size of copy matrix
    // have a pointer to the actual matrix values starting at the starting index

    // eWidth tells us total columms in a row, so for each row, add eWidth*3 to pointer to signifiy we are moving eWidth elements forward
    // matrix = &m[startingIndex];
    // everything below takes fullTable and adds it to 2D matrix
    int *upperLeft = &m[startingIndex];

    for (int i = 0; i < copyRow; i++)
    {
        matrix = upperLeft + (eWidth * 3) * i;
        for (int j = 0; j < (copyCol); j++)
        {
            copiedMatrix[i][j] = *matrix++;
        }
    }
    // printMatrix(copiedMatrix, copyRow, copyCol);
    return copiedMatrix;
    (void)dummy;
}
// paste function given an integer array
int pastePixels(int pRow, int pCol, int pWidth, int pHeight, int *m, int eWidth, int eHeight, int **copyMatrix)
{
    // check the bounds of the pasted image compared to original table and update boundaries as needed (saving the new dimensions in an array)
    // 0: row/height 1: col/width (includes 3 integers for every pixel)
    int dimensions[2] = {-1};
    int flag = 0;
    int startingIndex = getDimensionsOfValidMatrix(pRow, pCol, pWidth, pHeight, eWidth, eHeight, dimensions, &flag);
    int **copiedMatrix = copyMatrix;
    int *matrix = m;

    int pasteRow = dimensions[0];
    // width such that col/width = includes 3 integers for every pixel
    int pasteCol = dimensions[1];
    int *upperLeft = &m[startingIndex];

    // then iterate through new dimensions of copied image, and have pointer of full table at starting index, and start
    // replacing the full table with values from copied image

    // matrix refers to the full matrix
    for (int i = 0; i < pasteRow; i++)
    {
        matrix = upperLeft + (eWidth * 3) * i;
        // iterates through every column with the expectation that every pixel takes 3 elements
        for (int j = 0; j < pasteCol; j++)
        {
            *matrix++ = copiedMatrix[i][j];
        }
    }

    // return 1 if we had to change the upper right bound, else 0
    return flag;
}

// paste function given a letter matrix which only contains space and * values
// returns 1 if upper bound was changed else 0
int pastePixelsForLetter(int pRow, int pCol, int pWidth, int pHeight, int *m, int eWidth, int eHeight, char **letterMatrix, int *currentWidth)
{
    // check the bounds of the pasted image compared to original table and update boundaries as needed (saving the new dimensions in an array)
    // 0: row/height 1: col/width (includes 3 integers for every pixel)
    int dimensions[2] = {-1};
    int flag = 0;
    int startingIndex = getDimensionsOfValidMatrix(pRow, pCol, pWidth, pHeight, eWidth, eHeight, dimensions, &flag);
    int *matrix = m;
    if (flag == 1)
        return 1;

    int pasteRow = dimensions[0];
    // width such that the width represents 3 elements per pixel
    int pasteCol = dimensions[1];
    *currentWidth = pasteCol;
    int *upperLeft = &m[startingIndex];

    // then iterate through new dimensions of copied image, and have pointer of full table at starting index, and start
    // replacing the full table with values from copied image

    // matrix refers to the full matrix
    for (int i = 0; i < pasteRow; i++)
    {
        matrix = upperLeft + (eWidth * 3) * i;
        // iterates through every column with the expectation that every pixel takes 3 elements
        // we want to do something every 3 elements.
        for (int j = 0; j < pasteCol; j += 3)
        {
            // this handles every pixel we want to find
            // look at current value in letterMatrix. if it's a star, then set next 3 values in the matrix
            // to be 255 otherwise just skip them over
            char currentCharacter = letterMatrix[i][j / 3];
            if (currentCharacter == '*')
            {
                for (int k = 0; k < 3; k++)
                {
                    *matrix++ = 255;
                }
            }
            else
            {
                for (int k = 0; k < 3; k++)
                {
                    matrix++;
                }
            }
            // *matrix++ = letterMatrix[i][j];
        }
    }

    // return 1 if we had to change the upper right bound, else 0
    return flag;
}
// this method initializes a matrix to -1
void initializeMatrix(int **matrix, int width, int height)
{
    int **p = matrix;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            p[i][j] = -1;
        }
    }
    p = NULL;
}

// this method returns the index of the color we're looking for in the color table
int addToOrSearchColorTable(int **colorTable, int width, int height, int red,
                            int green, int blue, int *flagForNewColorAdded)
{
    int index = 0;
    int flag = 0;
    *flagForNewColorAdded = 0;

    // iterate through color table and check if our color is there
    for (int i = 0; (i < height) && (flag == 0); i++)
    {
        int tableRed = colorTable[i][0];
        int tableGreen = colorTable[i][1];
        int tableBlue = colorTable[i][2];
        // if all colors are equal, save index
        if (tableRed == red && tableGreen == green && tableBlue == blue)
        {
            flag = 1;
            index = i;
        }

        // if we reached the end of the color table, then we want to add the color to our table
        else if ((colorTable[i][0] == -1))
        {
            flag = 2;
            index = i;
            colorTable[i][0] = red;
            colorTable[i][1] = green;
            colorTable[i][2] = blue;
            *flagForNewColorAdded = 1;
        }
        // *sizeOfColorTable = i;
    }
    return index;
    (void)width;
}

// create color table and pixel data table. we just want to return the colortable, colortable size and
//  pixel data and pixel data size to actually write everything into the file
// return the size of the colortable
int createColorTableAndPixelDataTable(int *m, int **colorTable, int width, int height,
                                      int *pixelData)
{
    // iterate through m and add color to colorTable and update pixeld data for every iteration
    int sizeOfColorTable = 0;
    int flagForNewColorAdded = 0;
    int *matrix = m;
    int *pointerToPixelData = pixelData;
    for (int i = 0; i < width * height * 3; i += 3)
    {
        int red = *matrix++;
        int green = *matrix++;
        int blue = *matrix++;
        // pass these colors into addtoColorTable method and either find color or add it
        int index = addToOrSearchColorTable(colorTable, 3, width * height,
                                            red, green, blue, &flagForNewColorAdded);
        // add index to pixel array
        *pointerToPixelData++ = index;
        if (flagForNewColorAdded == 1)
            sizeOfColorTable += 1;
    }
    // set last value of pixel data to be -1 as a dummy value for adding in pixels into file
    *pointerToPixelData = -1;
    // get size of color table
    // sizeOfColorTable++;
    pointerToPixelData = pixelData;
    // for (int i = 0; i < height*width+1; i++){
    //     printf("%d ", *pointerToPixelData++);
    // }
    return sizeOfColorTable;
}

// handles iterating through the full table and adds info into the file
void saveMatrixToFile(char *outputFilePath, int *m, int width, int height)
{
    FILE *fp = fopen(outputFilePath, "a");
    // int n = width*height*3;
    int *matrix = m;
    for (int i = 0; i < height * width; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (j != 2)
                fprintf(fp, "%d ", *matrix++);
            else
                fprintf(fp, "%d\n", *matrix++);
        }
    }
    // fprintf(fp, "%c", ' ');
    fprintf(fp, "\n");
    // for (int i = 0; i < n; i++){
    //     fprintf(fp, "%d ", *matrix++);
    // }
    // *matrix = '\0';
    matrix = NULL;
    fclose(fp);
}

// function that writes into outputFilePath and saves it as an SBU or PPM file
void writeAndSaveOutputFile(char *outputFilePath, int flag, int *m, int width, int height)
{
    FILE *fp = fopen(outputFilePath, "w");
    char *header = (flag == 1) ? "P3" : "SBU";
    fprintf(fp, "%s\n%d %d\n", header, width, height);
    fclose(fp);
    // if flag = 0, then we're dealing with SBU file else, a PPM file
    if (flag == 1)
    {
        fp = fopen(outputFilePath, "a");
        fprintf(fp, "%d\n", 255);
        fclose(fp);
        saveMatrixToFile(outputFilePath, m, width, height);
    }
    else
    {
        // this means we will write to an SBU file
        // add info on how many elements in color table
        int **colorTable = createMatrix(3, width * height);
        initializeMatrix(colorTable, 3, width * height);

        int *pixelData = malloc((sizeof(int) * ((height * width) + 1)));

        int sizeOfColorTable = createColorTableAndPixelDataTable(m, colorTable, width, height, pixelData);
        // now start to acutally write to the file by reading in each number in the pixel data array
        fp = fopen(outputFilePath, "a");
        fprintf(fp, "%d\n", sizeOfColorTable);

        // code below prints the color table
        int flagForPrint = 0;

        for (int i = 0; (i < height * width) && (flagForPrint == 0); i++)
        {
            int tableRed = colorTable[i][0];
            int tableGreen = colorTable[i][1];
            int tableBlue = colorTable[i][2];
            if (tableRed == -1)
            {
                flagForPrint = 1;
            }
            else
                fprintf(fp, "%d %d %d ", tableRed, tableGreen, tableBlue);
        }
        // code below prints the pixel data
        int count = 1;
        // int i = 0;
        // pointer to pixel data
        int *p = pixelData;
        p++;
        int current = 0;
        while (*p != -1)
        {
            // check if value of current is equal to prev. if yes, update count
            int prevVal = *(p - 1);
            current = *p;
            if (current == prevVal)
            {
                count++;
            }
            // if current and prev are different, then update file
            else
            {
                // this means our previous number only appeared once. so update the written file
                if (count == 1)
                {
                    fprintf(fp, "%d ", prevVal);
                }
                else
                {
                    fprintf(fp, "*%d %d ", count, prevVal);
                }
                count = 1;
            }
            p++;
        }
        if (count == 1)
        {
            fprintf(fp, "%d ", current);
        }
        else
        {
            fprintf(fp, "*%d %d ", count, current);
        }

        fclose(fp);
        freeMatrix(colorTable, width * height);
        free(pixelData);
    }
    free(m);
}

// function to get the dimensions of input matrix based on the font file and returns font file number
int getFont1Dimensions(char *fontFilePath, int *dimensions)
{
    // figure out how to extract the number in the fontFilePath
    // first divide string based on slashes. then take the last substring, and divide by "."
    int *p = dimensions;
    char *copiedFontFilePath = copiedString(fontFilePath);
    char *token = copiedFontFilePath;
    token = strtok(copiedFontFilePath, "/");
    char *currentSubString = token;
    while (token != NULL)
    {
        currentSubString = token;
        token = strtok(NULL, "/");
    }
    char *font = strtok(currentSubString, ".");
    char fourthCharacterString[2] = {font[4], '\0'};
    int fontNum = strtoul(fourthCharacterString, NULL, 10);

    // based on the font file, dimensions stored as row/height and width/length
    switch (fontNum)
    {
    case 1:
        p[0] = 5;
        p[1] = 217;
        break;
    case 2:
        p[0] = 5;
        p[1] = 152;

        break;
    case 3:
        p[0] = 7;
        p[1] = 204;

        break;
    case 4:
        p[0] = 7;
        p[1] = 245;

        break;
    }
    // update the dimensions based on the font size
    // p[0] *= fontSize;
    // p[1] *= fontSize;
    free(copiedFontFilePath);
    return fontNum;
    (void)dimensions;
}

// function to read in font file returns final matrix of all letters
// char **readFontFile(char **inputMatrix, char *fontFilePath, int row, int col)
void readFontFile(char **inputMatrix, char *fontFilePath, int row, int col)
{
    FILE *fp = fopen(fontFilePath, "r");
    char **matrix = inputMatrix;
    char val = 0;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            val = fgetc(fp);
            while (val == '\n')
            {
                val = fgetc(fp);
            }

            matrix[i][j] = val;
        }
    }
    fclose(fp);
    // return inputMatrix;
}

void findLetterBoundaries(int **columnBoundaries, char **inputMatrix, int inputRowSize, int inputColSize, int fontNum)
{
    // char * input = inputMatrix;
    // int * letterBoundaries = columnBoundaries;
    int row = 0;
    int flag;
    // issue is that every font file has a different beginning and ending spaces
    int start = 0;
    switch (fontNum)
    {
    case 1:
        start = 0;
        inputColSize = inputColSize;
        break;
    case 2:
        start = 0;
        inputColSize = inputColSize;
        break;
    case 3:
        start = 1;
        inputColSize = inputColSize + 1;
        break;
    case 4:
        start = 0;
        inputColSize = inputColSize;
        break;
    }
    // columnBoundaries[0][start] = 0;
    columnBoundaries[0][0] = start;
    // iterate through every column in input matrix
    for (int i = start; i < inputColSize; i++)
    {
        // iterate through every row in input matrix
        flag = 0;
        for (int j = 0; j < inputRowSize; j++)
        {
            // printf("%c ", inputMatrix[j][i+start]);
            //CHECK EDGE CASE
            // if (inputMatrix[j][i + start] == ' ')
            if ((i + start < inputColSize) && inputMatrix[j][i + start] == ' ')
                flag++;
        }
        // check edge case of font file 2 and update the boundaries of those letters. so if we are
        // between letters q and r
        if (flag == inputRowSize && row == 'q' - 'a' && fontNum == 2)
        {
            // columnBoundaries[row][1] = i - 1 + start;
            //  columnBoundaries[row + 1][0] = i + 2 + start;
            columnBoundaries[row][1] = i - 1;
            columnBoundaries[row + 1][0] = i + 2;
            row++;
            i++;
        }
        // this means our current column is a space column
        else if (flag == inputRowSize)
        {
            // update boundaries of our letters
            columnBoundaries[row][1] = i - 1 + start;
            columnBoundaries[row + 1][0] = i + 1 + start;
            row++;
        }
    }
}
// create letter matrix: font 1
// function that adds the pixel values to create the image of a single letter with font size 1
void fillInLetterMatrix(char **matrix, int rowSize, int colSize, char **allLetters, int characterNum, int **columnBoundaries)
{

    // iterate through row size and col size, and copy the values in the allLetters matrix into the single character matrix
    for (int i = 0; i < rowSize; i++)
    {
        int startColumn = columnBoundaries[characterNum][0];
        int endColumn = columnBoundaries[characterNum][1];
        int k = 0;
        for (int j = startColumn; j <= endColumn; j++)
        {
            matrix[i][k++] = allLetters[i][j];
        }
    }
    (void)colSize;

    // }
}
void printMatrixToFile(char **matrix, int numRows, int numCols, const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            fprintf(fp, "%c", matrix[i][j]);
        }
        fprintf(fp, "\n"); // Add newline after each row
    }

    fclose(fp);
}

// function to handle font size. given a font size and the matrix of the current letter, create and fill in new matrix of the bigger letter
// char **handleFontedLetters(int fontSize, char **smallMatrix, int smallRowSize, int smallColSize, int **bigMatrix)
void handleFontedLetters(int fontSize, char **smallMatrix, int smallRowSize, int smallColSize, char **bigMatrix)
{
    // int bigRowSize = smallRowSize * fontSize;
    // int bigColSize = smallColSize * fontSize;
    // char **bigMatrix = createCharMatrix(bigColSize, bigRowSize);
    int rowIndex = 0;
    int colIndex = 0;
    char **p = bigMatrix;
    // iterate through small matrix first
    for (int i = 0; i < smallRowSize; i++)
    {
        // rowIndex =
        colIndex = 0;
        //
        for (int j = 0; j < smallColSize; j++)
        {
            char val = smallMatrix[i][j];
            for (int l = 0; l < fontSize; l++)
            {
                p[rowIndex][colIndex++] = val;
            }
        }
        // after reading in first row, iterate fontSize - 1 times through big matrix to copy rows above
        int k = rowIndex + 1;
        int n = 0;
        while (n < fontSize - 1)
        {
            for (int m = 0; m < smallColSize * fontSize; m++)
            {
                p[k][m] = p[k - 1][m];
            }
            k++;
            n++;
        }
        rowIndex = k;
    }
}

// get specific letter matrix
// this method takes in the current character, the column boundaries, the rowSize of the fontFile
char **getLetterMatrix(int currentCharacter, int **columnBoundaries, int rowSize, int fontSize, char **inputMatrix, int *sizeOfRowOfLetterMatrix, int *sizeOfColOfLetterMatrix)
{
    //   int currentCharacter = 'a' - 'a';
    int startColumn = columnBoundaries[currentCharacter][0];
    int endColumn = columnBoundaries[currentCharacter][1];
    int colSize = endColumn - startColumn + 1;
    // create matrix for the single letter
    char **letterMatrix = createCharMatrix(colSize, rowSize);
    // add pixel values into the matrix
    fillInLetterMatrix(letterMatrix, rowSize, colSize, inputMatrix, currentCharacter, columnBoundaries);
    *sizeOfRowOfLetterMatrix = rowSize;
    *sizeOfColOfLetterMatrix = colSize;
    // printMatrixToFile(letterMatrix, rowSize, colSize, "output.txt");

    // if font size is greater than 1, then create new matrix to account for font size
    if (fontSize > 1)
    {
        int bigRowSize = rowSize * fontSize;
        int bigColSize = colSize * fontSize;
        *sizeOfRowOfLetterMatrix = bigRowSize;
        *sizeOfColOfLetterMatrix = bigColSize;
        char **bigMatrix = createCharMatrix(bigColSize, bigRowSize);
        // freeCharMatrix(bigMatrix, rowSize*fontSize);
        handleFontedLetters(fontSize, letterMatrix, rowSize, colSize, bigMatrix);

        freeCharMatrix(letterMatrix, rowSize);
        return bigMatrix;
    }
    else
    {
        // printCharMatrix(letterMatrix, rowSize, colSize);
        return letterMatrix;
    }
}

// print message function
void printMessage(char *fontFilePath, char *message, int fontSize, int rRow, int rCol, int eWidth, int eHeight, int *m)
{
    // function below reads in inputMatrix
    //  0: row/height, 1: width/length
    int dimensionsOfInputMatrix[2] = {-1};
    // first figure out which font to use and what the dimensions of each letter is.
    int fontNum = getFont1Dimensions(fontFilePath, dimensionsOfInputMatrix);
    char **inputMatrix = createCharMatrix(dimensionsOfInputMatrix[1], dimensionsOfInputMatrix[0]);
    // now read in values from font file:
    readFontFile(inputMatrix, fontFilePath, dimensionsOfInputMatrix[0], dimensionsOfInputMatrix[1]);
    // printCharMatrix(inputMatrix, dimensionsOfInputMatrix[0], dimensionsOfInputMatrix[1]);

    // find column boundaries for each letter
    // extra row for dummy value
    // int columnBoundaries[26+1][2] = {0};
    int **columnBoundaries = createMatrix(2, 26 + 2);
    findLetterBoundaries(columnBoundaries, inputMatrix, dimensionsOfInputMatrix[0], dimensionsOfInputMatrix[1], fontNum);
    // printMatrix(columnBoundaries, 27, 2);

    int fontFlag = (fontSize > 1) ? 1 : 0;

    // iterate through the string and store each character.
    // CHANGE Z TO ANY ALPHABET
    char *messagePointer = message;
    int rowSize = dimensionsOfInputMatrix[0];
    int changedLetterDimensions = 0;
    int currentStartRow = rRow;
    int currentStartCol = rCol;
    // while we haven't reached the end of the string
    while (*messagePointer != 0 && changedLetterDimensions == 0)
    {
        // HANDLE CASE IF THERE'S A SPACE
        int currentCharacter = tolower((*messagePointer)) - 'a';
        // if our current character is a space then we just want to skip it
        if (currentCharacter == -65)
        {
            currentStartCol += 5;
        }
        else
        {
            // gives us size of letter matrix given.
            int sizeOfRowOfLetterMatrix = 0;
            // tells us all the columns of pixels (not multiplied by 3)
            int sizeOfColOfLetterMatrix = 0;
            char **letterMatrix = getLetterMatrix(currentCharacter, columnBoundaries, rowSize, fontSize, inputMatrix, &sizeOfRowOfLetterMatrix, &sizeOfColOfLetterMatrix);

            // width of our current letter gives us all columns of pixels and their RGBs (multiplied by 3) (will be updated by pastePixelsForLetter)
            int currentWidth = 0;
            // figure out how to paste letter matrix into our finalArray. Be very careful about the parameters
            changedLetterDimensions = pastePixelsForLetter(currentStartRow, currentStartCol, sizeOfColOfLetterMatrix, sizeOfRowOfLetterMatrix, m, eWidth, eHeight, letterMatrix, &currentWidth);

            // now update the column number of the main table to move to the next space
            currentStartCol += (currentWidth / 3) + 1;
            // check if current start column is over the boundary of the size of the input file
            if (currentStartCol >= eWidth)
                changedLetterDimensions = 1;
            // code below is used to free my current letter matrix
            if (fontFlag == 1)
            {
                // freeCharMatrixtemp(fontedLetterMatrix, rowSize * fontSize, colSize * fontSize);
                freeCharMatrix(letterMatrix, rowSize * fontSize);
            }
            // if our letter matrix is the original
            else
            {
                freeCharMatrix(letterMatrix, rowSize);
            }
        }

        messagePointer++;
    }
    // function to iterate string (convert to lowercase), and copy and paste letter into image

    freeCharMatrix(inputMatrix, dimensionsOfInputMatrix[0]);

    // have conditions to check if we are dealing with font 1 or bigger. if bigger, we have to free more memory space, otherwise we just free the regular letter matrix

    freeMatrix(columnBoundaries, 26 + 2);

    // DONT FORGET TO FREE LETTER MATRIX
    // freeCharMatrix(letterMatrix, rowSize);

    (void)message;
    (void)fontSize;
    (void)rRow;
    (void)rCol;
}

int main(int argc, char **argv)
{

    (void)argc;
    (void)argv;
    // we need pointers to input and output files, copied parameters (stored in unsigned long array), pasted parameters
    // (stored in unsigned long array), printedMessage(string), font file, printed parameters (stored in unsigned long array)
    //
    char *inputFilePath = NULL;
    char *outputFilePath = NULL;
    char *fontFilePath = NULL;
    char *message = NULL;
    unsigned long copiedParameters[4] = {-1};
    unsigned long pastedParameters[2] = {-1};
    unsigned long printedIntParameters[3] = {-1};

    // handle part 1:
    validateArgument(argc, argv, &inputFilePath, &outputFilePath, &fontFilePath, &message, copiedParameters, pastedParameters, printedIntParameters);

    char *copiedInputFile = copiedString(inputFilePath);
    int intputFlag = fileType(copiedInputFile);
    free(copiedInputFile);
    char *copiedOutputFile = copiedString(outputFilePath);
    int outputFlag = fileType(copiedOutputFile);
    free(copiedOutputFile);

    int width = 0;
    int height = 0;
    // pass in flag into other functions so they know which type of file we are dealing with
    // store data inside a matrix
    int *matrix = readInputFiles(intputFlag, inputFilePath, &width, &height);

    // have a method to check if we need to handle copy, paste and print function
    // if the user gives us infomraiton in copiedParameters, then go into method and copy matrix.
    // valid matrix dimensions for copied image
    // 0: row/height 1: col/width (includes 3 integers for every pixel)
    int dimensions[2] = {-1};
    int **copiedMatrix;
    int copyHeight = 0;
    if ((int)copiedParameters[0] != -1)
    {
        // this means the user entered copy information
        copiedMatrix = copyPixels(copiedParameters[0], copiedParameters[1], copiedParameters[2],
                                  copiedParameters[3], matrix, width, height, &copyHeight, dimensions);
        // printf("Main Dimensions: %d %d", dimensions[0], dimensions[1]);
    }
    int flag = -1;
    // do the same for pasted and printed methods
    if ((int)pastedParameters[0] != -1)
    {
        // call pasted method
        flag = pastePixels(pastedParameters[0], pastedParameters[1], dimensions[1] / 3, dimensions[0], matrix, width, height, copiedMatrix);
        //     freeMatrix(copiedMatrix, copyHeight);
        //    for (int i = 0; i < width*height*3; i++) {
        //     printf("%d ", matrix[i]);
        // }
        // printf("\n");
        freeMatrix(copiedMatrix, dimensions[0]);
    }

    // handle print function
    if ((int)printedIntParameters[0] != -1)
    {
        printMessage(fontFilePath, message, printedIntParameters[0], printedIntParameters[1], printedIntParameters[2], width, height, matrix);
    }
    // saveMatrixToFile("output.txt", matrix, width, height);
    // defintitely have to save output
    writeAndSaveOutputFile(outputFilePath, outputFlag, matrix, width, height);

    // will have to write into a file based on the file extension (use strtok where our key is . and take the last token to check)
    // free(matrix);
    matrix = NULL;
    (void)flag;
    return 0;
}
