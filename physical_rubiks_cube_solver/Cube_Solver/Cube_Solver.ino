//libraries
#include <CubeSolver.h>
#include <CubeCentral.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

//objects
Cube rubiksCube;                          //rubiks cube
WebServer CubeCentral(192, 168, 8, 167);  //change this to your IP adress
//stepper motor objects
AccelStepper topStepper(1, 1, 2);     // (driver type, STEP, DIR) driver #1
AccelStepper bottomStepper(1, 3, 4);  // (driver type, STEP, DIR) driver #2
AccelStepper frontStepper(1, 5, 6);   // (driver type, STEP, DIR) driver #3
AccelStepper backStepper(1, 7, 8);    // (driver type, STEP, DIR) driver #4
AccelStepper rightStepper(1, 9, 10);  // (driver type, STEP, DIR) driver #5
AccelStepper leftStepper(1, 11, 12);  // (driver type, STEP, DIR) driver #6
MultiStepper steppers;                // Create instance of MultiStepper

int pos[6] = { 0, 0, 0, 0, 0, 0 };  // An array to store the target positions for each stepper motor
float time_i;
int ENA = 0;       //enable pin for all for all of the stepper motor drivers
int sec, milli;    //variables to display time
int speed = 1000;  //max is 1300

void setup() {
  Serial.begin(115200);
  // Set maximum speed value for the steppers
  topStepper.setMaxSpeed(speed);
  bottomStepper.setMaxSpeed(speed);
  frontStepper.setMaxSpeed(speed);
  backStepper.setMaxSpeed(speed);
  rightStepper.setMaxSpeed(speed);
  leftStepper.setMaxSpeed(speed);
  // Adding the steppers to the steppersControl instance for multi stepper control
  steppers.addStepper(topStepper);
  steppers.addStepper(bottomStepper);
  steppers.addStepper(frontStepper);
  steppers.addStepper(backStepper);
  steppers.addStepper(rightStepper);
  steppers.addStepper(leftStepper);
  pinMode(ENA, OUTPUT);     //define enable pin as output
  digitalWrite(ENA, HIGH);  //sets the drivers off initially

  CubeCentral.begin();  //starts web server
}
void loop() {
  //if a button is pressed
  switch (CubeCentral.getStatus()) {
    case 0:
      Serial.println("Top Face Turned");
      moveStepper("U");
      break;
    case 1:
      Serial.println("Bottom Face Turned");
      moveStepper("D");
      break;
    case 2:
      Serial.println("Front Face Turned");
      moveStepper("F");
      break;
    case 3:
      Serial.println("Back Face Turned");
      moveStepper("B");
      break;
    case 4:
      Serial.println("Right Face Turned");
      moveStepper("R");
      break;
    case 5:
      Serial.println("Left Face Turned");
      moveStepper("L");
      break;
    case 6:  //Start Button Pressed
      Serial.println("Start Pressed");
      //defines the cube matrix in "CubeSolver.h" as the cube matrix in the "CubeCentral.h" WebServer
      for (int i = 0; i < 6; i++) {    //cycles throuch each face
        for (int j = 0; j < 9; j++) {  //cycles through each block
          rubiksCube.defineBlock(i, j, CubeCentral.getBlock(i, j));
        }
      }
      rubiksCube.display();                                        //prints the cube matrix
      rubiksCube.solve();                                          //solves the rubiks cube
      rubiksCube.display();                                        //prints the solved cube
      Serial.println((String)rubiksCube.getMoveNum() + " Moves");  //prints the number of moves
      CubeCentral.displayMoves(rubiksCube.getMoveNum());

      time_i = millis();
      //solves the physical rubiks cube
      for (int i = 0; i < rubiksCube.getMoveNum(); i++) {  //cycles through all of the moves
        moveStepper(rubiksCube.getMove(i));  //moves cube
        //delay(50);                                         //short delay in between moves
      }
      sec = (millis() - time_i) / 1000;  //how many seconds it took to solve
      milli = (int)((millis() - time_i) / 10) % 100;
      if (sec < 10) {
        CubeCentral.displayTime((String) "0" + sec + ":" + milli);
      }
      //
      else {
        CubeCentral.displayTime((String)sec + ":" + milli);
      }
      digitalWrite(ENA, HIGH);  //sets the drivers off
      break;
    case 7:  //Configure Button Pressed
      Serial.println("Configure Pressed");
      //defines the cube matrix in "CubeSolver.h" as the cube matrix in the "CubeCentral.h" WebServer
      for (int i = 0; i < 6; i++) {    //cycles throuch each face
        for (int j = 0; j < 9; j++) {  //cycles through each block
          rubiksCube.defineBlock(i, j, CubeCentral.getBlock(i, j));
        }
      }
      rubiksCube.display();                                        //prints the cube matrix
      rubiksCube.solve();                                          //solves the rubiks cube
      Serial.println((String)rubiksCube.getMoveNum() + " Moves");  //prints the number of moves
      rubiksCube.display();                                        //prints the solved cube
      CubeCentral.displayMoves(rubiksCube.getMoveNum());           //displays the number of moves to the WebServer
      break;
  }
  //if a button was just pressed, reset it to be in a low state (prevents continuous high status)
  if (CubeCentral.getStatus() != 8) {
    CubeCentral.resetStatus();
  }
  CubeCentral.display();  //displays web server
}

//moves one of the steppers depending on the rubiks cube move
void moveStepper(String move) {
  int len = move.length();                  //the length of the string
  int step = 100 * (len - 4 * (len == 3));  //if (len is 1, step == 100), (len is 2, step == 200), (len is 3, step == -100)
  char face = move.charAt((len != 1));      //gets the 0th char if move length is 1, gets the 1st char if length is 2 or 3
  switch (face) {
    case 'U':
      pos[0] += step;
      break;
    case 'D':
      pos[1] += step;
      break;
    case 'F':
      pos[2] += step;
      break;
    case 'B':
      pos[3] += step;
      break;
    case 'R':
      pos[4] += step;
      break;
    case 'L':
      pos[5] += step;
      break;
  }
  digitalWrite(ENA, LOW);         //sets the drivers on
  steppers.moveTo(pos);           // Calculates the required speed for all motors
  steppers.runSpeedToPosition();  // Blocks until all steppers are in position
  //turns the drivers off if not solving the cube
  if (CubeCentral.getStatus() != 6) {
    digitalWrite(ENA, HIGH);  //sets the drivers off

  }
}