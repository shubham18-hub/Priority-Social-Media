/*
 * PRIORITY SOCIAL MEDIA - GTK Web Frontend
 * Connects C backend with GTK frontend for web browser display
 * 
 * Team: MindIsFull
 * Course: B.tech DS-III-T022
 */

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

// Include our backend
#include "fullcode_multimedia.c"

// Global variables
static WebKitWebView *web_view;
static GtkWidget *window;
static User* current_user = NULL;

// Function prototypes
static void initialize_backend(void);
static void setup_web_interface(void);
static gboolean handle_web_request(WebKitWebView *web_view, WebKitURIRequest *request, gpointer user_data);
static void inject_javascript_api(WebKitWebView *web_view);
static char* create_json_response(const char* status, const char* message, const char* data);

// JavaScript API functions
static void js_register_user(const char* username, const char* password);
static void js_login_user(const char* username, const char* password);
static void js_create_post(const char* content, const char* media_type, const char* audience);
static char* js_get_feed(void);
static char* js_get_users(void);
static void js_follow_user(int user_id);
static void js_send_message(int receiver_id, const char* content);

// Initialize the C backend
static void initialize_backend(void) {
    printf("🚀 Initializing Priority Social Media Backend...\n");
    
    // Initialize global variables from backend
    users = NULL;
    posts = NULL;
    messages = NULL;
    follows = NULL;
    close_friends = NULL;
    notifications = NULL;
    
    next_user_id = 1;
    next_post_id = 1;
    next_message_id = 1;
    next_notif_id = 1;
    
    // Load existing data
    load_all_data();
    
    printf("✅ Backend initialized successfully!\n");
}

// Setup the web interface
static void setup_web_interface(void) {
    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "🚀 Priority Social Media");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    
    // Create WebKit web view
    web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    
    // Enable developer tools
    WebKitSettings *settings = webkit_web_view_get_settings(web_view);
    webkit_settings_set_enable_developer_extras(settings, TRUE);
    webkit_settings_set_enable_javascript(settings, TRUE);
    webkit_settings_set_javascript_can_access_clipboard(settings, TRUE);
    
    // Connect signals
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(web_view, "load-finished", G_CALLBACK(inject_javascript_api), NULL);
    
    // Add web view to window
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(web_view));
    
    // Load the HTML content
    const char* html_content = create_web_interface_html();
    webkit_web_view_load_html(web_view, html_content, "file://");
    
    // Show the window
    gtk_widget_show_all(window);
    
    printf("🌐 Web interface ready!\n");
}

