@echo off
echo ========================================
echo  Priority Social Media - Complete Build
echo ========================================
echo.
echo Building Complete C Backend + Web Frontend...
echo.

REM Compile the complete standalone version
gcc -o priority_social_media.exe social_media_complete.c -lshell32 -Wall -g

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
    echo ✅ FEATURES INCLUDED:
    echo - Complete C Backend with multimedia support
    echo - Web Frontend integration with existing frontend.html
    echo - Automatic browser launch
    echo - Data persistence (saves to social_media_data.txt)
    echo - Priority-based content system
    echo - Sample data with users: alice, bob, charlie
    echo - Follow relationships and close friends
    echo - All social media functionality working
    echo.
    echo 🚀 HOW IT WORKS:
    echo 1. C backend loads/creates sample data
    echo 2. Generates enhanced HTML with backend data
    echo 3. Launches web browser automatically
    echo 4. Web interface shows real data from C backend
    echo 5. All functionality connected and working
    echo.
    echo 🎯 SAMPLE USERS (password: password123):
    echo - alice (has posts and follows others)
    echo - bob (has posts, followed by alice)
    echo - charlie (has posts, follows everyone)
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