#include <iostream>
#include <vector>

// Include implementations directly so we can compile a single TU for quick testing
#include "RubiksCube.cpp"
#include "RubiksCube1dArray.cpp"
#include "RubiksCube3dArray.cpp"
#include "RubiksCubeBitboard.cpp"

using namespace std;

bool testMoveRestore1d() {
    RubiksCube1dArray c;
    RubiksCube1dArray orig = c;

    // U
    c.u();
    c.uPrime();
    if (!(c == orig)) { cout << "1d: u then u' failed\n"; return false; }
    c = orig;
    c.u(); c.u(); c.u(); c.u();
    if (!(c == orig)) { cout << "1d: u4 failed\n"; return false; }

    // L
    c = orig;
    c.l(); c.lPrime();
    if (!(c == orig)) { cout << "1d: l then l' failed\n"; return false; }
    c = orig; c.l(); c.l(); c.l(); c.l();
    if (!(c == orig)) { cout << "1d: l4 failed\n"; return false; }

    // F
    c = orig; c.f(); c.fPrime(); if (!(c == orig)) { cout << "1d: f then f' failed\n"; return false; }
    c = orig; c.f(); c.f(); c.f(); c.f(); if (!(c == orig)) { cout << "1d: f4 failed\n"; return false; }

    // R
    c = orig; c.r(); c.rPrime(); if (!(c == orig)) { cout << "1d: r then r' failed\n"; return false; }
    c = orig; c.r(); c.r(); c.r(); c.r(); if (!(c == orig)) { cout << "1d: r4 failed\n"; return false; }

    // B
    c = orig; c.b(); c.bPrime(); if (!(c == orig)) { cout << "1d: b then b' failed\n"; return false; }
    c = orig; c.b(); c.b(); c.b(); c.b(); if (!(c == orig)) { cout << "1d: b4 failed\n"; return false; }

    // D
    c = orig; c.d(); c.dPrime(); if (!(c == orig)) { cout << "1d: d then d' failed\n"; return false; }
    c = orig; c.d(); c.d(); c.d(); c.d(); if (!(c == orig)) { cout << "1d: d4 failed\n"; return false; }

    return true;
}

bool testMoveRestore3d() {
    RubiksCube3dArray c;
    RubiksCube3dArray orig = c;

    c.u(); c.uPrime(); if (!(c == orig)) { cout << "3d: u then u' failed\n"; return false; }
    c = orig; c.u(); c.u(); c.u(); c.u(); if (!(c == orig)) { cout << "3d: u4 failed\n"; return false; }

    c = orig; c.l(); c.lPrime(); if (!(c == orig)) { cout << "3d: l then l' failed\n"; return false; }
    c = orig; c.l(); c.l(); c.l(); c.l(); if (!(c == orig)) { cout << "3d: l4 failed\n"; return false; }

    c = orig; c.f(); c.fPrime(); if (!(c == orig)) { cout << "3d: f then f' failed\n"; return false; }
    c = orig; c.f(); c.f(); c.f(); c.f(); if (!(c == orig)) { cout << "3d: f4 failed\n"; return false; }

    c = orig; c.r(); c.rPrime(); if (!(c == orig)) { cout << "3d: r then r' failed\n"; return false; }
    c = orig; c.r(); c.r(); c.r(); c.r(); if (!(c == orig)) { cout << "3d: r4 failed\n"; return false; }

    c = orig; c.b(); c.bPrime(); if (!(c == orig)) { cout << "3d: b then b' failed\n"; return false; }
    c = orig; c.b(); c.b(); c.b(); c.b(); if (!(c == orig)) { cout << "3d: b4 failed\n"; return false; }

    c = orig; c.d(); c.dPrime(); if (!(c == orig)) { cout << "3d: d then d' failed\n"; return false; }
    c = orig; c.d(); c.d(); c.d(); c.d(); if (!(c == orig)) { cout << "3d: d4 failed\n"; return false; }

    return true;
}

bool testMoveRestoreBitboard() {
    RubiksCubeBitboard c;
    RubiksCubeBitboard orig = c;

    c.u(); c.uPrime(); if (!(c == orig)) { cout << "bit: u then u' failed\n"; return false; }
    c = orig; c.u(); c.u(); c.u(); c.u(); if (!(c == orig)) { cout << "bit: u4 failed\n"; return false; }

    c = orig; c.l(); c.lPrime(); if (!(c == orig)) { cout << "bit: l then l' failed\n"; return false; }
    c = orig; c.l(); c.l(); c.l(); c.l(); if (!(c == orig)) { cout << "bit: l4 failed\n"; return false; }

    c = orig; c.f(); c.fPrime(); if (!(c == orig)) { cout << "bit: f then f' failed\n"; return false; }
    c = orig; c.f(); c.f(); c.f(); c.f(); if (!(c == orig)) { cout << "bit: f4 failed\n"; return false; }

    c = orig; c.r(); c.rPrime(); if (!(c == orig)) { cout << "bit: r then r' failed\n"; return false; }
    c = orig; c.r(); c.r(); c.r(); c.r(); if (!(c == orig)) { cout << "bit: r4 failed\n"; return false; }

    c = orig; c.b(); c.bPrime(); if (!(c == orig)) { cout << "bit: b then b' failed\n"; return false; }
    c = orig; c.b(); c.b(); c.b(); c.b(); if (!(c == orig)) { cout << "bit: b4 failed\n"; return false; }

    c = orig; c.d(); c.dPrime(); if (!(c == orig)) { cout << "bit: d then d' failed\n"; return false; }
    c = orig; c.d(); c.d(); c.d(); c.d(); if (!(c == orig)) { cout << "bit: d4 failed\n"; return false; }

    return true;
}

int main() {
    bool ok1 = testMoveRestore1d();
    bool ok2 = testMoveRestore3d();
    bool ok3 = testMoveRestoreBitboard();

    if (ok1 && ok2 && ok3) cout << "All basic move/inverse and 4x-turn tests passed for 1d, 3d and bitboard implementations.\n";
    else cout << "Some tests failed — see messages above.\n";

    return (ok1 && ok2 && ok3) ? 0 : 1;
}
