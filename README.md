# 🧠 Gantt Chart Visualizer – CPU Scheduling Simulator

A desktop application built with **C++** and **Qt Framework** to simulate and visualize various CPU scheduling algorithms. The tool allows users to input process details, select a scheduling algorithm, and view the real-time **Gantt chart** and performance metrics.

---

## 🎯 Features

- 📝 Interactive process input (arrival time, burst time, priority, quantum, etc.)
- 📊 Real-time **Gantt chart** generation using `QGraphicsScene`
- 🧠 Supports multiple scheduling algorithms:
  - First-Come-First-Serve (FCFS)
  - Shortest Job First (SJF)
  - Priority Scheduling
  - Round Robin (RR)
- 📈 Displays calculated scheduling metrics (waiting time, turnaround time, etc.)
- 💡 Clean UI with stacked widget navigation and styled components

---

## 🚀 Technologies Used

- **C++** – Core logic and object-oriented design
- **Qt Widgets** – GUI design and layout
- **QGraphicsView & QGraphicsScene** – Gantt chart rendering
- **Signals & Slots** – Event-driven programming in Qt

---

## 🛠️ Getting Started

### Prerequisites
- Qt (5.x or 6.x)
- C++ compiler (e.g., GCC, MSVC)
- Qt Creator (optional but recommended)
