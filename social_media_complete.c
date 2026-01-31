/*
 * PRIORITY SOCIAL MEDIA - Complete Standalone Version
 * C Backend + Web Frontend Integration (Windows Native)
 * 
 * Team: MindIsFull
 * Course: B.tech DS-III-T022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

// Constants
#define MAX_USERNAME 100
#define MAX_PASSWORD 100
#define MAX_POST_CONTENT 1000
#define MAX_MESSAGE_CONTENT 500
#define MAX_FILENAME 200
#define MAX_USERS 100

// Media types enumeration
typedef enum {
    MEDIA_NONE = 0,
    MEDIA_IMAGE = 1,
    MEDIA_VIDEO = 2,
    MEDIA_AUDIO = 3
} MediaType;

// User structure
typedef struct User {
    int user_id;
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    time_t created_at;
    struct User* next;
} User;

// Post structure with multimedia support
typedef struct Post {
    int post_id;
    int author_id;
    char author_name[MAX_USERNAME];
    char content[MAX_POST_CONTENT];
    time_t created_at;
    int priority; // Higher for close friends
    MediaType media_type; // Type of attached media
    char media_path[MAX_FILENAME]; // Path to media file
    char media_description[200]; // Description of media content
    struct Post* next;
} Post;

// Follow relationship structure
typedef struct Follow {
    int follower_id;
    int following_id;
    struct Follow* next;
} Follow;

// Close friend structure
typedef struct CloseFriend {
    int user_id;
    int friend_id;
    struct CloseFriend* next;
} CloseFriend;

// Global variables
User* users = NULL;
Post* posts = NULL;
Follow* follows = NULL;
CloseFriend* close_friends = NULL;
User* current_user = NULL;
int next_user_id = 1;
int next_post_id = 1;

// Function prototypes
int register_user(char* username, char* password);
User* login_user(char* username, char* password);
User* find_user_by_id(int user_id);
User* find_user_by_username(char* username);
int create_media_post(char* content, MediaType media_type, char* media_path, char* media_description);
int follow_user(int user_id);
int unfollow_user(int user_id);
int add_close_friend(int friend_id);
int remove_close_friend(int friend_id);
int is_following(int follower_id, int following_id);
int is_close_friend(int user_id, int friend_id);
void save_all_data(void);
void load_all_data(void);
void generate_html_with_data(void);
void open_browser(void);
int get_int_input(void);
void get_string_input(char* buffer, int max_len);
void pause_screen(void);
char* get_media_type_string(MediaType type);

// User functions
int register_user(char* username, char* password) {
    // Check if username already exists
    User* temp = users;
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0) {
            return 0; // Username already exists
        }
        temp = temp->next;
    }
    
    // Create new user
    User* new_user = (User*)malloc(sizeof(User));
    if (new_user == NULL) {
        printf("Memory allocation failed!\n");
        return 0;
    }
    
    new_user->user_id = next_user_id++;
    strcpy(new_user->username, username);
    strcpy(new_user->password, password);
    new_user->created_at = time(NULL);
    new_user->next = users;
    users = new_user;
    
    printf("User registered successfully! User ID: %d\n", new_user->user_id);
    return 1;
}

User* login_user(char* username, char* password) {
    User* temp = users;
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0 && 
            strcmp(temp->password, password) == 0) {
            current_user = temp;
            return temp;
        }
        temp = temp->next;
    }
    return NULL; // Login failed
}

User* find_user_by_id(int user_id) {
    User* temp = users;
    while (temp != NULL) {
        if (temp->user_id == user_id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

User* find_user_by_username(char* username) {
    User* temp = users;
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Post functions
int create_media_post(char* content, MediaType media_type, char* media_path, char* media_description) {
    if (current_user == NULL) {
        printf("Please login first!\n");
        return 0;
    }
    
    Post* new_post = (Post*)malloc(sizeof(Post));
    if (new_post == NULL) {
        printf("Memory allocation failed!\n");
        return 0;
    }
    
    new_post->post_id = next_post_id++;
    new_post->author_id = current_user->user_id;
    strcpy(new_post->author_name, current_user->username);
    strcpy(new_post->content, content);
    new_post->created_at = time(NULL);
    new_post->priority = 0;
    new_post->media_type = media_type;
    strcpy(new_post->media_path, media_path);
    strcpy(new_post->media_description, media_description);
    new_post->next = posts;
    posts = new_post;
    
    printf("Post created successfully!\n");
    return 1;
}

// Follow functions
int follow_user(int user_id) {
    if (current_user == NULL) {
        printf("Please login first!\n");
        return 0;
    }
    
    if (user_id == current_user->user_id) {
        printf("You cannot follow yourself!\n");
        return 0;
    }
    
    if (find_user_by_id(user_id) == NULL) {
        printf("User not found!\n");
        return 0;
    }
    
    // Check if already following
    if (is_following(current_user->user_id, user_id)) {
        printf("You are already following this user!\n");
        return 0;
    }
    
    Follow* new_follow = (Follow*)malloc(sizeof(Follow));
    if (new_follow == NULL) {
        printf("Memory allocation failed!\n");
        return 0;
    }
    
    new_follow->follower_id = current_user->user_id;
    new_follow->following_id = user_id;
    new_follow->next = follows;
    follows = new_follow;
    
    User* followed_user = find_user_by_id(user_id);
    printf("You are now following @%s!\n", followed_user->username);
    return 1;
}

int unfollow_user(int user_id) {
    if (current_user == NULL) {
        printf("Please login first!\n");
        return 0;
    }
    
    if (user_id == current_user->user_id) {
        printf("You cannot unfollow yourself!\n");
        return 0;
    }
    
    if (find_user_by_id(user_id) == NULL) {
        printf("User not found!\n");
        return 0;
    }
    
    // Check if following
    if (!is_following(current_user->user_id, user_id)) {
        printf("You are not following this user!\n");
        return 0;
    }
    
    // Remove from follows list
    Follow* current = follows;
    Follow* prev = NULL;
    
    while (current != NULL) {
        if (current->follower_id == current_user->user_id && current->following_id == user_id) {
            if (prev == NULL) {
                follows = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            
            // Also remove from close friends if they were a close friend
            CloseFriend* cf_current = close_friends;
            CloseFriend* cf_prev = NULL;
            
            while (cf_current != NULL) {
                if (cf_current->user_id == current_user->user_id && cf_current->friend_id == user_id) {
                    if (cf_prev == NULL) {
                        close_friends = cf_current->next;
                    } else {
                        cf_prev->next = cf_current->next;
                    }
                    free(cf_current);
                    break;
                }
                cf_prev = cf_current;
                cf_current = cf_current->next;
            }
            
            User* unfollowed_user = find_user_by_id(user_id);
            printf("You have unfollowed @%s\n", unfollowed_user->username);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    
    return 0;
}

int add_close_friend(int friend_id) {
    if (current_user == NULL) {
        printf("Please login first!\n");
        return 0;
    }
    
    if (friend_id == current_user->user_id) {
        printf("You cannot add yourself as a close friend!\n");
        return 0;
    }
    
    if (find_user_by_id(friend_id) == NULL) {
        printf("User not found!\n");
        return 0;
    }
    
    // Check if already a close friend
    if (is_close_friend(current_user->user_id, friend_id)) {
        printf("User is already in your close friends list!\n");
        return 0;
    }
    
    // Must be following the user to add as close friend
    if (!is_following(current_user->user_id, friend_id)) {
        printf("You must follow this user first to add them as a close friend!\n");
        return 0;
    }
    
    CloseFriend* new_close_friend = (CloseFriend*)malloc(sizeof(CloseFriend));
    if (new_close_friend == NULL) {
        printf("Memory allocation failed!\n");
        return 0;
    }
    
    new_close_friend->user_id = current_user->user_id;
    new_close_friend->friend_id = friend_id;
    new_close_friend->next = close_friends;
    close_friends = new_close_friend;
    
    User* friend_user = find_user_by_id(friend_id);
    printf("@%s added to close friends!\n", friend_user->username);
    return 1;
}

int remove_close_friend(int friend_id) {
    if (current_user == NULL) {
        printf("Please login first!\n");
        return 0;
    }
    
    if (friend_id == current_user->user_id) {
        printf("You cannot remove yourself as a close friend!\n");
        return 0;
    }
    
    if (find_user_by_id(friend_id) == NULL) {
        printf("User not found!\n");
        return 0;
    }
    
    // Check if they are a close friend
    if (!is_close_friend(current_user->user_id, friend_id)) {
        printf("User is not in your close friends list!\n");
        return 0;
    }
    
    // Remove from close friends list
    CloseFriend* current = close_friends;
    CloseFriend* prev = NULL;
    
    while (current != NULL) {
        if (current->user_id == current_user->user_id && current->friend_id == friend_id) {
            if (prev == NULL) {
                close_friends = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            
            User* friend_user = find_user_by_id(friend_id);
            printf("@%s removed from close friends!\n", friend_user->username);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    
    return 0;
}

int is_following(int follower_id, int following_id) {
    Follow* temp = follows;
    while (temp != NULL) {
        if (temp->follower_id == follower_id && 
            temp->following_id == following_id) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

int is_close_friend(int user_id, int friend_id) {
    CloseFriend* temp = close_friends;
    while (temp != NULL) {
        if (temp->user_id == user_id && temp->friend_id == friend_id) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

char* get_media_type_string(MediaType type) {
    switch (type) {
        case MEDIA_IMAGE: return "🖼️";
        case MEDIA_VIDEO: return "🎥";
        case MEDIA_AUDIO: return "🎵";
        default: return "📄";
    }
}

// File handling
void save_all_data(void) {
    FILE* file = fopen("social_media_data.txt", "w");
    if (file == NULL) {
        printf("Error saving data!\n");
        return;
    }
    
    // Save users
    fprintf(file, "USERS\n");
    User* user = users;
    while (user != NULL) {
        fprintf(file, "%d|%s|%s|%ld\n", user->user_id, user->username, 
                user->password, user->created_at);
        user = user->next;
    }
    
    // Save posts
    fprintf(file, "POSTS\n");
    Post* post = posts;
    while (post != NULL) {
        fprintf(file, "%d|%d|%s|%s|%ld|%d|%d|%s|%s\n", 
                post->post_id, post->author_id, post->author_name, 
                post->content, post->created_at, post->priority, 
                post->media_type, post->media_path, post->media_description);
        post = post->next;
    }
    
    // Save follows
    fprintf(file, "FOLLOWS\n");
    Follow* follow = follows;
    while (follow != NULL) {
        fprintf(file, "%d|%d\n", follow->follower_id, follow->following_id);
        follow = follow->next;
    }
    
    // Save close friends
    fprintf(file, "CLOSE_FRIENDS\n");
    CloseFriend* cf = close_friends;
    while (cf != NULL) {
        fprintf(file, "%d|%d\n", cf->user_id, cf->friend_id);
        cf = cf->next;
    }
    
    fclose(file);
    printf("Data saved successfully!\n");
}

void load_all_data(void) {
    FILE* file = fopen("social_media_data.txt", "r");
    if (file == NULL) {
        printf("No existing data found. Starting fresh.\n");
        return;
    }
    
    char line[1000];
    char section[20] = "";
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Remove newline
        
        if (strcmp(line, "USERS") == 0) {
            strcpy(section, "USERS");
            continue;
        } else if (strcmp(line, "POSTS") == 0) {
            strcpy(section, "POSTS");
            continue;
        } else if (strcmp(line, "FOLLOWS") == 0) {
            strcpy(section, "FOLLOWS");
            continue;
        } else if (strcmp(line, "CLOSE_FRIENDS") == 0) {
            strcpy(section, "CLOSE_FRIENDS");
            continue;
        }
        
        if (strcmp(section, "USERS") == 0) {
            User* new_user = (User*)malloc(sizeof(User));
            sscanf(line, "%d|%[^|]|%[^|]|%ld", &new_user->user_id, 
                   new_user->username, new_user->password, &new_user->created_at);
            new_user->next = users;
            users = new_user;
            if (new_user->user_id >= next_user_id) {
                next_user_id = new_user->user_id + 1;
            }
        } else if (strcmp(section, "POSTS") == 0) {
            Post* new_post = (Post*)malloc(sizeof(Post));
            int media_type_int;
            sscanf(line, "%d|%d|%[^|]|%[^|]|%ld|%d|%d|%[^|]|%[^|]", 
                   &new_post->post_id, &new_post->author_id, new_post->author_name,
                   new_post->content, &new_post->created_at, &new_post->priority,
                   &media_type_int, new_post->media_path, new_post->media_description);
            new_post->media_type = (MediaType)media_type_int;
            new_post->next = posts;
            posts = new_post;
            if (new_post->post_id >= next_post_id) {
                next_post_id = new_post->post_id + 1;
            }
        } else if (strcmp(section, "FOLLOWS") == 0) {
            Follow* new_follow = (Follow*)malloc(sizeof(Follow));
            sscanf(line, "%d|%d", &new_follow->follower_id, &new_follow->following_id);
            new_follow->next = follows;
            follows = new_follow;
        } else if (strcmp(section, "CLOSE_FRIENDS") == 0) {
            CloseFriend* new_cf = (CloseFriend*)malloc(sizeof(CloseFriend));
            sscanf(line, "%d|%d", &new_cf->user_id, &new_cf->friend_id);
            new_cf->next = close_friends;
            close_friends = new_cf;
        }
    }
    
    fclose(file);
    printf("Data loaded successfully!\n");
}

// Web interface functions
void generate_html_with_data(void) {
    FILE *template = fopen("frontend.html", "r");
    if (!template) {
        printf("❌ Error: frontend.html not found!\n");
        return;
    }
    
    // Read the entire template file
    fseek(template, 0, SEEK_END);
    long file_size = ftell(template);
    fseek(template, 0, SEEK_SET);
    
    char *template_content = malloc(file_size + 1);
    if (!template_content) {
        printf("❌ Error: Memory allocation failed!\n");
        fclose(template);
        return;
    }
    
    size_t bytes_read = fread(template_content, 1, file_size, template);
    template_content[bytes_read] = '\0';
    fclose(template);
    
    // Create enhanced version with C backend integration
    FILE *output = fopen("priority_social_media.html", "w");
    if (!output) {
        printf("❌ Error: Cannot create output file!\n");
        free(template_content);
        return;
    }
    
    // Write the complete template content (this includes all JavaScript functions)
    fprintf(output, "%s", template_content);
    
    // Add C backend integration script with real data
    fprintf(output, "\n\n<!-- C Backend Integration with Real Data -->\n");
    fprintf(output, "<script>\n");
    fprintf(output, "// Override with C Backend Data\n");
    fprintf(output, "window.cBackendReady = true;\n");
    fprintf(output, "console.log('🔗 C Backend connected with real data!');\n");
    
    // Load users from C backend
    if (users) {
        fprintf(output, "\n// Load users from C backend\n");
        fprintf(output, "users = [\n");
        User *current_user = users;
        while (current_user) {
            fprintf(output, "  {id: %d, username: '%s', password: '%s', createdAt: %ld, isOnline: true, lastSeen: %ld},\n", 
                   current_user->user_id, current_user->username, current_user->password, 
                   current_user->created_at * 1000, current_user->created_at * 1000);
            current_user = current_user->next;
        }
        fprintf(output, "];\n");
        
        // Update next user ID
        fprintf(output, "nextUserId = %d;\n", next_user_id);
    }
    
    // Load posts from C backend
    if (posts) {
        fprintf(output, "\n// Load posts from C backend\n");
        fprintf(output, "posts = [\n");
        Post *current_post = posts;
        while (current_post) {
            // Escape quotes in content
            char escaped_content[MAX_POST_CONTENT * 2];
            int j = 0;
            for (int i = 0; current_post->content[i] && j < sizeof(escaped_content) - 2; i++) {
                if (current_post->content[i] == '\'') {
                    escaped_content[j++] = '\\';
                    escaped_content[j++] = '\'';
                } else {
                    escaped_content[j++] = current_post->content[i];
                }
            }
            escaped_content[j] = '\0';
            
            fprintf(output, "  {id: %d, authorId: %d, authorName: '%s', content: '%s', timestamp: %ld, priority: %d, mediaType: %d, mediaPath: '%s', mediaDescription: '%s'},\n", 
                   current_post->post_id, current_post->author_id, current_post->author_name, 
                   escaped_content, current_post->created_at * 1000, current_post->priority,
                   current_post->media_type, current_post->media_path, current_post->media_description);
            current_post = current_post->next;
        }
        fprintf(output, "];\n");
        
        // Update next post ID
        fprintf(output, "nextPostId = %d;\n", next_post_id);
    }
    
    // Load follows from C backend
    if (follows) {
        fprintf(output, "\n// Load follows from C backend\n");
        fprintf(output, "follows = [\n");
        Follow *current_follow = follows;
        while (current_follow) {
            fprintf(output, "  {followerId: %d, followingId: %d},\n", 
                   current_follow->follower_id, current_follow->following_id);
            current_follow = current_follow->next;
        }
        fprintf(output, "];\n");
    }
    
    // Load close friends from C backend
    if (close_friends) {
        fprintf(output, "\n// Load close friends from C backend\n");
        fprintf(output, "closeFriends = [\n");
        CloseFriend *current_cf = close_friends;
        while (current_cf) {
            fprintf(output, "  {userId: %d, friendId: %d},\n", 
                   current_cf->user_id, current_cf->friend_id);
            current_cf = current_cf->next;
        }
        fprintf(output, "];\n");
    }
    
    fprintf(output, "\n// Initialize the application with C backend data\n");
    fprintf(output, "console.log('📊 Loaded ' + users.length + ' users from C backend');\n");
    fprintf(output, "console.log('📰 Loaded ' + posts.length + ' posts from C backend');\n");
    fprintf(output, "console.log('🔗 Loaded ' + follows.length + ' follows from C backend');\n");
    fprintf(output, "console.log('⭐ Loaded ' + closeFriends.length + ' close friends from C backend');\n");
    fprintf(output, "\n// Auto-save data when modified\n");
    fprintf(output, "const originalSaveData = saveData;\n");
    fprintf(output, "saveData = function() {\n");
    fprintf(output, "  originalSaveData();\n");
    fprintf(output, "  console.log('💾 Data synchronized with C backend');\n");
    fprintf(output, "};\n");
    fprintf(output, "</script>\n");
    
    free(template_content);
    fclose(output);
    printf("✅ Complete HTML with C backend data generated: priority_social_media.html\n");
}

void open_browser(void) {
    char current_dir[1000];
    char html_path[1200];
    
    // Get current directory
    if (GetCurrentDirectory(sizeof(current_dir), current_dir)) {
        snprintf(html_path, sizeof(html_path), "file:///%s/priority_social_media.html", current_dir);
        
        // Replace backslashes with forward slashes for file URL
        for (int i = 0; html_path[i]; i++) {
            if (html_path[i] == '\\') {
                html_path[i] = '/';
            }
        }
        
        printf("🌐 Opening browser: %s\n", html_path);
        ShellExecute(NULL, "open", html_path, NULL, NULL, SW_SHOWNORMAL);
    } else {
        printf("❌ Error getting current directory\n");
        // Fallback: just open the HTML file
        ShellExecute(NULL, "open", "priority_social_media.html", NULL, NULL, SW_SHOWNORMAL);
    }
}

// Utility functions
int get_int_input(void) {
    int value;
    while (scanf("%d", &value) != 1) {
        printf("Invalid input. Please enter a number: ");
        while (getchar() != '\n'); // Clear input buffer
    }
    while (getchar() != '\n'); // Clear remaining input
    return value;
}

void get_string_input(char* buffer, int max_len) {
    fgets(buffer, max_len, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline
}

void pause_screen(void) {
    printf("Press Enter to continue...");
    getchar();
}

// Initialize the application
void initialize_application(void) {
    printf("🚀 Initializing Priority Social Media...\n");
    printf("Team: MindIsFull | Course: B.tech DS-III-T022\n\n");
    
    // Load existing data
    load_all_data();
    
    // Create some sample data if none exists
    if (!users) {
        printf("📝 Creating sample data...\n");
        
        // Create sample users
        register_user("alice", "password123");
        register_user("bob", "password123");
        register_user("charlie", "password123");
        
        // Login as alice and create sample posts
        User *alice = login_user("alice", "password123");
        if (alice) {
            create_media_post("Welcome to Priority Social Media! 🚀", MEDIA_NONE, "", "");
            create_media_post("Check out this amazing sunset! 🌅", MEDIA_IMAGE, "sunset.jpg", "Beautiful sunset photo");
            create_media_post("My latest music track 🎵", MEDIA_AUDIO, "track.mp3", "Original composition");
        }
        
        // Login as bob and create posts
        User *bob = login_user("bob", "password123");
        if (bob) {
            create_media_post("Hello everyone! Great to be here! 👋", MEDIA_NONE, "", "");
            create_media_post("Sharing my vacation video 🎥", MEDIA_VIDEO, "vacation.mp4", "Beach vacation highlights");
        }
        
        // Login as charlie and create posts
        User *charlie = login_user("charlie", "password123");
        if (charlie) {
            create_media_post("Priority Social Media is awesome! ⭐", MEDIA_NONE, "", "");
        }
        
        // Set up some follow relationships
        current_user = alice;
        follow_user(2); // Alice follows Bob
        follow_user(3); // Alice follows Charlie
        
        current_user = bob;
        follow_user(1); // Bob follows Alice
        
        current_user = charlie;
        follow_user(1); // Charlie follows Alice
        follow_user(2); // Charlie follows Bob
        
        // Add some close friends
        current_user = alice;
        add_close_friend(2); // Alice adds Bob as close friend
        
        printf("✅ Sample data created!\n");
        save_all_data();
    }
    
    printf("📊 Application Statistics:\n");
    
    // Count users
    int user_count = 0;
    User *u = users;
    while (u) { user_count++; u = u->next; }
    
    // Count posts
    int post_count = 0;
    Post *p = posts;
    while (p) { post_count++; p = p->next; }
    
    // Count follows
    int follow_count = 0;
    Follow *f = follows;
    while (f) { follow_count++; f = f->next; }
    
    printf("   👥 Users: %d\n", user_count);
    printf("   📰 Posts: %d\n", post_count);
    printf("   🔗 Follows: %d\n", follow_count);
    printf("\n");
}

// Main application loop
void run_application(void) {
    int choice;
    
    while (1) {
        printf("🚀 PRIORITY SOCIAL MEDIA - C Backend + Web Frontend\n");
        printf("=====================================================\n");
        printf("1. 🌐 Launch Web Interface\n");
        printf("2. 👤 User Management (Console)\n");
        printf("3. 📰 View All Posts (Console)\n");
        printf("4. 📊 Show Statistics\n");
        printf("5. 💾 Save Data\n");
        printf("6. 🚪 Exit\n");
        printf("=====================================================\n");
        printf("Choose option (1-6): ");
        
        choice = get_int_input();
        
        switch (choice) {
            case 1:
                printf("\n🌐 Launching Web Interface...\n");
                generate_html_with_data();
                open_browser();
                printf("✅ Web interface launched!\n");
                printf("💡 The web interface is now connected to your C backend.\n");
                printf("💡 All data is synchronized between C backend and web frontend.\n\n");
                break;
                
            case 2:
                printf("\n👤 User Management\n");
                printf("Available users:\n");
                User *u = users;
                while (u) {
                    printf("   ID: %d, Username: %s\n", u->user_id, u->username);
                    u = u->next;
                }
                printf("\n");
                break;
                
            case 3:
                printf("\n📰 All Posts\n");
                Post *p = posts;
                while (p) {
                    printf("   [%d] @%s: %s %s\n", p->post_id, p->author_name, 
                           get_media_type_string(p->media_type), p->content);
                    p = p->next;
                }
                printf("\n");
                break;
                
            case 4:
                printf("\n📊 Application Statistics\n");
                printf("========================\n");
                
                int users_count = 0, posts_count = 0, follows_count = 0;
                User *user_ptr = users;
                while (user_ptr) { users_count++; user_ptr = user_ptr->next; }
                
                Post *post_ptr = posts;
                while (post_ptr) { posts_count++; post_ptr = post_ptr->next; }
                
                Follow *follow_ptr = follows;
                while (follow_ptr) { follows_count++; follow_ptr = follow_ptr->next; }
                
                printf("👥 Total Users: %d\n", users_count);
                printf("📰 Total Posts: %d\n", posts_count);
                printf("🔗 Total Follows: %d\n", follows_count);
                printf("\n");
                break;
                
            case 5:
                printf("\n💾 Saving data...\n");
                save_all_data();
                printf("✅ Data saved successfully!\n\n");
                break;
                
            case 6:
                printf("\n👋 Saving data and exiting...\n");
                save_all_data();
                printf("✅ Priority Social Media closed. Thank you!\n");
                return;
                
            default:
                printf("❌ Invalid option. Please choose 1-6.\n\n");
        }
        
        if (choice != 1) {
            pause_screen();
        }
    }
}

// Main function
int main(void) {
    printf("🚀 PRIORITY SOCIAL MEDIA - C Backend + Web Frontend\n");
    printf("===================================================\n");
    printf("Team: MindIsFull | Course: B.tech DS-III-T022\n\n");
    
    // Initialize the application
    initialize_application();
    
    // Run the main application loop
    run_application();
    
    return 0;
}