#ifndef RUBIKS_CUBE_SOLVER_IDASTARSOLVER_H
#define RUBIKS_CUBE_SOLVER_IDASTARSOLVER_H

#include <bits/stdc++.h>
#include "../Model/RubiksCube.h"
#include "../PatternDatabases/CornerPatternDatabase.h"

template <typename T, typename H>
class IDAstarSolver
{
private:
    CornerPatternDatabase cornerDB;
    vector<RubiksCube::MOVE> resultMoves;
    unordered_set<T, H> visited;
    string fileName;
    // this is actual IDA* algorithm implementation
    // dfs() -> performs depth-first search and returns the minimum threshold
    int dfs(T &cube, int g, int bound, vector<RubiksCube::MOVE> &path)
    {
        int f = g + cornerDB.getNumMoves(cube);
        if (f > bound)
            return f;
        if (cube.isSolved())
            return -1;
        int minThreshold = INT_MAX;
        for (int i = 0; i < 18; i++)
        {
            RubiksCube::MOVE move = RubiksCube::MOVE(i);
            cube.move(move);

            if (visited.find(cube) == visited.end())
            {
                path.push_back(move);
                visited.insert(cube);

                int t = dfs(cube, g + 1, bound, path);

                if (t == -1)
                    return -1;
                if (t < minThreshold)
                    minThreshold = t;
                visited.erase(cube);
                path.pop_back();
            }

            cube.invert(move);
        }

        return minThreshold;
    }

public:
    T rubiksCube;

    IDAstarSolver(T _rubiksCube, string _fileName)
    {
        rubiksCube = _rubiksCube;
        fileName = _fileName;
        cornerDB.fromFile(fileName);
    }

    vector<RubiksCube::MOVE> solve()
    {
        int bound = cornerDB.getNumMoves(rubiksCube);
        vector<RubiksCube::MOVE> path;

        while (true)
        {
            visited.clear();
            visited.insert(rubiksCube);

            int t = dfs(rubiksCube, 0, bound, path);
            if (t == -1)
            {
                resultMoves = path;
                break;
            }

            if (t == INT_MAX)
                break;
            bound = t;
        }

        return resultMoves;
    }
};

#endif // RUBIKS_CUBE_SOLVER_IDASTARSOLVER_H
