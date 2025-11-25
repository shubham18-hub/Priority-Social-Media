// ============================================================================
// PRIORITY SOCIAL MEDIA - CLEAN IMPLEMENTATION
// All operations from C++ backend implemented
// ============================================================================

// Global Data
const DB = {
    users: [],
    posts: [],
    messages: [],
    follows: [],
    closeFriends: [],
    notifications: [],
    currentUser: null,
    nextUserId: 1,
    nextPostId: 1,
    nextMessageId: 1,
    nextNotifId: 1
};

let currentConversationUser = null;
let currentPostMedia = [];
let currentViewerMedia = [];
let currentViewerIndex = 0;

// ============================================================================
// INITIALIZATION
// ============================================================================

document.addEventListener('DOMContentLoaded', () => {
    loadFromStorage();
    initializeEventListeners();
    
    if (DB.currentUser) {
        showMainApp();
    } else {
        showPage('loginPage');
    }
});

// ============================================================================
// EVENT LISTENERS
// ============================================================================

function initializeEventListeners() {
    // Auth
    document.getElementById('loginBtn').addEventListener('click', handleLogin);
    document.getElementById('registerBtn').addEventListener('click', handleRegister);
    document.getElementById('goToRegister').addEventListener('click', () => showPage('registerPage'));
    document.getElementById('goToLogin').addEventListener('click', () => showPage('loginPage'));
    document.getElementById('logoutBtn').addEventListener('click', handleLogout);
    
    // Enter key for login/register
    document.getElementById('loginPassword').addEventListener('keypress', (e) => {
        if (e.key === 'Enter') handleLogin();
    });
    document.getElementById('registerConfirmPassword').addEventListener('keypress', (e) => {
        if (e.key === 'Enter') handleRegister();
    });
    
    // Navigation
    document.querySelectorAll('.nav-btn[data-page]').forEach(btn => {
        btn.addEventListener('click', (e) => {
            const page = e.target.getAttribute('data-page');
            showAppPage(page);
        });
    });
    
    // Feed
    document.getElementById('createPostBtn').addEventListener('click', () => openModal('createPostModal'));
    document.getElementById('searchUsersBtn').addEventListener('click', () => openModal('searchUsersModal'));
    document.getElementById('submitPostBtn').addEventListener('click', createPost);
    document.getElementById('searchPosts').addEventListener('input', renderFeed);
    
    // Search
    document.getElementById('searchUserBtn').addEventListener('click', searchUsers);
    document.getElementById('userSearchInput').addEventListener('keypress', (e) => {
        if (e.key === 'Enter') searchUsers();
    });
    
    // Messages
    document.getElementById('sendMessageBtn').addEventListener('click', sendMessage);
    document.getElementById('messageInput').addEventListener('keypress', (e) => {
        if (e.key === 'Enter') sendMessage();
    });
    
    // Notifications
    document.getElementById('markAllReadBtn').addEventListener('click', markAllNotificationsRead);
    
    // Profile
    document.getElementById('myPostsBtn').addEventListener('click', showMyPosts);
    document.getElementById('myFollowersBtn').addEventListener('click', showMyFollowers);
    document.getElementById('myFollowingBtn').addEventListener('click', showMyFollowing);
    document.getElementById('closeFriendsBtn').addEventListener('click', showCloseFriends);
    
    // Media Upload
    document.getElementById('mediaUpload').addEventListener('change', handleMediaUpload);
    
    // Modal close
    document.querySelectorAll('.modal-close').forEach(btn => {
        btn.addEventListener('click', (e) => {
            const modal = e.target.getAttribute('data-modal');
            closeModal(modal);
        });
    });
    
    // Close modal on background click
    document.querySelectorAll('.modal').forEach(modal => {
        modal.addEventListener('click', (e) => {
            if (e.target === modal) {
                modal.classList.remove('active');
            }
        });
    });
}

// ============================================================================
// STORAGE
// ============================================================================

function saveToStorage() {
    localStorage.setItem('prioritySocialDB', JSON.stringify(DB));
}

function loadFromStorage() {
    const saved = localStorage.getItem('prioritySocialDB');
    if (saved) {
        const data = JSON.parse(saved);
        Object.assign(DB, data);
    }
}

