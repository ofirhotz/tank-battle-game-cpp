# Tank Battle Game (C++)

🎮 A classic 2D retro arcade tank battle game implemented in C++ using Object-Oriented Programming (OOP) principles and console rendering.

---

## 📌 About the Project

**Tank Battle** is a tactical 2D console-based action game developed in C++ (C++17). The game features real-time directional tank navigation, dual-track steering simulation, bullet collision physics, mine clearance mechanics, multiple screen parsing, and deterministic game recording/playback.

The architecture emphasizes modular object-oriented design, encapsulation, deterministic state machines, and cross-standard file system parsing.

---

## 🚀 Key Features

* **Dual-Track Tank Physics & Steering:** Realistic tank rotation and movement controlled via independent right/left track forward/reverse mechanics.
* **Shooting & Destruction Mechanics:** 
  * Tanks stop momentarily when firing a cannon shell.
  * Legitimate shot rate limiting and active projectile tracking.
  * Shells can detonate explosive mines but ricochet/stop at solid walls.
  * Precise collision detection: only direct hits to the tank hull or mine contact cause fatal explosions.
* **Game Modes:**
  * **Human vs. Human:** Local 2-player tactical split-keyboard combat.
  * **Human vs. Computer:** Battle against algorithmic AI pathing.
  * **Computer vs. Computer:** Autonomous simulated duel.
* **Deterministic Record & Replay System:**
  * `-save`: Automatically records every keystroke and game tick to `.steps` files.
  * `-load`: Deterministically replays recorded matches step-by-step.
  * `-silent`: Runs headless playback in `-load` mode for automated result verification.
* **Dynamic Screen Loading:** Custom ASCII board parser supporting multiple consecutive maps (`.screen` files).

---

## 🛠️ Concepts & Technologies

* **Language Standard:** C++17 (`std::filesystem` for dynamic board directory indexing).
* **Object-Oriented Design (OOP):** Class inheritance (`Board`, `theTankBattle`), encapsulation (`Tank`, `Point`), and polymorphic event dispatching.
* **I/O & Persistence:** Custom file parsers for ASCII board screens (`.screen`) and binary/text step recording (`.steps`).
* **Console Graphics:** Windows Console API (`windows.h`) for buffer manipulation, custom color palettes, and flicker-free rendering.

---

## 🎮 Controls & Navigation

> **Note:** Set your keyboard input language to **English** for commands to register properly.

### Tank 1 (Player 1)
| Key | Action |
| :---: | :--- |
| `E` / `e` | Right track forward |
| `Q` / `q` | Left track forward |
| `D` / `d` | Right track backwards |
| `A` / `a` | Left track backwards |
| `S` / `s` | Stand still |
| `W` / `w` | Shoot cannon shell |

### Tank 2 (Player 2)
| Key | Action |
| :---: | :--- |
| `O` / `o` | Right track forward |
| `U` / `u` | Left track forward |
| `L` / `l` | Right track backwards |
| `J` / `j` | Left track backwards |
| `K` / `k` | Stand still |
| `I` / `i` | Shoot cannon shell |

* **`ESC`**: Pause / Return to Main Menu / Exit.

---

## ⚙️ Compilation & Execution

### Building with Visual Studio
1. Open `TankBattle.sln` in **Visual Studio**.
2. Ensure the project standard is configured to **C++17**:
   * Right-click `TankBattle` project > **Properties** > **Configuration Properties** > **General** > **C++ Language Standard** -> **ISO C++17 Standard (`/std:c++17`)**.
3. Build and run via **Ctrl + F5**.

### Command-Line Arguments
```bash
# Regular gameplay
TankBattle.exe

# Record match inputs
TankBattle.exe -save

# Replay recorded match
TankBattle.exe -load

# Automated silent evaluation
TankBattle.exe -load -silent