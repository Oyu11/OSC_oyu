#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 4444
int main() {
    int clientSocket, ret;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "[-]Error in connection." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "[+]Client Socket is created." << std::endl;
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret < 0) {
        std::cerr << "[-]Error in connection." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "[+]Connected to Server." << std::endl;
    while (true) {
        std::cout << "Client: \t";
        
        std::cin >> buffer;
        send(clientSocket, buffer, strlen(buffer), 0);
        if (strcmp(buffer, ":exit") == 0) {
            close(clientSocket);
            std::cout << "[-]Disconnected from server." << std::endl;
            exit(EXIT_SUCCESS);
        }
        if (recv(clientSocket, buffer, 1024, 0) < 0) {
            std::cerr << "[-]Error in receiving data." << std::endl;
        } else {
            std::cout << "Server: \t" << buffer << std::endl;
        }
    }
    return 0;
}
