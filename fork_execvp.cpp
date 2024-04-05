#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h> // Include this header for fork
#include <sys/types.h>

int main() {
    std::string processName;
    std::cout << "Enter the name of the process to run: ";
    std::cin >> processName;

    // Prepare arguments for execvp
    std::vector<char*> args;
    args.push_back(strdup(processName.c_str()));
    args.push_back(nullptr);

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork process." << std::endl;
        return 1;
    } else if (pid == 0) { 
        if (execvp(args[0], &args[0]) == -1) {
            std::cerr << "Failed to execute process." << std::endl;
            return 1;
        }
    }


    return 0;
}
