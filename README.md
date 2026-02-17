# cub3D - Raycasting Engine

This project is a 3D graphical engine developed using the **Raycasting** technique, inspired by the legendary 90s classic *Wolfenstein 3D*.

**Gameplay Preview**
<img width="1940" height="1135" alt="image" src="https://github.com/user-attachments/assets/ee818455-dad8-4a42-a100-087e9a6532a9" />


## 🎮 Controls

| Key / Input | Action |
| :--- | :--- |
| **W / S** | Move Forward / Backward |
| **A / D** | Rotate Camera |
| **Mouse Move** | Rotate Camera |
| **TAB** | Toggle Cursor (Show/Hide) |
| **E** | Open / Close Doors |
| **Space** | Trigger Weapon Animation (Shoot) |
| **M** | Toggle Minimap (Show/Hide) |
| **ESC** | Exit Game |

---

## 🛠 Installation & Launch

1. **Clone the repository:**
   ```Bash
   git clone https://github.com/SuPuHe/SuPuCub3D.git cub3D
   cd cub3D
   ```
2. **Compile using the Makefile:**
   ```Bash
   make
   ```
3. **Run with a map file:**
   ```Bash
   ./cub3D maps/best_map.cub
   ```
🗺 **Map Configuration**
The engine parses ```.cub``` files to generate the world. You can customize the textures, colors, and layout by editing or creating new map files.

**Texture & Color Setup**
Define paths to textures for different wall orientations, doors, and RGB colors for the floor and ceiling:

```
NO ./textures/TE_1E.png   # North wall
SO ./textures/TE_1E.png   # South wall
WE ./textures/TE_1E.png   # West wall
EA ./textures/TE_1E.png   # East wall
DOOR ./textures/DOOR_4A.png

F 128,128,128             # Floor RGB
C 128,128,128             # Ceiling RGB
```

**Grid Legend**
The map is a grid of characters where:

- 0: Empty space

- 1-9: Different wall types/textures

- D: Interactive Door

- N / S / E / W: Player spawn position and initial orientation

**Key Features Implemented**
- DDA Algorithm: Efficient raycasting for high-performance rendering.

- Dual-Wielding HUD: Animated weapon sprites triggered by user input.

- Dynamic Minimap: Real-time player tracking with toggle functionality.

- Interactive Environment: Ray-based door interaction system.

- Hybrid Control Scheme: Smooth camera rotation via both keyboard and mouse.

**Credits**

[SuPuHe](https://github.com/SuPuHe) and [Vspaceman11](https://github.com/Vspaceman11)
