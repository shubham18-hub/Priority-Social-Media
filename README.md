# Priority Social Media Platform

A modern social media web application with priority-based content delivery using data structures and algorithms.

## 🚀 Features Implemented

### 1. **User Authentication**
- ✅ User Registration (matching C++ `register_user`)
- ✅ User Login/Logout (matching C++ `login_user`/`logout_user`)
- ✅ Password validation
- ✅ Persistent sessions using localStorage

### 2. **User Profile Management**
- ✅ View own profile (matching C++ `display_user_profile`)
- ✅ View other users' profiles
- ✅ Display follower/following counts
- ✅ Profile statistics

### 3. **Social Network Features**
- ✅ Follow users (matching C++ `follow_user`)
- ✅ Unfollow users (matching C++ `unfollow_user`)
- ✅ View followers list (matching C++ `display_followers`)
- ✅ View following list (matching C++ `display_following`)
- ✅ Search users by username (matching C++ `handle_user_search`)

### 4. **Close Friends System**
- ✅ Add close friends (matching C++ `add_close_friend`)
- ✅ Remove close friends (matching C++ `remove_close_friend`)
- ✅ View close friends list (matching C++ `display_close_friends`)
- ✅ Priority marking for close friend content

### 5. **Content & Posts**
- ✅ Create posts (matching C++ `create_post`)
- ✅ View personalized feed (matching C++ `display_feed`)
- ✅ Priority posts from close friends
- ✅ Regular posts feed
- ✅ View user-specific posts (matching C++ `display_user_posts`)
- ✅ Search posts by content

### 6. **Messaging System**
- ✅ Send messages (matching C++ `send_message`)
- ✅ View all conversations (matching C++ `display_messages`)
- ✅ One-on-one chat interface
- ✅ Priority messages from close friends
- ✅ Unread message indicators
- ✅ Real-time message display

### 7. **Notifications**
- ✅ Follow notifications (matching C++ notification system)
- ✅ Post notifications
- ✅ Message notifications
- ✅ Priority notifications (⭐ for close friends)
- ✅ Mark as read functionality (matching C++ `mark_notification_read`)
- ✅ Unread notification badges

### 8. **Data Structures (Matching C++)**
```javascript
// User structure
{
    user_id: int,
    username: string,
    password: string,
    created_at: timestamp
}

// Post structure
{
    post_id: int,
    author_id: int,
    author_name: string,
    content: string,
    created_at: timestamp,
    priority: int
}

// Message structure
{
    message_id: int,
    sender_id: int,
    receiver_id: int,
    sender_name: string,
    content: string,
    timestamp: timestamp,
    priority: int,
    is_read: int
}

// Follow structure
{
    follower_id: int,
    following_id: int
}

// CloseFriend structure
{
    user_id: int,
    friend_id: int
}

// Notification structure
{
    notif_id: int,
    user_id: int,
    content: string,
    timestamp: timestamp,
    priority: int,
    is_read: int
}
```

### 9. **Priority System**
- ✅ Close friends get priority in feed
- ✅ Priority notifications marked with ⭐
- ✅ Priority messages highlighted
- ✅ Automatic priority calculation based on close friend status

### 10. **UI/UX Features**
- ✅ Modern Instagram-inspired design
- ✅ Purple/Black gradient theme
- ✅ Responsive design
- ✅ Smooth animations
- ✅ Modal dialogs
- ✅ Real-time search
- ✅ Toast notifications
- ✅ Loading states

## 📂 File Structure

```
d:/New PBL/
├── index.html          # Main HTML structure
├── styles.css          # Complete styling
├── app.js             # All functionality & data structures
├── fullcode.cpp       # C++ backend reference
├── mainwindow.cpp     # Qt UI (alternative)
├── mainwindow.h       # Qt headers
└── README.md          # This file
```

## 🎯 How to Use

1. **Open the Application**
   - Simply open `index.html` in any modern web browser

2. **Create an Account**
   - Click "Don't have an account? Sign up"
   - Enter username and password
   - Click "Sign Up"

3. **Login**
   - Enter your credentials
   - Click "Log In"

4. **Explore Features**
   - **Feed**: View posts from people you follow
   - **Create Post**: Click "✏️ Create Post" button
   - **Search Users**: Click "🔍 Search Users" button
   - **Messages**: Click "Messages" in navigation
   - **Notifications**: Click "🔔 Notifications"
   - **Profile**: View your profile and statistics

5. **Social Features**
   - Search for users
   - Follow/Unfollow users
   - Add close friends for priority content
   - Send messages
   - View notifications

## 🔧 Technical Implementation

### Data Persistence
- Uses browser's `localStorage` for data persistence
- All data structures are saved on every action
- Data is automatically loaded on page refresh
- Format compatible with C++ backend structure

### Algorithms Used
- **Priority Queue Logic**: Close friends content appears first
- **Search Algorithm**: Real-time substring matching
- **Sorting**: Messages/posts sorted by timestamp
- **Filtering**: Content filtered based on follow relationships

### Operations Complexity
- User search: O(n)
- Follow/Unfollow: O(1)
- Load feed: O(n * m) where n = posts, m = follows
- Send message: O(1)
- Notifications: O(n)

## 🎨 Design Features

- **Color Scheme**: Purple (#6B3FA0), Pink (#FF1493), Black (#1a1a1a)
- **Fonts**: Segoe UI, system fonts
- **Animations**: Smooth transitions and hover effects
- **Responsive**: Works on desktop and mobile
- **Accessibility**: Clear visual hierarchy

## 🔒 Security Note

This is a **demo application** for educational purposes. In production:
- Passwords should be hashed (bcrypt, argon2)
- Use HTTPS
- Implement CSRF protection
- Add rate limiting
- Use proper backend authentication (JWT, sessions)

## 📊 All C++ Operations Mapped

| C++ Function | JavaScript Function | Status |
|-------------|---------------------|--------|
| `register_user` | `handleRegister` | ✅ |
| `login_user` | `handleLogin` | ✅ |
| `logout_user` | `logout` | ✅ |
| `follow_user` | `followUser` | ✅ |
| `unfollow_user` | `unfollowUser` | ✅ |
| `add_close_friend` | `addCloseFriend` | ✅ |
| `remove_close_friend` | `removeCloseFriend` | ✅ |
| `create_post` | `createPost` | ✅ |
| `display_feed` | `loadFeed` | ✅ |
| `send_message` | `sendConversationMessage` | ✅ |
| `display_messages` | `loadAllMessages` | ✅ |
| `display_notifications` | `loadNotifications` | ✅ |
| `mark_notification_read` | `markNotificationRead` | ✅ |
| `display_user_profile` | `viewUserProfile` | ✅ |
| `display_followers` | `showMyFollowers` | ✅ |
| `display_following` | `showMyFollowing` | ✅ |
| `display_close_friends` | `showCloseFriendsList` | ✅ |
| `handle_user_search` | `searchUsers` | ✅ |

## 🚀 Future Enhancements (Optional)

- [ ] Image upload for posts and profiles
- [ ] Like/Comment on posts
- [ ] Delete posts
- [ ] Edit profile
- [ ] Dark mode toggle
- [ ] Export data to C++ format (.dat files)
- [ ] Real-time updates (WebSocket)
- [ ] Group messaging
- [ ] Story feature

## 👥 Team

**MindIsFull** | Course: DS-III-T022

---

**Note**: This web frontend uses the exact same data structures and logic as the C++ backend implementation in `fullcode.cpp`, providing a modern UI for the priority social media platform.
