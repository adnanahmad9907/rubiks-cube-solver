//
// Created by adnan ahmad.
//

#include <bits/stdc++.h>
#include "../Model/RubiksCube.h"

#ifndef RUBIKS_CUBE_SOLVER_BFSSOLVER_H
#define RUBIKS_CUBE_SOLVER_BFSSOLVER_H

// Typename T: RubiksCube Representation we can use which one we want 1d 3d or bitboard one
// Typename H: Corresponding Hash function hashing function that we will be using for this

template <typename T, typename H>
class BFSSolver
{
private:
    vector<RubiksCube::MOVE> moves;
    unordered_map<T, bool, H> visited;
    unordered_map<T, RubiksCube::MOVE, H> move_done;

    //    bfs() -> performs breadth-first-search and returns a solved Rubik's Cube
    //    move_done[] -> Back-Pointer map as to how we reached that state
    T bfs()
    {
        queue<T> q;
        q.push(rubiksCube);
        visited[rubiksCube] = true;

        while (!q.empty())
        {
            T node = q.front();
            q.pop();
            if (node.isSolved())
            {
                return node;
            }
            for (int i = 0; i < 18; i++)
            {
                // try all the 18 moves
                auto curr_move = RubiksCube::MOVE(i);
                node.move(curr_move);
                if (!visited[node])
                {
                    visited[node] = true;
                    // storing the parents for use in the the getting the steps to solve this
                    move_done[node] = curr_move;
                    q.push(node);
                }
                node.invert(curr_move);
            }
        }
        return rubiksCube;
    }

public:
    T rubiksCube;

    BFSSolver(T _rubiksCube)
    {
        rubiksCube = _rubiksCube;
    }

    //    Performs BFS and returns the vector of moves done to solve the cube
    vector<RubiksCube::MOVE> solve()
    {
        T solved_cube = bfs();
        assert(solved_cube.isSolved());
        T curr_cube = solved_cube;
        while (!(curr_cube == rubiksCube))
        {
            RubiksCube::MOVE curr_move = move_done[curr_cube];
            moves.push_back(curr_move);
            curr_cube.invert(curr_move);
        }
        rubiksCube = solved_cube;
        reverse(moves.begin(), moves.end());
        return moves;
    }
};

#endif // RUBIKS_CUBE_SOLVER_BFSSOLVER_H
