@echo off
echo Installing GTK4 for Windows...
echo.

echo Step 1: Download GTK4 runtime...
powershell -Command "Invoke-WebRequest -Uri 'https://github.com/wingtk/gvsbuild/releases/download/2023.8.1/GTK4_Gvsbuild_2023.8.1_x64.zip' -OutFile 'gtk4.zip'"

echo Step 2: Extract GTK4...
powershell -Command "Expand-Archive -Path 'gtk4.zip' -DestinationPath 'C:\gtk4' -Force"

echo Step 3: Set environment variables...
setx PATH "C:\gtk4\bin;%PATH%"
setx PKG_CONFIG_PATH "C:\gtk4\lib\pkgconfig"

echo.
echo GTK4 installation complete!
echo Please restart your command prompt and run compile_gtk_final.bat
echo.

pause