// ============================================================================
// PAGE NAVIGATION
// ============================================================================

function showPage(pageId) {
    document.querySelectorAll('.page').forEach(p => p.classList.remove('active'));
    document.getElementById(pageId).classList.add('active');
}

function showAppPage(pageId) {
    document.querySelectorAll('.app-page').forEach(p => p.classList.remove('active'));
    document.getElementById(pageId).classList.add('active');
    
    document.querySelectorAll('.nav-btn[data-page]').forEach(btn => {
        btn.classList.remove('active');
        if (btn.getAttribute('data-page') === pageId) {
            btn.classList.add('active');
        }
    });
    
    // Load page content
    switch(pageId) {
        case 'feed': renderFeed(); break;
        case 'messages': renderMessages(); break;
        case 'notifications': renderNotifications(); break;
        case 'profile': renderProfile(); break;
    }
}

function showMainApp() {
    showPage('mainApp');
    showAppPage('feed');
}

// ============================================================================
// MODAL MANAGEMENT
// ============================================================================

function openModal(modalId) {
    document.getElementById(modalId).classList.add('active');
}

function closeModal(modalId) {
    document.getElementById(modalId).classList.remove('active');
}

// ============================================================================
// AUTHENTICATION
// ============================================================================

function handleLogin() {
    const username = document.getElementById('loginUsername').value.trim();
    const password = document.getElementById('loginPassword').value;
    
    if (!username || !password) {
        showToast('Please enter username and password', 'error');
        return;
    }
    
    const user = DB.users.find(u => u.username === username && u.password === password);
    
    if (user) {
        DB.currentUser = user;
        saveToStorage();
        document.getElementById('loginUsername').value = '';
        document.getElementById('loginPassword').value = '';
        showMainApp();
        showToast(`Welcome back, ${username}!`);
    } else {
        showToast('Invalid credentials', 'error');
    }
}

function handleRegister() {
    const username = document.getElementById('registerUsername').value.trim();
    const password = document.getElementById('registerPassword').value;
    const confirm = document.getElementById('registerConfirmPassword').value;
    
    if (!username || !password || !confirm) {
        showToast('Please fill all fields', 'error');
        return;
    }
    
    if (password !== confirm) {
        showToast('Passwords do not match', 'error');
        return;
    }
    
    if (DB.users.find(u => u.username === username)) {
        showToast('Username already exists', 'error');
        return;
    }
    
    const newUser = {
        user_id: DB.nextUserId++,
        username: username,
        password: password,
        created_at: Date.now()
    };
    
    DB.users.push(newUser);
    saveToStorage();
    
    document.getElementById('registerUsername').value = '';
    document.getElementById('registerPassword').value = '';
    document.getElementById('registerConfirmPassword').value = '';
    
    showToast(`Account created! Welcome ${username}!`);
    setTimeout(() => showPage('loginPage'), 1500);
}

function handleLogout() {
    if (confirm('Are you sure you want to logout?')) {
        DB.currentUser = null;
        saveToStorage();
        showPage('loginPage');
        showToast('Logged out successfully');
    }
}

// ============================================================================
// FEED
// ============================================================================

function renderFeed() {
    const container = document.getElementById('postsContainer');
    const searchTerm = document.getElementById('searchPosts').value.toLowerCase();
    
    renderStories();
    
    const priorityPosts = [];
    const regularPosts = [];
    
    DB.posts.forEach(post => {
        const isMyPost = post.author_id === DB.currentUser.user_id;
        const isFollowing = DB.follows.some(f => 
            f.follower_id === DB.currentUser.user_id && f.following_id === post.author_id
        );
        
        // Skip if not my post and not following author
        if (!isMyPost && !isFollowing) return;
        
        // Skip if post is close friends only and I'm not a close friend (unless it's my own post)
        if (!isMyPost && post.close_friends_only && !isCloseFriend(post.author_id, DB.currentUser.user_id)) {
            return;
        }
        
        if (searchTerm) {
            if (!post.content.toLowerCase().includes(searchTerm) && 
                !post.author_name.toLowerCase().includes(searchTerm)) {
                return;
            }
        }
        
        const isPriority = isCloseFriend(DB.currentUser.user_id, post.author_id) || isMyPost;
        if (isPriority) {
            priorityPosts.push(post);
        } else {
            regularPosts.push(post);
        }
    });
    
    priorityPosts.sort((a, b) => b.created_at - a.created_at);
    regularPosts.sort((a, b) => b.created_at - a.created_at);
    
    let html = '';
    
    if (priorityPosts.length > 0) {
        html += '<div class="section-title">⭐ Priority Posts (Close Friends)</div>';
        html += priorityPosts.map(p => createPostHTML(p, true)).join('');
    }
    
    if (regularPosts.length > 0) {
        html += '<div class="section-title">📱 Regular Posts</div>';
        html += regularPosts.map(p => createPostHTML(p, false)).join('');
    }
    
    if (!html) {
        html = '<div class="empty-state">No posts to show. Follow users to see their posts!</div>';
    }
    
    container.innerHTML = html;
}

