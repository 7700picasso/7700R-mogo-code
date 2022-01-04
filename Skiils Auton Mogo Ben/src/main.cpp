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
// GPS                  gps           9               
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
const double UNITSIZE = 23.75; // its not 24

/*
dont touch
Diameter is the wheel
pie is pie dumbass
*/

void pre_auton(void) {
  vexcodeInit();
  Gyro.calibrate();
  GPS.calibrate();
  wait(2000, msec);

  // All activities that occur before the competition starts
  // gets pistons down before match
  // dont touch this 
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
      values[i] = values[i - 1] * 0.995; // decreases impact of previous values
    values[0] = val;
  }

  double mean() {
    double sum = 0;
    for (int i = 0; i < size; i++)
      sum += values[i];
    return sum / size;
  }
};

double minRots() {    
  double rots[] = {
    leftDrive1.position(rev), leftDrive2.position(rev), leftmiddle.position(rev),
    rightDrive1.position(rev), rightDrive2.position(rev), rightmiddle.position(rev)
  };
  double min = rots[0];
  
  for (int i = 1; i < 6; i++) {
    if (rots[i] < min) {
      min = rots[i];
    }
  }
  return min;
}

void getPos(double &x, double &y) {
  x = GPS.xPosition(mm) / 600, 
  y = GPS.yPosition(mm) / 600;
}

double pointTowardsPre(double x1, double y1, double x2, double y2, bool Reverse = false) { // get the angle at which the robot needs to turn to point towards point (x,y)
  double theta = atan2((x2 - x1), (y2 - y1)) * 180 / pi - Gyro.rotation(degrees); // get number of degrees robot must turn to point in a certain direction.
  //theta += (theta < 0 ? 1 : -1) * 180 * (y2 >= y1); // then maybe add ±180 to correct it
  return theta + (theta < 0 ? 1 : -1) * 180 * Reverse; // then maybe add ±180 to if its backwards
}

void brakeDrive() {
  leftDrive1.stop(brake);
  leftDrive2.stop(brake);
  leftmiddle.stop(brake);
  rightDrive1.stop(brake);
  rightDrive2.stop(brake);
  rightmiddle.stop(brake);
  //break code for the gryo balance code
  // actually change brake type
  leftDrive1.setStopping(brake);
  leftDrive2.setStopping(brake);
  leftmiddle.setStopping(brake);
  rightDrive1.setStopping(brake);
  rightDrive2.setStopping(brake);
  rightmiddle.setStopping(brake);
}

void coastDrive() {
  leftDrive1.stop(coast);
  leftDrive2.stop(coast);
  leftmiddle.stop(coast);
  rightDrive1.stop(coast); //coast drive
  rightDrive2.stop(coast); //saif put notes here with examples 
  rightmiddle.stop(coast);
  // actually set brake stop type to coast
  leftDrive1.setStopping(coast);
  leftDrive2.setStopping(coast);
  leftmiddle.setStopping(coast);
  rightDrive1.setStopping(coast); 
  rightDrive2.setStopping(coast);
  rightmiddle.setStopping(coast);
}


