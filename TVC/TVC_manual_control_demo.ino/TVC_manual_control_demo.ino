#include <Servo.h>

#define VRX  A0  // Analog stick X pin
#define VRY  A1  // Analog stick Y pin 
#define XMIN 45  // Minimum X range of motion
#define XMAX 150 // Maximum X range of motion
#define XMID 90  // Origin of X range of motion
#define YMIN 69  // Minimum Y range of motion
#define YMAX 128 // Maximum Y range of motion
#define YMID 90  // Origin of Y range of motion
#define SERVO_SPEED 720 // degrees per second

Servo servoX; // Setup for servos
Servo servoY;

int xval; // Analog X input
int yval; // Analog Y input
int xpos; // Servo X position
int ypos; // Servo Y position

void setup() {
  Serial.begin(9600); // To see current position and analog data

  // Setup the servos
  servoX.attach(3);
  servoY.attach(5);

  // Center motor at origin
  servoX.write(XMID);
  servoY.write(YMID);
}

void loop() {
  getServoPos();
  displayValues();
  moveServos(xpos, ypos);
}

void moveServos(int xpos, int ypos){
  // Get current position
  int currentX = servoX.read();
  int currentY = servoY.read();

  // Update the current position of X and Y servos one degree at 
  // a time at a given speed until reaching the desired servo 
  // orientation.
  for(;currentX != xpos || currentY != ypos;){
    if(currentX > xpos){
      currentX -= 1;
      servoX.write(currentX);
    }
    else if(currentX < xpos){
      currentX += 1;
      servoX.write(currentX);
    }
    
    if(currentY > ypos){
      currentY -= 1;
      servoY.write(currentY);
    }
    if(currentY < ypos){
      currentY += 1;
      servoY.write(currentY);
    }
    delay(1000 / SERVO_SPEED); // convert deg/sec to ms/deg for 1 deg of movement
  }
}

void getServoPos(void){
  xval = analogRead(VRX);
  yval = analogRead(VRY);

  // This section utilizes two ranges for X and Y for the case that
  // there are different ranges of motion above and below the point
  // of origin.
  if(xval >= 512){
    xpos = map(xval, 512, 1024, XMID, XMAX);
  }
  else{
    xpos = map(xval, 0, 511, XMIN, XMID - 1);
  }
  if(yval >= 512){
    ypos = map(yval, 512, 1024, YMID, YMAX);
  }
  else{
    ypos = map(yval, 0, 511, YMIN, YMID - 1);
  }
}

void displayValues(void){
  Serial.print("VRx: ");
  Serial.print(xval);
  Serial.print(" xpos: ");
  Serial.print(xpos);
  Serial.print(" VRy: ");
  Serial.print(yval);
  Serial.print(" ypos: ");
  Serial.println(ypos);
}
