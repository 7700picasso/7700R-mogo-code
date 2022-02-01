/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Sean and Jaehoon                                                       */
/*    Created:      sometime                                           */
/*    Description:  7700R code 2021-2022                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/
//7700R
//Sean
//Avery
//Matthew
//jaehoon
//Saif
//Josh
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// leftDrive1           motor         7               
// leftDrive2           motor         3               
// leftmiddle           motor         5               
// rightDrive1          motor         6               
// rightDrive2          motor         2               
// rightmiddle          motor         4               
// amogus               motor         18              
// lift1                motor         1               
// backHook             digital_out   H               
// claw                 digital_out   G               
// Inertial21           inertial      21              
// picasso              digital_out   A               
// Gyro                 inertial      19              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <vector>
#include <math.h>

using namespace vex;

// A global instance of competition,dont touch either
competition Competition;

// define your global Variables here
long double pi = 3.1415926535897932384626433832795028841971693993751058209749445923; // much more accurate than 3.14
double Diameter = 3.25;

// global vars for auton
double x = 0, y = 0; // x and y pos of robot relative to start pos.
const uint8_t UNITSIZE = 24;

/*
dont touch
Diameter is the wheel
pie is pie dumbass
*/

void pre_auton(void) {
  vexcodeInit();
  Gyro.calibrate();
  wait(2000, msec);

  // All activities that occur before the competition starts
  //gets pistons down before match
  //dont touch this 
}

void drive(int lspeed, int rspeed, int wt){
  
  leftDrive1.spin(forward, lspeed, pct);
  leftDrive2.spin(forward, lspeed, pct);
  leftmiddle .spin(forward, lspeed, pct);
  rightDrive1.spin(forward, rspeed, pct);
  rightDrive2 .spin(forward, rspeed, pct);
  rightmiddle .spin(forward, rspeed, pct);
  wait(wt, msec);
}
//use to go forward,backwards,left right etc,turning if your stupid
//use inchdrive to go forward and backwards,use gyro to turn

struct Integral {
  std::vector<double> values;
  uint16_t size = 1;

  void innit() {
    values = {};

    for (int i = 0; i < size; i++)
      values.push_back(0);
  }

  void addVal(double val) {
    for (int i = size; i > 0; --i)
      values[i] = values[i - 1];
    values[0] = val;
  }

  double mean() {
    double sum = 0;
    for (int i = 0; i < size; i++)
      sum += values[i];
    return sum / size;
  }
};

//use to go forward,backwards,left right etc,turning if your stupid
//use inchdrive to go forward and backwards,use gyro to turn

void brakeDrive() {
  leftDrive1.stop(brake);
  leftDrive2.stop(brake);
  leftmiddle.stop(brake);
  rightDrive1.stop(brake);
  rightDrive2.stop(brake);
  rightmiddle.stop(brake);
  //break code for the gryo balance code
}

void lift(int speed, int wt ){
  lift1.spin(forward, speed, pct);
  
  wait(wt, msec);
}
//makes lift go up or down
// its the lift speed then wait time
//example lift(-100,1200);  so lift 100% for 1200 msc
// 100 is up and -100 is down,or other way around,you can figure that out




void mogo (int speed, int wt){
  amogus.spin(forward, speed, pct);
  
  
  wait(wt, msec);
}
//makes mogo go up or down
// its the lift speed then wait time
//example mogo(-100,1200);  so mogo 100% for 1200 msc
// 100 is up and -100 is down,I know this 





void clawopen (bool open)
{claw.set (open);}

//idk maybe opens and closes the claw
//true is open and false is close
//examples
//claw.set(true);    open
//claw.set(false);   close

void backhooks (bool open)
{backHook.set (open);}

//opens and closes the back hook
//true is open and false is close
//examples
//backHook.set(true);   open
//backHook.set(false);   close


void picassos (bool open)
{picasso.set (open);}

//opens and closes thee picasso
//true is open and false is close
//examples
//picasso.set(true);     open
//picasso.set(false);    close

void inchDrive(float target, int speed){
  leftDrive1.setPosition(0,  rev);
    leftDrive2.setPosition(0,  rev);//might only need 1 of 3 of these but im a dumbass so leave it 
  leftmiddle.setPosition(0,  rev);
  float inches = 0.0;
  float turns= 0.0;
  float error = target; 
  float olderror = error; 
  float kp=10;
  float kd = 20.0;
  
  //dont use the drive function you dumbass
  //use inchdrive,this took me a while to code :(
  //its target/inches/amount then speed/percentage
  //examples
  //inchDrive(55, 100); go 55in forward at 100%
  //inchDrive(-55, 100); go 55in backwards at 100%


  while(fabs(error)>1.0){
    //did this late at night but this while is important 
   //fabs = absolute value
    //heading= Gyro.rotation(degrees);
  drive(speed, speed, 10);
      turns =leftmiddle.position(rev);
    inches = turns * Diameter * pi;   //took and hour to fix this I think,was off by like 10 inches lol
    olderror=error;
    error = target-inches; //the error gets smaller when u reach ur target
 //inches = turns * Diameter * pi;
    
    speed = kp*error+kd*(error-olderror); //big error go fast slow error go slow 

    Brain.Screen.printAt(1, 40, "turns = %0.2f    ", turns); //math fun
    Brain.Screen.printAt(1, 60, "speed = %0.2f    ", speed);
     Brain.Screen.printAt(1, 100, "inches = %.2f  f", inches);
     Brain.Screen.printAt(1, 120, "error = %.2f  f", error);
  }
   
  drive(0,0,0);
  }
