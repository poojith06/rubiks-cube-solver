# Rubiks Cube Solver

A C++ project that models a Rubik's Cube and includes several solver and pattern-database utilities.

## Quick Start

- **Build (single file):**

  ```bash
  gcc -g main.cpp -o main.exe
  ```

- **Or** use the provided VS Code task: `C/C++: gcc.exe build active file` (see .vscode/tasks.json).
- **Run:**

  ```bash
  ./main.exe
  ```

## Repository Layout

- `main.cpp` — example runner / entrypoint.
- `Model/` — cube model implementations:
  - `RubiksCube.cpp`, `RubiksCube.h` — base model and helpers
  - `RubiksCube1dArray.cpp` — 1D array representation
  - `RubiksCube3dArray.cpp` — 3D array representation
  - `RubiksCubeBitboard.cpp` — bitboard representation
  - `test_moves.cpp` — basic move tests
  - `PatternDatabase/` — pattern-database interfaces used by model
- `PatternDatabases/` — pattern database builders and utilities (CornerDBMaker, NibbleArray, PermutationIndexer, etc.).
- `Solver/` — solver headers (BFS, DFS, IDA*, IDDFS interfaces).
- `Databases/` — generated database files (example: `cornerDepth5V1.txt`).
- `Rubiks_cube_project.docx` — project spec (extracted to `Rubiks_cube_project.txt`).

## Notes

- I extracted `Rubiks_cube_project.docx` to `Rubiks_cube_project.txt` (UTF-8) for easy reading.
- The code is header-heavy; build only the files you need or use the `gcc` command shown above.
- Pattern database generation can be slow and may require additional RAM/disk space — consult the files in `PatternDatabases/` before running.

## Next steps I can take

- Run the project or build the pattern-database generator.
- Add a small `CMakeLists.txt` or Makefile for easier builds.

If you'd like, tell me which next step to do.