function createPostHTML(post, isPriority) {
    const mediaHTML = post.media && post.media.length > 0 ? createMediaHTML(post.media) : '';
    const closeFriendsBadge = post.close_friends_only ? '<span class="close-friends-badge">👥 Close Friends</span>' : '';
    
    return `
        <div class="post-card">
            <div class="post-header">
                <div class="post-avatar">😊</div>
                <div class="post-info">
                    <div class="post-username">${escapeHTML(post.author_name)}</div>
                    <div class="post-time">${formatTime(post.created_at)}</div>
                </div>
                ${isPriority ? '<span class="priority-badge">⭐</span>' : ''}
            </div>
            ${closeFriendsBadge}
            ${post.content ? `<div class="post-content">${escapeHTML(post.content)}</div>` : ''}
            ${mediaHTML}
            <div class="post-actions">
                <button class="post-action-btn" onclick="openUserConversation(${post.author_id})">💬 Message</button>
            </div>
        </div>
    `;
}

function createMediaHTML(mediaArray) {
    if (!mediaArray || mediaArray.length === 0) return '';
    
    const count = mediaArray.length;
    let gridClass = 'single';
    if (count === 2) gridClass = 'double';
    else if (count === 3) gridClass = 'triple';
    else if (count === 4) gridClass = 'quad';
    else if (count > 4) gridClass = 'many';
    
    const displayCount = count > 6 ? 6 : count;
    const remaining = count - displayCount;
    
    const mediaItems = mediaArray.slice(0, displayCount).map((media, index) => {
        const isLast = index === displayCount - 1 && remaining > 0;
        const mediaElement = media.type === 'video' ?
            `<video src="${media.data}" controls></video>` :
            `<img src="${media.data}" alt="Post media">`;
        
        return `
            <div class="media-item ${isLast ? 'more-indicator' : ''}" 
                 ${isLast ? `data-more="${remaining}"` : ''}
                 onclick='openMediaViewer(${JSON.stringify(mediaArray)}, ${index})'>
                ${mediaElement}
            </div>
        `;
    }).join('');
    
    return `<div class="post-media"><div class="post-media-grid ${gridClass}">${mediaItems}</div></div>`;
}

function renderStories() {
    const container = document.getElementById('storiesSection');
    const stories = [{ name: 'You', emoji: '😊' }];
    
    DB.follows.filter(f => f.follower_id === DB.currentUser.user_id).forEach(f => {
        const user = DB.users.find(u => u.user_id === f.following_id);
        if (user) {
            stories.push({ name: user.username, emoji: '🌟' });
        }
    });
    
    container.innerHTML = stories.map(s => `
        <div class="story">
            <div class="story-circle">${s.emoji}</div>
            <div class="story-name">${escapeHTML(s.name)}</div>
        </div>
    `).join('');
}