//if gyro needs calibrating add a 10ms wait or something
 void balance() {
  double pitch = Gyro.pitch(degrees);
  double oldpitch = pitch;                //work in progress code
  inchDrive(20, 100);
  Brain.Screen.clearScreen();
  double Kp = 4;
  double Ki = 0.5;
  double Kd = 90;
  double speed;

  Integral pitches;
  pitches.size = 10;
  pitches.innit();

double stopAng = 5; // stop when fabs(pitch) is at most 5°
while(fabs(pitch) > stopAng)
{
  pitch = Gyro.pitch(degrees);
  pitches.addVal(pitch);
  speed = Kp * pitch + Ki * pitches.mean() + Kd * (pitch - oldpitch);
  drive(speed, speed, 10);
  oldpitch = pitch;
  Brain.Screen.printAt(1, 100, "pitch=   %.3f   ",pitch);
}
brakeDrive();
Brain.Screen.printAt(1, 150, "i am done ");
}


void gyroturn(float target){ //idk maybe turns the robot with the gyro,so dont use the drive function use the gyro
 float kp=2.0;
 float kd = 16.0;
  Gyro.setRotation(0, degrees); //centers,calibrates gyro to 0
  float heading = 0.0;
  float speed = 100;
  float error = target;
  float olderror=error;
  while(fabs(error)>2.0){ //fabs = absolute value while loop again
    heading= Gyro.rotation(degrees);
    olderror=error;
    error = target-heading; //error gets smaller closer you get,robot slows down
    drive(speed, -speed, 10);
    speed = kp*error+kd*(error-olderror); //big error go fast slow error go slow 
    Brain.Screen.printAt(1, 40, "heading = %0.2f    degrees", heading); //math thing again,2 decimal places
    Brain.Screen.printAt(1, 60, "speed = %0.2f    degrees", speed);
    //all ths print screen helps test to make sure gyro not broke
  }
  drive(0,0,0);
  heading= Gyro.rotation(degrees);//prints the gyro rotation degress
  Brain.Screen.printAt(1, 40, "heading = %0.2f    degrees", heading);
}


//wow maybe the auton code,this auton is the left side auton,works well
void auton() {
 
 

  




  
  









}

//driver controls,dont change unless your jaehoon or sean
void driver() {
  // User control code here, inside the loop
  //2 joy sticks
  //rstick is axis 2 and lstick is axis 3,why its 2,3 and not 1,2 idk ask vex
  while (true) {
    int rstick=Controller1.Axis2.position();
    int lstick=Controller1.Axis3.position();
    drive(lstick, rstick,10);

    if (Controller1.ButtonR1.pressing())  //mogo up
    {
      amogus.spin(directionType::fwd,100,velocityUnits::pct);
    }
    else if (Controller1.ButtonR2.pressing()) //mogo down
    {
      amogus.spin(directionType::rev,100,velocityUnits::pct);
    }
    else
    {
      amogus.stop(brakeType::hold); //hold,do nothing if nothing is being pressed
    }

    if (Controller1.ButtonL1.pressing())  //lift up
    {
      lift1.spin(directionType::fwd,100,velocityUnits::pct);
    }
    else if (Controller1.ButtonL2.pressing())  //lift down
    {
      lift1.spin(directionType::rev,100,velocityUnits::pct);
    }
    else
    {
      lift1.stop(brakeType::hold); //hold,do nothing if nothing is being pressed
    }

    if (Controller1.ButtonY.pressing()) //claw down
    {
      backHook.set(false);
    }
    else if (Controller1.ButtonB.pressing()) //claw up or maybe the opposite 
    {
      backHook.set(true);
    }
    
    if (Controller1.ButtonX.pressing()) //claw close
    {
      claw.set(false);
    }
    else if (Controller1.ButtonA.pressing()) //claw open
    {
      claw.set(true);
    }

    if (Controller1.ButtonUp.pressing()) //picasso open or whaever you say
    {
      picasso.set(false);
    }
    else if (Controller1.ButtonRight.pressing()) //picasso do opposite
    {
      picasso.set(true);
    }
    if (Controller1.ButtonDown.pressing())  //mogo up
    {
      balance();
    }

    wait(20, msec); // dont waste air 
  }
}

    
    


  

  
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(auton);
  Competition.drivercontrol(driver);

  // Run the pre-autonomous function.
  pre_auton();

 
  // Stops main from exiting in the infinite loop.
  while (true) {
    wait(100, msec);
  }
}
