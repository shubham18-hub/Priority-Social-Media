#include "mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QPixmap>
#include <QScrollArea>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QColor>
#include <QStackedWidget>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_stackedWidget(new QStackedWidget(this))
{
    // Set light green background for main window
    setStyleSheet("QMainWindow { background-color: #E6FFEA; }");
    
    setCentralWidget(m_stackedWidget);
    
    // Create all pages
    setupLoginPage();
    setupFeedPage();
    setupMessagesPage();
    setupProfilePage();
    
    // Start with login
    m_stackedWidget->setCurrentWidget(m_loginPage);
}

MainWindow::~MainWindow()
{
}

// ============================================================================
// LOGIN PAGE SETUP
// ============================================================================

void MainWindow::setupLoginPage()
{
    m_loginPage = new QWidget();
    m_loginPage->setStyleSheet("background-color: #E6FFEA;");
    
    QVBoxLayout* mainLayout = new QVBoxLayout(m_loginPage);
    mainLayout->setAlignment(Qt::AlignCenter);
    
    // Container for the panda + login box
    QWidget* loginContainer = new QWidget();
    loginContainer->setFixedSize(400, 500);
    
    QVBoxLayout* containerLayout = new QVBoxLayout(loginContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(0);
    
    // === PANDA IMAGE (overlapping top of box) ===
    QLabel* pandaLabel = new QLabel();
    QPixmap pandaPixmap("panda.png");
    if (pandaPixmap.isNull()) {
        pandaPixmap = QPixmap(":/assets/panda.png"); // Try resource path
    }
    if (!pandaPixmap.isNull()) {
        pandaLabel->setPixmap(pandaPixmap.scaled(180, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        pandaLabel->setText("🐼");
        pandaLabel->setStyleSheet("font-size: 120px;");
    }
    pandaLabel->setAlignment(Qt::AlignCenter);
    pandaLabel->setFixedHeight(140);
    containerLayout->addWidget(pandaLabel);
    
    // === LOGIN BOX (the box with inputs) ===
    QFrame* loginBox = new QFrame();
    loginBox->setObjectName("loginBox");
    loginBox->setStyleSheet(
        "#loginBox {"
        "   background-color: white;"
        "   border-radius: 20px;"
        "   padding: 40px 30px 30px 30px;"
        "}"
    );
    
    // Add shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(30);
    shadow->setXOffset(0);
    shadow->setYOffset(10);
    shadow->setColor(QColor(0, 0, 0, 50));
    loginBox->setGraphicsEffect(shadow);
    
    QVBoxLayout* boxLayout = new QVBoxLayout(loginBox);
    boxLayout->setSpacing(20);
    
    // Title
    QLabel* titleLabel = new QLabel("Welcome Back!");
    titleLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: bold;"
        "color: #6B3FA0;"
        "margin-bottom: 10px;"
    );
    titleLabel->setAlignment(Qt::AlignCenter);
    boxLayout->addWidget(titleLabel);
    
    // Username input
    m_usernameInput = new QLineEdit();
    m_usernameInput->setPlaceholderText("Username");
    m_usernameInput->setObjectName("loginInput");
    m_usernameInput->setMinimumHeight(45);
    boxLayout->addWidget(m_usernameInput);
    
    // Password input
    m_passwordInput = new QLineEdit();
    m_passwordInput->setPlaceholderText("Password");
    m_passwordInput->setEchoMode(QLineEdit::Password);
    m_passwordInput->setObjectName("loginInput");
    m_passwordInput->setMinimumHeight(45);
    boxLayout->addWidget(m_passwordInput);
    
    // Login button (bright gradient)
    QPushButton* loginButton = new QPushButton("Log In");
    loginButton->setObjectName("gradientButton");
    loginButton->setMinimumHeight(50);
    loginButton->setCursor(Qt::PointingHandCursor);
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);
    boxLayout->addWidget(loginButton);
    
    // Signup link (optional)
    QLabel* signupLabel = new QLabel("<a href='#' style='color: #6B3FA0;'>Don't have an account? Sign up</a>");
    signupLabel->setAlignment(Qt::AlignCenter);
    signupLabel->setOpenExternalLinks(false);
    boxLayout->addWidget(signupLabel);
    
    containerLayout->addWidget(loginBox, 0, Qt::AlignTop);
    mainLayout->addWidget(loginContainer);
    
    m_stackedWidget->addWidget(m_loginPage);
}

// ============================================================================
// FEED PAGE SETUP
// ============================================================================

void MainWindow::setupFeedPage()
{
    m_feedPage = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(m_feedPage);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Header with app title and navigation
    QWidget* header = new QWidget();
    header->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1a1a1a, stop:1 #6B3FA0); padding: 15px;");
    header->setFixedHeight(60);
    QHBoxLayout* headerLayout = new QHBoxLayout(header);
    
    QLabel* appTitle = new QLabel("Priority Social");
    appTitle->setStyleSheet("color: white; font-size: 20px; font-weight: bold;");
    headerLayout->addWidget(appTitle);
    
    headerLayout->addStretch();
    
    // Navigation buttons
    QPushButton* feedBtn = new QPushButton("Feed");
    QPushButton* messagesBtn = new QPushButton("Messages");
    QPushButton* profileBtn = new QPushButton("Profile");
    
    QString navBtnStyle = "QPushButton { background: transparent; color: white; border: none; padding: 8px 15px; font-size: 14px; }"
                          "QPushButton:hover { background: rgba(255,255,255,0.2); border-radius: 5px; }";
    feedBtn->setStyleSheet(navBtnStyle);
    messagesBtn->setStyleSheet(navBtnStyle);
    profileBtn->setStyleSheet(navBtnStyle);
    
    connect(feedBtn, &QPushButton::clicked, this, &MainWindow::showFeed);
    connect(messagesBtn, &QPushButton::clicked, this, &MainWindow::showMessages);
    connect(profileBtn, &QPushButton::clicked, this, &MainWindow::showProfile);
    
    headerLayout->addWidget(feedBtn);
    headerLayout->addWidget(messagesBtn);
    headerLayout->addWidget(profileBtn);
    
    mainLayout->addWidget(header);
    
    // === STORIES BAR ===
    QScrollArea* storiesScroll = new QScrollArea();
    storiesScroll->setWidgetResizable(true);
    storiesScroll->setFixedHeight(120);
    storiesScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    storiesScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    storiesScroll->setStyleSheet("background-color: #F5F5F5; border: none;");
    
    QWidget* storiesContainer = new QWidget();
    QHBoxLayout* storiesLayout = new QHBoxLayout(storiesContainer);
    storiesLayout->setSpacing(15);
    storiesLayout->setContentsMargins(15, 10, 15, 10);
    
    QStringList storyUsers = {"You", "Alice", "Bob", "Charlie", "Diana", "Eve"};
    for (const QString& user : storyUsers) {
        storiesLayout->addWidget(createStoryItem(user));
    }
    storiesLayout->addStretch();
    
    storiesScroll->setWidget(storiesContainer);
    mainLayout->addWidget(storiesScroll);
    
    // === FEED SCROLL AREA ===
    m_feedScrollArea = new QScrollArea();
    m_feedScrollArea->setWidgetResizable(true);
    m_feedScrollArea->setStyleSheet("background-color: #E6FFEA; border: none;");
    
    QWidget* feedContainer = new QWidget();
    m_feedLayout = new QVBoxLayout(feedContainer);
    m_feedLayout->setSpacing(20);
    m_feedLayout->setContentsMargins(20, 20, 20, 20);
    m_feedLayout->setAlignment(Qt::AlignTop);
    
    m_feedScrollArea->setWidget(feedContainer);
    mainLayout->addWidget(m_feedScrollArea);
    
    m_stackedWidget->addWidget(m_feedPage);
}

// ============================================================================
// MESSAGES PAGE SETUP
// ============================================================================

void MainWindow::setupMessagesPage()
{
    m_messagesPage = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(m_messagesPage);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Header
    QWidget* header = new QWidget();
    header->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1a1a1a, stop:1 #6B3FA0); padding: 15px;");
    header->setFixedHeight(60);
    QHBoxLayout* headerLayout = new QHBoxLayout(header);
    
    QPushButton* backBtn = new QPushButton("← Back");
    backBtn->setStyleSheet("background: transparent; color: white; border: none; font-size: 14px;");
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showFeed);
    headerLayout->addWidget(backBtn);
    
    QLabel* chatTitle = new QLabel("Messages");
    chatTitle->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    headerLayout->addWidget(chatTitle);
    headerLayout->addStretch();
    
    mainLayout->addWidget(header);
    
    // Messages scroll area
    m_messagesScrollArea = new QScrollArea();
    m_messagesScrollArea->setWidgetResizable(true);
    m_messagesScrollArea->setStyleSheet("background-color: #F5F5F5; border: none;");
    
    QWidget* messagesContainer = new QWidget();
    m_messagesLayout = new QVBoxLayout(messagesContainer);
    m_messagesLayout->setSpacing(10);
    m_messagesLayout->setContentsMargins(15, 15, 15, 15);
    m_messagesLayout->setAlignment(Qt::AlignTop);
    
    m_messagesScrollArea->setWidget(messagesContainer);
    mainLayout->addWidget(m_messagesScrollArea);
    
    // Message input area
    QWidget* inputArea = new QWidget();
    inputArea->setStyleSheet("background-color: white; border-top: 1px solid #ddd;");
    inputArea->setFixedHeight(70);
    QHBoxLayout* inputLayout = new QHBoxLayout(inputArea);
    inputLayout->setContentsMargins(15, 10, 15, 10);
    
    m_messageInput = new QLineEdit();
    m_messageInput->setPlaceholderText("Type a message...");
    m_messageInput->setStyleSheet("border: 1px solid #ccc; border-radius: 20px; padding: 10px 15px; font-size: 14px;");
    m_messageInput->setMinimumHeight(40);
    inputLayout->addWidget(m_messageInput);
    
    QPushButton* sendBtn = new QPushButton("Send");
    sendBtn->setObjectName("purpleButton");
    sendBtn->setMinimumHeight(40);
    sendBtn->setFixedWidth(80);
    sendBtn->setCursor(Qt::PointingHandCursor);
    connect(sendBtn, &QPushButton::clicked, this, &MainWindow::sendMessage);
    inputLayout->addWidget(sendBtn);
    
    mainLayout->addWidget(inputArea);
    
    m_stackedWidget->addWidget(m_messagesPage);
}

