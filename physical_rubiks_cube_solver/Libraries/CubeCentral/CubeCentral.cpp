#include "CubeCentral.h"
#include "NativeEthernet.h"
#include "SPI.h"

//**GLOBAL VARIABLES************************************************************************
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  //physical mac address Must be unique on your network
byte ip[4];                                           //ip of device (see video here to figure it out)
byte gateway[] = { 10, 0, 0, 1 };                     //router address (default is .1 on last IP byte)((notice .16 from above))
byte subnet[] = { 255, 255, 255, 0 };                 //subnet mask
EthernetServer server(80);                            //web server port
String header;

float opacity;
byte color[6][9], space = 75, face = 0, num[6], enable, status = 8;  //values for positioning the buttons
byte side_color[6][4] = { { 5, 4, 3, 2 }, { 5, 4, 2, 3 }, { 5, 4, 0, 1 }, { 4, 5, 0, 1 }, { 2, 3, 0, 1 }, { 3, 2, 0, 1 } };
int offset[9][2] = { { -space, -space }, { -space, 0 }, { -space, space }, { 0, -space }, { 0, 0 }, { 0, space }, { space, -space }, { space, 0 }, { space, space } };
int center[6][2] = { { 285, 420 }, { 435, 420 }, { 585, 420 }, { 285, 580 }, { 435, 580 }, { 585, 580 } };
String face_name[6] = { "Top", "Bottom", "Front", "Back", "Right", "Left" };                                                               //the names of each face
String btnType[6] = { "buttonR buttonR", "buttonR buttonO", "buttonR buttonW", "buttonR buttonY", "buttonR buttonG", "buttonR buttonB" };  //the 6 different button colors
String square_type, cursor, time = "00:00", numMoves = "0";
char colorChar;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

//**FUNCTIONS***************************************************************************************
//turns a side of the cube display
void _turnSide(int side, int turn) {
  //defines the block number (of the old cube) that will be in the place of the corresponding index after one _right turn
  //this is the same assignment for all sides
  //for example: the new block[0] is the old block[2]
  int adoptA[9] = { 2, 5, 8, 1, 4, 7, 0, 3, 6 };  //adoption assignments for the main face
  //adoption assignments for the 4 sides affected by a turn
  //each level is for each side. Each level has 4 sub arrays with the name of the face and the blocks affected by the turn
  int adoptB[6][4][4] = { { { _left, 6, 3, 0 }, { _back, 6, 3, 0 }, { _right, 6, 3, 0 }, { _front, 6, 3, 0 } },    //_top
                          { { _left, 2, 5, 8 }, { _front, 2, 5, 8 }, { _right, 2, 5, 8 }, { _back, 2, 5, 8 } },    //_bottom
                          { { _left, 8, 7, 6 }, { _top, 2, 5, 8 }, { _right, 0, 1, 2 }, { _bottom, 6, 3, 0 } },    //_front
                          { { _right, 8, 7, 6 }, { _top, 6, 3, 0 }, { _left, 0, 1, 2 }, { _bottom, 2, 5, 8 } },    //_back
                          { { _front, 8, 7, 6 }, { _top, 8, 7, 6 }, { _back, 0, 1, 2 }, { _bottom, 8, 7, 6 } },    //_right
                          { { _back, 8, 7, 6 }, { _top, 0, 1, 2 }, { _front, 0, 1, 2 }, { _bottom, 0, 1, 2 } } };  //_left

  int copy[6][9];                    //copy of the colors
  int faceO, blockO, faceN, blockN;  //old face and block as well as new face and block

  //rotates the side 'turn' number of times
  for (int i = 0; i < turn; i++) {
    //creates a copy of the cube
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 9; j++) {
        copy[i][j] = color[i][j];
      }
    }
    //rotates the primary face (aka the side being turned)
    for (int i = 0; i < 9; i++) {
      color[side][i] = copy[side][adoptA[i]];
    }
    //rotates the secondary faces faces (the other 4 faces partially affected by the turn)
    for (int k = 0; k < 4; k++) {
      for (int p = 1; p < 4; p++) {
        faceN = adoptB[side][k][0];   //defines the new face
        blockN = adoptB[side][k][p];  //defines the new block
        //when k is 3 there is a special case for defining the old face and block
        if (k == 3) {
          faceO = adoptB[side][0][0];   //defines the old face
          blockO = adoptB[side][0][p];  //defines the old block
        }
        //when k is not 3
        else {
          faceO = adoptB[side][k + 1][0];   //defines the old face
          blockO = adoptB[side][k + 1][p];  //defines the old block
        }
        color[faceO][blockO] = copy[faceN][blockN];  //replaces the old with the new
      }
    }
  }
}
//resets the cube
void _resetCube() {
  //resets the cube to its original colors
  for (int i = 0; i < 6; i++) {    //cycles through each face
    for (int j = 0; j < 9; j++) {  //cycles through each block
      color[i][j] = i;
    }
  }
}

