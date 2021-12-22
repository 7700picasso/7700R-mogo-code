/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Sean Jaehoon and Saif                                     */
/*    Created:      sometime                                                  */
/*    Description:  7700R code 2021-2022  Skills                              */
/*               Benjamin 12-20-21                                            */
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

void brakeDrive() {
  leftDrive1.stop(brake);
  leftDrive2.stop(brake);
  leftmiddle.stop(brake);
  rightDrive1.stop(brake);
  rightDrive2.stop(brake);
  rightmiddle.stop(brake);
  //break code for the gryo balance code
}

void coastDrive() {
  leftDrive1.stop(coast);
  leftDrive2.stop(coast);
  leftmiddle.stop(coast);
  rightDrive1.stop(coast); //coast drive
  rightDrive2.stop(coast); //saif put notes here with examples 
  rightmiddle.stop(coast);
}


void lift(double angle, int WT = -1, int speed = 100) {
  lift1.setVelocity(speed, percent);
  lift1.setStopping(hold);

  lift1.spinFor(forward, 9 * angle, degrees, WT == -1);
  
  if (WT != -1) {
    wait(WT,msec);
  }
  else {
    lift1.stop(hold);
  }
}

void lift(int speed, int duration, bool stop) {//, int WT = -1) {
  lift1.spin(forward,speed,percent);
  wait(duration,msec);
  if (stop) {
    lift1.stop(hold);
  }
}

//makes lift go up or down
// its the lift speed then wait time
//example lift(-100,1200);  so lift 100% for 1200 msc
// 100 is up and -100 is down,or other way around,you can figure that out

void mogo(double angle, int WT = -1, int speed = 100) {
  amogus.setVelocity(speed, percent);
  amogus.setStopping(hold);

  amogus.spinFor(forward, 5 * angle, degrees, WT == -1);
  
  if (WT != -1) {
    wait(WT,msec);
  }
  else {
    amogus.stop(hold);
  }
}

void mogo(int speed, int duration, bool stop = false) {//, int WT = -1) {
  amogus.spin(forward,speed,percent);
  wait(duration,msec);
  if (stop) {
    amogus.stop(hold);
  }
}

//makes mogo go up or down
// its the lift speed then wait time
//mogo(100, 1500, false);  so mogo 100% for 1500 msc with some werid stop thing
// 100 is up and -100 is down,I know this 


void clawopen (bool open) {
  claw.set (open);
}

//idk maybe opens and closes the claw
//true is open and false is close
//examples
//claw.set(true);    open
//claw.set(false);   close

void backhooks (bool open) {
  backHook.set (open);
}

//opens and closes the back hook
//true is open and false is close
//examples
//backHook.set(true);   open
//backHook.set(false);   close


void picassos (bool open) {
  picasso.set (open);
}

//opens and closes thee picasso
//true is open and false is close
//examples
//picasso.set(true);     open
//picasso.set(false);    close

