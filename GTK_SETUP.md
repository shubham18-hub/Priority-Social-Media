# GTK Frontend Setup

## Windows Setup (MSYS2)

1. **Install MSYS2:**
   - Download from: https://www.msys2.org/
   - Install and update: `pacman -Syu`

2. **Install GTK4:**
   ```bash
   pacman -S mingw-w64-x86_64-gtk4
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-pkg-config
   ```

3. **Compile:**
   ```bash
   gcc -o gtk_frontend gtk_frontend.c `pkg-config --cflags --libs gtk4`
   ```

4. **Run:**
   ```bash
   ./gtk_frontend.exe
   ```

## Features

- **Login/Register:** User authentication
- **Post Creation:** Create and share posts
- **Feed View:** See all posts in chronological order
- **Logout:** Switch users

## Alternative: Online Compiler

Use an online C compiler with GTK support:
- Replit.com
- CodeChef IDE
- OnlineGDB (with GTK libraries)