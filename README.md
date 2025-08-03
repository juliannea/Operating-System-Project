🖥️ Operating System Simulation Project
This project is a C++ library that simulates a basic operating system, including CPU scheduling, memory management, and disk I/O operations. It was developed for an Operating Systems course and follows strict coding and structural guidelines.

🚀 Features
Priority-based preemptive CPU scheduling

Contiguous memory management using worst-fit allocation

Disk management using FCFS queues

Process lifecycle simulation: creation, forking, waiting, exiting, and zombie handling

Memory and I/O tracking

CPU Scheduling
Preemptive priority-based scheduling

Higher priority processes (higher integer value) will preempt currently running ones.

Memory Management
Uses worst-fit contiguous memory allocation.

The first largest hole (by address) is chosen for allocation.

Memory usage is tracked using the MemoryItem structure.

Disk Management
FCFS (First-Come, First-Served) queue per disk

Disk read requests block the process until the operation completes

📦 Key Classes & Structures
class SimOS – Core class with all simulation methods

MemoryItem – Describes memory chunks

FileReadRequest – Represents file I/O operations

