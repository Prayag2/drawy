<p align="center">
  <img src="assets/logo-256.png" width=100/>
  <h1 align="center">Drawy</h1>
  <p align="center">Your handy, infinite, brainstorming tool!</center>
</p>
<img src="./assets/screenshot.png" style="width: 100%"/>

Drawy is a work-in-progress infinite whiteboard tool written in Qt/C++, which aims to be a native-desktop alternative to the amazing web-based Excalidraw.


# Installation  
<a href="https://github.com/Prayag2/drawy/releases/download/1.0.0-alpha/Drawy-47b7552-x86_64.AppImage">
<img alt="Static Badge" src="https://img.shields.io/badge/Download-Linux-F7B601?style=for-the-badge">
</a>

<a href="https://github.com/Prayag2/drawy/releases/download/1.0.0-alpha/drawy-windows-x86_64.zip">
<img alt="Static Badge" src="https://img.shields.io/badge/Download-Windows-007CF7?style=for-the-badge">
</a>

## Compiling from Source
- Install `cmake` and `g++`
- Install Qt 6.9 or above from [here](https://www.qt.io/download-qt-installer-oss) or using [aqtinstall](https://github.com/miurahr/aqtinstall)
- Clone this repository: `git clone https://github.com/prayag2/drawy && cd drawy`
- Setup cmake: `cmake -B build -S . -DCMAKE_BUILD_TYPE=Release`
- Compile: `cmake --build build --config Release`
- Run: `./build/drawy`

# Keyboard Shortcuts
Future releases will allow you to change the keyboard shortcuts. For now they are hardcoded. Here's a list of all available keyboard shortcuts:
| Key Combination      | Description            |
| :------------------ | :---------------------: |
| <kbd>Ctrl+Z</kbd>   | Undo                   |
| <kbd>Ctrl+Y</kbd>, <kbd>Ctrl+Shift+Z</kbd> | Redo   |
| <kbd>Ctrl++</kbd>   | Zoom In                |
| <kbd>Ctrl+-</kbd>   | Zoom Out               |
| <kbd>P</kbd>, <kbd>B</kbd> | Freeform Tool   |
| <kbd>E</kbd>        | Eraser Tool            |
| <kbd>S</kbd>        | Selection Tool         |
| <kbd>R</kbd>        | Rectangle Tool         |
| <kbd>O</kbd>        | Ellipse Tool           |
| <kbd>L</kbd>        | Line Tool              |
| <kbd>A</kbd>        | Arrow Tool             |
| <kbd>M</kbd>        | Move Tool              |
| <kbd>T</kbd>        | Text Tool              |
| <kbd>Ctrl+A</kbd>   | Select All             |
| <kbd>Delete</kbd>   | Delete                 |
| <kbd>Ctrl+S</kbd>   | Save                   |
| <kbd>Ctrl+O</kbd>   | Open File              |

# License
This project uses the GNU General Public License V3.

# Support Me ♥
If you liked this project, then please consider supporting me!  
  
<a href="https://liberapay.com/Prayag/donate"><img alt="Donate using Liberapay" src="https://img.shields.io/badge/LIBERAPAY-%231A171B?style=for-the-badge&logo=liberapay"/></a>
<a href="https://ko-fi.com/O5O1FJ70D"><img alt="Buy Me A Coffee" src="https://img.shields.io/badge/Buy%20Me%20A%20Coffee-%234D798C?style=for-the-badge&logo=ko-fi"/></a>
<a href="https://coindrop.to/prayagjain"><img alt="Done using PayPal" src="https://img.shields.io/badge/PayPal-%23F2BA37?style=for-the-badge&logo=paypal"/></a>

# TODOs
Started: `2025-01-02 04:40PM`  
Development is divided into phases.  
The project will eventually be open sourced. However, I will work on the first few phases myself, to maximize learning.  
The following is a list of features I'll be planning to add to it:  

## Phase 1 (Basic Features)
- [x] A simple fixed size canvas to draw on using a black coloured stroke.
- [x] Different shapes like rectangle, ellipse, arrow, line and stroke.
- [x] An eraser to erase the strokes (deleting the strokes).
- [x] Testing.

## Phase 2
- [x] Refactor to try to adhere to SOLID principles and utilize useful design patterns
- [x] A custom Qt layout for toolbar and properties bar
- [x] Make canvas infinite and add ability to move the viewport
- [x] Use an LRU cache based uniform grid to optimize moving the canvas around (now 100% faster!!)
- [x] A properties bar to change the following properties:
    - [x] Colour of strokes
    - [x] Stroke width  
  
## Phase 3
- [x] Buttons to zoom in/out
- [x] Pressure senstivity for drawing tablets
- [x] Selection tool to select items and do these actions:
    - [x] Move items
    - [x] Delete items
    - [ ] Transform items (resize) (Low priority)
    - [ ] Rotate items (Low priority)
- [x] Undo/redo support
- [x] Basic keybinding support
- [x] Saving the drawings
- [x] Text support
- [ ] Exporting drawings to PNGs
- [ ] Image support
- [ ] Text formatting like bold, underline, italics, etc.
- [ ] Better freeform smoothing algorithms
- [ ] Allow snapping
- [ ] Ability to store preferences
- [ ] A "settings" page 
- [ ] Better widgets
- [ ] Online collaboration

Feature requests are welcome!
