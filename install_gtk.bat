@echo off
echo Installing GTK4 on Windows...
echo.

echo Step 1: Downloading MSYS2 installer...
powershell -Command "Invoke-WebRequest -Uri 'https://github.com/msys2/msys2-installer/releases/download/2023-05-26/msys2-x86_64-20230526.exe' -OutFile 'msys2-installer.exe'"

echo Step 2: Running MSYS2 installer...
echo Please follow the installer prompts and then run this script again.
start msys2-installer.exe

echo.
echo After MSYS2 installation:
echo 1. Open MSYS2 terminal
echo 2. Run: pacman -Syu
echo 3. Run: pacman -S mingw-w64-x86_64-gtk4 mingw-w64-x86_64-gcc mingw-w64-x86_64-pkg-config
echo 4. Add C:\msys64\mingw64\bin to your PATH
echo 5. Run compile_gtk_final.bat

pause