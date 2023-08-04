#include <windows.h>
#include <stdio.h>

#define REGISTRY_KEY "Software\\VintageComputerMuseum\\InstalledApps"

void run_executable(const char* path) {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, REGISTRY_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD valueType;
        DWORD dataSize = MAX_PATH;
        char installedExePath[MAX_PATH];
        if (RegQueryValueEx(hKey, path, NULL, &valueType, (BYTE*)installedExePath, &dataSize) == ERROR_SUCCESS) {
            printf("Executable %s is already installed.\n", path);
            RegCloseKey(hKey);
            return;
        }
        RegCloseKey(hKey);
    }

    printf("Installing %s...\n", path);

    const char* installation_directory = "C:\\InstalledApps\\";
    char installed_path[MAX_PATH];
    snprintf(installed_path, sizeof(installed_path), "%s%s", installation_directory, path);
    CopyFile(path, installed_path, FALSE);

    if (RegCreateKey(HKEY_CURRENT_USER, REGISTRY_KEY, &hKey) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, path, 0, REG_SZ, (BYTE*)installed_path, strlen(installed_path));
        RegCloseKey(hKey);
    }

    printf("Running %s...\n", installed_path);
    ShellExecute(NULL, "open", installed_path, NULL, NULL, SW_SHOWNORMAL);
}
