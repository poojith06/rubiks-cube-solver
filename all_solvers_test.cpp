#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include "RubiksCube.cpp"
#include "RubiksCube3dArray.cpp"
#include "RubiksCube1dArray.cpp"
#include "math.cpp"
#include "NibbleArray.cpp"
#include "PatternDatabase.cpp"
#include "CornerPatternDatabase.cpp"
#include "CornerDBMaker.cpp"
#include "DFSSolver.h"
#include "IDDFSSolver.h"
#include "BFSSolver.h"
#include "IDAstarSolver.h"

using namespace std;
using namespace std::chrono;

static void printMoves(const vector<RubiksCube::MOVE> &moves) {
    for (auto move : moves) {
        cout << RubiksCube::getMove(move) << ' ';
    }
    cout << '\n';
}

template<typename T, typename H>
void testRepresentation(const string &name, const string &db_file) {
    cout << "--- " << name << " ---\n";

    T cube;
    cube.f();
    cube.u();
    cube.r();

    {
        T working = cube;
        auto t0 = high_resolution_clock::now();
        BFSSolver<T, H> solver(working);
        vector<RubiksCube::MOVE> solution = solver.solve();
        auto t1 = high_resolution_clock::now();
        double ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;

        T check = cube;
        for (auto move : solution) check.move(move);
        cout << "BFSSolver: time=" << ms << " ms, moves=" << solution.size() << ", solved=" << (check.isSolved() ? "YES" : "NO") << '\n';
        if (!solution.empty()) {
            cout << "Solution: ";
            printMoves(solution);
        }
    }

    {
        T working = cube;
        auto t0 = high_resolution_clock::now();
        DFSSolver<T, H> solver(working, 6);
        vector<RubiksCube::MOVE> solution = solver.solve();
        auto t1 = high_resolution_clock::now();
        double ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;

        T check = cube;
        for (auto move : solution) check.move(move);
        cout << "DFSSolver: time=" << ms << " ms, moves=" << solution.size() << ", solved=" << (check.isSolved() ? "YES" : "NO") << '\n';
        if (!solution.empty()) {
            cout << "Solution: ";
            printMoves(solution);
        }
    }

    {
        T working = cube;
        auto t0 = high_resolution_clock::now();
        IDDFSSolver<T, H> solver(working, 6);
        vector<RubiksCube::MOVE> solution = solver.solve();
        auto t1 = high_resolution_clock::now();
        double ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;

        T check = cube;
        for (auto move : solution) check.move(move);
        cout << "IDDFSSolver: time=" << ms << " ms, moves=" << solution.size() << ", solved=" << (check.isSolved() ? "YES" : "NO") << '\n';
        if (!solution.empty()) {
            cout << "Solution: ";
            printMoves(solution);
        }
    }

    {
        T working = cube;
        IDAstarSolver<T, H> solver(working, db_file);
        auto t0 = high_resolution_clock::now();
        vector<RubiksCube::MOVE> solution = solver.solve();
        auto t1 = high_resolution_clock::now();
        double ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;

        T check = cube;
        for (auto move : solution) check.move(move);
        cout << "IDAstarSolver: time=" << ms << " ms, moves=" << solution.size() << ", solved=" << (check.isSolved() ? "YES" : "NO") << '\n';
        if (!solution.empty()) {
            cout << "Solution: ";
            printMoves(solution);
        }
    }

    cout << '\n';
}

int main(int argc, char **argv) {
    string db_file = "corner_db.bin";

    if (argc >= 2) db_file = argv[1];

    ifstream db_in(db_file, ios::binary);
    if (!db_in.good()) {
        cout << "Building corner database file: " << db_file << '\n';
        CornerDBMaker maker(db_file, 0xF);
        if (!maker.bfsAndStore()) {
            cout << "Failed to generate the pattern database.\n";
            return 1;
        }
    } else {
        cout << "Using existing corner database file: " << db_file << '\n';
    }

    cout << "Manual scramble: F U R\n\n";

    testRepresentation<RubiksCubeBitboard, HashBitboard>("Bitboard", db_file);
    testRepresentation<RubiksCube3dArray, Hash3d>("3D Array", db_file);
    testRepresentation<RubiksCube1dArray, Hash1d>("1D Array", db_file);

    return 0;
}




// test with random scrambles is down below. You can uncomment and use it if you want to test with random scrambles instead of the fixed scramble .


// #include <iostream>
// #include <vector>
// #include <chrono>
// #include <fstream>

// #include "RubiksCube.cpp"
// #include "RubiksCube3dArray.cpp"
// #include "RubiksCube1dArray.cpp"
// #include "math.cpp"
// #include "NibbleArray.cpp"
// #include "PatternDatabase.cpp"
// #include "CornerPatternDatabase.cpp"
// #include "CornerDBMaker.cpp"
// #include "DFSSolver.h"
// #include "IDDFSSolver.h"
// #include "BFSSolver.h"
// #include "IDAstarSolver.h"

