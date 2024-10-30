#include "hw4.h"

# define BOARD_SIZE 8
void initialize_game(ChessGame *game) {
    game->moveCount = 0;
    game->capturedCount = 0;
    game->currentPlayer = 0;

    // intialize the chess board

    // use mem copy
    // question: how come strncpy gives me the error of ‘strncpy’ output truncated before terminating nul copying 8 bytes from a string of the same length
    // strncpy(game->chessboard[0], "rnbqkbnr", 8);
    // strncpy(game->chessboard[1], "pppppppp", 8);
    // strncpy(game->chessboard[2], "........", 8);
    // for (int i = 3; i < 6; i++){
    // strncpy(game->chessboard[i], game->chessboard[2], 8);
    // }
    // strncpy(game->chessboard[6], "PPPPPPPP", 8);
    // strncpy(game->chessboard[7], "RNBQKBNR", 8);

     char initialBoard[BOARD_SIZE][BOARD_SIZE] = {
        "rnbqkbnr",
        "pppppppp",
        "........",
        "........",
        "........",
        "........",
        "PPPPPPPP",
        "RNBQKBNR"
    };
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->chessboard[i][j] = initialBoard[i][j];
        }
    }
    display_chessboard(game);
}
// write helper function to initialize buffer
void initializeBuffer (char * arr){
    for (int i = 0; i < BUFFER_SIZE; i++){
        arr[i] = '\0';
    }
}
void chessboard_to_fen(char fen[], ChessGame *game) {
    // logic: if we encounter a '.', then start counting how many of them occur and when we reach either the end of the row or 
    // reach another piece, then add the count to the string initialize fen with null terminators
    // for (int i = 0; i < BUFFER_SIZE; i++){
    //     fen[i] = '\0';
    // }
    initializeBuffer(fen);
    char * p = fen;
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            if (game->chessboard[i][j] != '.') {
                if (count > 0) {
                    *p++ = count + '0';
                    count = 0;
                    }
                *p++ = game->chessboard[i][j];
                }
            else {
                count++;
                if (j == BOARD_SIZE-1){
                    *p++ = count + '0';
                    count = 0;
                }
                }
        }
        if (i < BOARD_SIZE - 1)
            *p++ = '/';
    }
    *p++ = ' ';
    *p = (game->currentPlayer == 0) ? 'w' : 'b';

    char * x = fen;
    while (*x != '\0'){
        printf("%c", *x++);
    }
}