void inchDrive(double target, double accuracy = 1) {
  leftDrive1.setPosition(0,  rev);
  leftDrive2.setPosition(0,  rev); // might only need 1 of 3 of these but im a dumbass so leave it 
  leftmiddle.setPosition(0,  rev);

  double 
    speed,
    inches = 0,
    error = target,
    olderror = error,
    Kp = 50 / 3, // about 16.667, was previously 10
    Ki = 5, // to increase speed if its taking too long. Adds a bit over 50% speed when 12 inches left.
    Kd = 40 / 3; // about 13.333, was previously 20.0

  Integral errors;
  errors.size = 25; // takes about 0.25 seconds to fill up
  errors.innit();

  /*
  dont use the drive function you dumbass AND DONT USE "//" for multiple lines
  use inchdrive,this took me a while to code :(
  its target/inches/amount then speed/percentage
  examples
  inchDrive(55, 100); go 55in forward at 100%
  inchDrive(-55, 100); go 55in backwards at 100%
  */

  while(fabs(error) > accuracy){
    // did this late at night but this while is important 
    // fabs = absolute value
    error = target - leftmiddle.position(rev) * Diameter * pi; //the error gets smaller when u reach ur target
    errors.addVal(error);
    speed = Kp * error + Ki * errors.mean() + Kd * (error - olderror); // big error go fast slow error go slow 
    drive(speed, speed, 10);
    olderror = error;
 //inches = turns * Diameter * pi;
    Brain.Screen.printAt(1, 40, "turns = %0.2f    ", leftmiddle.position(rev)); //math fun
    Brain.Screen.printAt(1, 60, "speed = %0.2f    ", speed);
    Brain.Screen.printAt(1, 100, "inches = %.2f  f", inches);
    Brain.Screen.printAt(1, 120, "error = %.2f  f", error);
  }
  brakeDrive();
  // update x and y
  double 
    r = leftmiddle.position(rev) * Diameter * pi / UNITSIZE,
    theta = Gyro.rotation(degrees) * pi / 180;
  x += r * cos(theta), y += r * sin(theta);

  Brain.Screen.printAt(50, 180, "Position:   (%.3f, %.3f)", x, y);
}

//if gyro needs calibrating add a 10ms wait or something, gyro cal takes about 1.5 sec
//1 sec if your good