// Create the HTML content for the web interface
static const char* create_web_interface_html(void) {
    static char html_buffer[50000];
    
    snprintf(html_buffer, sizeof(html_buffer), 
    "<!DOCTYPE html>\n"
    "<html lang='en'>\n"
    "<head>\n"
    "    <meta charset='UTF-8'>\n"
    "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
    "    <title>🚀 Priority Social Media - C Backend + GTK Frontend</title>\n"
    "    <style>\n"
    "        * { margin: 0; padding: 0; box-sizing: border-box; }\n"
    "        body {\n"
    "            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\n"
    "            background: linear-gradient(135deg, #0f172a 0%%, #1e293b 25%%, #334155 50%%, #475569 75%%, #64748b 100%%);\n"
    "            background-attachment: fixed;\n"
    "            min-height: 100vh;\n"
    "            color: #333;\n"
    "        }\n"
    "        .container { max-width: 1400px; margin: 0 auto; padding: 20px; }\n"
    "        .header {\n"
    "            background: linear-gradient(135deg, rgba(255, 255, 255, 0.95) 0%%, rgba(255, 255, 255, 0.85) 100%%);\n"
    "            backdrop-filter: blur(20px);\n"
    "            border: 1px solid rgba(255, 255, 255, 0.2);\n"
    "            padding: 25px;\n"
    "            border-radius: 20px;\n"
    "            margin-bottom: 20px;\n"
    "            text-align: center;\n"
    "        }\n"
    "        .header h1 {\n"
    "            background: linear-gradient(135deg, #22c55e 0%%, #3b82f6 50%%, #a855f7 100%%);\n"
    "            -webkit-background-clip: text;\n"
    "            -webkit-text-fill-color: transparent;\n"
    "            font-size: 2.5em;\n"
    "            margin-bottom: 10px;\n"
    "        }\n"
    "        .main-content {\n"
    "            display: grid;\n"
    "            grid-template-columns: 320px 1fr 320px;\n"
    "            gap: 25px;\n"
    "        }\n"
    "        .panel {\n"
    "            background: linear-gradient(135deg, rgba(255, 255, 255, 0.95) 0%%, rgba(255, 255, 255, 0.85) 100%%);\n"
    "            backdrop-filter: blur(20px);\n"
    "            border: 1px solid rgba(255, 255, 255, 0.2);\n"
    "            border-radius: 20px;\n"
    "            padding: 25px;\n"
    "        }\n"
    "        .input-group { margin-bottom: 15px; }\n"
    "        .input-group label { display: block; margin-bottom: 5px; font-weight: 600; }\n"
    "        .input-group input, .input-group textarea, .input-group select {\n"
    "            width: 100%%;\n"
    "            padding: 12px;\n"
    "            border: 2px solid rgba(203, 213, 225, 0.5);\n"
    "            border-radius: 12px;\n"
    "            font-size: 14px;\n"
    "            background: rgba(255, 255, 255, 0.9);\n"
    "        }\n"
    "        button {\n"
    "            background: linear-gradient(135deg, #22c55e 0%%, #3b82f6 50%%, #a855f7 100%%);\n"
    "            color: white;\n"
    "            border: none;\n"
    "            padding: 12px 24px;\n"
    "            border-radius: 12px;\n"
    "            cursor: pointer;\n"
    "            font-weight: 600;\n"
    "            width: 100%%;\n"
    "            margin-bottom: 10px;\n"
    "        }\n"
    "        button:hover { transform: translateY(-2px); }\n"
    "        .post {\n"
    "            background: rgba(255, 255, 255, 0.9);\n"
    "            padding: 20px;\n"
    "            border-radius: 16px;\n"
    "            margin-bottom: 20px;\n"
    "            border-left: 5px solid #22c55e;\n"
    "        }\n"
    "        .post.priority { border-left-color: #a855f7; background: rgba(243, 232, 255, 0.9); }\n"
    "        .hidden { display: none; }\n"
    "        .section-title {\n"
    "            color: #22c55e;\n"
    "            margin-bottom: 20px;\n"
    "            font-size: 20px;\n"
    "            font-weight: 700;\n"
    "        }\n"
    "        #status { padding: 10px; margin: 10px 0; border-radius: 8px; }\n"
    "        .success { background: #d4edda; color: #155724; }\n"
    "        .error { background: #f8d7da; color: #721c24; }\n"
    "    </style>\n"
    "</head>\n"
    "<body>\n"
    "    <div class='container'>\n"
    "        <div class='header'>\n"
    "            <h1>🚀 Priority Social Media</h1>\n"
    "            <p>C Backend + GTK Frontend + Web Browser</p>\n"
    "            <p style='font-size: 12px; opacity: 0.8;'>Team: MindIsFull | Course: B.tech DS-III-T022</p>\n"
    "        </div>\n"
    "        \n"
    "        <div id='status'></div>\n"
    "        \n"
    "        <!-- Authentication Section -->\n"
    "        <div id='auth-section' class='main-content'>\n"
    "            <div></div>\n"
    "            <div class='panel'>\n"
    "                <div id='login-form'>\n"
    "                    <h2 class='section-title'>🔐 Login</h2>\n"
    "                    <div class='input-group'>\n"
    "                        <label>Username</label>\n"
    "                        <input type='text' id='login-username' placeholder='Enter username'>\n"
    "                    </div>\n"
    "                    <div class='input-group'>\n"
    "                        <label>Password</label>\n"
    "                        <input type='password' id='login-password' placeholder='Enter password'>\n"
    "                    </div>\n"
    "                    <button onclick='loginUser()'>Login</button>\n"
    "                    <button onclick='showRegister()' style='background: #6c757d;'>Create Account</button>\n"
    "                </div>\n"
    "                \n"
    "                <div id='register-form' class='hidden'>\n"
    "                    <h2 class='section-title'>📝 Register</h2>\n"
    "                    <div class='input-group'>\n"
    "                        <label>Username</label>\n"
    "                        <input type='text' id='register-username' placeholder='Choose username'>\n"
    "                    </div>\n"
    "                    <div class='input-group'>\n"
    "                        <label>Password</label>\n"
    "                        <input type='password' id='register-password' placeholder='Choose password'>\n"
    "                    </div>\n"
    "                    <button onclick='registerUser()'>Register</button>\n"
    "                    <button onclick='showLogin()' style='background: #6c757d;'>Back to Login</button>\n"
    "                </div>\n"
    "            </div>\n"
    "            <div></div>\n"
    "        </div>\n"
    "        \n"
    "        <!-- Main Application Section -->\n"
    "        <div id='main-section' class='main-content hidden'>\n"
    "            <!-- Sidebar -->\n"
    "            <div class='panel'>\n"
    "                <h3 class='section-title'>📱 Navigation</h3>\n"
    "                <button onclick='showFeed()'>🏠 My Feed</button>\n"
    "                <button onclick='showCreatePost()'>✨ Create Post</button>\n"
    "                <button onclick='showUsers()'>👥 Find Users</button>\n"
    "                <button onclick='showMessages()'>💬 Messages</button>\n"
    "                <button onclick='logout()'>🚪 Logout</button>\n"
    "                \n"
    "                <div id='user-stats' style='margin-top: 20px;'>\n"
    "                    <h4>📊 Your Stats</h4>\n"
    "                    <p>Posts: <span id='posts-count'>0</span></p>\n"
    "                    <p>Following: <span id='following-count'>0</span></p>\n"
    "                </div>\n"
    "            </div>\n"
    "            \n"
    "            <!-- Main Content -->\n"
    "            <div class='panel'>\n"
    "                <div id='feed-section'>\n"
    "                    <h2 class='section-title'>📰 My Feed</h2>\n"
    "                    <div id='posts-container'></div>\n"
    "                </div>\n"
    "                \n"
    "                <div id='create-post-section' class='hidden'>\n"
    "                    <h2 class='section-title'>✨ Create New Post</h2>\n"
    "                    <div class='input-group'>\n"
    "                        <label>📝 Post Content</label>\n"
    "                        <textarea id='post-content' rows='4' placeholder='What\\'s on your mind?'></textarea>\n"
    "                    </div>\n"
    "                    <div class='input-group'>\n"
    "                        <label>🎯 Post Type</label>\n"
    "                        <select id='post-type'>\n"
    "                            <option value='text'>📄 Text Post</option>\n"
    "                            <option value='image'>🖼️ Image Post</option>\n"
    "                            <option value='video'>🎥 Video Post</option>\n"
    "                            <option value='audio'>🎵 Audio Post</option>\n"
    "                        </select>\n"
    "                    </div>\n"
    "                    <div class='input-group'>\n"
    "                        <label>👥 Audience</label>\n"
    "                        <select id='post-audience'>\n"
    "                            <option value='all'>🌍 All Followers</option>\n"
    "                            <option value='close-friends'>⭐ Close Friends Only</option>\n"
    "                        </select>\n"
    "                    </div>\n"
    "                    <button onclick='createPost()'>✨ Create Post</button>\n"
    "                    <button onclick='showFeed()' style='background: #6c757d;'>Cancel</button>\n"
    "                </div>\n"
    "                \n"
    "                <div id='users-section' class='hidden'>\n"
    "                    <h2 class='section-title'>👥 Find Users</h2>\n"
    "                    <div id='users-container'></div>\n"
    "                </div>\n"
    "            </div>\n"
    "            \n"
    "            <!-- Right Panel -->\n"
    "            <div class='panel'>\n"
    "                <h3 class='section-title'>🔔 Notifications</h3>\n"
    "                <div id='notifications-container'>\n"
    "                    <p>🎉 Welcome to Priority Social Media!</p>\n"
    "                </div>\n"
    "            </div>\n"
    "        </div>\n"
    "    </div>\n"
    "    \n"
    "    <script>\n"
    "        // Global variables\n"
    "        let currentUser = null;\n"
    "        \n"
    "        // Show status message\n"
    "        function showStatus(message, type = 'success') {\n"
    "            const status = document.getElementById('status');\n"
    "            status.textContent = message;\n"
    "            status.className = type;\n"
    "            setTimeout(() => status.textContent = '', 3000);\n"
    "        }\n"
    "        \n"
    "        // Authentication functions\n"
    "        function showRegister() {\n"
    "            document.getElementById('login-form').classList.add('hidden');\n"
    "            document.getElementById('register-form').classList.remove('hidden');\n"
    "        }\n"
    "        \n"
    "        function showLogin() {\n"
    "            document.getElementById('register-form').classList.add('hidden');\n"
    "            document.getElementById('login-form').classList.remove('hidden');\n"
    "        }\n"
    "        \n"
    "        function registerUser() {\n"
    "            const username = document.getElementById('register-username').value;\n"
    "            const password = document.getElementById('register-password').value;\n"
    "            \n"
    "            if (!username || !password) {\n"
    "                showStatus('Please fill all fields', 'error');\n"
    "                return;\n"
    "            }\n"
    "            \n"
    "            // Call C backend function\n"
    "            if (window.cBackend && window.cBackend.registerUser) {\n"
    "                const result = window.cBackend.registerUser(username, password);\n"
    "                if (result.success) {\n"
    "                    showStatus('Registration successful! Please login.', 'success');\n"
    "                    showLogin();\n"
    "                } else {\n"
    "                    showStatus(result.message, 'error');\n"
    "                }\n"
    "            } else {\n"
    "                showStatus('Backend not connected', 'error');\n"
    "            }\n"
    "        }\n"
    "        \n"
    "        function loginUser() {\n"
    "            const username = document.getElementById('login-username').value;\n"
    "            const password = document.getElementById('login-password').value;\n"
    "            \n"
    "            if (!username || !password) {\n"
    "                showStatus('Please fill all fields', 'error');\n"
    "                return;\n"
    "            }\n"
    "            \n"
    "            // Call C backend function\n"
    "            if (window.cBackend && window.cBackend.loginUser) {\n"
    "                const result = window.cBackend.loginUser(username, password);\n"
    "                if (result.success) {\n"
    "                    currentUser = result.user;\n"
    "                    showMainApp();\n"
    "                    showStatus('Welcome back, ' + username + '!', 'success');\n"
    "                } else {\n"
    "                    showStatus(result.message, 'error');\n"
    "                }\n"
    "            } else {\n"
    "                showStatus('Backend not connected', 'error');\n"
    "            }\n"
    "        }\n"
    "        \n"
    "        function logout() {\n"
    "            currentUser = null;\n"
    "            document.getElementById('auth-section').classList.remove('hidden');\n"
    "            document.getElementById('main-section').classList.add('hidden');\n"
    "            showLogin();\n"
    "        }\n"
    "        \n"
    "        function showMainApp() {\n"
    "            document.getElementById('auth-section').classList.add('hidden');\n"
    "            document.getElementById('main-section').classList.remove('hidden');\n"
    "            loadFeed();\n"
    "        }\n"
    "        \n"
    "        // Navigation functions\n"
    "        function showFeed() {\n"
    "            hideAllSections();\n"
    "            document.getElementById('feed-section').classList.remove('hidden');\n"
    "            loadFeed();\n"
    "        }\n"
    "        \n"
    "        function showCreatePost() {\n"
    "            hideAllSections();\n"
    "            document.getElementById('create-post-section').classList.remove('hidden');\n"
    "        }\n"
    "        \n"
    "        function showUsers() {\n"
    "            hideAllSections();\n"
    "            document.getElementById('users-section').classList.remove('hidden');\n"
    "            loadUsers();\n"
    "        }\n"
    "        \n"
    "        function hideAllSections() {\n"
    "            document.getElementById('feed-section').classList.add('hidden');\n"
    "            document.getElementById('create-post-section').classList.add('hidden');\n"
    "            document.getElementById('users-section').classList.add('hidden');\n"
    "        }\n"
    "        \n"
    "        // Post functions\n"
    "        function createPost() {\n"
    "            const content = document.getElementById('post-content').value;\n"
    "            const type = document.getElementById('post-type').value;\n"
    "            const audience = document.getElementById('post-audience').value;\n"
    "            \n"
    "            if (!content) {\n"
    "                showStatus('Please enter post content', 'error');\n"
    "                return;\n"
    "            }\n"
    "            \n"
    "            if (window.cBackend && window.cBackend.createPost) {\n"
    "                const result = window.cBackend.createPost(content, type, audience);\n"
    "                if (result.success) {\n"
    "                    showStatus('Post created successfully!', 'success');\n"
    "                    document.getElementById('post-content').value = '';\n"
    "                    showFeed();\n"
    "                } else {\n"
    "                    showStatus(result.message, 'error');\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "        \n"
    "        function loadFeed() {\n"
    "            if (window.cBackend && window.cBackend.getFeed) {\n"
    "                const feed = window.cBackend.getFeed();\n"
    "                const container = document.getElementById('posts-container');\n"
    "                \n"
    "                if (feed.posts && feed.posts.length > 0) {\n"
    "                    container.innerHTML = feed.posts.map(post => \n"
    "                        `<div class='post ${post.priority ? 'priority' : ''}'>\n"
    "                            <h4>${post.media_icon} @${post.author_name}</h4>\n"
    "                            <p>${post.content}</p>\n"
    "                            <small>${new Date(post.created_at * 1000).toLocaleString()}</small>\n"
    "                            ${post.priority ? '<span style=\"color: #a855f7;\">⭐ Priority</span>' : ''}\n"
    "                        </div>`\n"
    "                    ).join('');\n"
    "                } else {\n"
    "                    container.innerHTML = '<p>No posts yet. Create your first post!</p>';\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "        \n"
    "        function loadUsers() {\n"
    "            if (window.cBackend && window.cBackend.getUsers) {\n"
    "                const users = window.cBackend.getUsers();\n"
    "                const container = document.getElementById('users-container');\n"
    "                \n"
    "                if (users.users && users.users.length > 0) {\n"
    "                    container.innerHTML = users.users.map(user => \n"
    "                        `<div style='padding: 10px; margin: 10px 0; background: #f8f9fa; border-radius: 8px;'>\n"
    "                            <strong>👤 @${user.username}</strong>\n"
    "                            <button onclick='followUser(${user.user_id})' style='width: auto; padding: 5px 10px; margin-left: 10px;'>Follow</button>\n"
    "                        </div>`\n"
    "                    ).join('');\n"
    "                } else {\n"
    "                    container.innerHTML = '<p>No other users found.</p>';\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "        \n"
    "        function followUser(userId) {\n"
    "            if (window.cBackend && window.cBackend.followUser) {\n"
    "                const result = window.cBackend.followUser(userId);\n"
    "                if (result.success) {\n"
    "                    showStatus('User followed successfully!', 'success');\n"
    "                    loadUsers();\n"
    "                } else {\n"
    "                    showStatus(result.message, 'error');\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "        \n"
    "        // Initialize when page loads\n"
    "        window.addEventListener('load', function() {\n"
    "            showStatus('🚀 Priority Social Media loaded! C Backend + GTK Frontend ready.', 'success');\n"
    "        });\n"
    "    </script>\n"
    "</body>\n"
    "</html>");
    
    return html_buffer;
}

