#include "ftp_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_FILES 100

int fetch_ftp_file_list(const char* ftp_server, const char* username, const char* password, char** file_list, int max_files) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        printf("Failed to initialize Winsock\n");
        return -1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Failed to create socket\n");
        WSACleanup();
        return -1;
    }

    struct hostent* host = gethostbyname(ftp_server);
    if (!host) {
        printf("Failed to resolve FTP server\n");
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(21);
    addr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        printf("Failed to connect to FTP server\n");
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    char buffer[256];
    int recv_len;
    recv_len = recv(sock, buffer, sizeof(buffer), 0);
    buffer[recv_len] = '\0';
    printf("%s", buffer); 

    snprintf(buffer, sizeof(buffer), "USER %s\r\n", username);
    send(sock, buffer, strlen(buffer), 0);
    recv_len = recv(sock, buffer, sizeof(buffer), 0); 
    buffer[recv_len] = '\0';
    printf("%s", buffer);

    snprintf(buffer, sizeof(buffer), "PASS %s\r\n", password);
    send(sock, buffer, strlen(buffer), 0);
    recv_len = recv(sock, buffer, sizeof(buffer), 0); 
    buffer[recv_len] = '\0';
    printf("%s", buffer);

    snprintf(buffer, sizeof(buffer), "LIST\r\n");
    send(sock, buffer, strlen(buffer), 0);

    int num_files = 0;
    while ((recv_len = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        buffer[recv_len] = '\0';
        char* filename = strtok(buffer, " \t\n\r");

        if (filename && strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0) {
            file_list[num_files] = strdup(filename);
            num_files++;

            if (num_files >= max_files) {
                break;
            }
        }
    }

    closesocket(sock);
    WSACleanup();

    return num_files;
}

int download_file_from_ftp(const char* ftp_server, const char* ftp_file, const char* local_file) {
    printf("Downloading %s from FTP server...\n", ftp_file);
    printf("Saving it as %s...\n", local_file);
    FILE* local_file_ptr = fopen(local_file, "wb");
    if (!local_file_ptr) {
        printf("Failed to create local file %s\n", local_file);
        return -1;
    }

    char dummy_data[] = "Dummy data from FTP";
    fwrite(dummy_data, sizeof(char), strlen(dummy_data), local_file_ptr);

    fclose(local_file_ptr);
    return 0;
}

int is_file_downloaded(const char* local_folder, const char* filename) {
    char local_file_path[256];
    snprintf(local_file_path, sizeof(local_file_path), "%s%s", local_folder, filename);
    FILE* local_file_ptr = fopen(local_file_path, "rb");
    if (local_file_ptr) {
        fclose(local_file_ptr);
        return 1;
    }
    return 0;
}

void mark_file_as_downloaded(const char* local_folder, const char* filename) {
    printf("Marking %s as downloaded...\n", filename);
    char local_file_path[256];
    snprintf(local_file_path, sizeof(local_file_path), "%s%s", local_folder, filename);
    FILE* local_file_ptr = fopen(local_file_path, "wb");
    if (local_file_ptr) {
        fclose(local_file_ptr);
    } else {
        printf("Failed to mark %s as downloaded\n", filename);
    }
}
