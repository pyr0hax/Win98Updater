#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>
#include <windows.h>
#include <winerror.h>
#include "ftp_utils.h"
#pragma comment(lib, "ws2_32.lib")
#define MAX_FILES 100
#define REGISTRY_KEY "Software\\MyApp\\DownloadedFiles"


int main() {
    const char* ftp_server = "your_ftp_server_address";
    const char* ftp_username = "your_ftp_username";
    const char* ftp_password = "your_ftp_password";
    const char* local_folder = "C:\\local_folder\\";

    char* available_files[MAX_FILES];
    int num_available_files = fetch_ftp_file_list(ftp_server, ftp_username, ftp_password, available_files, MAX_FILES);

    if (num_available_files <= 0) {
        printf("No files found on the FTP server.\n");
        return 1;
    }

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        printf("Failed to initialize Winsock\n");
        return 1;
    }

    HKEY hKey;
    if (RegCreateKey(HKEY_CURRENT_USER, REGISTRY_KEY, &hKey) != ERROR_SUCCESS) {
        printf("Failed to create registry key\n");
        WSACleanup();
        return 1;
    }

    for (int i = 0; i < num_available_files; i++) {
        const char* filename = available_files[i];
        char local_file_path[256];

        snprintf(local_file_path, sizeof(local_file_path), "%s%s", local_folder, filename);
        if (is_file_downloaded(local_folder, filename)) {
            printf("File %s is already downloaded\n", filename);
            continue;
        }

        if (download_file_from_ftp(ftp_server, filename, local_file_path) == 0) {
            printf("File %s downloaded successfully\n", filename);

            mark_file_as_downloaded(local_folder, filename);

            RegSetValueEx(hKey, filename, 0, REG_SZ, (BYTE*)local_file_path, strlen(local_file_path));

            run_executable(local_file_path);
        }
    }

    RegCloseKey(hKey);
    WSACleanup();
    return 0;
}