void liftDeg(double angle, int WT = -1, int speed = 100) {
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

void liftTime(int speed, int duration, bool stop) {//, int WT = -1) {
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

void mogoDeg(double angle, int WT = -1, int speed = 100) {
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

void mogoTime(int speed, int duration, bool stop = false) {//, int WT = -1) {
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


void claw(bool open) {
  wait(50 * open, msec);
  Claw.set(open);
  wait(50 * !open, msec);
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

void inchDrive(double target, double accuracy = 0.25) {
	 double Kp = 50 / 3; // about 16.667, was previously 10
	 double Ki = 5; // to increase speed if its taking too long. Adds a bit over 50% speed when 12 inches left.
	 double Kd = 40 / 3; // about 13.333, was previously 20.0
	 double speed;
	 double error = target;
	 double olderror = error;
	 
	 /*dist += target;
	 target = dist;*/
	 
	 leftDrive1.setPosition(0, rev);
	 leftDrive2.setPosition(0, rev);
	 leftmiddle.setPosition(0, rev);
	 rightDrive1.setPosition(0, rev);
	 rightDrive2.setPosition(0, rev);
	 rightmiddle.setPosition(0, rev);
	 
	 Integral errors;
	 errors.size = 25; // takes about 0.25 seconds to fill up
	 errors.innit(error);
	 
	 while(fabs(error) > accuracy) {
	 // did this late at night but this while is important 
	 // fabs = absolute value
	 error = target - minRots() * Diameter * pi; //the error gets smaller when u reach ur target
	 errors.addVal(error);
	 speed = Kp * error + Ki * errors.mean() + Kd * (error - olderror); // big error go fast slow error go slow 
	 drive(speed, speed, 10);
	 olderror = error;
	 Brain.Screen.printAt(1, 40, "turns = %0.2f ", minRots()); // math fun
	 Brain.Screen.printAt(1, 60, "speed = %0.2f ", speed);
	 //Brain.Screen.printAt(1, 100, "inches = %.2f f", inches);
	 Brain.Screen.printAt(1, 120, "error = %.2f f", error);
	 }
	 brakeDrive();
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

void driveTo(double x2, double y2, bool Rev = 0, double accuracy = 1/48) {
  // point towards target
  double x1, y1;
  getPos(x1, y1);
  double rotError = pointTowardsPre(x1, y1, x2, y2, Rev);
  double facing = Gyro.rotation(degrees); // i need this ig
  gyroturn(rotError, facing);

  // go to target

  double speed;
  double distError = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
  double oldDistError = distError;
  double distKp = 50 / 3; // about 16.667, was previously 10
  double distKi = 5; // to increase speed if its taking too long. Adds a bit over 50% speed when 12 inches left.
  double distKd = 40 / 3; // about 13.333, was previously 20.0

  Integral distErrors;
  distErrors.size = 25; // takes about 0.25 seconds to fill up
  distErrors.innit();

  // angle correction
  double diff; // like speed
  // rotError already exists
  double oldRotError = rotError;
  double rotKp = 0.333;
  double rotKi = 0.667;
  double rotKd = 5;

  Integral rotErrors;
  rotErrors.size = 25; // takes about 0.25 seconds to fill up
  rotErrors.innit();

  while(fabs(distError) > accuracy){
    // did this late at night but this while is important 
    // fabs = absolute value
    getPos(x1, y1);
    rotError = pointTowardsPre(x1, y1, x2, y2, Rev);
    distError = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    distErrors.addVal(distError);
    rotErrors.addVal(rotError);
    speed = distKp * distError + distKi * distErrors.mean() + distKd * (distError - oldDistError); // big error go fast slow error go slow 
    diff = rotKp * rotError + rotKi * rotErrors.mean() + rotKd * (rotError - oldRotError); // big error go fast slow error go slow 
    drive(speed + diff, speed - diff, 10);
    oldDistError = distError;
    oldRotError = rotError;
  }
  brakeDrive();
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

  driveTo(0, 0);
/*
  // FIRST ALLIANCE (dont picasso)
  brakeDrive(); // set motors to brake
  mogoTime(-100, 750, false); // lower lift for 750 msec
  inchDrive(-17);
  mogoDeg(45.0, 500); // raise by 45 degrees
  inchDrive(4);                       //please put notes for all functions in this auton for troubleshooting 
  // OTHER ALLIANCE
  gyroturn(-90, facing);
  claw.set(true); // open claw
  inchDrive(-UNITSIZE);
  gyroturn(-90, facing);
  inchDrive(3.333 * UNITSIZE);
  claw.set(false);
  // SHOVE FIRST YELLOW TO THE OTHER SIDE
  liftDeg(90.0, 20);
  inchDrive(-8);
  gyroturn(-90, facing);
  inchDrive(2.25 * UNITSIZE);
  inchDrive(-6);
  gyroturn(-45,facing);
  inchDrive(1.41421 * UNITSIZE);
  gyroturn(45,facing);
  inchDrive(6);
  claw.set(true);
  liftDeg(-95.0, 20);
  inchDrive(-6); //
  // GET SECOND YELLOW
  gyroturn(90, facing);
  mogoDeg(-45);
  inchDrive(16);
  claw.set(false);
  // PLATFORM FIRST YELLOW
  liftDeg(90.0, 0);
  mogoDeg(90.0, 20);
  inchDrive(-1.667 * UNITSIZE);
  gyroturn(-90, facing);
  inchDrive(6);
  claw.set(true);
  liftDeg(-95.0, 20);
  inchDrive(-6);
  // GET LAST ALLIANCE
  while (lift1.position(degrees) > 270) { // make sure that the lift is low enought b4 continuing
    wait(10, msec);
  }
  gyroturn(-90, facing);
  inchDrive(8);
  claw.set(false);
  liftDeg(90.0, 20);
  // PLATFORM LAST ALLIANCE
  inchDrive(-1.83333 * UNITSIZE);
  gyroturn(90,facing);
  inchDrive(6);
  claw.set(true);
  // GET OTHER SHORT YELLOW
  liftDeg(-95.0, 20);
  inchDrive(-6);
  gyroturn(-135, facing);
  inchDrive(2 * UNITSIZE);
  claw.set(false);
  // PLATFORM OTHER SHORT YELLOW
  liftDeg(90.0, 20);
  inchDrive(-2 * UNITSIZE);
  gyroturn(45, facing);
  inchDrive(-6);
  gyroturn(90, facing);
  inchDrive(6);
  claw.set(true);
  // GET TALL GOAL
  liftDeg(-95.0,20);
  inchDrive(-6);
  gyroturn(170.5376778, facing);
  inchDrive(1.520690633 * UNITSIZE);
  claw.set(false);
  // PICASSO ALLIANCE GOAL NEAR PLATFORM
  gyroturn(35.53767779,facing);
  mogoDeg(-95.0, 0);
  inchDrive(-2.474873734 * UNITSIZE);
  gyroturn(-90,facing);
  inchDrive(-UNITSIZE);
  mogoDeg(100);
  picasso.set(false);
  // FAR ALLIANCE GOAL 
  mogoDeg(-110, 0);
  inchDrive(1 * UNITSIZE);
  gyroturn(-45);
  inchDrive(4 * UNITSIZE);
  mogoDeg(60, 500);
  // ALIGN TO PARK
  liftDeg(90,0);
  gyroturn(90);
  inchDrive(4 * UNITSIZE);
  gyroturn(-90);
  inchDrive(0.75 * UNITSIZE);
  liftDeg(-90,3000); // bring down the platform
  // PARK
  inchDrive(1 * UNITSIZE);
  balance();*/
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
