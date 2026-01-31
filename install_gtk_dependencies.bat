@echo off
echo ========================================
echo  GTK Dependencies Installation
echo ========================================
echo.
echo Installing required libraries for Priority Social Media...
echo.

REM Check if MSYS2 is installed
if not exist "C:\msys64\usr\bin\pacman.exe" (
    echo ❌ MSYS2 not found!
    echo.
    echo Please install MSYS2 first:
    echo 1. Download from: https://www.msys2.org/
    echo 2. Install to C:\msys64
    echo 3. Run this script again
    echo.
    pause
    exit /b 1
)

echo ✅ MSYS2 found. Installing packages...
echo.

REM Install GTK3 and WebKit2GTK development packages
C:\msys64\usr\bin\pacman.exe -S --noconfirm ^
    mingw-w64-x86_64-gtk3 ^
    mingw-w64-x86_64-webkit2gtk ^
    mingw-w64-x86_64-json-c ^
    mingw-w64-x86_64-pkg-config ^
    mingw-w64-x86_64-gcc ^
    mingw-w64-x86_64-make

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo  ✅ DEPENDENCIES INSTALLED!
    echo ========================================
    echo.
    echo Installed packages:
    echo - ✅ GTK3 development libraries
    echo - ✅ WebKit2GTK for web browser functionality
    echo - ✅ JSON-C for data handling
    echo - ✅ GCC compiler and build tools
    echo.
    echo You can now run: compile_gtk_web.bat
    echo.
) else (
    echo.
    echo ========================================
    echo  ❌ INSTALLATION FAILED!
    echo ========================================
    echo.
    echo Please check your MSYS2 installation and try again.
    echo.
)

pause