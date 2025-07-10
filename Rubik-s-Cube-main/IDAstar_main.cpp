#include <iostream>

// Implementation files
#include "Model/RubiksCube.cpp"
#include "Model/RubiksCube3dArray.cpp"
#include "Solver/IDAstarSolver.h"
#include "Solver/DFSSolver.h"
#include "Solver/BFSSolver.h"
#include "Solver/IDDFSSolver.h"
#include "PatternDatabases/PatternDatabase.cpp"
#include "PatternDatabases/CornerPatternDatabase.cpp"
#include "PatternDatabases/NibbleArray.cpp"
#include "PatternDatabases/PermutationIndexer.h"
#include "PatternDatabases/math.cpp"

using namespace std;

int main()
{

    string fileName = "Databases/cornerDepth5V1.txt";
    RubiksCube3dArray cube;
    auto shuffleMoves = cube.randomShuffleCube(8);
    cout << "Shuffled Cube with moves: ";
    for (auto move : shuffleMoves)
        cout << cube.getMove(move) << " ";
    cout << "\n";
    cube.print();

    // Solve using IDA* solver
    IDAstarSolver<RubiksCube3dArray, Hash3d> idaStarSolver(cube, fileName);
    auto moves = idaStarSolver.solve();

    // Print the solution
    cout << "Solved using IDA* with moves: ";
    for (auto move : moves)
        cout << cube.getMove(move) << " ";
    cout << "\n";

    // Print final solved cube
    idaStarSolver.rubiksCube.print();

    return 0;
}
