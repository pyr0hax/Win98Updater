@echo off
echo =====================================================================
echo This Updater will now be insttalled.
echo =====================================================================
pause
move schedular.bat "C:\Windows\Start Menu\Programs\Startup\"
MKDIR C:\updater
move updater.exe C:\updater\
pause
echo =====================================================================
echo Please note that the schedular will run each time on startup so you will have an annoying black box popping up. 
echo It will check for updates.
echo =====================================================================
pause