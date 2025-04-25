# Memory Allocator CLI Project

## Overview
A simple memory allocator implemented in C++ with a Command-Line Interface (CLI). It supports adding and removing processes, displaying memory status, and compacting memory to handle external fragmentation.

## Features
- Fixed memory size (1000 units).
- First-Fit allocation strategy.
- Commands:
  - `add <id> <size>`: Add a process.
  - `remove <id>`: Remove a process.
  - `stat`: Display current memory layout.
  - `compact`: Compact memory to reduce fragmentation.
  - `exit`: Exit the program.

## Demo Scenario
1. Add processes A (200), B (300), and C (100).
2. Remove process B.
3. Attempt to add process D (350) — should fail due to external fragmentation.
4. Compact memory.
5. Add process D again — should succeed.

## How to Run
```bash
g++ -o memory_allocator main.cpp
./memory_allocator
