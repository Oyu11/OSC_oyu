#include <windows.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>

int main() {
    std::string processName;
    std::cout << "Enter the name of the process to run: ";
    std::cin >> processName;

    // Append .exe extension if not already present
    if (processName.find(".exe") == std::string::npos) {
        processName += ".exe";
    }

    // CreateProcess parameters
    LPCTSTR lpApplicationName = processName.c_str();
    STARTUPINFO startupInfo = { sizeof(startupInfo) };
    PROCESS_INFORMATION processInfo;

    // Create the process
    BOOL bSuccess = CreateProcess(lpApplicationName, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
    if (!bSuccess) {
        std::cerr << "Failed to create process: " << GetLastError() << std::endl;
        return 1;
    }

    // Close handles
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    return 0;
}
