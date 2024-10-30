#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "hw4.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    ChessGame game;
    int connfd = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    // Connect to the server
    if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(connfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    initialize_game(&game);
    display_chessboard(&game);

    while (1) {
    //     Prompt the client player for input.
    // Call send_command to send the command to the server.  If send_command returns COMMAND_ERROR or COMMAND_UNKNOWN, prompt the client again for a different command. If the client’s command is to forfeit, close the socket and terminate the client.
        int sendCommand = -1;
        while (sendCommand == COMMAND_ERROR || sendCommand == COMMAND_UNKNOWN){
            printf("[Client] Enter message: ");
            memset(buffer, 0, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strlen(buffer)-1] = '\0';
            // check if send() works in send_command
            sendCommand = send_command(&game, buffer, connfd, true);
            if (sendCommand == COMMAND_FORFEIT){
            printf("[Client] Quitting...\n");
            break;
            }
        // Call send_command to attempt sending the command to the client. If send_command returns COMMAND_ERROR or COMMAND_UNKNOWN, prompt the server again for a different command. If the server’s command is to forfeit, close the socket and terminate the server.
        }
        if (sendCommand == COMMAND_FORFEIT) break;
        // Call read to accept the server’s input over the socket.
        memset(buffer, 0, BUFFER_SIZE);
        int nbytes = read(connfd, buffer, BUFFER_SIZE);
        if (nbytes <= 0) {
            perror("[Client] read() failed.");
            break;
        }
        printf("[Client] Received from server: %s\n", buffer);
        int receiveCommand = receive_command(&game, buffer, connfd, true);     // Call receive_command to process the server’s input. If the server’s command is to forfeit, close the socket and terminate the client.
        if (receiveCommand == COMMAND_FORFEIT){
            printf("[Client] Server chatter quitting...\n");
            break;
    }
    }

    // Please ensure that the following lines of code execute just before your program terminates.
    // If necessary, copy and paste it to other parts of your code where you terminate your program.
    FILE *temp = fopen("./fen.txt", "w");
    char fen[200];
    chessboard_to_fen(fen, &game);
    fprintf(temp, "%s", fen);
    fclose(temp);
    close(connfd);
    return 0;
}
