@echo off
echo Compiling GTK Frontend...
echo.
echo Note: You need GTK4 development libraries installed.
echo For Windows, install MSYS2 and run:
echo   pacman -S mingw-w64-x86_64-gtk4
echo.

gcc -o gtk_frontend gtk_frontend.c `pkg-config --cflags --libs gtk4`

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Run: gtk_frontend.exe
) else (
    echo Compilation failed. Make sure GTK4 is installed.
    echo Alternative: Use online C compiler with GTK support
)
pause