function createPost() {
    const content = document.getElementById('postContentInput').value.trim();
    const visibility = document.querySelector('input[name="postVisibility"]:checked').value;
    
    if (!content && currentPostMedia.length === 0) {
        showToast('Please write something or add media', 'error');
        return;
    }
    
    if (content.length > 1024) {
        showToast('Post too long (max 1024 characters)', 'error');
        return;
    }
    
    const newPost = {
        post_id: DB.nextPostId++,
        author_id: DB.currentUser.user_id,
        author_name: DB.currentUser.username,
        content: content,
        media: [...currentPostMedia],
        created_at: Date.now(),
        priority: 0,
        close_friends_only: visibility === 'closefriends'
    };
    
    DB.posts.push(newPost);
    
    // Notify followers (only close friends if post is close friends only)
    DB.follows.filter(f => f.following_id === DB.currentUser.user_id).forEach(f => {
        // Skip notification if post is close friends only and follower is not close friend
        if (newPost.close_friends_only && !isCloseFriend(DB.currentUser.user_id, f.follower_id)) {
            return;
        }
        
        const priority = isCloseFriend(f.follower_id, DB.currentUser.user_id) ? 1 : 0;
        const notifText = newPost.close_friends_only 
            ? `${DB.currentUser.username} shared a close friends post ⭐`
            : `${DB.currentUser.username} created a new post`;
        addNotification(f.follower_id, notifText, priority);
    });
    
    saveToStorage();
    document.getElementById('postContentInput').value = '';
    document.getElementById('mediaUpload').value = '';
    document.getElementById('mediaPreview').innerHTML = '';
    document.querySelector('input[name="postVisibility"][value="all"]').checked = true;
    currentPostMedia = [];
    closeModal('createPostModal');
    renderFeed();
    
    const visibilityMsg = visibility === 'closefriends' ? 'Close friends post created! ⭐' : 'Post created!';
    showToast(visibilityMsg);
}

// ============================================================================
// MEDIA HANDLING
// ============================================================================

function handleMediaUpload(event) {
    const files = Array.from(event.target.files);
    
    if (currentPostMedia.length + files.length > 10) {
        showToast('Maximum 10 media files allowed', 'error');
        return;
    }
    
    files.forEach(file => {
        if (file.size > 10 * 1024 * 1024) { // 10MB limit
            showToast(`${file.name} is too large (max 10MB)`, 'error');
            return;
        }
        
        const reader = new FileReader();
        reader.onload = (e) => {
            const mediaType = file.type.startsWith('video/') ? 'video' : 'image';
            const mediaData = {
                type: mediaType,
                data: e.target.result,
                name: file.name
            };
            
            currentPostMedia.push(mediaData);
            renderMediaPreview();
        };
        reader.readAsDataURL(file);
    });
}

function renderMediaPreview() {
    const container = document.getElementById('mediaPreview');
    
    container.innerHTML = currentPostMedia.map((media, index) => {
        const element = media.type === 'video' ?
            `<video src="${media.data}"></video>` :
            `<img src="${media.data}" alt="${media.name}">`;
        
        return `
            <div class="media-preview-item">
                ${element}
                <button class="media-preview-remove" onclick="removeMediaPreview(${index})">×</button>
            </div>
        `;
    }).join('');
}

function removeMediaPreview(index) {
    currentPostMedia.splice(index, 1);
    renderMediaPreview();
}

function openMediaViewer(mediaArray, startIndex) {
    currentViewerMedia = mediaArray;
    currentViewerIndex = startIndex;
    document.getElementById('mediaViewer').classList.add('active');
    renderMediaViewer();
}

function closeMediaViewer() {
    document.getElementById('mediaViewer').classList.remove('active');
    currentViewerMedia = [];
    currentViewerIndex = 0;
}

function navigateMedia(direction) {
    currentViewerIndex += direction;
    if (currentViewerIndex < 0) currentViewerIndex = currentViewerMedia.length - 1;
    if (currentViewerIndex >= currentViewerMedia.length) currentViewerIndex = 0;
    renderMediaViewer();
}

function renderMediaViewer() {
    const media = currentViewerMedia[currentViewerIndex];
    const container = document.getElementById('mediaViewerContent');
    
    if (media.type === 'video') {
        container.innerHTML = `<video src="${media.data}" controls autoplay></video>`;
    } else {
        container.innerHTML = `<img src="${media.data}" alt="Media">`;
    }
}

// Keyboard navigation for media viewer
document.addEventListener('keydown', (e) => {
    if (document.getElementById('mediaViewer').classList.contains('active')) {
        if (e.key === 'ArrowLeft') navigateMedia(-1);
        if (e.key === 'ArrowRight') navigateMedia(1);
        if (e.key === 'Escape') closeMediaViewer();
    }
});

// ============================================================================
// USER SEARCH & FOLLOW
// ============================================================================