bool is_valid_pawn_move_helper(char piece, int src_row, int src_col, int dest_row, int dest_col, int start_row, ChessGame *game){
    // also check to make sure we aren't overlapping an existing pieces unless we are capturing and it's a piece from the opposing team
     // move 2 spaces at first move
        bool isBlack = (islower(piece)) ? true : false;
        if ((src_row - dest_row == 2 && src_col == dest_col && src_row == start_row && 
        ((!isBlack && game->chessboard[dest_row][dest_col] == '.' && game->chessboard[src_row-1][dest_col] == '.')
        || (isBlack && game->chessboard[src_row][src_col] == '.' && game->chessboard[src_row-1][dest_col] == '.'))) 
        // move one space up/down
        || (src_row - dest_row == 1 && src_col == dest_col && ((isBlack == false && game->chessboard[dest_row][dest_col] == '.') || 
        (isBlack && game->chessboard[src_row][src_col] == '.')))
        // move diagonal left
        || (src_col - dest_col == 1 && src_row - dest_row == 1 && ((isBlack == false && game->chessboard[dest_row][dest_col] != '.') || 
        (isBlack && game->chessboard[src_row][src_col] != '.')))
        // move diagonal right
        || (src_col - dest_col == -1 && src_row - dest_row == 1 && ((isBlack == false && game->chessboard[dest_row][dest_col] != '.') || 
        (isBlack && game->chessboard[src_row][src_col] != '.')))
        ) return true;
        else return false;
        (void) piece;
}
bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    // handling white pieces
    if (isupper(piece)){
        return is_valid_pawn_move_helper(piece, src_row, src_col, dest_row, dest_col, 6, game);
    }
    // handling black pieces
    else{
        return is_valid_pawn_move_helper(piece, dest_row, dest_col, src_row, src_col, 3, game);
    }
    (void)game;
    return false;
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    // if rook is moving horizontally
    if (src_row - dest_row == 0){
        // moving left to right
        if (dest_col - src_col > 0){
            // iterate through row from src_col to dest_col to make sure there aren't any pieces in between
            for (int i = src_col+1; i < dest_col; i++){
                if (game->chessboard[src_row][i] != '.')
                return false;
            }
        }
        // moving right to left
        else{
            for (int i = src_col-1; i > dest_col; i--){
                if (game->chessboard[src_row][i] != '.')
                return false;
            }
        }
        
        return true;
    }
    // if rook is moving vertically
    else if (src_col - dest_col == 0) {
        // moving top to bottom
        if (dest_row - src_row > 0){
            for (int i = src_row+1; i < dest_row; i++){
            if (game->chessboard[i][src_col] != '.')
            return false;
            }
        }
        // moving bottom to top
        else {
            for (int i = src_row-1; i > dest_row; i--){
            if (game->chessboard[i][src_col] != '.')
            return false;
            }
        }
        return true;
    }
    return false;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    if ((dest_row == src_row - 2 && dest_col == src_col + 1)
    || (dest_row == src_row - 2 && dest_col == src_col - 1)
    || (dest_row == src_row - 1 && dest_col == src_col + 2)
    || (dest_row == src_row - 1 && dest_col == src_col - 2)
    || (dest_row == src_row + 1 && dest_col == src_col + 2)
    || (dest_row == src_row + 1 && dest_col == src_col - 2)
    || (dest_row == src_row + 2 && dest_col == src_col + 1)
    || (dest_row == src_row + 2 && dest_col == src_col - 1)
    ) return true;
    return false;
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (abs(src_row - dest_row) != abs(src_col - dest_col)) return false;
    // dest in upper left
    if (src_row > dest_row && src_col > dest_col){
        for (int i = 1; i < abs(src_row - dest_row); i++){
            if (game->chessboard[dest_row + i][dest_col + i] != '.') return false;
        }
    }
    // dest in upper right
    else if (src_row > dest_row && src_col < dest_col){
        for (int i = 1; i < abs(src_row - dest_row); i++){
            if (game->chessboard[src_row - i][src_col + i] != '.') return false;
    } 
    }
    // dest in lower left
    else if (src_row < dest_row && src_col > dest_col){
        for (int i = 1; i < abs(src_row - dest_row); i++){
            if (game->chessboard[dest_row - i][dest_col + i] != '.') return false;
    } 
    }
    // dest in lower right
    else if (src_row < dest_row && src_col < dest_col){
        for (int i = 1; i < abs(src_row - dest_row); i++){
            if (game->chessboard[src_row + i][src_col + i] != '.') return false;
    } 
    }
    return true;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    return (is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game) || is_valid_rook_move(src_row, src_col, dest_row, dest_col, game));
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    if (abs(dest_row - src_row) <= 1 && abs(dest_col - src_col) <= 1) return true;
    return false;
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    // check if there is a piece at the given source location. make sure it's a valid piece depending on whose move it is 
    // strlen(piece + "") < 1
    if ( piece == '\0'|| src_row >= BOARD_SIZE || src_col >= BOARD_SIZE || dest_col >= BOARD_SIZE || dest_row >= BOARD_SIZE
    || src_row < 0 || src_col < 0 || dest_col < 0 || dest_row < 0 || game->chessboard[src_row][src_col] != piece 
    ) return false;

    // make sure we're moving the right player's piece
    if ((game->currentPlayer == 0 && islower(piece)) || (game->currentPlayer == 1 && isupper(piece))){
        return false;
    }
    // make sure if we're capturing an opposing piece, it's of the opposite type
    if ((game->currentPlayer == 0 && isupper(game->chessboard[dest_row][dest_col]))
    || (game->currentPlayer == 1 && islower(game->chessboard[dest_row][dest_col]))
    ) return false;

    if (piece == 'p' || piece == 'P') return (is_valid_pawn_move(piece, src_row, src_col, dest_row, dest_col, game));
    else if (piece == 'r' || piece == 'R') return (is_valid_rook_move(src_row, src_col, dest_row, dest_col, game));
    else if (piece == 'n' || piece == 'N') return (is_valid_knight_move(src_row, src_col, dest_row, dest_col));
    else if (piece == 'b' || piece == 'B') return (is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game));
    else if (piece == 'q' || piece == 'Q') return (is_valid_queen_move(src_row, src_col, dest_row, dest_col, game));
    else if (piece == 'k' || piece == 'K') return (is_valid_king_move(src_row, src_col, dest_row, dest_col));
    
    return false;
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    int i = 0;
    int j = 0;
    const char * p = fen;
    while (i < BOARD_SIZE){
        j = 0;
        while (j < BOARD_SIZE){
            // if it's a number
            if (*p >= '1' && *p <= '8'){
                for (int k = 0; k < *p - '0'; k++){
                    game->chessboard[i][j++] = '.';
                }
                p++;
            }
            else if (*p == '/'){
                p++;
            }
            // if it's a character letter
            else{
                game->chessboard[i][j++] = *p++;
            }
        }
        i++;
    }
    // change the turn of the player if needed
    p++;
    game->currentPlayer = (*p == 'w') ? 0 : 1;
    display_chessboard(game);
}

