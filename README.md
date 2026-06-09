#  Rubik's Cube Solver

A high-performance Rubik's Cube solver implemented in C++ that models the cube in three distinct representations and solves it using multiple search algorithms, culminating in Korf's IDA* algorithm with a Corner Pattern Database heuristic.

---

<!-- ## 📌 Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Cube Representations](#cube-representations)
- [Solving Algorithms](#solving-algorithms)
- [Pattern Database & Heuristics](#pattern-database--heuristics)
- [Key Concepts](#key-concepts)
- [Build & Run](#build--run)
- [OOP Design](#oop-design)
- [Key Learnings](#key-learnings)

--- -->

## Overview

A Rubik's Cube has **43,252,003,274,489,856,000** possible states. Despite this, any configuration can be solved in **20 moves or fewer** (God's Number). This project:

- Models the cube in **3 different representations** (3D array, 1D array, Bitboard)
- Solves it using **4 algorithms**: DFS, BFS, IDDFS, and IDA*
- Uses a precomputed **Corner Pattern Database** (~42 MB) as an admissible heuristic for IDA*, enabling near-optimal solve times

---

<!-- ## Project Structure

```
rubiks-cube-solver/
│
├── Model/
│   ├── RubiksCube.h / .cpp          # Abstract base class (18 moves, print, shuffle, isSolved)
│   ├── RubiksCube3dArray.cpp        # 3D array representation [6][3][3]
│   ├── RubiksCube1dArray.cpp        # 1D array representation [54]
│   └── RubiksCubeBitboard.cpp       # Bitboard representation (6 × 64-bit integers)
│
├── Solver/
│   ├── DFSSolver.h                  # Depth-First Search solver (template class)
│   ├── BFSSolver.h                  # Breadth-First Search solver (template class)
│   ├── IDDFSSolver.h                # Iterative Deepening DFS solver (template class)
│   └── IDAstarSolver.h              # IDA* solver with Corner Pattern Database heuristic
│
├── PatternDatabases/
│   ├── NibbleArray.h / .cpp         # Custom 4-bit array using uint8_t
│   ├── PatternDatabase.h / .cpp     # Abstract base class for pattern databases
│   ├── CornerPatternDatabase.h / .cpp  # Corner DB: 8! × 3^7 = 88,179,840 states
│   ├── CornerDBMaker.h / .cpp       # BFS to precompute and store heuristic values
│   ├── PermutationIndexer.h         # Sequential permutation indexer (black box)
│   ├── math.h / math.cpp            # Math utilities for indexing
│
├── Databases/                       # Precomputed pattern database files (.dat)
├── main.cpp                         # Entry point
└── main.exe                         # Compiled binary (Windows)
```

--- -->


## Key Concepts

**Standard Notation** — 18 possible moves:
```
F  F' F2    U  U' U2    L  L' L2
D  D' D2    R  R' R2    B  B' B2
```
(Clockwise, Counter-clockwise, Double rotation for each of 6 faces)

**Solvability** — Not all color assignments are valid. Only 1/12 of all permutations are reachable from a solved cube. The `randomShuffle()` function always applies legal moves to a solved cube, guaranteeing a solvable state.

**Planar Representation** — The cube is printed as an unfolded cross layout for visualization. 
<!-- using the `getColor(face, row, col)` virtual function implemented in each derived class. -->

---


## Cube Representations

All representations derive from the abstract `RubiksCube` base class and implement the same 18 move operations (F, F', F2, U, U', U2, ..., B, B', B2), along with `print()`, `isSolved()`, `randomShuffle()`.
 <!-- and custom hash/equality/assignment operators for use with `unordered_map`. -->

### 3D Array — `cube[6][3][3]`
Stores the color of each sticker as `cube[face][row][col]`. The most intuitive representation. Face indexing: Up=0, Left=1, Front=2, Right=3, Back=4, Down=5.

### 1D Array — `cube[54]`
Flattens all 54 stickers into a single array. Slightly faster for cache performance due to contiguous memory layout.

### Bitboard — `bitboard[6]` (six 64-bit integers)
Each face is stored as a 64-bit integer. Each of the 8 non-center stickers is encoded in 8 bits using a one-hot scheme:

| Color  | Encoding   |
|--------|------------|
| White  | 00000001   |
| Green  | 00000010   |
| Red    | 00000100   |
| Blue   | 00001000   |
| Orange | 00010000   |
| Yellow | 00100000   |

Stickers are indexed **clockwise** around each face (0→7), enabling a face rotation to be performed with a single 16-bit circular shift on the 64-bit integer — making this the fastest representation for move generation.

---

## Solving Algorithms

All solvers are **generic template classes** compatible with any of the three cube representations.
 <!-- plus their corresponding hash function. -->

### DFS Solver
Depth-limited Depth-First Search with a configurable max depth (default: 20, since all cubes are solvable in ≤ 20 moves). The visited map is intentionally omitted to avoid the pathological case where a state reached via a long path blocks a shorter solution path.

### BFS Solver
Breadth-First Search guarantees the **minimum number of moves** to solve the cube. Practical for random shuffles up to ~6–7 moves before memory becomes a bottleneck.

### IDDFS Solver
Iterative Deepening DFS combines BFS's optimality with DFS's O(d) space complexity. Repeatedly runs DFS at increasing depth limits. Most top-level nodes are revisited multiple times, but since a tree's bottom level dominates node count, the overhead is acceptable.

### IDA* Solver (Korf's Algorithm)
The most powerful solver. Combines IDDFS with A*'s guided heuristic search:

- **f(n) = g(n) + h(n)**, where `g(n)` = moves from start, `h(n)` = heuristic estimate from Corner Pattern Database
- Prunes branches where `f(n)` exceeds the current bound
- Tracks the minimum pruned `f(n)` as the next bound
- Iterates until the solved state is reached

The Corner Pattern Database provides an admissible (never overestimating) heuristic, ensuring IDA* finds an **optimal solution**.

---

<!-- ## Pattern Database & Heuristics

### Corner Pattern Database

There are **8 corner cubies**, each placeable in 8 positions (8! permutations) and orientable in 3 ways. The 7th corner's orientation determines the 8th (cube law), giving:

```
8! × 3^7 = 40,320 × 2,187 = 88,179,840 states
```

All corner states are solvable in **≤ 11 moves**, so each answer fits in 4 bits (a nibble).

### Nibble Array
A custom data structure storing 4-bit values packed into `uint8_t` arrays using bit manipulation. This halves memory from ~84 MB to **~42 MB**, small enough to load entirely into RAM for O(1) lookup.

```
Array[idx] → holds two 4-bit values:
  - bits [7:4] → value at position 2*idx
  - bits [3:0] → value at position 2*idx + 1
```

### Permutation Indexing
Corner permutations are **sequentially indexed** — each permutation maps to a unique integer in [0, 8!). Combined with orientation:

```
DB Index = (permutation_rank × 3^7) + orientation_rank
```

This allows the heuristic for any corner configuration to be retrieved in O(1) from the precomputed database file.

### Creating the Database (DBMaker)
A BFS from the solved state, computing the number of moves to reach each of the 88 million corner states. States beyond a configurable max depth are assigned a conservative underestimate (e.g., `max_depth + 1`), keeping the heuristic admissible. The result is written to a `.dat` file and loaded at solver startup.

--- -->

<!-- ## Key Concepts

**Standard Notation** — 18 possible moves:
```
F  F' F2    U  U' U2    L  L' L2
D  D' D2    R  R' R2    B  B' B2
```
(Clockwise, Counter-clockwise, Double rotation for each of 6 faces)

**Solvability** — Not all color assignments are valid. Only 1/12 of all permutations are reachable from a solved cube. The `randomShuffle()` function always applies legal moves to a solved cube, guaranteeing a solvable state.

**Planar Representation** — The cube is printed as an unfolded cross layout for visualization, using the `getColor(face, row, col)` virtual function implemented in each derived class.

--- -->

<!-- ## Build & Run

### Requirements
- C++17 or later
- g++ (Linux/macOS) or MSVC / MinGW (Windows)

### Compile

```bash
g++ -O2 -std=c++17 main.cpp Model/*.cpp PatternDatabases/*.cpp -o rubiks_solver
```

### Run

```bash
./rubiks_solver
```

### Generate Corner Pattern Database (first-time setup)

The solver requires a precomputed database file. Run the DBMaker with a desired depth:

```bash
# Inside main.cpp, configure and call CornerDBMaker with desired BFS depth
./rubiks_solver --build-db
```

The `.dat` file will be saved to the `Databases/` directory.

--- -->

<!-- ## OOP Design

The project demonstrates several core C++ OOP principles:

| Concept | Usage |
|---|---|
| Abstract Classes | `RubiksCube` base with pure virtual move functions |
| Virtual Functions | `getColor()` enables generic `print()` in base class |
| Inheritance | 3dArray, 1dArray, Bitboard all derive from `RubiksCube` |
| Operator Overloading | `operator==` and `operator=` for all three representations |
| Template Classes | All solvers are generic over cube type + hash function |
| Custom Hash Functions | Enables `unordered_map<RubiksCubeType, bool>` for visited tracking |

--- -->

<!-- ## Key Learnings

- Modelling a real-world object into multiple computer representations with different performance trade-offs
- How BFS, DFS, IDDFS, and IDA* apply to AI-style state-space search problems
- Why the visited map in DFS causes pathological failures in bounded search, and how removing it fixes it
- Admissible heuristics and how underestimates guarantee IDA* optimality
- Permutation indexing as a technique to achieve O(1) lookup across 88 million states
- Bit manipulation for compact storage (Nibble Array, Bitboard representation)
- C++ template metaprogramming for generic, reusable solver components -->





## References

- **Korf, R. E. (1997)** — *Finding Optimal Solutions to Rubik's Cube Using Pattern Databases*
  [https://www.cs.princeton.edu/courses/archive/fall06/cos402/papers/korfrubik.pdf](https://www.cs.princeton.edu/courses/archive/fall06/cos402/papers/korfrubik.pdf)

- **Rubik's Cube Notation Simulator** — For visualizing and testing move notations
  [https://ruwix.com/the-rubiks-cube/notation/advanced/](https://ruwix.com/the-rubiks-cube/notation/advanced/)



  ## References

- **Korf, R. E. (1997)** — [Finding Optimal Solutions to Rubik's Cube Using Pattern Databases](https://www.cs.princeton.edu/courses/archive/fall06/cos402/papers/korfrubik.pdf)

- **Rubik's Cube Notation** — [Interactive move notation simulator](https://ruwix.com/the-rubiks-cube/notation/advanced/)