function searchUsers() {
    const term = document.getElementById('userSearchInput').value.trim().toLowerCase();
    const container = document.getElementById('searchResults');
    
    if (!term) {
        container.innerHTML = '<div class="empty-state">Enter a username to search</div>';
        return;
    }
    
    const results = DB.users.filter(u => u.username.toLowerCase().includes(term));
    
    if (results.length === 0) {
        container.innerHTML = '<div class="empty-state">No users found</div>';
        return;
    }
    
    container.innerHTML = results.map(u => createUserCardHTML(u)).join('');
}

function createUserCardHTML(user) {
    const isMe = user.user_id === DB.currentUser.user_id;
    const following = isFollowing(DB.currentUser.user_id, user.user_id);
    const closeFriend = isCloseFriend(DB.currentUser.user_id, user.user_id);
    const followers = DB.follows.filter(f => f.following_id === user.user_id).length;
    const followingCount = DB.follows.filter(f => f.follower_id === user.user_id).length;
    
    return `
        <div class="user-card">
            <div class="user-avatar">👤</div>
            <div class="user-info">
                <div class="user-username">@${escapeHTML(user.username)} ${closeFriend ? '⭐' : ''}</div>
                <div class="user-stats">${followers} Followers • ${followingCount} Following</div>
            </div>
            <div class="user-actions">
                ${!isMe ? `
                    ${following ? 
                        `<button class="user-btn btn-unfollow" onclick="unfollowUser(${user.user_id})">Unfollow</button>` :
                        `<button class="user-btn btn-follow" onclick="followUser(${user.user_id})">Follow</button>`
                    }
                    ${following && !closeFriend ?
                        `<button class="user-btn btn-close-friend" onclick="addToCloseFriends(${user.user_id})">Add Close Friend</button>` : ''
                    }
                    ${closeFriend ?
                        `<button class="user-btn btn-unfollow" onclick="removeFromCloseFriends(${user.user_id})">Remove Close Friend</button>` : ''
                    }
                    <button class="user-btn btn-message" onclick="openUserConversation(${user.user_id})">Message</button>
                ` : '<span style="color: #999;">This is you</span>'}
            </div>
        </div>
    `;
}

function followUser(userId) {
    if (isFollowing(DB.currentUser.user_id, userId)) {
        showToast('Already following', 'error');
        return;
    }
    
    DB.follows.push({
        follower_id: DB.currentUser.user_id,
        following_id: userId
    });
    
    const user = DB.users.find(u => u.user_id === userId);
    
    // Check if the person being followed has current user as close friend (priority notification)
    const priority = isCloseFriend(userId, DB.currentUser.user_id) ? 1 : 0;
    addNotification(userId, `${DB.currentUser.username} started following you`, priority);
    
    saveToStorage();
    showToast(`Now following @${user.username}`);
    searchUsers();
}

function unfollowUser(userId) {
    const index = DB.follows.findIndex(f => 
        f.follower_id === DB.currentUser.user_id && f.following_id === userId
    );
    
    if (index === -1) {
        showToast('Not following', 'error');
        return;
    }
    
    DB.follows.splice(index, 1);
    
    // Also remove from close friends
    const cfIndex = DB.closeFriends.findIndex(c =>
        c.user_id === DB.currentUser.user_id && c.friend_id === userId
    );
    if (cfIndex !== -1) {
        DB.closeFriends.splice(cfIndex, 1);
    }
    
    const user = DB.users.find(u => u.user_id === userId);
    saveToStorage();
    showToast(`Unfollowed @${user.username}`);
    searchUsers();
}

function addToCloseFriends(userId) {
    if (!isFollowing(DB.currentUser.user_id, userId)) {
        showToast('Must follow first', 'error');
        return;
    }
    
    if (isCloseFriend(DB.currentUser.user_id, userId)) {
        showToast('Already in close friends', 'error');
        return;
    }
    
    DB.closeFriends.push({
        user_id: DB.currentUser.user_id,
        friend_id: userId
    });
    
    const user = DB.users.find(u => u.user_id === userId);
    
    // Notify the user they've been added to close friends (priority notification)
    addNotification(userId, `${DB.currentUser.username} added you to their close friends ⭐`, 1);
    
    saveToStorage();
    showToast(`@${user.username} added to close friends ⭐`);
    searchUsers();
}

