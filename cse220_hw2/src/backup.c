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
    extern int optind;
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

// function to print a matrix
void printMatrix(int **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
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
    int **matrix;
    // creates rows for our 2D array by storing a pointer to each row in the array
    matrix = malloc(sizeof(int *) * height);
    // allocate space for columns. for each pointer in the array, we will have it point to an array of ints
    for (int i = 0; i < height; i++)
    {
        // this allocates space for each row in our array
        matrix[i] = malloc(sizeof(int) * width);
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
    }
    free(matrix);
}

// function to free memory of 2D matrix (character matrix)
void freeCharMatrix(char **matrix, int height)
{
    for (int i = 0; i < height; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
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
        // printf("color table:\n");

        // for (int a = 0; a < numOfEntries; a++)
        // {
        //     for (int b = 0; b < 3; b++)
        //     {
        //         // printf("%d ", fullTable[i][j]);
        //         printf("%d ", colorTable[a][b]);
        //     }
        //     printf("\n");
        // }
        // read in the pixel and do something about parsing them
        // we will be reading in pixels as strings

        // char s[20] = "zzzzzzzzzzzzzzzzzzzz";
        char s[5] = "zzzzz";
        // int iteration = 0;
        int table_index;
        while (fscanf(fp, "%s", s) != EOF)
        // while (fgets(s, sizeof(s), fp) != NULL)
        {
            {
                // iteration++;
                // (void)iteration;
                // process each value
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

                    // // call function to handle adding color to final matrix
                    // for (long int i = 0; i < count; i++)
                    // {

                    //     int *rowPtr = colorTable[table_index];
                    //     for (int j = 0; j < 3; j++)
                    //     {
                    //         int x = *rowPtr++;
                    //         *fullTablePointer++ = x;

                    //         // *fullTablePointer++ = **colorTablePointer++;
                    //     }
                    // }
                }
                // this means we reached a regular number
                else
                {
                    char *temp = s;
                    returnSubstring(temp, &substring);

                    int count = 1;
                    // int length = strlen(substring);
                    table_index = (int)strtoul(substring, NULL, 10);

                    // printf("count: %d, table_index: %d\n", count, table_index); // Print count and table_index

                    // for (long int i = 0; i < count; i++)
                    // {

                    //     int *rowPtr = colorTable[table_index];
                    //     for (int j = 0; j < 3; j++)
                    //     {
                    //         int x = *rowPtr++;
                    //         *fullTablePointer++ = x;
                    //     }
                    // }
                    fullTablePointer = addColorToMatrix(count, colorTable, fullTablePointer, table_index);

                    // int ran = 0;
                    // if (fp == NULL)
                    //     {
                    //         ran = 1;
                    //         printf("error");

                    //     }
                    //     (void)ran;
                    // call function to handle adding color to final matrix
                }

                // s = NULL;
            }
        }

        freeMatrix(colorTable, numOfEntries);
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
    // Printing the matrix (just for verification)
    // int *p = fullTable;
    // printf("Matrix:\n");

    // for (int i = 0; i < height; ++i)
    // {
    //     for (int j = 0; j < width * 3; ++j)
    //     {
    //         // printf("%d ", fullTable[i][j]);
    //         printf("%d ", *p++);
    //     }
    //     printf("\n");
    // }
    fclose(fp);
    return fullTable;
}

