#ifndef FTP_UTILS_H
#define FTP_UTILS_H

#include <winsock.h>

#define MAX_FILES 100

int fetch_ftp_file_list(const char* ftp_server, const char* username, const char* password, char** file_list, int max_files);
int download_file_from_ftp(const char* ftp_server, const char* ftp_file, const char* local_file);
void run_executable(const char* path);
int is_file_downloaded(const char* local_folder, const char* filename);
void mark_file_as_downloaded(const char* local_folder, const char* filename);

#endif
