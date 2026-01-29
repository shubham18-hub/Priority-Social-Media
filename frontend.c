#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Minimal structures and globals
typedef struct User {
    int user_id;
    char username[100];
    char password[100];
    time_t created_at;
    struct User* next;
} User;

User* users_head = NULL;
User* current_user = NULL;
int next_user_id = 1;

// Basic functions
int register_user(char* username, char* password) {
    User* new_user = malloc(sizeof(User));
    new_user->user_id = next_user_id++;
    strcpy(new_user->username, username);
    strcpy(new_user->password, password);
    new_user->created_at = time(NULL);
    new_user->next = users_head;
    users_head = new_user;
    printf("User registered! ID: %d\n", new_user->user_id);
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

void logout_user() { current_user = NULL; }
void create_post(char* content) { printf("Post created: %s\n", content); }
void display_feed() { printf("=== FEED ===\nNo posts yet.\n"); }
void follow_user(int id) { printf("Following user %d\n", id); }
void send_message(int id, char* msg) { printf("Message sent to %d: %s\n", id, msg); }
void display_notifications() { printf("No notifications.\n"); }
void save_data() { printf("Data saved.\n"); }
void load_data() { printf("Data loaded.\n"); }

void show_menu() {
    printf("\n=== PRIORITY SOCIAL MEDIA ===\n");
    if (current_user == NULL) {
        printf("1. Register\n2. Login\n3. Exit\n");
    } else {
        printf("1. Create Post\n2. View Feed\n3. Follow User\n4. Send Message\n");
        printf("5. View Notifications\n6. Logout\n7. Exit\n");
    }
    printf("Choice: ");
}

int main() {
    load_data();
    int choice, user_id;
    char username[100], password[100], content[500];
    
    while (1) {
        show_menu();
        scanf("%d", &choice);
        getchar(); // consume newline
        
        if (current_user == NULL) {
            switch (choice) {
                case 1:
                    printf("Username: ");
                    fgets(username, 100, stdin);
                    username[strcspn(username, "\n")] = 0;
                    printf("Password: ");
                    fgets(password, 100, stdin);
                    password[strcspn(password, "\n")] = 0;
                    register_user(username, password);
                    break;
                case 2:
                    printf("Username: ");
                    fgets(username, 100, stdin);
                    username[strcspn(username, "\n")] = 0;
                    printf("Password: ");
                    fgets(password, 100, stdin);
                    password[strcspn(password, "\n")] = 0;
                    if (login_user(username, password))
                        printf("Login successful!\n");
                    else
                        printf("Login failed!\n");
                    break;
                case 3:
                    save_data();
                    return 0;
            }
        } else {
            switch (choice) {
                case 1:
                    printf("Post content: ");
                    fgets(content, 500, stdin);
                    content[strcspn(content, "\n")] = 0;
                    create_post(content);
                    break;
                case 2:
                    display_feed();
                    break;
                case 3:
                    printf("User ID to follow: ");
                    scanf("%d", &user_id);
                    follow_user(user_id);
                    break;
                case 4:
                    printf("Receiver ID: ");
                    scanf("%d", &user_id);
                    getchar();
                    printf("Message: ");
                    fgets(content, 500, stdin);
                    content[strcspn(content, "\n")] = 0;
                    send_message(user_id, content);
                    break;
                case 5:
                    display_notifications();
                    break;
                case 6:
                    logout_user();
                    break;
                case 7:
                    save_data();
                    return 0;
            }
        }
    }
}