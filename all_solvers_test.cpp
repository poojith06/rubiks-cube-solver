#include <iostream>
#include <vector>
#include <chrono>

#include "RubiksCube.cpp"
#include "RubiksCubeBitboard.cpp"
#include "RubiksCube3dArray.cpp"
#include "RubiksCube1dArray.cpp"
#include "DFSSolver.h"
#include "IDDFSSolver.h"
#include "BFSSolver.h"

using namespace std;
using namespace std::chrono;

static void printMoves(const vector<RubiksCube::MOVE> &moves) {
    for (auto move : moves) cout << RubiksCube::getMove(move) << ' ';
    cout << '\n';
}

template<typename T, typename H>
void testRepresentation(const string &name, const vector<RubiksCube::MOVE> &scramble, int max_depth) {
    cout << "--- " << name << " ---\n";

    // create scrambled cube by applying the same scramble
    T cube;
    for (auto m: scramble) cube.move(m);

    // BFSSolver
    {
        T working = cube;
        auto t0 = high_resolution_clock::now();
        BFSSolver<T, H> solver(working);
        vector<RubiksCube::MOVE> solution = solver.solve();
        auto t1 = high_resolution_clock::now();
        auto ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;

        T check = cube;
        for (auto mv: solution) check.move(mv);
        bool ok = check.isSolved();

        cout << "BFSSolver: time=" << ms << " ms, moves=" << solution.size() << ", solved=" << (ok ? "YES" : "NO") << "\n";
        if (!solution.empty()) {
            cout << "Solution: ";
            printMoves(solution);
        }
    }

    // DFSSolver
    {
        T working = cube;
        auto t0 = high_resolution_clock::now();
        DFSSolver<T, H> solver(working, max_depth);
        vector<RubiksCube::MOVE> solution = solver.solve();
        auto t1 = high_resolution_clock::now();
        auto ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;

        T check = cube;
        for (auto mv: solution) check.move(mv);
        bool ok = check.isSolved();

        cout << "DFSSolver: time=" << ms << " ms, moves=" << solution.size() << ", solved=" << (ok ? "YES" : "NO") << "\n";
        if (!solution.empty()) {
            cout << "Solution: ";
            printMoves(solution);
        }
    }

    // IDDFSSolver
    {
        T working = cube;
        auto t0 = high_resolution_clock::now();
        IDDFSSolver<T, H> iddSolver(working, max_depth);
        vector<RubiksCube::MOVE> idd_solution = iddSolver.solve();
        auto t1 = high_resolution_clock::now();
        auto ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;

        T check = cube;
        for (auto mv: idd_solution) check.move(mv);
        bool ok = check.isSolved();

        cout << "IDDFSSolver: time=" << ms << " ms, moves=" << idd_solution.size() << ", solved=" << (ok ? "YES" : "NO") << "\n";
        if (!idd_solution.empty()) {
            cout << "Solution: ";
            printMoves(idd_solution);
        }
    }

    cout << '\n';
}

int main(int argc, char **argv) {
    int scramble_moves = 5;
    int max_depth = 9;

    if (argc >= 2) scramble_moves = max(0, stoi(argv[1]));
    if (argc >= 3) max_depth = max(1, stoi(argv[2]));

    // Generate a scramble using Bitboard (single source of randomness)
    RubiksCubeBitboard generator;
    vector<RubiksCube::MOVE> scramble = generator.randomShuffleCube(scramble_moves);

    cout << "Scramble moves (generated once): ";
    printMoves(scramble);
    cout << "Max depth limit: " << max_depth << "\n\n";

    testRepresentation<RubiksCubeBitboard, HashBitboard>("Bitboard", scramble, max_depth);
    testRepresentation<RubiksCube3dArray, Hash3d>("3D Array", scramble, max_depth);
    testRepresentation<RubiksCube1dArray, Hash1d>("1D Array", scramble, max_depth);

    return 0;
}
