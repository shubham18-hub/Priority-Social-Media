/*
 * PRIORITY SOCIAL MEDIA - Simple GTK Frontend
 * C Backend + GTK Frontend (Simplified Version)
 * 
 * Team: MindIsFull
 * Course: B.tech DS-III-T022
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include our backend (simplified version)
typedef struct User {
    int user_id;
    char username[100];
    char password[100];
    struct User* next;
} User;

typedef struct Post {
    int post_id;
    int author_id;
    char author_name[100];
    char content[1000];
    char media_type[20];
    int priority;
    struct Post* next;
} Post;

// Global variables
static User* users = NULL;
static Post* posts = NULL;
static User* current_user = NULL;
static int next_user_id = 1;
static int next_post_id = 1;

// GTK widgets
static GtkWidget *window;
static GtkWidget *main_stack;
static GtkWidget *login_entry_username;
static GtkWidget *login_entry_password;
static GtkWidget *register_entry_username;
static GtkWidget *register_entry_password;
static GtkWidget *post_content_entry;
static GtkWidget *post_type_combo;
static GtkWidget *post_audience_combo;
static GtkWidget *posts_listbox;
static GtkWidget *status_label;

// Function prototypes
static void setup_ui(void);
static void on_login_clicked(GtkWidget *widget, gpointer data);
static void on_register_clicked(GtkWidget *widget, gpointer data);
static void on_create_post_clicked(GtkWidget *widget, gpointer data);
static void show_main_interface(void);
static void show_login_interface(void);
static void update_status(const char* message, gboolean is_error);
static void refresh_posts_display(void);
static User* find_user(const char* username);
static void add_user(const char* username, const char* password);
static void add_post(const char* content, const char* media_type, const char* audience);

// Setup the user interface
static void setup_ui(void) {
    GtkWidget *login_box, *register_box, *main_box;
    GtkWidget *login_grid, *register_grid, *main_grid;
    GtkWidget *login_button, *register_button, *show_register_button, *show_login_button;
    GtkWidget *create_post_button, *logout_button;
    GtkWidget *scrolled_window;
    
    // Create main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "🚀 Priority Social Media - C + GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 700);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Create main stack for switching between login and main interface
    main_stack = gtk_stack_new();
    gtk_container_add(GTK_CONTAINER(window), main_stack);
    
    // Status label
    status_label = gtk_label_new("🚀 Welcome to Priority Social Media!");
    gtk_widget_set_margin_top(status_label, 10);
    gtk_widget_set_margin_bottom(status_label, 10);
    
    // === LOGIN INTERFACE ===
    login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_left(login_box, 50);
    gtk_widget_set_margin_right(login_box, 50);
    gtk_widget_set_margin_top(login_box, 50);
    gtk_widget_set_margin_bottom(login_box, 50);
    
    // Login title
    GtkWidget *login_title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(login_title), "<span size='x-large' weight='bold'>🔐 Login to Priority Social Media</span>");
    gtk_box_pack_start(GTK_BOX(login_box), login_title, FALSE, FALSE, 0);
    
    // Login form
    login_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(login_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(login_grid), 10);
    
    gtk_grid_attach(GTK_GRID(login_grid), gtk_label_new("Username:"), 0, 0, 1, 1);
    login_entry_username = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(login_entry_username), "Enter your username");
    gtk_grid_attach(GTK_GRID(login_grid), login_entry_username, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(login_grid), gtk_label_new("Password:"), 0, 1, 1, 1);
    login_entry_password = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(login_entry_password), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(login_entry_password), "Enter your password");
    gtk_grid_attach(GTK_GRID(login_grid), login_entry_password, 1, 1, 1, 1);
    
    login_button = gtk_button_new_with_label("🔐 Login");
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), NULL);
    gtk_grid_attach(GTK_GRID(login_grid), login_button, 0, 2, 2, 1);
    
    show_register_button = gtk_button_new_with_label("📝 Create New Account");
    g_signal_connect(show_register_button, "clicked", G_CALLBACK(gtk_stack_set_visible_child_name), "register");
    gtk_grid_attach(GTK_GRID(login_grid), show_register_button, 0, 3, 2, 1);
    
    gtk_box_pack_start(GTK_BOX(login_box), login_grid, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(login_box), status_label, FALSE, FALSE, 0);
    
    gtk_stack_add_named(GTK_STACK(main_stack), login_box, "login");
    
    // === REGISTER INTERFACE ===
    register_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_left(register_box, 50);
    gtk_widget_set_margin_right(register_box, 50);
    gtk_widget_set_margin_top(register_box, 50);
    gtk_widget_set_margin_bottom(register_box, 50);
    
    // Register title
    GtkWidget *register_title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(register_title), "<span size='x-large' weight='bold'>📝 Create Account</span>");
    gtk_box_pack_start(GTK_BOX(register_box), register_title, FALSE, FALSE, 0);
    
    // Register form
    register_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(register_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(register_grid), 10);
    
    gtk_grid_attach(GTK_GRID(register_grid), gtk_label_new("Username:"), 0, 0, 1, 1);
    register_entry_username = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(register_entry_username), "Choose a username");
    gtk_grid_attach(GTK_GRID(register_grid), register_entry_username, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(register_grid), gtk_label_new("Password:"), 0, 1, 1, 1);
    register_entry_password = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(register_entry_password), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(register_entry_password), "Choose a password");
    gtk_grid_attach(GTK_GRID(register_grid), register_entry_password, 1, 1, 1, 1);
    
    register_button = gtk_button_new_with_label("📝 Register");
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_clicked), NULL);
    gtk_grid_attach(GTK_GRID(register_grid), register_button, 0, 2, 2, 1);
    
    show_login_button = gtk_button_new_with_label("🔐 Back to Login");
    g_signal_connect(show_login_button, "clicked", G_CALLBACK(gtk_stack_set_visible_child_name), "login");
    gtk_grid_attach(GTK_GRID(register_grid), show_login_button, 0, 3, 2, 1);
    
    gtk_box_pack_start(GTK_BOX(register_box), register_grid, TRUE, TRUE, 0);
    
    gtk_stack_add_named(GTK_STACK(main_stack), register_box, "register");
    
    // === MAIN INTERFACE ===
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_left(main_box, 20);
    gtk_widget_set_margin_right(main_box, 20);
    gtk_widget_set_margin_top(main_box, 20);
    gtk_widget_set_margin_bottom(main_box, 20);
    
    // Main title
    GtkWidget *main_title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(main_title), "<span size='x-large' weight='bold'>🚀 Priority Social Media</span>");
    gtk_box_pack_start(GTK_BOX(main_box), main_title, FALSE, FALSE, 0);
    
    // Create post section
    GtkWidget *create_post_frame = gtk_frame_new("✨ Create New Post");
    GtkWidget *create_post_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_left(create_post_box, 10);
    gtk_widget_set_margin_right(create_post_box, 10);
    gtk_widget_set_margin_top(create_post_box, 10);
    gtk_widget_set_margin_bottom(create_post_box, 10);
    
    // Post content
    GtkWidget *content_label = gtk_label_new("📝 Post Content:");
    gtk_widget_set_halign(content_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(create_post_box), content_label, FALSE, FALSE, 0);
    
    post_content_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(post_content_entry), "What's on your mind?");
    gtk_box_pack_start(GTK_BOX(create_post_box), post_content_entry, FALSE, FALSE, 0);
    
    // Post type
    GtkWidget *type_label = gtk_label_new("🎯 Post Type:");
    gtk_widget_set_halign(type_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(create_post_box), type_label, FALSE, FALSE, 0);
    
    post_type_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(post_type_combo), "📄 Text Post");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(post_type_combo), "🖼️ Image Post");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(post_type_combo), "🎥 Video Post");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(post_type_combo), "🎵 Audio Post");
    gtk_combo_box_set_active(GTK_COMBO_BOX(post_type_combo), 0);
    gtk_box_pack_start(GTK_BOX(create_post_box), post_type_combo, FALSE, FALSE, 0);
    
    // Post audience
    GtkWidget *audience_label = gtk_label_new("👥 Audience:");
    gtk_widget_set_halign(audience_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(create_post_box), audience_label, FALSE, FALSE, 0);
    
    post_audience_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(post_audience_combo), "🌍 All Followers");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(post_audience_combo), "⭐ Close Friends Only");
    gtk_combo_box_set_active(GTK_COMBO_BOX(post_audience_combo), 0);
    gtk_box_pack_start(GTK_BOX(create_post_box), post_audience_combo, FALSE, FALSE, 0);
    
    // Create post button
    create_post_button = gtk_button_new_with_label("✨ Create Post");
    g_signal_connect(create_post_button, "clicked", G_CALLBACK(on_create_post_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(create_post_box), create_post_button, FALSE, FALSE, 0);
    
    gtk_container_add(GTK_CONTAINER(create_post_frame), create_post_box);
    gtk_box_pack_start(GTK_BOX(main_box), create_post_frame, FALSE, FALSE, 0);
    
    // Posts display section
    GtkWidget *posts_frame = gtk_frame_new("📰 Your Feed");
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), 
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled_window, -1, 300);
    
    posts_listbox = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), posts_listbox);
    gtk_container_add(GTK_CONTAINER(posts_frame), scrolled_window);
    gtk_box_pack_start(GTK_BOX(main_box), posts_frame, TRUE, TRUE, 0);
    
    // Logout button
    logout_button = gtk_button_new_with_label("🚪 Logout");
    g_signal_connect(logout_button, "clicked", G_CALLBACK(show_login_interface), NULL);
    gtk_box_pack_start(GTK_BOX(main_box), logout_button, FALSE, FALSE, 0);
    
    gtk_stack_add_named(GTK_STACK(main_stack), main_box, "main");
    
    // Start with login interface
    gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "login");
}

// Event handlers
static void on_login_clicked(GtkWidget *widget, gpointer data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(login_entry_username));
    const char *password = gtk_entry_get_text(GTK_ENTRY(login_entry_password));
    
    if (strlen(username) == 0 || strlen(password) == 0) {
        update_status("❌ Please enter both username and password", TRUE);
        return;
    }
    
    User *user = find_user(username);
    if (user && strcmp(user->password, password) == 0) {
        current_user = user;
        show_main_interface();
        update_status("✅ Welcome back, " + strlen(username) > 0 ? username : "User", FALSE);
    } else {
        update_status("❌ Invalid username or password", TRUE);
    }
}

static void on_register_clicked(GtkWidget *widget, gpointer data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(register_entry_username));
    const char *password = gtk_entry_get_text(GTK_ENTRY(register_entry_password));
    
    if (strlen(username) == 0 || strlen(password) == 0) {
        update_status("❌ Please enter both username and password", TRUE);
        return;
    }
    
    if (strlen(username) < 3) {
        update_status("❌ Username must be at least 3 characters", TRUE);
        return;
    }
    
    if (find_user(username)) {
        update_status("❌ Username already exists", TRUE);
        return;
    }
    
    add_user(username, password);
    update_status("✅ Registration successful! Please login.", FALSE);
    gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "login");
    
    // Clear register form
    gtk_entry_set_text(GTK_ENTRY(register_entry_username), "");
    gtk_entry_set_text(GTK_ENTRY(register_entry_password), "");
}

static void on_create_post_clicked(GtkWidget *widget, gpointer data) {
    const char *content = gtk_entry_get_text(GTK_ENTRY(post_content_entry));
    
    if (strlen(content) == 0) {
        update_status("❌ Please enter post content", TRUE);
        return;
    }
    
    if (!current_user) {
        update_status("❌ Please login first", TRUE);
        return;
    }
    
    // Get selected post type and audience
    gchar *type_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(post_type_combo));
    gchar *audience_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(post_audience_combo));
    
    add_post(content, type_text ? type_text : "text", audience_text ? audience_text : "all");
    
    update_status("✅ Post created successfully!", FALSE);
    gtk_entry_set_text(GTK_ENTRY(post_content_entry), "");
    refresh_posts_display();
    
    if (type_text) g_free(type_text);
    if (audience_text) g_free(audience_text);
}

// Helper functions
static void show_main_interface(void) {
    gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "main");
    refresh_posts_display();
}

static void show_login_interface(void) {
    current_user = NULL;
    gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "login");
    gtk_entry_set_text(GTK_ENTRY(login_entry_username), "");
    gtk_entry_set_text(GTK_ENTRY(login_entry_password), "");
    update_status("🚀 Welcome to Priority Social Media!", FALSE);
}

static void update_status(const char* message, gboolean is_error) {
    if (is_error) {
        gchar *markup = g_markup_printf_escaped("<span color='red'>%s</span>", message);
        gtk_label_set_markup(GTK_LABEL(status_label), markup);
        g_free(markup);
    } else {
        gchar *markup = g_markup_printf_escaped("<span color='green'>%s</span>", message);
        gtk_label_set_markup(GTK_LABEL(status_label), markup);
        g_free(markup);
    }
}

static void refresh_posts_display(void) {
    // Clear existing posts
    GList *children = gtk_container_get_children(GTK_CONTAINER(posts_listbox));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
    
    // Add posts
    Post *current_post = posts;
    if (!current_post) {
        GtkWidget *no_posts_label = gtk_label_new("📭 No posts yet. Create your first post!");
        gtk_widget_set_margin_top(no_posts_label, 20);
        gtk_widget_set_margin_bottom(no_posts_label, 20);
        gtk_list_box_insert(GTK_LIST_BOX(posts_listbox), no_posts_label, -1);
        gtk_widget_show_all(posts_listbox);
        return;
    }
    
    while (current_post) {
        GtkWidget *post_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        gtk_widget_set_margin_left(post_box, 10);
        gtk_widget_set_margin_right(post_box, 10);
        gtk_widget_set_margin_top(post_box, 10);
        gtk_widget_set_margin_bottom(post_box, 10);
        
        // Post header
        gchar *header_text = g_strdup_printf("👤 @%s", current_post->author_name);
        GtkWidget *header_label = gtk_label_new(header_text);
        gtk_widget_set_halign(header_label, GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(post_box), header_label, FALSE, FALSE, 0);
        g_free(header_text);
        
        // Post content
        GtkWidget *content_label = gtk_label_new(current_post->content);
        gtk_label_set_line_wrap(GTK_LABEL(content_label), TRUE);
        gtk_widget_set_halign(content_label, GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(post_box), content_label, FALSE, FALSE, 0);
        
        // Post metadata
        gchar *meta_text = g_strdup_printf("🎯 %s %s", 
                                          current_post->media_type,
                                          current_post->priority ? "⭐ Priority" : "");
        GtkWidget *meta_label = gtk_label_new(meta_text);
        gtk_widget_set_halign(meta_label, GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(post_box), meta_label, FALSE, FALSE, 0);
        g_free(meta_text);
        
        // Add separator
        GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_box_pack_start(GTK_BOX(post_box), separator, FALSE, FALSE, 0);
        
        gtk_list_box_insert(GTK_LIST_BOX(posts_listbox), post_box, -1);
        current_post = current_post->next;
    }
    
    gtk_widget_show_all(posts_listbox);
}

// Backend functions
static User* find_user(const char* username) {
    User *current = users;
    while (current) {
        if (strcmp(current->username, username) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static void add_user(const char* username, const char* password) {
    User *new_user = malloc(sizeof(User));
    new_user->user_id = next_user_id++;
    strcpy(new_user->username, username);
    strcpy(new_user->password, password);
    new_user->next = users;
    users = new_user;
}

static void add_post(const char* content, const char* media_type, const char* audience) {
    Post *new_post = malloc(sizeof(Post));
    new_post->post_id = next_post_id++;
    new_post->author_id = current_user->user_id;
    strcpy(new_post->author_name, current_user->username);
    strcpy(new_post->content, content);
    strcpy(new_post->media_type, media_type);
    new_post->priority = (strstr(audience, "Close Friends") != NULL) ? 1 : 0;
    new_post->next = posts;
    posts = new_post;
}

// Main function
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    printf("🚀 Starting Priority Social Media - C Backend + GTK Frontend\n");
    printf("Team: MindIsFull | Course: B.tech DS-III-T022\n\n");
    
    setup_ui();
    gtk_widget_show_all(window);
    
    printf("✅ GTK Interface ready!\n");
    printf("🎯 Features available:\n");
    printf("   - User registration and login\n");
    printf("   - Post creation with media types\n");
    printf("   - Audience selection (All vs Close Friends)\n");
    printf("   - Priority-based content display\n");
    printf("   - Real-time feed updates\n\n");
    
    gtk_main();
    
    // Cleanup
    User *current_user_ptr = users;
    while (current_user_ptr) {
        User *temp = current_user_ptr;
        current_user_ptr = current_user_ptr->next;
        free(temp);
    }
    
    Post *current_post = posts;
    while (current_post) {
        Post *temp = current_post;
        current_post = current_post->next;
        free(temp);
    }
    
    printf("👋 Priority Social Media closed.\n");
    return 0;
}