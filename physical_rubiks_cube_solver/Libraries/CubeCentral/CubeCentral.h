#ifndef CubeCentral_H
#define CubeCentral_H

//global constants
#define _top 0     //top face/side array index
#define _bottom 1  //bottom side/face array index
#define _front 2   //front side/face array index
#define _back 3    //back side/face array index
#define _right 4   //right side/face array index
#define _left 5    //left side/face array index

#include "arduino.h"

class WebServer { //single servo class
  public:
    //class functions
    WebServer(byte ip0, byte ip1, byte ip2, byte ip3);
    void printIP();
    void display(); //defines the cube with a 6 x 9 array
    void begin(); //starts the webserver
    void displayTime(String new_time);
    void displayMoves(String new_numMoves);
    int getStatus();
    char getBlock(byte face, byte block);
    void resetDisplay();
    void resetStatus();
};

#endif