int parse_move(const char *move, ChessMove *parsed_move) {
    // Return this error code if the length of the move string is not equal to 4 or 5 characters.
    // Return this error code if the row letter is not in the range 'a' through 'h'.
    if ((strlen(move) != 4 && strlen(move) != 5) || move[0] < 'a' || move[0] > 'h' || move[2] < 'a' || move[2] > 'h' 
    // || (strlen(move) == 5 && move[4] < 'a') || (strlen(move) == 5 && move[0] > 'h')
    )
    return PARSE_MOVE_INVALID_FORMAT;
    // Return this error code if there is an attempt to move a piece outside of the 8x8 grid, or if the move reference a position outside the grid.
    else if (move[1] - '0' > 8 || move[1] - '0'< 1 || move[3] - '0' > 8 || move[3] - '0' < 1) return PARSE_MOVE_OUT_OF_BOUNDS;
    // This error case applies only to strings of length 5, which correspond with pawn promotions.
    // Return this error if the destination square is not on the appropriate row. Promotions for White must happen on row 8, and row 1 for Black.
    // QUESTION: what about the case where the pawn doesn't move rows?
    else if (strlen(move) == 5 && (move[3] != '8' && move[3] != '1')) return PARSE_MOVE_INVALID_DESTINATION;
    else if (strlen(move) == 5 && move[4] != 'q' && move[4] != 'r' && move[4] != 'b' && move[4] != 'n') return PARSE_MOVE_INVALID_PROMOTION;

    strncpy(parsed_move->startSquare, move, 2);
    parsed_move->startSquare[2] = '\0';
    if (strlen(move) == 4){
        strncpy(parsed_move->endSquare, move+2, 2);    
        parsed_move->endSquare[2] = '\0';
        parsed_move->endSquare[3] = '\0';
    }
    else{
        strncpy(parsed_move->endSquare, move+2, 3);
        parsed_move->endSquare[3] = '\0';
    }
    
    return 0;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    char * srcPosition = move->startSquare;
    int srcRow = BOARD_SIZE - (srcPosition[1] - '0');
    int srcCol = srcPosition[0] - 'a';
    char * destPosition = move->endSquare;
    int destRow = BOARD_SIZE - (destPosition[1] - '0');
    int destCol = destPosition[0] - 'a';
    char promotion = destPosition[2];

    if (validate_move){
        //  Moving out of turn is not allowed. If the client tries to move when it's the server's turn, or vice versa, return this error code. 
        if ((is_client && game->currentPlayer == 1) || (!is_client && game->currentPlayer == 0)) return MOVE_OUT_OF_TURN;
        //  Attempting to move from an empty square is not allowed. 
        if (game->chessboard[srcRow][srcCol] == '.') return MOVE_NOTHING;
        // Attempting to move the opponent's piece is not allowed. If the current player tries to move a piece of the opposite color, return this error code. 
        if((is_client && islower(game->chessboard[srcRow][srcCol])) || (!is_client && isupper(game->chessboard[srcRow][srcCol]))) return MOVE_WRONG_COLOR;
        // Capturing your own pieces is not allowed.
        if((is_client && isupper(game->chessboard[destRow][destCol])) || (!is_client && islower(game->chessboard[destRow][destCol]))) return MOVE_SUS;
        // The move string has a length of 5 characters (indicating a pawn promotion), but the piece at the start square is not a pawn ('P' or 'p'). You are not allowed to promote pieces that are not pawns. 
        if (promotion != '\0' && game->chessboard[srcRow][srcCol] != 'p' && game->chessboard[srcRow][srcCol] != 'P') return MOVE_NOT_A_PAWN;
        // The move string has a length of 4 characters, but the destination square is on the row for pawn promotion and the piece being moved is a pawn. 
        if (promotion == '\0' && (game->chessboard[srcRow][srcCol] == 'p' || game->chessboard[srcRow][srcCol] == 'P') && (destPosition[1] == '8' || destPosition[1] == '1')) return MOVE_MISSING_PROMOTION;
        // Return this error code if is_valid_move() returned false.
        if (!is_valid_move(game->chessboard[srcRow][srcCol], srcRow, srcCol, destRow, destCol, game)) return MOVE_WRONG;
    }
    // add move to moves list
    // game->moves[game->moveCount] = *move;
    (game->moveCount)++;
    if (game->chessboard[destRow][destCol] != '.'){
        // add captured piece into captured piece list
        game->capturedPieces[game->capturedCount] = game->chessboard[destRow][destCol];
        (game->capturedCount)++;
    }
    // move piece to destination position
    // handle edge case if the piece is a pawn that needs to be promoted
    if (promotion != '\0' && (game->chessboard[srcRow][srcCol] == 'p' || game->chessboard[srcRow][srcCol] == 'P') && (destPosition[1] == '8' || destPosition[1] == '1')){
        if (game->chessboard[srcRow][srcCol] == 'p') game->chessboard[srcRow][srcCol] = promotion;
        else if (game->chessboard[srcRow][srcCol] == 'P') game->chessboard[srcRow][srcCol] = toupper(promotion);
    }

    game->chessboard[destRow][destCol] = game->chessboard[srcRow][srcCol];
    game->chessboard[srcRow][srcCol] = '.';

    // update the current player field
    game->currentPlayer = (game->currentPlayer == 0) ? 1 : 0;
    return 0;
}


