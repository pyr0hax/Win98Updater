Automatic Updater for Windows 98 that will be hosted in Cloud.
Using FTP Server to collect and then enumerates through all downloaded files.

Logic to be built to handle each installation as silent.

To Compile, use below:

    gcc -o updater.exe ftp_utils.c execute_utils.c main.c -lws2_32