void balance() {
  double pitch = Gyro.pitch(degrees);
  double oldpitch = pitch;                //work in progress code
  inchDrive(10, 100);
  Brain.Screen.clearScreen();
  double Kp = 4;
  double Ki = 1;
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

// modded gyro code, sadge
void gyroturn(double target, double &idealDir) { // idk maybe turns the robot with the gyro,so dont use the drive function use the gyro
  double Kp = 1.25; // was 2.0
  double Ki = 0.1; // adds a bit less than 50% when there is 90° left.
  double Kd = 1.0; // was 16.0

  Integral directions;
  directions.size = 15;
  directions.innit();
 
  double currentDir = Gyro.rotation(degrees);
  double speed = 100;
  double error = target;
  double olderror = error;
  
  idealDir += target;
  target = currentDir + idealDir - Gyro.rotation(degrees);
  
  while(fabs(error) > 1.25){ //fabs = absolute value while loop again
    currentDir = Gyro.rotation(degrees);
    error = target - currentDir; //error gets smaller closer you get,robot slows down
    directions.addVal(error);
    speed = Kp * error + Ki * directions.mean() + Kd * (error - olderror); // big error go fast slow error go slow 
    drive(speed, -speed, 10);
    Brain.Screen.printAt(1, 40, "heading = %0.2f    degrees", currentDir); //math thing again,2 decimal places
    Brain.Screen.printAt(1, 60, "speed = %0.2f    degrees", speed);
    //all ths print screen helps test to make sure gyro not broke
    olderror = error;
  }
  brakeDrive();
  currentDir = Gyro.rotation(degrees); //prints the gyro rotation degress
  Brain.Screen.printAt(1, 40, "heading = %0.2f    degrees", currentDir);
}


// This auton is the skills auton,not fully tested
void auton() {
  double facing = 0;

  backHook.set(false);
  picasso.set(false);

  while (Gyro.isCalibrating()) { // dont start until gyro is calibrated
    wait(10, msec);
  }

  Gyro.setRotation(0, degrees);

  // FIRST ALLIANCE (dont picasso)
  brakeDrive(); // set motors to brake
  mogo(-100, 750, false); // lower lift for 750 msec
  inchDrive(-17);
  mogo(45.0, 500); // raise by 45 degrees
  inchDrive(4);                       //please put notes for all functions in this auton for troubleshooting 
  // OTHER ALLIANCE
  gyroturn(-90, facing);
  claw.set(true); // open claw
  inchDrive(-UNITSIZE);
  gyroturn(-90, facing);
  inchDrive(3.333 * UNITSIZE);
  claw.set(false);
  // SHOVE FIRST YELLOW TO THE OTHER SIDE
  lift(90.0, 20);
  inchDrive(-8);
  gyroturn(-90, facing);
  inchDrive(2.25 * UNITSIZE);
  // PLATFORM ALLIANCE GOAL
  inchDrive(-6);
  gyroturn(-45,facing);
  inchDrive(1.41421 * UNITSIZE);
  gyroturn(45,facing);
  inchDrive(6);
  claw.set(true);
  lift(-95.0, 20);
  inchDrive(-6); //
  // GET SECOND YELLOW
  gyroturn(90, facing);
  mogo(-45);
  inchDrive(16);
  claw.set(false);
  // PLATFORM FIRST YELLOW
  lift(90.0, 0);
  mogo(90.0, 20);
  inchDrive(-1.667 * UNITSIZE);
  gyroturn(-90, facing);
  inchDrive(6);
  claw.set(true);
  lift(-95.0, 20);
  inchDrive(-6);
  // GET LAST ALLIANCE
  while (lift1.position(degrees) > 270) { // make sure that the lift is low enought b4 continuing
    wait(10, msec);
  }
  gyroturn(-90, facing);
  inchDrive(8);
  claw.set(false);
  lift(90.0, 20);
  // PLATFORM LAST ALLIANCE
  inchDrive(-1.83333 * UNITSIZE);
  gyroturn(90,facing);
  inchDrive(6);
  claw.set(true);
  // GET OTHER SHORT YELLOW
  lift(-95.0, 20);
  inchDrive(-6);
  gyroturn(-135, facing);
  inchDrive(2 * UNITSIZE);
  claw.set(false);
  // PLATFORM OTHER SHORT YELLOW
  lift(90.0, 20);
  inchDrive(-2 * UNITSIZE);
  gyroturn(45, facing);
  inchDrive(-6);
  gyroturn(90, facing);
  inchDrive(6);
  claw.set(true);
  // GET TALL GOAL
  lift(-95.0,20);
  inchDrive(-6);
  gyroturn(170.5376778, facing);
  inchDrive(1.520690633 * UNITSIZE);
  claw.set(false);
  // GET ALLIANCE GOAL NEAR PLATFORM
  gyroturn(35.53767779,facing);
  mogo(-95.0, 0);
  inchDrive(-2.474873734 * UNITSIZE);
  gyroturn(-90,facing);
  inchDrive(-UNITSIZE);
  mogo(100);
  picasso.set(false);

  // PICCASO FIRST ALLIANCE GOAL
  /*mogo(-100, 1200);
  inchDrive(-17);
  mogo(100, 1500, false);
  picasso.set(true);
  mogo(0, 0);
  inchDrive(4);                       //please put notes for all functions in this auton for troubleshooting 
  // GRAB FIRST NEUTRAL GOAL
  gyroturn(90, facing);
  claw.set(true); // open claw
  inchDrive(55);
  claw.set(false); // close claw
  inchDrive(-8);
  // put it on the platform
  gyroturn(140, facing);
  lift(100, 1700);
  inchDrive(43); // WAS 38
  lift(-60, 400);
  claw.set(true);
  lift(60, 400);
  inchDrive(-22);   
  gyroturn(-100, facing);
  lift(-100, 1500);
  inchDrive(28);
  claw.set(false);
  inchDrive(-10);
  // platform it
  gyroturn(110, facing);
  lift(100, 1700);
  inchDrive(39);
  lift(-60,400);
  claw.set(true);
  lift(60, 400);
  lift(0, 0);
  inchDrive(-30);
  // middle goal
  gyroturn(80, facing);
  mogo(-100,1200);
  inchDrive(-40);
  mogo(100, 1500);
  gyroturn(-100, facing);
  lift(-100, 1500);
  inchDrive(20);
  claw.set(false);*/
}

//driver controls,dont change unless your jaehoon or sean
void driver() {
  // User control code here, inside the loop
  //2 joy sticks
  //rstick is axis 2 and lstick is axis 3,why its 2,3 and not 1,2 idk ask vex
  coastDrive(); // set drive motors to coast
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
