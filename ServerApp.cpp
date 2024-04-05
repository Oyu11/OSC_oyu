#include <iostream>
#include <cstring>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 4444

int main() {
    WSADATA wsData;
    int wsResult = WSAStartup(MAKEWORD(2, 2), &wsData);
    if (wsResult != 0) {
        std::cerr << "WSAStartup failed with error: " << wsResult << std::endl;
        return 1;
    }

    SOCKET sockfd, newSocket;
    struct sockaddr_in serverAddr, newAddr;

    int addr_size = sizeof(serverAddr);

    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error in connection." << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "[+]Server Socket is created." << std::endl;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(sockfd, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr));
    if (ret == SOCKET_ERROR) {
        std::cerr << "Error in binding." << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    if (listen(sockfd, 10) == 0) {
        std::cout << "[+]Listening...." << std::endl;
    } else {
        std::cerr << "Error in listening." << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    newSocket = accept(sockfd, reinterpret_cast<struct sockaddr*>(&newAddr), &addr_size);
    if (newSocket == INVALID_SOCKET) {
        std::cerr << "Error in accepting." << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    std::cout << "Connection accepted from " << inet_ntoa(newAddr.sin_addr) << ":" << ntohs(newAddr.sin_port) << std::endl;

    while (true) {
        recv(newSocket, buffer, sizeof(buffer), 0);
        if (strcmp(buffer, ":exit") == 0) {
            std::cout << "Disconnected from " << inet_ntoa(newAddr.sin_addr) << ":" << ntohs(newAddr.sin_port) << std::endl;
            break;
        } else {
            std::cout << "Client: \t" << buffer << std::endl;
            std::cout << "Server: \t";
            std::cin >> buffer;
            send(newSocket, buffer, strlen(buffer), 0);
        }
    }

    closesocket(sockfd);
    closesocket(newSocket);
    WSACleanup();
    return 0;
}
