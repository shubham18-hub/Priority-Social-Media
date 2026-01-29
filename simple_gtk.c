#include <gtk/gtk.h>

typedef struct {
    char username[100];
    char password[100];
    struct User* next;
} User;

User* users = NULL;
User* current_user = NULL;

GtkWidget *username_entry, *password_entry, *post_entry, *feed_view;

void on_login(GtkWidget *widget, gpointer data) {
    const char *user = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const char *pass = gtk_entry_get_text(GTK_ENTRY(password_entry));
    
    User* new_user = malloc(sizeof(User));
    strcpy(new_user->username, user);
    strcpy(new_user->password, pass);
    new_user->next = users;
    users = new_user;
    current_user = new_user;
    
    printf("Logged in as: %s\n", user);
}

void on_post(GtkWidget *widget, gpointer data) {
    const char *content = gtk_entry_get_text(GTK_ENTRY(post_entry));
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(feed_view));
    
    char post_text[500];
    snprintf(post_text, 500, "@%s: %s\n", current_user ? current_user->username : "Anonymous", content);
    gtk_text_buffer_insert_at_cursor(buffer, post_text, -1);
    gtk_entry_set_text(GTK_ENTRY(post_entry), "");
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Social Media");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 500);
    
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Username");
    
    password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Password");
    
    GtkWidget *login_btn = gtk_button_new_with_label("Login");
    g_signal_connect(login_btn, "clicked", G_CALLBACK(on_login), NULL);
    
    post_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(post_entry), "What's happening?");
    
    GtkWidget *post_btn = gtk_button_new_with_label("Post");
    g_signal_connect(post_btn, "clicked", G_CALLBACK(on_post), NULL);
    
    feed_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(feed_view), FALSE);
    
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), feed_view);
    gtk_widget_set_size_request(scroll, -1, 200);
    
    gtk_box_append(GTK_BOX(box), username_entry);
    gtk_box_append(GTK_BOX(box), password_entry);
    gtk_box_append(GTK_BOX(box), login_btn);
    gtk_box_append(GTK_BOX(box), post_entry);
    gtk_box_append(GTK_BOX(box), post_btn);
    gtk_box_append(GTK_BOX(box), scroll);
    
    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.social", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}