// Inject JavaScript API to connect with C backend
static void inject_javascript_api(WebKitWebView *web_view) {
    const char* js_api = 
    "window.cBackend = {\n"
    "    registerUser: function(username, password) {\n"
    "        return window.webkit.messageHandlers.registerUser.postMessage({username: username, password: password});\n"
    "    },\n"
    "    loginUser: function(username, password) {\n"
    "        return window.webkit.messageHandlers.loginUser.postMessage({username: username, password: password});\n"
    "    },\n"
    "    createPost: function(content, type, audience) {\n"
    "        return window.webkit.messageHandlers.createPost.postMessage({content: content, type: type, audience: audience});\n"
    "    },\n"
    "    getFeed: function() {\n"
    "        return window.webkit.messageHandlers.getFeed.postMessage({});\n"
    "    },\n"
    "    getUsers: function() {\n"
    "        return window.webkit.messageHandlers.getUsers.postMessage({});\n"
    "    },\n"
    "    followUser: function(userId) {\n"
    "        return window.webkit.messageHandlers.followUser.postMessage({userId: userId});\n"
    "    }\n"
    "};\n"
    "console.log('🔗 C Backend API connected!');\n";
    
    webkit_web_view_run_javascript(web_view, js_api, NULL, NULL, NULL);
}

// Main function
int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);
    
    printf("🚀 Starting Priority Social Media - C Backend + GTK Frontend\n");
    printf("Team: MindIsFull | Course: B.tech DS-III-T022\n\n");
    
    // Initialize backend
    initialize_backend();
    
    // Setup web interface
    setup_web_interface();
    
    printf("🌐 Application ready! Open your web browser to use the platform.\n");
    
    // Start GTK main loop
    gtk_main();
    
    // Save data before exit
    save_all_data();
    
    printf("👋 Priority Social Media closed. Data saved.\n");
    return 0;
}