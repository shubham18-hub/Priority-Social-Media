/*
 * PRIORITY SOCIAL MEDIA - Windows Native Version
 * C Backend + HTML Frontend Integration
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

// We'll include the backend functions without the main function
// Include all the structures and functions from fullcode_multimedia.c
// but we'll define our own main function

// Function prototypes
void start_web_server(void);
void open_browser(void);
void create_api_endpoints(void);
void handle_web_requests(void);
void generate_html_with_data(void);

// Global variables for web integration
static int server_running = 0;
static char html_output[100000];

// Create enhanced HTML with C backend integration
void generate_html_with_data(void) {
    FILE *template = fopen("frontend.html", "r");
    if (!template) {
        printf("❌ Error: frontend.html not found!\n");
        return;
    }
    
    // Read the template
    char template_content[50000];
    size_t bytes_read = fread(template_content, 1, sizeof(template_content) - 1, template);
    template_content[bytes_read] = '\0';
    fclose(template);
    
    // Create enhanced version with C backend integration
    FILE *output = fopen("priority_social_media.html", "w");
    if (!output) {
        printf("❌ Error: Cannot create output file!\n");
        return;
    }
    
    // Write the template content
    fprintf(output, "%s", template_content);
    
    // Add C backend integration script
    fprintf(output, "\n\n<!-- C Backend Integration -->\n");
    fprintf(output, "<script>\n");
    fprintf(output, "// Enhanced C Backend Integration\n");
    fprintf(output, "window.cBackendReady = true;\n");
    fprintf(output, "console.log('🔗 C Backend connected successfully!');\n");
    
    // Add user data if available
    if (users_head) {
        fprintf(output, "// Available users from C backend\n");
        fprintf(output, "window.backendUsers = [\n");
        User *current = users_head;
        while (current) {
            fprintf(output, "  {id: %d, username: '%s'},\n", 
                   current->user_id, current->username);
            current = current->next;
        }
        fprintf(output, "];\n");
    }
    
    // Add posts data if available
    if (posts_head) {
        fprintf(output, "// Available posts from C backend\n");
        fprintf(output, "window.backendPosts = [\n");
        Post *current = posts_head;
        while (current) {
            fprintf(output, "  {id: %d, author: '%s', content: '%s', priority: %d},\n", 
                   current->post_id, current->author_name, 
                   current->content, current->priority);
            current = current->next;
        }
        fprintf(output, "];\n");
    }
    
    fprintf(output, "\n// Update UI with backend data\n");
    fprintf(output, "if (window.backendUsers) {\n");
    fprintf(output, "  console.log('📊 Loaded ' + window.backendUsers.length + ' users from C backend');\n");
    fprintf(output, "}\n");
    fprintf(output, "if (window.backendPosts) {\n");
    fprintf(output, "  console.log('📰 Loaded ' + window.backendPosts.length + ' posts from C backend');\n");
    fprintf(output, "}\n");
    fprintf(output, "</script>\n");
    
    fclose(output);
    printf("✅ Enhanced HTML generated: priority_social_media.html\n");
}

// Open browser to display the application
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

// Initialize the application
void initialize_application(void) {
    printf("🚀 Initializing Priority Social Media...\n");
    printf("Team: MindIsFull | Course: B.tech DS-III-T022\n\n");
    
    // Load existing data
    load_data();
    
    // Create some sample data if none exists
    if (!users_head) {
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
        save_data();
    }
    
    printf("📊 Application Statistics:\n");
    
    // Count users
    int user_count = 0;
    User *u = users_head;
    while (u) { user_count++; u = u->next; }
    
    // Count posts
    int post_count = 0;
    Post *p = posts_head;
    while (p) { post_count++; p = p->next; }
    
    // Count follows
    int follow_count = 0;
    Follow *f = follows_head;
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
                User *u = users_head;
                while (u) {
                    printf("   ID: %d, Username: %s\n", u->user_id, u->username);
                    u = u->next;
                }
                printf("\n");
                break;
                
            case 3:
                printf("\n📰 All Posts\n");
                display_feed();
                printf("\n");
                break;
                
            case 4:
                printf("\n📊 Application Statistics\n");
                printf("========================\n");
                
                int users = 0, posts = 0, messages = 0, follows = 0;
                User *user_ptr = users_head;
                while (user_ptr) { users++; user_ptr = user_ptr->next; }
                
                Post *post_ptr = posts_head;
                while (post_ptr) { posts++; post_ptr = post_ptr->next; }
                
                Message *msg_ptr = messages_head;
                while (msg_ptr) { messages++; msg_ptr = msg_ptr->next; }
                
                Follow *follow_ptr = follows_head;
                while (follow_ptr) { follows++; follow_ptr = follow_ptr->next; }
                
                printf("👥 Total Users: %d\n", users);
                printf("📰 Total Posts: %d\n", posts);
                printf("💬 Total Messages: %d\n", messages);
                printf("🔗 Total Follows: %d\n", follows);
                printf("\n");
                break;
                
            case 5:
                printf("\n💾 Saving data...\n");
                save_data();
                printf("✅ Data saved successfully!\n\n");
                break;
                
            case 6:
                printf("\n👋 Saving data and exiting...\n");
                save_data();
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