// ============================================================================
// PROFILE PAGE SETUP
// ============================================================================

void MainWindow::setupProfilePage()
{
    m_profilePage = new QWidget();
    m_profilePage->setStyleSheet("background-color: #E6FFEA;");
    QVBoxLayout* mainLayout = new QVBoxLayout(m_profilePage);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Header
    QWidget* header = new QWidget();
    header->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1a1a1a, stop:1 #6B3FA0); padding: 15px;");
    header->setFixedHeight(60);
    QHBoxLayout* headerLayout = new QHBoxLayout(header);
    
    QPushButton* backBtn = new QPushButton("← Back");
    backBtn->setStyleSheet("background: transparent; color: white; border: none; font-size: 14px;");
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showFeed);
    headerLayout->addWidget(backBtn);
    
    QLabel* profileTitle = new QLabel("Profile");
    profileTitle->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    headerLayout->addWidget(profileTitle);
    headerLayout->addStretch();
    
    mainLayout->addWidget(header);
    
    // Profile content
    QWidget* contentWidget = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(20);
    contentLayout->setAlignment(Qt::AlignTop);
    
    // Avatar
    m_profileAvatar = new QLabel();
    m_profileAvatar->setFixedSize(120, 120);
    m_profileAvatar->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #6B3FA0, stop:1 #FF1493);"
        "border-radius: 60px;"
        "border: 4px solid white;"
    );
    m_profileAvatar->setAlignment(Qt::AlignCenter);
    m_profileAvatar->setText("👤");
    QFont avatarFont = m_profileAvatar->font();
    avatarFont.setPointSize(48);
    m_profileAvatar->setFont(avatarFont);
    contentLayout->addWidget(m_profileAvatar, 0, Qt::AlignHCenter);
    
    // Username
    m_profileUsername = new QLabel("@username");
    m_profileUsername->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    m_profileUsername->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(m_profileUsername);
    
    // Stats
    m_profileStats = new QLabel("150 Followers • 200 Following");
    m_profileStats->setStyleSheet("font-size: 14px; color: #666;");
    m_profileStats->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(m_profileStats);
    
    // Close Friends toggle
    m_closeFriendToggle = new QPushButton("★ Close Friends Mode: OFF");
    m_closeFriendToggle->setObjectName("purpleButton");
    m_closeFriendToggle->setMinimumHeight(50);
    m_closeFriendToggle->setCursor(Qt::PointingHandCursor);
    m_closeFriendToggle->setCheckable(true);
    connect(m_closeFriendToggle, &QPushButton::toggled, [this](bool checked) {
        m_closeFriendToggle->setText(checked ? "★ Close Friends Mode: ON" : "★ Close Friends Mode: OFF");
        saveCloseFriendStatus(checked);
    });
    contentLayout->addWidget(m_closeFriendToggle);
    
    contentLayout->addStretch();
    mainLayout->addWidget(contentWidget);
    
    m_stackedWidget->addWidget(m_profilePage);
}

