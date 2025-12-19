#include "stylesheet.h"

void StyleSheetManager::applyTheme(QApplication &app) {
    app.setStyleSheet(R"(
        QMainWindow, QDialog { background-color: #0f111a; color: #eceff4; font-family: 'Segoe UI', sans-serif; }
        QWidget#sidebar { background-color: #1a1c25; border-right: 1px solid #2e3440; }

        QLineEdit, QComboBox {
            background-color: #242933;
            border: 1px solid #3b4252;
            border-radius: 6px;
            color: #d8dee9;
            padding: 5px 8px;
        }

        QComboBox::drop-down { border: none; width: 25px; }
        QComboBox QAbstractItemView { background-color: #242933; selection-background-color: #3b4252; color: #d8dee9; }

        QLineEdit:focus, QComboBox:focus { border: 1px solid #88c0d0; }

        QListWidget { border: none; background: transparent; }
        QListWidget::item { background-color: #242933; color: #d8dee9; padding: 12px; border-radius: 4px; margin: 4px 8px; }
        QListWidget::item:selected { background-color: #3b4252; color: #88c0d0; border-left: 3px solid #88c0d0; font-weight: bold; }

        QPushButton { background-color: #3b4252; color: #eceff4; border-radius: 4px; padding: 10px; border: 1px solid #434c5e; }
        QPushButton:hover { background-color: #434c5e; }
        QPushButton#addBtn, QPushButton[text="Save Contact"] { background-color: #5e81ac; border: 1px solid #81a1c1; }
        QPushButton#deleteBtn { background-color: #bf616a; border: 1px solid #d08770; }

        QLabel#nameLabel { font-size: 20px; font-weight: bold; }
        QLabel#photoLabel { background-color: #1a1c25; border: 2px solid #3b4252; border-radius: 8px; }
    )");
}