// using namespace std;
// using namespace std::chrono;

// static void printMoves(const vector<RubiksCube::MOVE> &moves) {
//     for (auto move : moves) cout << RubiksCube::getMove(move) << ' ';
//     cout << '\n';
// }

// template<typename T, typename H>
// void testRepresentation(const string &name, const vector<RubiksCube::MOVE> &scramble, int max_depth, const string &db_file) {
//     cout << "--- " << name << " ---\n";

//     // create scrambled cube by applying the same scramble
//     T cube;
//     for (auto m: scramble) cube.move(m);

//     // BFSSolver
//     {
//         T working = cube;
//         auto t0 = high_resolution_clock::now();
//         BFSSolver<T, H> solver(working);
//         vector<RubiksCube::MOVE> solution = solver.solve();
//         auto t1 = high_resolution_clock::now();
//         auto ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;

//         T check = cube;
//         for (auto mv: solution) check.move(mv);
//         bool ok = check.isSolved();

//         cout << "BFSSolver: time=" << ms << " ms, moves=" << solution.size() << ", solved=" << (ok ? "YES" : "NO") << "\n";
//         if (!solution.empty()) {
//             cout << "Solution: ";
//             printMoves(solution);
//         }
//     }

//     // DFSSolver
//     {
//         T working = cube;
//         auto t0 = high_resolution_clock::now();
//         DFSSolver<T, H> solver(working, max_depth);
//         vector<RubiksCube::MOVE> solution = solver.solve();
//         auto t1 = high_resolution_clock::now();
//         auto ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;

//         T check = cube;
//         for (auto mv: solution) check.move(mv);
//         bool ok = check.isSolved();

//         cout << "DFSSolver: time=" << ms << " ms, moves=" << solution.size() << ", solved=" << (ok ? "YES" : "NO") << "\n";
//         if (!solution.empty()) {
//             cout << "Solution: ";
//             printMoves(solution);
//         }
//     }

//     // IDDFSSolver
//     {
//         T working = cube;
//         auto t0 = high_resolution_clock::now();
//         IDDFSSolver<T, H> iddSolver(working, max_depth);
//         vector<RubiksCube::MOVE> idd_solution = iddSolver.solve();
//         auto t1 = high_resolution_clock::now();
//         auto ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;

//         T check = cube;
//         for (auto mv: idd_solution) check.move(mv);
//         bool ok = check.isSolved();

//         cout << "IDDFSSolver: time=" << ms << " ms, moves=" << idd_solution.size() << ", solved=" << (ok ? "YES" : "NO") << "\n";
//         if (!idd_solution.empty()) {
//             cout << "Solution: ";
//             printMoves(idd_solution);
//         }
//     }

//     // IDAstarSolver
//     {
//         T working = cube;
//         IDAstarSolver<T, H> idaSolver(working, db_file);
//         auto t0 = high_resolution_clock::now();
//         vector<RubiksCube::MOVE> ida_solution = idaSolver.solve();
//         auto t1 = high_resolution_clock::now();
//         auto ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;

//         T check = cube;
//         for (auto mv: ida_solution) check.move(mv);
//         bool ok = check.isSolved();

//         cout << "IDAstarSolver: time=" << ms << " ms, moves=" << ida_solution.size() << ", solved=" << (ok ? "YES" : "NO") << "\n";
//         if (!ida_solution.empty()) {
//             cout << "Solution: ";
//             printMoves(ida_solution);
//         }
//     }

//     cout << '\n';
// }

// int main(int argc, char **argv) {
//     int scramble_moves = 5;
//     int max_depth = 7;
//     string db_file = "corner_db.bin";

//     if (argc >= 2) scramble_moves = max(0, stoi(argv[1]));
//     if (argc >= 3) max_depth = max(1, stoi(argv[2]));
//     if (argc >= 4) db_file = argv[3];

//     ifstream db_in(db_file, ios::binary);
//     if (!db_in.good()) {
//         cout << "Building corner database file: " << db_file << "\n";
//         CornerDBMaker maker(db_file, 0xF);
//         if (!maker.bfsAndStore()) {
//             cout << "Failed to generate the pattern database.\n";
//             return 1;
//         }
//     } else {
//         cout << "Using existing corner database file: " << db_file << "\n";
//     }

//     // Generate a scramble using Bitboard (single source of randomness)
//     RubiksCubeBitboard generator;
//     vector<RubiksCube::MOVE> scramble = generator.randomShuffleCube(scramble_moves);

//     cout << "Scramble moves (generated once): ";
//     printMoves(scramble);
//     cout << "Max depth limit: " << max_depth << "\n\n";

//     testRepresentation<RubiksCubeBitboard, HashBitboard>("Bitboard", scramble, max_depth, db_file);
//     testRepresentation<RubiksCube3dArray, Hash3d>("3D Array", scramble, max_depth, db_file);
//     testRepresentation<RubiksCube1dArray, Hash1d>("1D Array", scramble, max_depth, db_file);

//     return 0;
// }