// ============================================================================
// WIDGET CREATION HELPERS
// ============================================================================

QWidget* MainWindow::createStoryItem(const QString& username)
{
    QWidget* storyWidget = new QWidget();
    storyWidget->setFixedSize(70, 90);
    
    QVBoxLayout* layout = new QVBoxLayout(storyWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);
    
    // Story circle (gradient border)
    QLabel* circle = new QLabel();
    circle->setFixedSize(64, 64);
    circle->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #FF1493, stop:1 #6B3FA0);"
        "border-radius: 32px;"
    );
    circle->setAlignment(Qt::AlignCenter);
    circle->setText("😊");
    QFont circleFont = circle->font();
    circleFont.setPointSize(24);
    circle->setFont(circleFont);
    layout->addWidget(circle, 0, Qt::AlignHCenter);
    
    // Username
    QLabel* nameLabel = new QLabel(username);
    nameLabel->setStyleSheet("font-size: 11px; color: #333;");
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setWordWrap(true);
    layout->addWidget(nameLabel);
    
    return storyWidget;
}

QWidget* MainWindow::createPostCard(const Post& post, int index)
{
    QFrame* card = new QFrame();
    card->setObjectName("postCard");
    card->setStyleSheet(
        "#postCard {"
        "   background-color: #1a1a1a;"
        "   border-radius: 15px;"
        "   padding: 15px;"
        "}"
    );
    
    // Add shadow
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(15);
    shadow->setXOffset(0);
    shadow->setYOffset(5);
    shadow->setColor(QColor(0, 0, 0, 30));
    card->setGraphicsEffect(shadow);
    
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(12);
    
    // Header (avatar + username + priority star)
    QHBoxLayout* headerLayout = new QHBoxLayout();
    
    QLabel* avatar = new QLabel();
    avatar->setFixedSize(40, 40);
    avatar->setStyleSheet("background: #6B3FA0; border-radius: 20px;");
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setText("😊");
    headerLayout->addWidget(avatar);
    
    QLabel* username = new QLabel(post.username);
    username->setStyleSheet("color: white; font-weight: bold; font-size: 15px;");
    headerLayout->addWidget(username);
    
    if (post.isPriority) {
        QLabel* star = new QLabel("⭐");
        star->setStyleSheet("font-size: 18px;");
        star->setToolTip("Priority Post (Close Friend)");
        headerLayout->addWidget(star);
    }
    
    headerLayout->addStretch();
    
    QLabel* timestamp = new QLabel(post.timestamp);
    timestamp->setStyleSheet("color: #999; font-size: 12px;");
    headerLayout->addWidget(timestamp);
    
    cardLayout->addLayout(headerLayout);
    
    // Content area (purple bubble)
    QFrame* contentBubble = new QFrame();
    contentBubble->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6B3FA0, stop:1 #4B2A70);"
        "border-radius: 12px;"
        "padding: 15px;"
    );
    
    QVBoxLayout* bubbleLayout = new QVBoxLayout(contentBubble);
    QLabel* contentLabel = new QLabel(post.content);
    contentLabel->setStyleSheet("color: white; font-size: 14px; line-height: 1.5;");
    contentLabel->setWordWrap(true);
    bubbleLayout->addWidget(contentLabel);
    
    cardLayout->addWidget(contentBubble);
    
    // Action buttons
    QHBoxLayout* actionsLayout = new QHBoxLayout();
    
    QPushButton* likeBtn = new QPushButton(QString("❤️ %1").arg(post.likes));
    QPushButton* commentBtn = new QPushButton(QString("💬 %1").arg(post.comments));
    QPushButton* shareBtn = new QPushButton("📤 Share");
    
    QString actionBtnStyle = 
        "QPushButton {"
        "   background: transparent;"
        "   color: #FF1493;"
        "   border: 1px solid #FF1493;"
        "   border-radius: 8px;"
        "   padding: 8px 15px;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background: #FF1493;"
        "   color: white;"
        "}";
    
    likeBtn->setStyleSheet(actionBtnStyle);
    commentBtn->setStyleSheet(actionBtnStyle);
    shareBtn->setStyleSheet(actionBtnStyle);
    
    likeBtn->setCursor(Qt::PointingHandCursor);
    commentBtn->setCursor(Qt::PointingHandCursor);
    shareBtn->setCursor(Qt::PointingHandCursor);
    
    connect(likeBtn, &QPushButton::clicked, [this, index]() { likePost(index); });
    
    actionsLayout->addWidget(likeBtn);
    actionsLayout->addWidget(commentBtn);
    actionsLayout->addWidget(shareBtn);
    actionsLayout->addStretch();
    
    cardLayout->addLayout(actionsLayout);
    
    return card;
}

