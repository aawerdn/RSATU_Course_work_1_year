# Maze Editor & Pathfinding Visualizer

A first-year university coursework project (RSATU). A C++ application built with SFML that lets you interactively edit a maze and watch a pathfinding algorithm search for a route in real time.

## 🚀 Features

- **Maze editor**: draw and erase walls with the mouse.
- **Start / finish placement**: set the start and finish cells anywhere on a walkable tile.
- **Random maze generation**: generate a random maze using recursive division.
- **Pathfinding**: step-by-step visualization of a stack-based pathfinding algorithm.
- **Step-by-step mode**: run the search manually, one step at a time.
- **Info panel**: shows the current mode and the search result.

## 🛠️ Tech Stack

- **Language**: C++17
- **Graphics**: SFML 3.1.0 (bundled locally in `libs/SFML-3.1.0`)
- **Build system**: CMake (minimum version 3.16)
- **Third-party libraries**: SFML only (Graphics, Window, System)
