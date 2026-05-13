@echo off
setlocal

set "ENGINE_DIR=G:\Workspace\UnrealEngine\UE_5.7"
set "PROJECT_FILE=%~dp0MUMOSA.uproject"

call "%ENGINE_DIR%\Engine\Build\BatchFiles\Build.bat" DerelictCorridorMegascansEditor Win64 Development -Project="%PROJECT_FILE%" -WaitMutex
if errorlevel 1 exit /b %errorlevel%

start "" "%ENGINE_DIR%\Engine\Binaries\Win64\UnrealEditor.exe" "%PROJECT_FILE%"