QWidget* MainWindow::createMessageBubble(const Message& msg)
{
    QWidget* bubbleWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(bubbleWidget);
    layout->setContentsMargins(0, 5, 0, 5);
    
    if (msg.isOutgoing) {
        layout->addStretch();
    }
    
    QFrame* bubble = new QFrame();
    bubble->setMaximumWidth(400);
    
    if (msg.isOutgoing) {
        // Black bubble for sent messages
        bubble->setStyleSheet(
            "background-color: #1a1a1a;"
            "color: white;"
            "border-radius: 15px;"
            "padding: 12px 16px;"
        );
    } else {
        // Purple bubble for received messages
        bubble->setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6B3FA0, stop:1 #4B2A70);"
            "color: white;"
            "border-radius: 15px;"
            "padding: 12px 16px;"
        );
    }
    
    QVBoxLayout* bubbleLayout = new QVBoxLayout(bubble);
    bubbleLayout->setSpacing(5);
    
    QLabel* senderLabel = new QLabel(msg.sender);
    senderLabel->setStyleSheet("font-weight: bold; font-size: 12px; color: #FFD700;");
    bubbleLayout->addWidget(senderLabel);
    
    QLabel* contentLabel = new QLabel(msg.content);
    contentLabel->setStyleSheet("font-size: 14px;");
    contentLabel->setWordWrap(true);
    bubbleLayout->addWidget(contentLabel);
    
    QLabel* timeLabel = new QLabel(msg.timestamp);
    timeLabel->setStyleSheet("font-size: 11px; color: rgba(255,255,255,0.7);");
    timeLabel->setAlignment(Qt::AlignRight);
    bubbleLayout->addWidget(timeLabel);
    
    layout->addWidget(bubble);
    
    if (!msg.isOutgoing) {
        layout->addStretch();
    }
    
    return bubbleWidget;
}

