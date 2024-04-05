#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main() {
    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "[-]Error in connection." << std::endl;
        exit(1);
    }
    std::cout << "[+]Server Socket is created." << std::endl;

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr));
    if (ret < 0) {
        std::cerr << "[-]Error in binding." << std::endl;
        exit(1);
    }

    if (listen(sockfd, 10) == 0) {
        std::cout << "[+]Listening...." << std::endl;
    } else {
        std::cerr << "[-]Error in binding." << std::endl;
    }

    while (true) {
        addr_size = sizeof(newAddr);
        newSocket = accept(sockfd, reinterpret_cast<struct sockaddr*>(&newAddr), &addr_size);
        if (newSocket < 0) {
            exit(1);
        }
        std::cout << "Connection accepted from " << inet_ntoa(newAddr.sin_addr) << ":" << ntohs(newAddr.sin_port) << std::endl;

        if ((childpid = fork()) == 0) {
            close(sockfd);

            while (true) {
                recv(newSocket, buffer, 1024, 0);
                if (strcmp(buffer, ":exit") == 0) {
                    std::cout << "Disconnected from " << inet_ntoa(newAddr.sin_addr) << ":" << ntohs(newAddr.sin_port) << std::endl;
                    break;
                } else {
                    std::cout << "Client: \t" << buffer << std::endl;
                    std::cout << "Server: \t";
                    std::cin >> buffer;
                    send(newSocket, buffer, strlen(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));
                }
            }
        }
    }

    close(newSocket);
    return 0;
}
