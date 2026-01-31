@echo off
echo ========================================
echo  Priority Social Media - Native Build
echo ========================================
echo.
echo Building C Backend + Web Frontend Integration...
echo.

REM Compile the native Windows version
gcc -o priority_social_media.exe priority_social_media.c -lshell32 -Wall -Wextra -g

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo  ✅ BUILD SUCCESSFUL!
    echo ========================================
    echo.
    echo Your Priority Social Media platform is ready!
    echo.
    echo To run: priority_social_media.exe
    echo.
    echo Features:
    echo - ✅ Complete C Backend with multimedia support
    echo - ✅ Web Frontend integration
    echo - ✅ Automatic browser launch
    echo - ✅ Data persistence
    echo - ✅ Priority-based content system
    echo - ✅ Sample data included
    echo.
    echo The application will:
    echo 1. Load/create sample data in C backend
    echo 2. Generate enhanced HTML with backend data
    echo 3. Launch your web browser automatically
    echo 4. Provide full social media functionality
    echo.
) else (
    echo.
    echo ========================================
    echo  ❌ BUILD FAILED!
    echo ========================================
    echo.
    echo Please ensure you have GCC installed.
    echo You can install it via:
    echo - MinGW-w64
    echo - MSYS2
    echo - Or any other GCC distribution for Windows
    echo.
)

pause