// ============================================================================
// BACKEND INTEGRATION HOOKS
// ============================================================================

bool MainWindow::authenticateUser(const QString& username, const QString& password)
{
    // TODO: Replace with actual backend call
    // Example: return authenticate_user_c(username.toStdString().c_str(), password.toStdString().c_str());
    
    // For now, accept any non-empty credentials
    return !username.isEmpty() && !password.isEmpty();
}

QVector<Post> MainWindow::loadFeedPosts()
{
    // TODO: Replace with actual backend call to read posts.dat
    // Example:
    // Post* posts_array;
    // int count = load_posts_c(&posts_array);
    // Convert C array to QVector<Post>
    
    // Sample data for demonstration
    QVector<Post> posts;
    
    posts.append({
        "alice_wonder",
        "Just had the most amazing coffee at the new café downtown! ☕✨ The ambiance is perfect for working on creative projects. Highly recommend!",
        "2 hours ago",
        142,
        23,
        true, // Priority post
        ""
    });
    
    posts.append({
        "bob_builder",
        "Finally finished my C++ project! 🎉 The feeling of seeing everything compile without errors is unmatched. Time to celebrate! 🚀",
        "5 hours ago",
        89,
        15,
        false,
        ""
    });
    
    posts.append({
        "charlie_dev",
        "Hot take: Qt is underrated for building desktop apps in 2025. The widget system is so powerful and the cross-platform support is chef's kiss 👨‍🍳💋",
        "1 day ago",
        256,
        47,
        true, // Priority post
        ""
    });
    
    return posts;
}