// function to handle copying strings. returns the copied string
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
int getDimensionsOfValidMatrix(int cRow, int cCol, int cWidth, int cHeight, int eWidth, int eHeight, int *copyMatrix)
{
    // printf("%d %d", cRow, cCol);
    int copyA, copyB, copyC, copyD, validA, validB, validC, validD;
    // store upper left value index
    // copyA = m[(cRow * eWidth * 3) + (cCol * 3)];
    copyA = (cRow * eWidth * 3) + (cCol * 3);
    validA = copyA;
    // store upper right value index (includes right border index)
    copyB = copyA + (cWidth * 3);
    int startingIndexRowOfA = (int)(copyA / (eWidth * 3)) * (eWidth * 3);
    validB = startingIndexRowOfA + (eWidth * 3);
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
    copyMatrix[0] = copyRow;
    copyMatrix[1] = copyCol;

    // now check the bounds of the upper right and lower left. don't need to check lower right
    // printf("copyA: %d\n", copyA);
    // printf("copyB: %d\n", copyB);
    // printf("copyC: %d\n", copyC);
    // printf("copyD: %d\n", copyD);
    // printf("copyRow: %d\n", copyRow);
    // printf("copyCol: %d\n", copyCol);
    // printf("copyA: %d\n", m[copyA]);
    // printf("copyB: %d\n", m[copyB]);
    // printf("copyC: %d\n", m[copyC]);
    // printf("copyD: %d\n", m[copyD]);
    (void)copyA;
    (void)copyB;
    (void)copyC;
    (void)copyD;
    (void)validA;
    (void)validB;
    (void)validC;
    (void)validD;
    (void)eHeight;
    (void)distBtwnAB;
    (void)cHeight;
    return copyA;
}

// this function copies pixels into a matrix and returns the pointer to the dynamically created matrix
int **copyPixels(int cRow, int cCol, int cWidth, int cHeight, int *m, int eWidth, int eHeight, int *copiedHeight, int *dimensions)
{
    int *matrix = m;
    // 0: row/height 1: col/width (includes 3 integers for every pixel)
    // int dimensions[2] = {-1};
    // save starting index of where to start copying
    int startingIndex = getDimensionsOfValidMatrix(cRow, cCol, cWidth, cHeight, eWidth, eHeight, dimensions);
    // printf("copyPixel Dimensions: %d %d", dimensions[0], dimensions[1]);

    int **copiedMatrix = createMatrix(dimensions[1], dimensions[0]);
    *copiedHeight = dimensions[0];

    int copyRow = dimensions[0];
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
}
// paste function
void pastePixels(int pRow, int pCol, int pWidth, int pHeight, int *m, int eWidth, int eHeight, int **copyMatrix)
{
    // check the bounds of the pasted image compared to original table and update boundaries as needed (saving the new dimensions in an array)
    // 0: row/height 1: col/width (includes 3 integers for every pixel)
    int dimensions[2] = {-1};
    int startingIndex = getDimensionsOfValidMatrix(pRow, pCol, pWidth, pHeight, eWidth, eHeight, dimensions);
    int **copiedMatrix = copyMatrix;
    int *matrix = m;
    int pasteRow = dimensions[0];
    int pasteCol = dimensions[1];
    int *upperLeft = &m[startingIndex];

    // then iterate through new dimensions of copied image, and have pointer of full table at starting index, and start
    // replacing the full table with values from copied image
    for (int i = 0; i < pasteRow; i++)
    {
        matrix = upperLeft + (eWidth * 3) * i;
        for (int j = 0; j < pasteCol; j++)
        {
            *matrix++ = copiedMatrix[i][j];
        }
    }
}
// print function
// take an SBU file and convert it PPM. returns a new PPM file
// FILE *convertSBUtoPPM(FILE *SBUFile)
// {
//     if (SBUFile != NULL)
//         printf("Conversion successful! PPM file created.\n");
//     printf("Hello world");
//     const char *outputfile = "./src/standardizedPPM";
//     FILE *outputFile = fopen(outputfile, "w");
//     if (outputFile == NULL)
//     {
//         perror("Error opening file");
//         exit(EXIT_FAILURE);
//     }
//     return 0;
// }

// take a PPM file and convert it to an SBU file
// FILE *convertPPMtoSBU(FILE *PPMFile) {

// }
// void saveMatrixToFile(int *matrix, int width, int height)
// {
//     // Open the file for writing
//     FILE *file = fopen("tests/images/pastedDesert.ppm", "w");
//     if (file == NULL)
//     {
//         printf("Error opening file.\n");
//         return;
//     }

