#include "CubeCentral.h"
WebServer CubeCentral(192, 168, 8, 167);  //IP address

void setup() {
  Serial.begin(115200);
  CubeCentral.begin();
  //CubeCentral.printIP(); //prints IP address
}

void loop() {
  CubeCentral.display();  //displays web server
  switch (CubeCentral.getStatus()) {
    case 0:
      Serial.println("Top Face Turned");
      break;
    case 1:
      Serial.println("Bottom Face Turned");
      break;
    case 2:
      Serial.println("Front Face Turned");
      break;
    case 3:
      Serial.println("Back Face Turned");
      break;
    case 4:
      Serial.println("Right Face Turned");
      break;
    case 5:
      Serial.println("Left Face Turned");
      break;
    case 6:
      Serial.println("Start Pressed");
      CubeCentral.resetDisplay();
      CubeCentral.displayTime("10:30");
      break;
    case 7:
      Serial.println("Stop Pressed");
      CubeCentral.resetDisplay();
      CubeCentral.displayTime("12:30");
      break;
  }
  //if a button was just pressed, reset it to be in a low state (prevents continuous high status)
  if(CubeCentral.getStatus() != 9){
    CubeCentral.resetStatus();
  }
  //Serial.println(CubeCentral.getBlock(0, 0));
}