function removeFromCloseFriends(userId) {
    const index = DB.closeFriends.findIndex(c =>
        c.user_id === DB.currentUser.user_id && c.friend_id === userId
    );
    
    if (index === -1) {
        showToast('Not in close friends', 'error');
        return;
    }
    
    DB.closeFriends.splice(index, 1);
    
    const user = DB.users.find(u => u.user_id === userId);
    saveToStorage();
    showToast(`@${user.username} removed from close friends`);
    searchUsers();
}

// ============================================================================
// MESSAGES
// ============================================================================

function renderMessages() {
    const container = document.getElementById('messagesList');
    
    const conversations = {};
    DB.messages.forEach(msg => {
        if (msg.sender_id === DB.currentUser.user_id || msg.receiver_id === DB.currentUser.user_id) {
            const otherId = msg.sender_id === DB.currentUser.user_id ? msg.receiver_id : msg.sender_id;
            if (!conversations[otherId]) {
                conversations[otherId] = [];
            }
            conversations[otherId].push(msg);
        }
    });
    
    const convList = Object.keys(conversations).map(userId => {
        const user = DB.users.find(u => u.user_id === parseInt(userId));
        const msgs = conversations[userId];
        const lastMsg = msgs.sort((a, b) => b.timestamp - a.timestamp)[0];
        const unreadCount = msgs.filter(m => m.receiver_id === DB.currentUser.user_id && !m.is_read).length;
        
        return { user, lastMsg, unreadCount };
    }).sort((a, b) => b.lastMsg.timestamp - a.lastMsg.timestamp);
    
    if (convList.length === 0) {
        container.innerHTML = '<div class="empty-state">No messages yet. Search for users to start chatting!</div>';
        return;
    }
    
    container.innerHTML = convList.map(c => `
        <div class="message-preview" onclick="openUserConversation(${c.user.user_id})">
            <div class="message-preview-avatar">💬</div>
            <div class="message-preview-info">
                <div class="message-preview-user">@${escapeHTML(c.user.username)}</div>
                <div class="message-preview-text">${escapeHTML(c.lastMsg.content.substring(0, 50))}${c.lastMsg.content.length > 50 ? '...' : ''}</div>
            </div>
            ${c.unreadCount > 0 ? `<div class="unread-badge">${c.unreadCount}</div>` : ''}
        </div>
    `).join('');
}

function openUserConversation(userId) {
    const user = DB.users.find(u => u.user_id === userId);
    if (!user) {
        showToast('User not found', 'error');
        return;
    }
    
    currentConversationUser = user;
    document.getElementById('conversationUsername').textContent = `Chat with @${user.username}`;
    openModal('conversationModal');
    renderConversation();
}

function renderConversation() {
    const container = document.getElementById('conversationMessages');
    
    const msgs = DB.messages.filter(m =>
        (m.sender_id === DB.currentUser.user_id && m.receiver_id === currentConversationUser.user_id) ||
        (m.sender_id === currentConversationUser.user_id && m.receiver_id === DB.currentUser.user_id)
    ).sort((a, b) => a.timestamp - b.timestamp);
    
    // Mark as read
    msgs.forEach(m => {
        if (m.receiver_id === DB.currentUser.user_id) {
            m.is_read = 1;
        }
    });
    saveToStorage();
    
    if (msgs.length === 0) {
        container.innerHTML = '<div class="empty-state">No messages yet. Start the conversation!</div>';
        return;
    }
    
    container.innerHTML = msgs.map(m => {
        const isOutgoing = m.sender_id === DB.currentUser.user_id;
        const sender = DB.users.find(u => u.user_id === m.sender_id);
        
        return `
            <div class="message-bubble-wrap ${isOutgoing ? 'outgoing' : ''}">
                <div class="message-bubble ${isOutgoing ? 'outgoing' : 'incoming'}">
                    <div class="message-sender">${escapeHTML(sender.username)}</div>
                    <div class="message-text">${escapeHTML(m.content)}</div>
                    <div class="message-timestamp">${formatTime(m.timestamp)}</div>
                </div>
            </div>
        `;
    }).join('');
    
    setTimeout(() => {
        container.scrollTop = container.scrollHeight;
    }, 100);
}

