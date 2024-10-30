#include "hw4.h"

int main() {
    ChessGame game;
    int listenfd, connfd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set options to reuse the IP address and IP port if either is already in use
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))");
        return -1;
    }
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))");
        return -1;
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(listenfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(listenfd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    INFO("Server listening on port %d", PORT);
    // Accept incoming connection
    if ((connfd = accept(listenfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    INFO("Server accepted connection");
    initialize_game(&game);
    display_chessboard(&game);

    while (1) {
        // Call read to accept the client’s input over the socket.
        memset(buffer, 0, BUFFER_SIZE);
        int nbytes = read(connfd, buffer, BUFFER_SIZE);
        if (nbytes <= 0) {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }
        printf("[Server] Received from client: %s\n", buffer);
        int request = receive_command(&game, buffer, connfd, false); // Call receive_command to process the client’s input. If the client request is to forfeit, close the socket and terminate the server.
        if (request == COMMAND_FORFEIT){
            printf("[Server] Quitting...\n");
            break;
        }
        // Prompt the server player for input.
        int sendCommand = -1;
        while (sendCommand == COMMAND_ERROR || sendCommand == COMMAND_UNKNOWN){
            printf("[Server] Enter message: ");
            memset(buffer, 0, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strlen(buffer)-1] = '\0';
            sendCommand = send_command(&game, buffer, connfd, false);
            if (sendCommand == COMMAND_FORFEIT){
            printf("[Server] Quitting...\n");
            break;
        }
        // Call send_command to attempt sending the command to the client. If send_command returns COMMAND_ERROR or COMMAND_UNKNOWN, prompt the server again for a different command. If the server’s command is to forfeit, close the socket and terminate the server.
        }
        if (sendCommand == COMMAND_FORFEIT) break;
        // send(connfd, buffer, strlen(buffer), 0);
        
    }
    printf("[Server] Shutting down.\n");
    close(connfd);
    close(listenfd);
    return 0;
}
