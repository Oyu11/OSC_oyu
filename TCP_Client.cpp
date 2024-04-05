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

    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error in creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "[+]Client Socket is created." << std::endl;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    int ret = connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    if (ret == SOCKET_ERROR) {
        std::cerr << "Error in connection: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "[+]Connected to Server." << std::endl;

    while (true) {
        std::cout << "Client: \t";
        std::cin >> buffer;
        send(clientSocket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, ":exit") == 0) {
            closesocket(clientSocket);
            std::cout << "[-]Disconnected from server." << std::endl;
            break;
        }

        ret = recv(clientSocket, buffer, 1024, 0);
        if (ret == SOCKET_ERROR) {
            std::cerr << "Error in receiving data: " << WSAGetLastError() << std::endl;
        } else {
            buffer[ret] = '\0'; // Null-terminate received data
            std::cout << "Server: \t" << buffer << std::endl;
        }
    }

    WSACleanup();
    return 0;
}
