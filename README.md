<h1 align="center">
  🧠 Terminal Notepad
</h1>

<p align="center">
  <img src="https://img.shields.io/badge/C++-17-blue?style=for-the-badge&logo=c%2B%2B">
  <img src="https://img.shields.io/badge/terminal-based-lightgrey?style=for-the-badge&logo=gnubash">
  <img src="https://img.shields.io/github/license/sanecodeguy/Terminal-Notepad?style=for-the-badge">
</p>

<p align="center">
  🚀 A slick terminal-based notepad written in C++ using raw pointers and custom doubly linked lists.<br>
  No dependencies. No ncurses. Just pure terminal magic.
</p>

---

## 🖥️ Preview

https://user-images.githubusercontent.com/00000000/your-demo-gif.gif  

---

## ✨ Features

- Cursor movement (← ↑ ↓ →) 
- Insert/delete characters and lines
- Multiline support with linked memory
- Built from scratch (no `std::vector`, no `std::string`)
- Works directly on terminal using ANSI escape codes

---

## 🧠 Architecture

Every character is a node in a 2D doubly linked list:

```text
H <-> e <-> l <-> l <-> o
          ↓
          w <-> o <-> r <-> l <-> d
```
Node* next;  // Right
Node* prev;  // Left
Node* up;    // Above
Node* down;  // Below

### 🚦 Controls

| Key         | Action             |
| ----------- | ------------------ |
| `← ↑ ↓ →`   | Move cursor        |
| `Backspace` | Delete char before |
| `Del`       | Delete char after  |
| `Enter`     | Insert new line    |
| `Any char`  | Insert character   |
| `Ctrl + C`  | Exit               |

### How to Use
```
git clone git@github.com:sanecodeguy/Notepad.git
cd Notepad
g++ main.cpp -o notepad
./notepad
```
### Requirements

    Linux / macOS terminal

    g++ (C++17 or higher)

    No libraries required

📜 License

MIT — Open Source

