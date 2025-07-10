--------------------------------------------------------------------------------------------------------------------------
Rubiks-Cube-Solver
---------------------------------------------------------------------------------------------------------------------------

**Key Learnings from the project:**

 1. I learned how to take a real world object, like a Rubik’s Cube, and model it into something that the computer can understand.
 2. I learned how to take a larger problem, like the complete Rubik’s Cube solver, and break it down into smaller problems, using basic models and controllers, that is, Rubik’s cube representations and solvers.
 3. I learned how to break down multiple representations into classes and create base classes that can be further extended. Namely, creating Rubik’s Cube base class and Pattern Database Base class.
 4. I also learned many practical examples of Object-Oriented Programming in use, like the following:
    - Classes and Objects
    - Virtual Functions and Abstract Classes
    - Inheritance
    - Operator Overloading
 5. I learned how path finding algorithms like BFS, DFS, IDDFS and IDA* can be used to solve real world problems related to AI. Many more problems in the field of AI can be solved using these algorithms.
 6. I learned about the concept of Heuristic Search Algorithms like A* and IDA* and how Pattern Databases can be created to provide that heuristic.
 7. Miscellaneous things I learned along the way:
    - How unordered_map<> works for custom objects
    - How to create custom hash functions
    - How bits can be used to store information
    - How to save and load files 
    - The basic idea of Black boxing, where we can give an input and expect a certain output from the Black Box (in our case, the Permutation Indexer)
-------------------------------------------------------------------------------------------------------------------------------------------------------------
Rubik's-Cube-Solver- Using Korf's IDA* Algorithm
-------------------------------------------------------------------------------------------------------------------------------------------------------------

**[Korf's Research Paper](https://cdn.aaai.org/AAAI/1997/AAAI97-109.pdf)**

- Modeled a virtual **Rubik's Cube (3x3)** in 3 different models/Classes using standard data-structures present in C++.
- Achieved a solving time **under 3 seconds** for Rubik's Cube **jumbled 8 times** using **BFS**,**DFS** and **IDDFS** algorithm.
- Implemented the Korf's IDA*  Algorithm, achieving a solving time of **under 10 seconds** for a Rubik's Cube **jumbled 13 times**.

