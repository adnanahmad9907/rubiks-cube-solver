#include <iostream>
#include <chrono>                      // For measuring time
#include "Model/RubiksCube3dArray.cpp" // Including the .cpp file directly
#include "Solver/BFSSolver.h"
#include "Solver/DFSSolver.h"
#include "Solver/IDDFSSolver.h" // Include IDDFSSolver header
#include "Model/RubiksCube.cpp"
using namespace std;

int main()
{
    // coyt << "a";
    // Create a Rubik's Cube object
    RubiksCube3dArray rubiksCube;
    int a;
    cout << "Enter the number of moves to shuffle the cube: ";
    cin >> a;
    // cout << "Adnan";

    // Shuffle the cube 4 times
    vector<RubiksCube::MOVE> shuffleMoves = rubiksCube.randomShuffleCube(a);
    cout << "Shuffled Cube with moves: ";
    for (const auto &move : shuffleMoves)
    {
        cout << RubiksCube::getMove(move) << " ";
    }
    // cout << "adnan" << endl;
    cout << endl;
    rubiksCube.print();

    // Solve using BFS Solver
    auto start = chrono::high_resolution_clock::now();
    BFSSolver<RubiksCube3dArray, Hash3d> bfsSolver(rubiksCube);
    vector<RubiksCube::MOVE> bfsMoves = bfsSolver.solve();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Solved using BFS with moves: ";
    for (const auto &move : bfsMoves)
    {
        cout << RubiksCube::getMove(move) << " ";
    }
    cout << endl;
    bfsSolver.rubiksCube.print();
    cout << "Time taken by BFS: " << duration << " ms" << endl;

    // Solve using DFS Solver
    start = chrono::high_resolution_clock::now();
    DFSSolver<RubiksCube3dArray, Hash3d> dfsSolver(rubiksCube);
    vector<RubiksCube::MOVE> dfsMoves = dfsSolver.solve();
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Solved using DFS with moves: ";
    for (const auto &move : dfsMoves)
    {
        cout << RubiksCube::getMove(move) << " ";
    }
    cout << endl;
    dfsSolver.rubiksCube.print();
    cout << "Time taken by DFS: " << duration << " ms" << endl;

    // Solve using IDDFS Solver
    start = chrono::high_resolution_clock::now();
    IDDFSSolver<RubiksCube3dArray, Hash3d> iddfsSolver(rubiksCube);
    vector<RubiksCube::MOVE> iddfsMoves = iddfsSolver.solve();
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Solved using IDDFS with moves: ";
    for (const auto &move : iddfsMoves)
    {
        cout << RubiksCube::getMove(move) << " ";
    }
    cout << endl;
    iddfsSolver.rubiksCube.print();
    cout << "Time taken by IDDFS: " << duration << " ms" << endl;

    return 0;
}
