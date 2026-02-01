#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <json-c/json.h>

// Data Structures
typedef struct User {
    int id;
    char username[50];
    char password[100];
    time_t created_at;
    int is_online;
    struct User* next;
} User;

typedef struct Post {
    int id;
    int author_id;
    char author_name[50];
    char content[500];
    time_t timestamp;
    int is_close_friends_only;
    char media_type[20];
    char media_path[200];
    char media_description[200];
    int priority_weight;
    struct Post* next;
} Post;

typedef struct Follow {
    int follower_id;
    int following_id;
    struct Follow* next;
} Follow;

typedef struct Message {
    int id;
    int sender_id;
    int receiver_id;
    char content[500];
    time_t timestamp;
    int is_read;
    struct Message* next;
} Message;

// App State
typedef struct {
    User* users;
    Post* posts;
    Follow* follows;
    Message* messages;
    User* current_user;
    char theme[20];
    char current_view[50];
} AppState;

// Global app state
AppState app_state = {NULL, NULL, NULL, NULL, NULL, "glassmorphic", "feed"};

// Hash Map for efficient user lookup
#define HASH_SIZE 1000
User* user_hash_map[HASH_SIZE];

// Hash function
unsigned int hash(int id) {
    return id % HASH_SIZE;
}

// Add user to hash map
void add_user_to_hash(User* user) {
    unsigned int index = hash(user->id);
    user->next = user_hash_map[index];
    user_hash_map[index] = user;
}