//     // Loop through each value in the matrix and save it to the file
//     for (int i = 0; i < width * height * 3; i++)
//     {
//         fprintf(file, "%d ", matrix[i]);
//     }

//     // Close the file
//     fclose(file);
// }

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
    // for (int i = 0; i < n; i++){
    //     fprintf(fp, "%d ", *matrix++);
    // }
    // *matrix = '\0';
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
        // add info on how many elements in color table
    }
}

// function to get the dimensions of a single letter based on the font file
void getFont1Dimensions(char *fontFilePath, int *dimensions, int fontSize)
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
        p[1] = 7;
        break;
    case 2:
        p[0] = 5;
        p[1] = 4;

        break;
    case 3:
        p[0] = 7;
        p[1] = 7;

        break;
    case 4:
        p[0] = 7;
        p[1] = 9;

        break;
    }
    // update the dimensions based on the font size
    // p[0] *= fontSize;
    // p[1] *= fontSize;
    (void)fontNum;
    (void)dimensions;
}

// function to read in font file and handles font size. returns final matrix of all letters
char** readFontFile (char **tempMatrix, int fontSize, char*fontFilePath, int tempRow, int tempCol, int spaceColumn){
    // char ** allLetters
    // read in each character but for every cahracter we read in, print them fontSize times. unless we 
    // are currently at a space column
    // space column = 
    for (int i = 0; i < tempCol; i++){

    }
}
// print message function
void printMessage(char *fontFilePath, char *message, int fontSize, int rRow, int rCol)
{
        // 0: row/height, 1: width/length
    int fontDimensions1[2] = {-1};
    // first figure out which font to use and what the dimensions of each letter is.
    getFont1Dimensions(fontFilePath, fontDimensions1, fontSize);
    int fontDimensionsWithFontSize[2] = {fontDimensions1[0] *fontSize, fontDimensions1[1] * fontSize};
    // use dimensions of letter to figure out the size of our matrix to store all letters
    // there will be one matrix that updates the column correctly.
    int tempRow = fontDimensions1[0];
    int tempCol = fontDimensionsWithFontSize[1] * 26;
    char **tempMatrix = createCharMatrix(tempCol, tempRow);
    int spaceColumn = fontDimensionsWithFontSize[1];
       // function to read in data file

    // then create another matrix to handle the increased row size

    int allLettersRow = fontDimensions1[0];
    int allLettersCol = fontDimensions1[1] * 26;
    char **allLetters = readFontFile(tempMatrix, fontSize, fontFilePath, tempRow, tempCol, spaceColumn);

    // function to store all letters. within function, handle font size
    // return all letters matrix, update dimensions of one letter matrix based on the font
    // function to iterate string (convert to lowercase), and copy and paste letter into image
    (void)message;
    (void)fontSize;
    (void)rRow;
    (void)rCol;
    freeCharMatrix(tempMatrix, tempRow);
    freeCharMatrix(allLetters, allLettersRow);
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

    // do the same for pasted and printed methods
    if ((int)pastedParameters[0] != -1)
    {
        // call pasted method
        pastePixels(pastedParameters[0], pastedParameters[1], dimensions[1] / 3, dimensions[0], matrix, width, height, copiedMatrix);
        //     freeMatrix(copiedMatrix, copyHeight);
        //    for (int i = 0; i < width*height*3; i++) {
        //     printf("%d ", matrix[i]);
        // }
        // printf("\n");
    }

    // handle print function
    if ((int)printedIntParameters[0] != -1)
    {
        printMessage(fontFilePath, message, printedIntParameters[0], printedIntParameters[1], printedIntParameters[2]);
    }
    // saveMatrixToFile(matrix, width, height);
    // defintitely have to save output
    writeAndSaveOutputFile(outputFilePath, outputFlag, matrix, width, height);

    // will have to write into a file based on the file extension (use strtok where our key is . and take the last token to check)
    free(matrix);
    (void)matrix;
    return 0;
}
