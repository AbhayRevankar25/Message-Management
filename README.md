# Message-Management

# 📬 AVL-MessageBox

A console-based user messaging system built in C that uses an **AVL Tree** to manage users and a **queue** for each user's messages. Supports creating users, sending messages, viewing inboxes, and keyword search — all with efficient data structures and an interactive UI.

---

## 🛠️ Features

- 📥 Create users using an AVL tree (balanced for fast search/insert)
- 📤 Send messages from one user to another
- 📨 Store received messages in a queue (FIFO order)
- 🔍 Search for messages by keyword
- 🧾 View all messages for a user or globally
- 🎨 Windows console UI with colors and input handling

---

## ⚙️ How It Works

- Each user is a node in an AVL Tree (self-balancing binary tree)
- Each user node contains a `messageQueue` linked list
- Messages are sent by enqueuing to the recipient's queue
- Messages can be searched or viewed via console options

---


## 🧰 Setup Instructions

### ✅ Prerequisites

- A C compiler like GCC or TDM-GCC
- Windows Command Prompt / Terminal (for `conio.h` and `system("cls")` support)

### 💻 How to Compile & Run

Using GCC:

```bash
gcc main.c -o messagebox
./messagebox
