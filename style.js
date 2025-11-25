/* ============================================================================
   PRIORITY SOCIAL MEDIA - STYLESHEET
   Modern Instagram-like theme with black + purple accents
   ============================================================================ */

/* Global Styles */
* {
    font-family: "Segoe UI", "Helvetica Neue", Arial, sans-serif;
}

QWidget {
    background-color: #E6FFEA;
}

/* Login Input Fields */
#loginInput {
    border: 2px solid #E0E0E0;
    border-radius: 12px;
    padding: 12px 16px;
    font-size: 15px;
    background-color: #F9F9F9;
}

#loginInput:focus {
    border: 2px solid #6B3FA0;
    background-color: white;
}

/* Gradient Login Button */
#gradientButton {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, 
                stop:0 #FF1493, stop:1 #FF6B6B);
    color: white;
    border: none;
    border-radius: 12px;
    font-size: 16px;
    font-weight: bold;
    padding: 12px;
}

#gradientButton:hover {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, 
                stop:0 #FF006E, stop:1 #FF4545);
}

#gradientButton:pressed {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, 
                stop:0 #CC0057, stop:1 #CC3030);
}

/* Purple Action Buttons */
#purpleButton {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #6B3FA0, stop:1 #4B2A70);
    color: white;
    border: none;
    border-radius: 10px;
    font-size: 15px;
    font-weight: bold;
    padding: 10px 20px;
}

#purpleButton:hover {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #7B4FB0, stop:1 #5B3A80);
}

#purpleButton:pressed {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #5B2F90, stop:1 #3B1A60);
}

/* Scroll Areas */
QScrollArea {
    border: none;
}

QScrollBar:vertical {
    border: none;
    background: #F0F0F0;
    width: 10px;
    margin: 0px;
}

QScrollBar::handle:vertical {
    background: #6B3FA0;
    border-radius: 5px;
    min-height: 30px;
}

QScrollBar::handle:vertical:hover {
    background: #7B4FB0;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QScrollBar:horizontal {
    border: none;
    background: #F0F0F0;
    height: 10px;
    margin: 0px;
}

QScrollBar::handle:horizontal {
    background: #6B3FA0;
    border-radius: 5px;
    min-width: 30px;
}

QScrollBar::handle:horizontal:hover {
    background: #7B4FB0;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

/* Message Input */
QLineEdit {
    selection-background-color: #6B3FA0;
    selection-color: white;
}
