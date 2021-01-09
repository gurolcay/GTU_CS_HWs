/////////Ball and Plate///////////////////////////////
/*
  BALL AND PLATE PID CONTROL
*/
//////////////////////////////////////////////////////
///Libraries///
#include <PID_v1.h>
#include "TouchScreen.h"
#include<Servo.h>
// Definitions TOUCH PINS
#define YP A2
#define XM A3
#define YM 3   // can be a digital pin
#define XP 4   // can be a digital pin
TouchScreen ts = TouchScreen(XP, YP, XM, YM);

// PID values
double SetpointX, InputX, OutputX; //for X
double SetpointY, InputY, OutputY; //for Y

int stable = 0;
int noTouchCount = 0;
// servos
Servo servoX; //X axis
Servo servoY; //Y axis

/////TIME SAMPLE
int Ts = 50;
//PID const
float KpX = 0.11; //0.135 //0.3
float KiX = 0.0075; //0.0035 //0.03
float KdX = 0.0325; //0.014 //0.13

float KpY = 0.075; //0.075//0.1 //0.3
float KiY = 0.0075;//0.0075//0.0025 //0.08
float KdY = 0.0325;//0.0325 //0.01 //0.13

//INIT PID
PID pidX(&InputX, &OutputX, &SetpointX, KpX, KiX, KdX, DIRECT);
PID pidY(&InputY, &OutputY, &SetpointY, KpY, KiY, KdY, DIRECT);

const int flatX = 90;
const int flatY = 90;
const int senX = 20;
const int senY = 8;

String input;
int squareMod;
int flag;

int x;
int y;

double k;

void setup()
{
  //INIT OF TOUSCHSCREEN
  TSPoint p = ts.getPoint();
  InputX = 500;
  InputY = 500;
  //INIT SETPOINT
  SetpointX = 486;//486
  SetpointY = 523;//523
  //// Make plate flat
  servoX.attach(9);
  servoY.attach(7);
  OutputX = flatX;
  OutputY = flatY;
  servoX.write(OutputX);
  servoY.write(OutputY);

  //Zapnutie PID
  pidX.SetMode(AUTOMATIC);
  pidX.SetOutputLimits(20, 160);
  pidY.SetMode(AUTOMATIC);
  pidY.SetOutputLimits(20, 160);
  // TIME SAMPLE
  pidX.SetSampleTime(Ts);
  pidY.SetSampleTime(Ts);

  Serial.begin(115200);
  Serial.setTimeout(0);
  Serial.print("0,0,0,0-");

  k = 10;
  flag = 0;
  squareMod = 0;

  delay(100);
}

void loop()
{

  while (stable < 125) //REGULATION LOOP
  {
    while (!Serial.available())
    {

    }

    input = Serial.readString();


    if (input.equals("circle")) {
      flag = 1;
    }

    if (input.equals("square")) {
      flag = 2;
    }

    if (input.equals("rect")) {
      flag = 3;
    }

    if (input.equals("reset")) {
      flag = 0;
      SetpointX = 486;//486
      SetpointY = 523;//523
    }

    if (input.equals("array")) {
      flag = 4;
    }

    /*if(flag == 0)
      {
      SetpointX = input.substring(0,4).toInt();
      SetpointY = input.substring(5).toInt();
      }*/

    if (flag == 1)
    {
      SetpointX = 486 + 100 * cos(k);
      SetpointY = 523 + 100 * sin(k);
      k = k - 0.01;
    }

    if (flag == 2)
    {
      if (squareMod == 0)
      {
        SetpointX = 358;
        SetpointY = 422;
      }

      if (squareMod == 1)
      {
        SetpointX = 602;
        SetpointY = 422;
      }

      if (squareMod == 2)
      {
        SetpointX = 602;
        SetpointY = 626;
      }

      if (squareMod == 3)
      {
        SetpointX = 358;
        SetpointY = 626;
      }
    }

    TSPoint p = ts.getPoint();   //measure pressure on plate
    if (p.x > 50 && p.x < 920 && p.y > 110 && p.y < 900) //ball is on plate
    {
      noTouchCount = 0;
      InputX = p.x; // read coordinate
      InputY = p.y; // read coordinate

      if (InputX > SetpointX - senX && InputX < SetpointX + senX && InputY > SetpointY - senY && InputY < SetpointY + senY) //if ball is close to setpoint
      {
        ++stable; //increment STABLE
        OutputX = flatX; //make plate flat
        OutputY = flatY;
        servoX.write(OutputX);
        servoY.write(OutputY);
      }
      pidX.Compute(); //action control X compute
      pidY.Compute(); //action control Y compute
    }
    else //if there is no ball on plate
    {
      ++noTouchCount; //increment no touch count

      if (noTouchCount == 75)
      {
        OutputX = flatX; //make plate flat
        OutputY = flatY;
        servoX.write(OutputX);
        servoY.write(OutputY);
      }
    }
    servoX.write(OutputX);//control
    servoY.write(OutputY);//control

    Serial.print(p.x);   Serial.print(",");  Serial.print(p.y);  Serial.print(","); Serial.print((int)OutputX); Serial.print(","); Serial.print((int)OutputY); Serial.print("-");

    if (flag == 2)
    {
      if (p.x < 383 && p.x > 333 && p.y < 447 && p.y > 397)
      {
        squareMod = 1;
      }

      if (p.x < 627 && p.x > 577 && p.y < 447 && p.y > 397)
      {
        squareMod = 2;
      }

      if (p.x < 627 && p.x > 577 && p.y < 651 && p.y > 601)
      {
        squareMod = 3;
      }

      if (p.x < 383 && p.x > 333 && p.y < 651 && p.y > 601)
      {
        squareMod = 0;
      }
    }


  }////END OF REGULATION LOOP///

  if (flag == 0)
  {
    ///KONTROLA STABILITY////
    while (stable == 125) //if is stable
    { //still measure actual postiion
      TSPoint p = ts.getPoint();
      OutputX = flatX; //make plate flat
      OutputY = flatY;
      servoX.write(OutputX);
      servoY.write(OutputY);
      InputX = p.x; //read X
      InputY = p.y; //read Y
      if (InputX < SetpointX - senX || InputX > SetpointX + senX || InputY > SetpointY + senY || InputY < SetpointY - senY) //if ball isnt close to setpoint
      {
        stable = 0; //change STABLE state
      }
    }//end of STABLE LOOP
  }
}//loop endS