int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    char * messageCopy = malloc(strlen(message) + 1);
    char * p = messageCopy;
    strcpy(messageCopy, message);
    messageCopy[strlen(messageCopy)] = '\0';
    char * command = strtok(messageCopy, " ");
    char * arg = strtok(NULL, " ");
    // if (arg == NULL){
    // free(messageCopy);
    // return COMMAND_UNKNOWN;
    // }
    int returnVal = COMMAND_UNKNOWN;
    if (strcmp(command, "/move") == 0){
        // ChessMove *move;
        if (parse_move(arg, &(game->moves[game->moveCount])) != 0 || make_move(game, &(game->moves[game->moveCount]), is_client, true) != 0) returnVal = COMMAND_ERROR;
        else returnVal = COMMAND_MOVE;
    }
    else if (strcmp(message, "/forfeit") == 0){
        // send message over socket
        returnVal = COMMAND_FORFEIT;
    }
    else if (strcmp(message, "/chessboard") == 0){
        display_chessboard(game);
        returnVal = COMMAND_DISPLAY;
    }
    else if (strcmp(command, "/import") == 0 && is_client == false){
        fen_to_chessboard(arg, game);
        // call send to send a message over the socket
        returnVal = COMMAND_IMPORT;
    }
     else if (strcmp(command, "/load") == 0){
        // check if arguments are valid
        char * gameNum = strtok(NULL, " ");
        if (gameNum == NULL){
        free(p);
        return COMMAND_ERROR;
        }
        if (load_game(game, arg, "game_database.txt", atoi(gameNum)) == -1)  returnVal = COMMAND_ERROR;
        // call send to send message over the socket
        else returnVal = COMMAND_LOAD;
    }
    else if (strcmp(command, "/save") == 0){
        if (save_game(game, arg, "game_database.txt") == -1) returnVal = COMMAND_ERROR;

        else returnVal = COMMAND_SAVE;
    }

    else returnVal = COMMAND_UNKNOWN;

    if (!(strcmp(command, "/chessboard") == 0 || strcmp(command, "/save") == 0)){
        send(socketfd, message, strlen(message), 0); // call send to send message over the socket
    }
    
    free(p);
    return returnVal;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    char * messageCopy = malloc(strlen(message) + 1);
    char * p = messageCopy;
    strcpy(messageCopy, message);
    messageCopy[strlen(messageCopy)] = '\0';
    char * command = strtok(messageCopy, " ");
    char * arg = strtok(NULL, " ");
    // if (arg == NULL){
    // free(p);
    // return COMMAND_ERROR;
    // }
    int returnVal = COMMAND_ERROR;
    if (strcmp(command, "/move") == 0){
        // ChessMove *move;
        if (parse_move(arg, &(game->moves[game->moveCount])) != 0 || make_move(game, &(game->moves[game->moveCount]), is_client, false) != 0) returnVal = COMMAND_ERROR;
        else returnVal = COMMAND_MOVE;
    }
    else if (strcmp(message, "/forfeit") == 0){
        int x = close(socketfd);
        returnVal = (x == -1) ? -1 : COMMAND_FORFEIT;
    }
    else if (strcmp(command, "/import") == 0 && is_client == true){
        fen_to_chessboard(arg, game);
        returnVal = COMMAND_IMPORT;
    }
    else if (strcmp(command, "/load") == 0){
        // check if arguments are valid
        char * gameNum = strtok(NULL, " ");
        if (gameNum == NULL){
        free(p);
        return COMMAND_ERROR;
        }
        if (load_game(game, arg, "game_database.txt", atoi(gameNum)) == -1)  returnVal = COMMAND_ERROR;
        // call send to send message over the socket
        else returnVal = COMMAND_LOAD;
    }
    free(p);
    return returnVal;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    // Generate the FEN string of the current game state using the chessboard_to_fen function.
    char fen[BUFFER_SIZE];
    chessboard_to_fen(fen, game);
    // Write the username and FEN string to the file in the format "username:fen\n". Return 0 to indicate a successful save operation. Return -1 on error.
    if (strlen(username) == 0) return -1;
    const char * p = username;
    while (*p){
        if (*p++ == ' ') return -1;
    }
    // Open the database file.
    FILE * file = fopen(db_filename, "a");
    if (file == NULL) return -1;
    // In the event that the client has already saved before, write another entry to the end of the file. Do not delete/replace the previous save file.
    fputs(username, file);
    fputc(':', file);
    fputs(fen, file);
    fputs("\n", file);
    fclose(file);
    return 0;
}

// QUESTION: do we assume that the db_filename contains only info about the gamestate of 1 particular player, or can it contain info of multiple players
int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    if (save_number < 1) return -1;
    // Open the database file.
    FILE * file = fopen(db_filename, "r");
    if (file == NULL) return -1;
    char temp[BUFFER_SIZE];
    char * fen;
    int count = 0; 
    // Read the file line by line, searching for a line for the given player. The argument save_number tells you which one of the saved games we want. For example, suppose username = "Daniel" and save_number = 3. This means we want to find the third game in the database for the user "Daniel". Start counting at 1, not 0.
    while (fgets(temp, BUFFER_SIZE, file) && count < save_number){
        char * name = strtok(temp, ":");
        if (strcmp(username, name)== 0){
            count++;
            if (count == save_number){
                fen = strtok(NULL, " ");
                fen_to_chessboard(fen, game);
            }
        }
    }
    fclose(file);
    if (count < save_number) return -1;
    // Search for matching game state. If no matching game state is found for the given username, return -1 to indicate that the game was not found.

    return 0;
}

void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}