QVector<Message> MainWindow::loadMessages()
{
    // TODO: Replace with actual backend call to read messages.dat
    // Example: Message* msgs; int count = load_messages_c(current_user, &msgs);
    
    QVector<Message> messages;
    
    messages.append({
        "Alice",
        "Hey! Did you see the new features?",
        "10:23 AM",
        false // incoming
    });
    
    messages.append({
        "You",
        "Yes! The UI looks amazing with the new design!",
        "10:25 AM",
        true // outgoing
    });
    
    messages.append({
        "Alice",
        "I know right! The panda login screen is so cute 🐼",
        "10:26 AM",
        false
    });
    
    messages.append({
        "You",
        "Haha yes! Can't wait to show this to everyone",
        "10:28 AM",
        true
    });
    
    return messages;
}

User MainWindow::loadUserProfile()
{
    // TODO: Replace with actual backend call to read users.dat
    // Example: User user = get_user_profile_c(current_user.toStdString().c_str());
    
    User profile;
    profile.username = m_currentUser.isEmpty() ? "demo_user" : m_currentUser;
    profile.displayName = "Demo User";
    profile.avatarPath = "";
    profile.followerCount = 150;
    profile.followingCount = 200;
    profile.isCloseFriend = false;
    
    return profile;
}

void MainWindow::saveCloseFriendStatus(bool status)
{
    // TODO: Replace with actual backend call
    // Example: update_close_friend_status_c(current_user.toStdString().c_str(), status);
    
    qDebug() << "Close friend status updated:" << status;
    
    // Optionally reload feed to show/hide priority posts
    if (status) {
        QMessageBox::information(this, "Close Friends Mode", 
            "You will now see priority posts from your close friends! ⭐");
    }
}

