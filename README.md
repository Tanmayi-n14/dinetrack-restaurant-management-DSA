# 🍽️ DineTrack — Restaurant Management & Billing System

A terminal-based restaurant order, inventory, and billing automation system developed in **C** as a **Data Structures & Algorithms (DSA)** course project.

---

## 📌 Overview

**DineTrack** streamlines restaurant operations by combining dynamic data structures for real-time order processing with CSV file persistence for transaction records.

---

## 🧠 Core Data Structures & Concepts

- **Sorted Singly Linked List**: Dynamically stores and manages menu items ordered by unique item ID. Enables $O(N)$ insertion maintaining sorted order without array reallocations.
- **Dynamic Memory Management**: Allocates and deallocates memory on demand (`malloc`, `free`) to scale with customer orders and menu modifications.
- **CSV File Persistence**: Automatically logs billing transactions to `billing.csv` for audit trails and record-keeping.
- **String & Pointer Manipulation**: Safe parsing of customer details, order item lists, and tabular formatting.

---

## ✨ Features

- 📜 **Dynamic Menu Display**: Displays current items, prices, and live stock availability.
- 🛒 **Order Processing**: Takes customer orders, verifies stock levels, and recalculates totals.
- 🧾 **Bill Generation**: Outputs clean formatted receipts and appends data to `billing.csv`.
- 📦 **Inventory Management**: Automatically decrements quantities upon checkout to prevent overselling.

---

## 📂 Project Structure

```text
dinetrack-restaurant-management-DSA/
├── src/
│   └── DineTrack.c        # Complete C implementation
├── Makefile               # Build script
├── .gitignore
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites
- GCC compiler (`gcc`) or any standard C99/C11 compiler.

### Compilation
Using GCC:
```bash
gcc -Wall -O2 src/DineTrack.c -o dinetrack
```
Or with `make`:
```bash
make
```

### Execution
On Windows:
```powershell
.\dinetrack.exe
```
On Linux/macOS:
```bash
./dinetrack
```

---

## 👩‍💻 Author
- **Tanmayi-n14**