//**LIBRARY CONTENT***************************************************************************************
WebServer::WebServer(byte ip0, byte ip1, byte ip2, byte ip3) {
  ip[0] = ip0;
  ip[1] = ip1;
  ip[2] = ip2;
  ip[3] = ip3;
}
void WebServer::printIP() {
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}
void WebServer::begin() {
  Ethernet.begin(mac, ip);
  server.begin();
  _resetCube();
}
void WebServer::display() {
  EthernetClient client = server.available();
  if (client) {  // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    //Serial.println("New Client.");                                             // print a message out in the serial port
    String currentLine = "";                                                   // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {  // if there's bytes to read from the client,
        char c = client.read();  // read a byte, then
        //Serial.write(c);         // print it out the serial monitor
        header += c;
        if (c == '\n') {  // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            //if the start button is pressed
            if (header.indexOf((String) "GET /start") >= 0 && status == 8 && enable == 1) {
              status = 6;
            }
            //if the configure button is pressed
            if (header.indexOf((String) "GET /config") >= 0 && status == 8 && enable == 1) {
              status = 7;
            }
            //if the reset button is pressed
            if (header.indexOf((String) "GET /reset") >= 0) {
              _resetCube();  //resets cube display
            }
            //checks if a button on the cube display has been pressed
            for (int i = 0; i < 9; i++) {  //cycles through each block
              if (i != 4) {
                //if a button is pressed on a non center block
                if (header.indexOf((String) "GET /" + face_name[face] + i) >= 0) {
                  color[face][i] += 1 - 6 * (color[face][i] == 5);  // increments color, but makes it 0 if the color is 5
                }
              }
            }
            //resets num array
            for (int i = 0; i < 6; i++) {
              num[i] = 0;
            }
            //calculates the number of blocks of each color
            for (int i = 0; i < 6; i++) {
              for (int j = 0; j < 9; j++) {
                num[color[i][j]]++;
              }
            }
            //if there is 9 of each color, then enable will be equal to 1
            enable = (num[0] == 9) * (num[1] == 9) * (num[2] == 9) * (num[3] == 9) * (num[4] == 9) * (num[5] == 9);
            opacity = 1;
            cursor = "pointer";
            if (enable == 0) {
              opacity = 0.3;
              cursor = "not-allowed";
            }

            // CSS to style buttons and squares
            //CSS for block buttons and center square
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".buttonR {background-color: #E42604; border: 1px solid black; color: white; width: 70px; height: 70px; cursor: pointer; border-radius: 8px;}");  //red button definition
            client.println(".buttonO {background-color: #FF8F11;}");                                                                                                         //orange button definition
            client.println(".buttonW {background-color: #FFFFFF;}");                                                                                                         //white button definition
            client.println(".buttonY {background-color: #EBEE00;}");                                                                                                         //yellow button definition
            client.println(".buttonG {background-color: #3F9D0D;}");                                                                                                         //green button definition
            client.println(".buttonB {background-color: #0053CB;}");                                                                                                         //blue button definition
            //mini squares
            client.println(".square {width: 35px; height: 35px; border-radius: 4px;}");
            client.println(".square2 {width: 10px; height: 60px; border-radius: 4px;}");
            client.println(".square3 {width: 60px; height: 10px; border-radius: 4px;}");
            //medium sized text
            client.println("mtext {font-size: 40px;}");
            //normal buttons
            client.println((String) ".buttonRA {background-color: opacity: " + opacity + "; cursor: " + cursor + "; #E42604; font-size: 20px; color: white; width: 100px; height: 30px;}");  //red button definition
            client.println((String) ".buttonOA {background-color: #FF8F11; opacity: " + opacity + "; cursor: " + cursor + "; font-size: 20px; color: white; width: 100px; height: 30px;}");  //orange button
            client.println((String) ".buttonWA {background-color: #FFFFFF; opacity: " + opacity + "; cursor: " + cursor + "; font-size: 20px; color: black; width: 100px; height: 30px;}");  //white button
            client.println((String) ".buttonYA {background-color: #EBEE00; opacity: " + opacity + "; cursor: " + cursor + "; font-size: 20px; color: black; width: 100px; height: 30px;}");  //yellow button
            client.println((String) ".buttonGA {background-color: #3F9D0D; opacity: " + opacity + "; cursor: " + cursor + "; font-size: 20px; color: white; width: 100px; height: 30px;}");
            client.println((String) ".buttonBA {background-color: #0053CB; opacity: " + opacity + "; cursor: " + cursor + "; font-size: 20px; color: white; width: 100px; height: 30px;}");
            client.println(".buttonReset {background-color: #E76EF3; cursor: pointer; font-size: 40px; color: white; width: 200px; height: 100px; border-radius: 12px;}");
            client.println((String)".buttonConfig {background-color: #00A1F7; opacity: " + opacity + "; cursor: " + cursor + ";font-size: 40px; color: white; width: 200px; height: 100px; border-radius: 12px;}");  //green button
            client.println((String) ".buttonStart {background-color: #4CAF50; opacity: " + opacity + "; cursor: " + cursor + "; font-size: 40px; color: white; width: 200px; height: 100px; border-radius: 12px;}");
            client.println(".buttonMove {background-color: #8C7FFF; font-size: 20px; color: white; width: 100px; height: 30px;}</style></head>");

            //background color
            client.println("<body style=\"background-color:#C1C1C1;\">");
            //header
            client.println("<div style = \"position:absolute; left:940px; top:50px;\">");
            client.println("<p><h1>Rubik's Cube Central</h1>");  // Web Page Heading
            client.println("</div>");
            //author
            client.println("<div style = \"position:absolute; left:1040px; top:90px;\">");
            client.println("<p><font size = \"3\">(By Aaed Musa)</font></p>");  // Web Page Heading
            client.println("</div>");
            //moves
            client.println("<div style = \"position:absolute; left:900px; top:450px;\">");
            client.println("<mtext>Number of Moves: " + numMoves + "</mtext>");  // Web Page Heading
            client.println("</div>");
            //time
            client.println("<div style = \"position:absolute; left:900px; top:500px;\">");
            client.println("<mtext>Elapsed Time: " + time + "s</mtext>");  // Web Page Heading
            client.println("</div>");
            //if the next side button is pressed
            if (header.indexOf((String) "GET /next") >= 0) {
              face += 1 - 6 * (face == 5);  // increments face, but makes it 0 if face is 5
            }
            //next side button
            client.println("<div style = \"position:absolute; left:530px; top:285px;\">");
            client.println("<p><a href=\"/next\"><button class=\"buttonR buttonMove\">Next</button></a></p>");
            client.println("</div>");
            //if the previous side button is pressed
            if (header.indexOf((String) "GET /prev") >= 0) {
              face += 6 * (face == 0) - 1;  // increments face, but makes it 5 if face is 0
            }
            //Previous side button
            client.println("<div style = \"position:absolute; left:280px; top:285px;\">");
            client.println("<p><a href=\"/prev\"><button class=\"buttonR buttonMove\">Previous</button></a></p>");
            client.println("</div>");
            //if the turn side button is pressed
            if (header.indexOf((String) "GET /turn" + face_name[face]) >= 0 && status == 8 && enable == 1) {
              _turnSide(face, 1);  //turns the side
              status = face;
            }
            //turn side button
            client.println("<div style = \"position:absolute; left:405px; top:285px;\">");
            client.println((String) "<p><a href=\"/turn" + face_name[face] + "\"><button class=\"" + btnType[face] + "A\">" + face_name[face] + "</button></a></p>");
            client.println("</div>");
            //displays general cube face (displays 8 buttons)
            for (int i = 0; i < 9; i++) {  //cycles through each block
              if (i != 4) {
                //displays block button of respective color at an absolute location
                client.println((String) "<div style = \"position:absolute; left:" + (420 + offset[i][0]) + "px; top:" + (100 + offset[i][1]) + "px;\">");  //positions the button absolutely
                client.println((String) "<p><a href=\"/" + face_name[face] + i + "\"><button class=\"" + btnType[color[face][i]] + "\"></button></a></p>");
                client.println("</div>");
              }
            }
            //center block square
            client.println("<div style = \"position:absolute; left:419px; top:115px;\" class=\"" + btnType[face] + "\"></div>");
            //entire cube display
            for (int i = 0; i < 6; i++) {  //cycles through each face
              //displays face name
              client.println((String) "<div style = \"position:absolute; left:" + (center[i][0]) + "px; top:" + (center[i][1] - 80) + "px;\">");
              client.println("<p><font size = \"3\">" + face_name[i] + "</font></p>");  // Web Page Heading
              client.println("</div>");
              for (int j = 0; j < 9; j++) {  //cycles through each block
                client.println((String) "<div style = \"position:absolute; left:" + (center[i][0] + offset[j][0] * 0.52) + "px; top:" + (center[i][1] + offset[j][1] * 0.52) + "px;\" class=\"" + btnType[color[i][j]] + " square\"></div>");
              }
            }

            //displays side blocks
            for (int i = 0; i < 4; i++) {  //cycles through each side of the display cube
              square_type = " square2";
              if (i > 1) {
                square_type = " square3";
              }
              int X = 328 + 241 * (i == 1) + 96 * (i > 1);
              int Y = 119 - 94 * (i == 2) + 146 * (i == 3);
              client.println((String) "<div style = \"position:absolute; left:" + X + "px; top:" + Y + "px;\" class=\"" + btnType[side_color[face][i]] + square_type + "\"></div>");
            }

            //Start button
            client.println("<div style = \"position:absolute; left:995px; top:300px;\">");
            client.println("<p><a href=\"/start\"><button class=\"buttonStart\">START</button></a></p>");
            client.println("</div>");
            //Configure button
            client.println("<div style = \"position:absolute; left:870px; top:170px;\">");
            client.println("<p><a href=\"/config\"><button class=\"buttonConfig\">CONFIG</button></a></p>");
            client.println("</div>");
            //Reset button
            client.println("<div style = \"position:absolute; left:1120px; top:170px;\">");
            client.println("<p><a href=\"/reset\"><button class=\"buttonReset\">RESET</button></a></p>");
            client.println("</div>");

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else {  // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }
}
void WebServer::displayTime(String new_time) {
  time = new_time;
}
void WebServer::displayMoves(String new_numMoves) {
  numMoves = new_numMoves;
}
int WebServer::getStatus() {
  return status;
}
char WebServer::getBlock(byte face, byte block) {
  switch (color[face][block]) {
    case 0:
      colorChar = 'R';
      break;
    case 1:
      colorChar = 'O';
      break;
    case 2:
      colorChar = 'W';
      break;
    case 3:
      colorChar = 'Y';
      break;
    case 4:
      colorChar = 'G';
      break;
    case 5:
      colorChar = 'B';
      break;
  }
  return colorChar;
}
void WebServer::resetDisplay() {
  _resetCube();
}
void WebServer::resetStatus() {
  status = 8;
}