// ============================================================================
// SLOTS (Event Handlers)
// ============================================================================

void MainWindow::handleLogin()
{
    QString username = m_usernameInput->text().trimmed();
    QString password = m_passwordInput->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please enter both username and password.");
        return;
    }
    
    // Authenticate with backend
    if (authenticateUser(username, password)) {
        m_currentUser = username;
        
        // Load feed data
        m_posts = loadFeedPosts();
        
        // Clear existing feed and populate with posts
        QLayoutItem* item;
        while ((item = m_feedLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        
        for (int i = 0; i < m_posts.size(); ++i) {
            m_feedLayout->addWidget(createPostCard(m_posts[i], i));
        }
        
        // Load profile data
        m_userProfile = loadUserProfile();
        m_profileUsername->setText("@" + m_userProfile.username);
        m_profileStats->setText(QString("%1 Followers • %2 Following")
                                .arg(m_userProfile.followerCount)
                                .arg(m_userProfile.followingCount));
        
        // Switch to feed page
        m_stackedWidget->setCurrentWidget(m_feedPage);
        
        QMessageBox::information(this, "Welcome!", 
            QString("Welcome back, %1! 🎉").arg(username));
    } else {
        QMessageBox::critical(this, "Login Failed", 
            "Invalid username or password. Please try again.");
    }
}

void MainWindow::showFeed()
{
    m_stackedWidget->setCurrentWidget(m_feedPage);
}

void MainWindow::showMessages()
{
    // Load messages
    m_messages = loadMessages();
    
    // Clear existing messages
    QLayoutItem* item;
    while ((item = m_messagesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Populate messages
    for (const Message& msg : m_messages) {
        m_messagesLayout->addWidget(createMessageBubble(msg));
    }
    
    m_stackedWidget->setCurrentWidget(m_messagesPage);
    
    // Scroll to bottom
    QTimer::singleShot(100, [this]() {
        m_messagesScrollArea->verticalScrollBar()->setValue(
            m_messagesScrollArea->verticalScrollBar()->maximum()
        );
    });
}

void MainWindow::showProfile()
{
    m_stackedWidget->setCurrentWidget(m_profilePage);
}

void MainWindow::likePost(int postIndex)
{
    if (postIndex >= 0 && postIndex < m_posts.size()) {
        // TODO: Call backend to save like
        // Example: like_post_c(m_posts[postIndex].postId, current_user);
        
        m_posts[postIndex].likes++;
        
        // Refresh the feed to show updated like count
        QWidget* oldCard = m_feedLayout->itemAt(postIndex)->widget();
        QWidget* newCard = createPostCard(m_posts[postIndex], postIndex);
        m_feedLayout->replaceWidget(oldCard, newCard);
        delete oldCard;
        
        qDebug() << "Liked post by" << m_posts[postIndex].username;
    }
}

void MainWindow::sendMessage()
{
    QString messageText = m_messageInput->text().trimmed();
    
    if (messageText.isEmpty()) {
        return;
    }
    
    // TODO: Call backend to save message
    // Example: send_message_c(current_user, recipient, messageText.toStdString().c_str());
    
    // Create message object
    Message newMsg;
    newMsg.sender = "You";
    newMsg.content = messageText;
    newMsg.timestamp = QTime::currentTime().toString("h:mm AP");
    newMsg.isOutgoing = true;
    
    // Add to messages list
    m_messages.append(newMsg);
    
    // Add to UI
    m_messagesLayout->addWidget(createMessageBubble(newMsg));
    
    // Clear input
    m_messageInput->clear();
    
    // Scroll to bottom
    QTimer::singleShot(50, [this]() {
        m_messagesScrollArea->verticalScrollBar()->setValue(
            m_messagesScrollArea->verticalScrollBar()->maximum()
        );
    });
    
    qDebug() << "Message sent:" << messageText;
}


 
