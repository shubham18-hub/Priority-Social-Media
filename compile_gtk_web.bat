@echo off
echo ========================================
echo  Priority Social Media - GTK Web Build
echo ========================================
echo.
echo Building C Backend + GTK Frontend...
echo.

REM Check if required libraries are installed
echo Checking dependencies...

REM Compile with GTK3 and WebKit2GTK
gcc -o priority_social_media_gtk ^
    gtk_web_frontend.c ^
    -I"C:\msys64\mingw64\include\gtk-3.0" ^
    -I"C:\msys64\mingw64\include\glib-2.0" ^
    -I"C:\msys64\mingw64\lib\glib-2.0\include" ^
    -I"C:\msys64\mingw64\include\pango-1.0" ^
    -I"C:\msys64\mingw64\include\cairo" ^
    -I"C:\msys64\mingw64\include\gdk-pixbuf-2.0" ^
    -I"C:\msys64\mingw64\include\atk-1.0" ^
    -I"C:\msys64\mingw64\include\webkit2gtk-4.0" ^
    -I"C:\msys64\mingw64\include\libsoup-2.4" ^
    -I"C:\msys64\mingw64\include\json-c" ^
    -L"C:\msys64\mingw64\lib" ^
    -lgtk-3 -lgdk-3 -lglib-2.0 -lgobject-2.0 -lgio-2.0 ^
    -lwebkit2gtk-4.0 -ljson-c ^
    -Wall -Wextra -g

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo  ✅ BUILD SUCCESSFUL!
    echo ========================================
    echo.
    echo Your Priority Social Media platform is ready!
    echo.
    echo To run: priority_social_media_gtk.exe
    echo.
    echo Features:
    echo - ✅ C Backend with all social media functionality
    echo - ✅ GTK Frontend with modern web interface
    echo - ✅ Web browser display capability
    echo - ✅ Priority-based content system
    echo - ✅ Multimedia support
    echo.
) else (
    echo.
    echo ========================================
    echo  ❌ BUILD FAILED!
    echo ========================================
    echo.
    echo Please ensure you have installed:
    echo - MSYS2 with MinGW64
    echo - GTK3 development libraries
    echo - WebKit2GTK development libraries
    echo - JSON-C development libraries
    echo.
    echo Run install_gtk_dependencies.bat first
    echo.
)

pause