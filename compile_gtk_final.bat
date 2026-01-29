@echo off
echo Compiling GTK Frontend...
echo.

REM Check if MSYS2 is in PATH
where gcc >nul 2>&1
if %errorlevel% neq 0 (
    echo Adding MSYS2 to PATH...
    set PATH=C:\msys64\mingw64\bin;%PATH%
)

REM Check GTK4
pkg-config --exists gtk4
if %errorlevel% neq 0 (
    echo GTK4 not found. Please install via MSYS2:
    echo pacman -S mingw-w64-x86_64-gtk4
    pause
    exit /b 1
)

echo Compiling with GTK4...
gcc -o gtk_frontend gtk_frontend.c `pkg-config --cflags --libs gtk4`

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running GTK frontend...
    gtk_frontend.exe
) else (
    echo Compilation failed.
)

pause