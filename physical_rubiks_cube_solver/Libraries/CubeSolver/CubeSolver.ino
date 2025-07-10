#include "CubeSolver.h"
Cube rubiksCube;
char cubeMatrix[6][9] = { { 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R' },   //top side (red)
                          { 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O' },   //bottom side (orange)
                          { 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W' },   //front side (white)
                          { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'Y' },   //back side (yellow)
                          { 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G' },   //right side (green)
                          { 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B' } };  //left side (blue)
void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(A2));

  rubiksCube.define(cubeMatrix);
  randomizeCube(6666);
  rubiksCube.display();
  rubiksCube.solve();
  rubiksCube.display();
  Serial.println(rubiksCube.getMoveNum());
}

void loop() {
  // put your main code here, to run repeatedly:
}

//input a number of random turns to make on the cube
void randomizeCube(int num) {
  int rand_face, rand_turn;
  //randomizes cube
  for (int i = 0; i < num; i++) {    //makes num number of random turns on the cube
    rand_face = random(0, 6);        //random face number (0-5)
    rand_turn = random(0, 4);        //random number of turns (0-3)
    rubiksCube.turn(rand_face, rand_turn);  //turns a random side a random number of times
  }
}