function sendMessage() {
    const content = document.getElementById('messageInput').value.trim();
    
    if (!content) return;
    
    if (content.length > 512) {
        showToast('Message too long (max 512 characters)', 'error');
        return;
    }
    
    // Check if receiver has sender as close friend for priority notification
    const isPriorityNotif = isCloseFriend(currentConversationUser.user_id, DB.currentUser.user_id) ? 1 : 0;
    
    const newMsg = {
        message_id: DB.nextMessageId++,
        sender_id: DB.currentUser.user_id,
        receiver_id: currentConversationUser.user_id,
        sender_name: DB.currentUser.username,
        content: content,
        timestamp: Date.now(),
        priority: isPriorityNotif,
        is_read: 0
    };
    
    DB.messages.push(newMsg);
    
    // Send priority notification if sender is a close friend
    const notifText = isPriorityNotif 
        ? `⭐ New message from ${DB.currentUser.username} (Close Friend)`
        : `New message from ${DB.currentUser.username}`;
    
    addNotification(
        currentConversationUser.user_id,
        notifText,
        isPriorityNotif
    );
    
    saveToStorage();
    document.getElementById('messageInput').value = '';
    renderConversation();
}

// ============================================================================
// NOTIFICATIONS
// ============================================================================

function addNotification(userId, content, priority) {
    DB.notifications.push({
        notif_id: DB.nextNotifId++,
        user_id: userId,
        content: content,
        timestamp: Date.now(),
        priority: priority,
        is_read: 0
    });
    saveToStorage();
}

function renderNotifications() {
    const container = document.getElementById('notificationsList');
    
    const allNotifs = DB.notifications.filter(n => n.user_id === DB.currentUser.user_id);
    
    // Separate into priority (close friends) and regular notifications
    const priorityNotifs = allNotifs.filter(n => n.priority === 1).sort((a, b) => b.timestamp - a.timestamp);
    const regularNotifs = allNotifs.filter(n => n.priority === 0).sort((a, b) => b.timestamp - a.timestamp);
    
    if (allNotifs.length === 0) {
        container.innerHTML = '<div class="empty-state">No notifications</div>';
        return;
    }
    
    let html = '';
    
    // Priority notifications (from close friends)
    if (priorityNotifs.length > 0) {
        html += '<div class="notif-section-title">⭐ Priority Notifications (Close Friends)</div>';
        html += priorityNotifs.map(n => `
            <div class="notification ${!n.is_read ? 'unread' : ''} priority">
                <div class="notif-icon">⭐</div>
                <div class="notif-content">
                    <div class="notif-text">${escapeHTML(n.content)}</div>
                    <div class="notif-time">${formatTime(n.timestamp)}</div>
                </div>
                ${!n.is_read ? `<button class="small-btn" onclick="markNotificationRead(${n.notif_id})">Read</button>` : ''}
            </div>
        `).join('');
    }
    
    // Regular notifications
    if (regularNotifs.length > 0) {
        html += '<div class="notif-section-title">🔔 Regular Notifications</div>';
        html += regularNotifs.map(n => `
            <div class="notification ${!n.is_read ? 'unread' : ''}">
                <div class="notif-icon">🔔</div>
                <div class="notif-content">
                    <div class="notif-text">${escapeHTML(n.content)}</div>
                    <div class="notif-time">${formatTime(n.timestamp)}</div>
                </div>
                ${!n.is_read ? `<button class="small-btn" onclick="markNotificationRead(${n.notif_id})">Read</button>` : ''}
            </div>
        `).join('');
    }
    
    container.innerHTML = html;
}

function markNotificationRead(notifId) {
    const notif = DB.notifications.find(n => n.notif_id === notifId);
    if (notif) {
        notif.is_read = 1;
        saveToStorage();
        renderNotifications();
    }
}

function markAllNotificationsRead() {
    DB.notifications.forEach(n => {
        if (n.user_id === DB.currentUser.user_id) {
            n.is_read = 1;
        }
    });
    saveToStorage();
    renderNotifications();
    showToast('All notifications marked as read');
}

// ============================================================================
// PROFILE
// ============================================================================

