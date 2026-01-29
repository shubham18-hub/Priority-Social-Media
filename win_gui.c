#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_LOGIN 1001
#define ID_REGISTER 1002
#define ID_POST 1003
#define ID_LOGOUT 1004
#define ID_USERNAME 1005
#define ID_PASSWORD 1006
#define ID_POSTCONTENT 1007
#define ID_FEED 1008

// Backend structures
typedef struct User {
    int user_id;
    char username[100];
    char password[100];
    struct User* next;
} User;

typedef struct Post {
    char author[100];
    char content[500];
    struct Post* next;
} Post;

User* users_head = NULL;
User* current_user = NULL;
Post* posts_head = NULL;
int next_user_id = 1;

// Windows
HWND hMainWnd, hUsername, hPassword, hPostContent, hFeed, hStatus;
HWND hLoginBtn, hRegisterBtn, hPostBtn, hLogoutBtn;

// Backend functions
int register_user(char* username, char* password) {
    User* new_user = malloc(sizeof(User));
    new_user->user_id = next_user_id++;
    strcpy(new_user->username, username);
    strcpy(new_user->password, password);
    new_user->next = users_head;
    users_head = new_user;
    return 1;
}

User* login_user(char* username, char* password) {
    User* temp = users_head;
    while (temp) {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0) {
            current_user = temp;
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void create_post(char* content) {
    if (!current_user) return;
    Post* new_post = malloc(sizeof(Post));
    strcpy(new_post->author, current_user->username);
    strcpy(new_post->content, content);
    new_post->next = posts_head;
    posts_head = new_post;
}

void update_feed() {
    char feed_text[2000] = "";
    Post* temp = posts_head;
    while (temp) {
        char post_line[600];
        sprintf(post_line, "@%s: %s\r\n\r\n", temp->author, temp->content);
        strcat(feed_text, post_line);
        temp = temp->next;
    }
    SetWindowText(hFeed, feed_text);
}

void show_login_view() {
    ShowWindow(hUsername, SW_SHOW);
    ShowWindow(hPassword, SW_SHOW);
    ShowWindow(hLoginBtn, SW_SHOW);
    ShowWindow(hRegisterBtn, SW_SHOW);
    ShowWindow(hPostContent, SW_HIDE);
    ShowWindow(hPostBtn, SW_HIDE);
    ShowWindow(hLogoutBtn, SW_HIDE);
    ShowWindow(hFeed, SW_HIDE);
}

void show_main_view() {
    ShowWindow(hUsername, SW_HIDE);
    ShowWindow(hPassword, SW_HIDE);
    ShowWindow(hLoginBtn, SW_HIDE);
    ShowWindow(hRegisterBtn, SW_HIDE);
    ShowWindow(hPostContent, SW_SHOW);
    ShowWindow(hPostBtn, SW_SHOW);
    ShowWindow(hLogoutBtn, SW_SHOW);
    ShowWindow(hFeed, SW_SHOW);
    update_feed();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // Create controls
            hUsername = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                50, 50, 200, 25, hwnd, (HMENU)ID_USERNAME, NULL, NULL);
            hPassword = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
                50, 80, 200, 25, hwnd, (HMENU)ID_PASSWORD, NULL, NULL);
            hLoginBtn = CreateWindow("BUTTON", "Login", WS_VISIBLE | WS_CHILD,
                50, 110, 80, 30, hwnd, (HMENU)ID_LOGIN, NULL, NULL);
            hRegisterBtn = CreateWindow("BUTTON", "Register", WS_VISIBLE | WS_CHILD,
                140, 110, 80, 30, hwnd, (HMENU)ID_REGISTER, NULL, NULL);
            
            hPostContent = CreateWindow("EDIT", "", WS_CHILD | WS_BORDER,
                50, 50, 300, 25, hwnd, (HMENU)ID_POSTCONTENT, NULL, NULL);
            hPostBtn = CreateWindow("BUTTON", "Post", WS_CHILD,
                360, 50, 60, 25, hwnd, (HMENU)ID_POST, NULL, NULL);
            hLogoutBtn = CreateWindow("BUTTON", "Logout", WS_CHILD,
                430, 50, 60, 25, hwnd, (HMENU)ID_LOGOUT, NULL, NULL);
            hFeed = CreateWindow("EDIT", "", WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
                50, 90, 440, 300, hwnd, (HMENU)ID_FEED, NULL, NULL);
            
            hStatus = CreateWindow("STATIC", "Welcome! Please login or register.", WS_VISIBLE | WS_CHILD,
                50, 400, 400, 20, hwnd, NULL, NULL, NULL);
            
            // Set initial text
            SetWindowText(hUsername, "Username");
            SetWindowText(hPassword, "Password");
            SetWindowText(hPostContent, "What's on your mind?");
            
            show_login_view();
            break;
            
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_LOGIN: {
                    char username[100], password[100];
                    GetWindowText(hUsername, username, 100);
                    GetWindowText(hPassword, password, 100);
                    
                    if (login_user(username, password)) {
                        char welcome[150];
                        sprintf(welcome, "Welcome, %s!", current_user->username);
                        SetWindowText(hStatus, welcome);
                        show_main_view();
                    } else {
                        SetWindowText(hStatus, "Login failed!");
                    }
                    break;
                }
                case ID_REGISTER: {
                    char username[100], password[100];
                    GetWindowText(hUsername, username, 100);
                    GetWindowText(hPassword, password, 100);
                    
                    if (register_user(username, password)) {
                        SetWindowText(hStatus, "Registration successful! Please login.");
                    } else {
                        SetWindowText(hStatus, "Registration failed!");
                    }
                    break;
                }
                case ID_POST: {
                    char content[500];
                    GetWindowText(hPostContent, content, 500);
                    if (strlen(content) > 0) {
                        create_post(content);
                        SetWindowText(hPostContent, "");
                        update_feed();
                    }
                    break;
                }
                case ID_LOGOUT:
                    current_user = NULL;
                    SetWindowText(hStatus, "Logged out");
                    show_login_view();
                    break;
            }
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char* CLASS_NAME = "SocialMediaWindow";
    
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClass(&wc);
    
    hMainWnd = CreateWindowEx(0, CLASS_NAME, "Priority Social Media",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 500,
        NULL, NULL, hInstance, NULL);
    
    if (hMainWnd == NULL) return 0;
    
    ShowWindow(hMainWnd, nCmdShow);
    
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}