// Find user by ID (O(1) average case)
User* find_user_by_id(int id) {
    unsigned int index = hash(id);
    User* current = user_hash_map[index];
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Priority Feed Algorithm - Move-to-Front heuristic
void prioritize_posts() {
    if (app_state.posts == NULL || app_state.current_user == NULL) return;
    
    Post* prev = NULL;
    Post* current = app_state.posts;
    Post* priority_posts = NULL;
    
    // Extract close friends posts and move to front
    while (current != NULL) {
        Post* next = current->next;
        
        // Check if post is from close friend
        int is_close_friend = 0;
        Follow* follow = app_state.follows;
        while (follow != NULL) {
            if (follow->follower_id == app_state.current_user->id && 
                follow->following_id == current->author_id) {
                is_close_friend = 1;
                break;
            }
            follow = follow->next;
        }
        
        if (is_close_friend || current->is_close_friends_only) {
            // Remove from current position
            if (prev != NULL) {
                prev->next = next;
            } else {
                app_state.posts = next;
            }
            
            // Add to priority list
            current->next = priority_posts;
            priority_posts = current;
            current->priority_weight = 10; // High priority
        } else {
            current->priority_weight = 1; // Normal priority
            prev = current;
        }
        
        current = next;
    }
    
    // Reattach priority posts to front
    if (priority_posts != NULL) {
        Post* last_priority = priority_posts;
        while (last_priority->next != NULL) {
            last_priority = last_priority->next;
        }
        last_priority->next = app_state.posts;
        app_state.posts = priority_posts;
    }
}

// Create JSON for frontend injection
char* create_posts_json() {
    json_object* posts_array = json_object_new_array();
    
    Post* current = app_state.posts;
    while (current != NULL) {
        json_object* post_obj = json_object_new_object();
        
        json_object_object_add(post_obj, "id", json_object_new_int(current->id));
        json_object_object_add(post_obj, "authorId", json_object_new_int(current->author_id));
        json_object_object_add(post_obj, "authorName", json_object_new_string(current->author_name));
        json_object_object_add(post_obj, "content", json_object_new_string(current->content));
        json_object_object_add(post_obj, "timestamp", json_object_new_int64(current->timestamp * 1000)); // JS expects milliseconds
        json_object_object_add(post_obj, "isCloseFriendsOnly", json_object_new_boolean(current->is_close_friends_only));
        json_object_object_add(post_obj, "mediaType", json_object_new_string(current->media_type));
        json_object_object_add(post_obj, "mediaPath", json_object_new_string(current->media_path));
        json_object_object_add(post_obj, "mediaDescription", json_object_new_string(current->media_description));
        json_object_object_add(post_obj, "priorityWeight", json_object_new_int(current->priority_weight));
        
        json_object_array_add(posts_array, post_obj);
        current = current->next;
    }
    
    const char* json_string = json_object_to_json_string(posts_array);
    char* result = malloc(strlen(json_string) + 1);
    strcpy(result, json_string);
    
    json_object_put(posts_array); // Free JSON object
    return result;
}

// Initialize sample data
void initialize_data() {
    // Create sample users
    User* alice = malloc(sizeof(User));
    alice->id = 1;
    strcpy(alice->username, "alice");
    strcpy(alice->password, "password123");
    alice->created_at = time(NULL);
    alice->is_online = 1;
    alice->next = NULL;
    add_user_to_hash(alice);
    app_state.users = alice;
    
    User* bob = malloc(sizeof(User));
    bob->id = 2;
    strcpy(bob->username, "bob");
    strcpy(bob->password, "password123");
    bob->created_at = time(NULL);
    bob->is_online = 1;
    bob->next = alice;
    add_user_to_hash(bob);
    
    // Create sample posts
    Post* post1 = malloc(sizeof(Post));
    post1->id = 1;
    post1->author_id = 1;
    strcpy(post1->author_name, "alice");
    strcpy(post1->content, "Welcome to Priority Social Media! 🚀");
    post1->timestamp = time(NULL);
    post1->is_close_friends_only = 0;
    strcpy(post1->media_type, "text");
    strcpy(post1->media_path, "");
    strcpy(post1->media_description, "");
    post1->priority_weight = 1;
    post1->next = NULL;
    app_state.posts = post1;
    
    // Create sample follow relationship
    Follow* follow1 = malloc(sizeof(Follow));
    follow1->follower_id = 2;
    follow1->following_id = 1;
    follow1->next = NULL;
    app_state.follows = follow1;
}

// Save state to encrypted binary file
void save_state_to_file() {
    FILE* file = fopen("app_state.dat", "wb");
    if (file == NULL) {
        printf("Error: Could not save state to file\n");
        return;
    }
    
    // Simple encryption (XOR with key)
    char key = 0xAB;
    
    // Save users count and data
    int user_count = 0;
    User* user = app_state.users;
    while (user != NULL) {
        user_count++;
        user = user->next;
    }
    
    fwrite(&user_count, sizeof(int), 1, file);
    user = app_state.users;
    while (user != NULL) {
        // Encrypt user data
        User encrypted_user = *user;
        char* data = (char*)&encrypted_user;
        for (int i = 0; i < sizeof(User); i++) {
            data[i] ^= key;
        }
        fwrite(&encrypted_user, sizeof(User), 1, file);
        user = user->next;
    }
    
    fclose(file);
    printf("State saved successfully\n");
}

// Load state from encrypted binary file
void load_state_from_file() {
    FILE* file = fopen("app_state.dat", "rb");
    if (file == NULL) {
        printf("No saved state found, using default data\n");
        initialize_data();
        return;
    }
    
    char key = 0xAB;
    int user_count;
    fread(&user_count, sizeof(int), 1, file);
    
    for (int i = 0; i < user_count; i++) {
        User* user = malloc(sizeof(User));
        fread(user, sizeof(User), 1, file);
        
        // Decrypt user data
        char* data = (char*)user;
        for (int j = 0; j < sizeof(User); j++) {
            data[j] ^= key;
        }
        
        add_user_to_hash(user);
        user->next = app_state.users;
        app_state.users = user;
    }
    
    fclose(file);
    printf("State loaded successfully\n");
}

// Cleanup memory
void cleanup() {
    // Free users
    User* user = app_state.users;
    while (user != NULL) {
        User* next = user->next;
        free(user);
        user = next;
    }
    
    // Free posts
    Post* post = app_state.posts;
    while (post != NULL) {
        Post* next = post->next;
        free(post);
        post = next;
    }
    
    // Free follows
    Follow* follow = app_state.follows;
    while (follow != NULL) {
        Follow* next = follow->next;
        free(follow);
        follow = next;
    }
    
    // Free messages
    Message* message = app_state.messages;
    while (message != NULL) {
        Message* next = message->next;
        free(message);
        message = next;
    }
}

// Main function
int main() {
    printf("🚀 Priority Social Media - C Backend Starting...\n");
    
    // Initialize hash map
    for (int i = 0; i < HASH_SIZE; i++) {
        user_hash_map[i] = NULL;
    }
    
    // Load or initialize data
    load_state_from_file();
    
    // Set current user for demo
    app_state.current_user = find_user_by_id(1);
    
    // Apply priority algorithm
    prioritize_posts();
    
    // Generate JSON for frontend
    char* posts_json = create_posts_json();
    printf("Posts JSON for frontend injection:\n%s\n", posts_json);
    free(posts_json);
    
    // Save state on exit
    save_state_to_file();
    
    // Cleanup
    cleanup();
    
    printf("Backend shutdown complete.\n");
    return 0;
}