function renderProfile() {
    document.getElementById('profileUsername').textContent = `@${DB.currentUser.username}`;
    
    const followers = DB.follows.filter(f => f.following_id === DB.currentUser.user_id).length;
    const following = DB.follows.filter(f => f.follower_id === DB.currentUser.user_id).length;
    
    document.getElementById('profileStats').textContent = `${followers} Followers • ${following} Following`;
}

function showMyPosts() {
    const posts = DB.posts
        .filter(p => p.author_id === DB.currentUser.user_id)
        .sort((a, b) => b.created_at - a.created_at);
    
    const html = posts.length > 0 ?
        '<div class="section-title">My Posts</div>' + posts.map(p => createPostHTML(p, false)).join('') :
        '<div class="empty-state">You haven\'t posted anything yet</div>';
    
    openModal('userListModal');
    document.getElementById('userListTitle').textContent = 'My Posts';
    document.getElementById('userListContent').innerHTML = html;
}

function showMyFollowers() {
    const followers = DB.follows.filter(f => f.following_id === DB.currentUser.user_id);
    const users = followers.map(f => DB.users.find(u => u.user_id === f.follower_id)).filter(u => u);
    
    openModal('userListModal');
    document.getElementById('userListTitle').textContent = 'My Followers';
    document.getElementById('userListContent').innerHTML = users.length > 0 ?
        users.map(u => createUserCardHTML(u)).join('') :
        '<div class="empty-state">No followers yet</div>';
}

function showMyFollowing() {
    const following = DB.follows.filter(f => f.follower_id === DB.currentUser.user_id);
    const users = following.map(f => DB.users.find(u => u.user_id === f.following_id)).filter(u => u);
    
    openModal('userListModal');
    document.getElementById('userListTitle').textContent = 'Following';
    document.getElementById('userListContent').innerHTML = users.length > 0 ?
        users.map(u => createUserCardHTML(u)).join('') :
        '<div class="empty-state">Not following anyone yet</div>';
}

function showCloseFriends() {
    const closeFriends = DB.closeFriends.filter(c => c.user_id === DB.currentUser.user_id);
    const users = closeFriends.map(c => DB.users.find(u => u.user_id === c.friend_id)).filter(u => u);
    
    openModal('userListModal');
    document.getElementById('userListTitle').textContent = 'Close Friends ⭐';
    document.getElementById('userListContent').innerHTML = users.length > 0 ?
        users.map(u => createUserCardHTML(u)).join('') :
        '<div class="empty-state">No close friends yet. Add some to see priority content!</div>';
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

function isFollowing(followerId, followingId) {
    return DB.follows.some(f => f.follower_id === followerId && f.following_id === followingId);
}

function isCloseFriend(userId, friendId) {
    return DB.closeFriends.some(c => c.user_id === userId && c.friend_id === friendId);
}

function formatTime(timestamp) {
    const now = Date.now();
    const diff = now - timestamp;
    const minutes = Math.floor(diff / 60000);
    const hours = Math.floor(diff / 3600000);
    const days = Math.floor(diff / 86400000);
    
    if (minutes < 1) return 'Just now';
    if (minutes < 60) return `${minutes}m ago`;
    if (hours < 24) return `${hours}h ago`;
    if (days < 7) return `${days}d ago`;
    return new Date(timestamp).toLocaleDateString();
}

function escapeHTML(str) {
    const div = document.createElement('div');
    div.textContent = str;
    return div.innerHTML;
}

function showToast(message, type = 'success') {
    const toast = document.createElement('div');
    toast.textContent = message;
    toast.style.cssText = `
        position: fixed;
        top: 20px;
        right: 20px;
        background: ${type === 'error' ? 'linear-gradient(135deg, #FF4545, #CC3030)' : 'linear-gradient(135deg, #6B3FA0, #4B2A70)'};
        color: white;
        padding: 15px 25px;
        border-radius: 10px;
        box-shadow: 0 5px 20px rgba(0,0,0,0.3);
        z-index: 10000;
        font-weight: 500;
    `;
    
    document.body.appendChild(toast);
    
    setTimeout(() => {
        toast.style.opacity = '0';
        toast.style.transition = 'opacity 0.3s';
        setTimeout(() => document.body.removeChild(toast), 300);
    }, 3000);
}

// Save on page unload
window.addEventListener('beforeunload', saveToStorage);

console.log('✅ Priority Social Media loaded successfully!');
