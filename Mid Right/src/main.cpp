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
// DistFront            distance      15              
// DistBack             distance      16              
// DistClaw             distance      17              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <math.h>

using namespace vex;

// A global instance of competition,dont touch either
competition Competition;

// define your global Variables here
const long double pi = 3.1415926535897932384626433832795028841971693993751058209749445923;
const double Diameter = 3.25;

//dont touch
//Diameter is the wheel
//pie is pie dumbass it also tastes good bc thats the wrong pi

// for auton 
double x = 0, y = 0;
const uint8_t UNITSIZE = 24; // tile size

void pre_auton(void) {
  vexcodeInit();
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

/*double MinRots() {
  double rotations[] = {leftDrive1.position(rev), leftmiddle.position(rev), leftDrive2.position(rev), leftDrive1.position(rev), leftmiddle.position(rev), rightDrive2.position(rev)};
}*/

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

void lift(double angle, int WT = -1, double speed = 100) { // WT of -1 means wait for completion
  // gear ratio is 1 / 9
  lift1.setVelocity(speed, percent);
  lift1.spinFor(forward, 7 * angle, degrees, WT == -1); // wait for competion is determined by whether or not WT is -1

  if (WT >= 0)
    wait(WT, msec);
  else
    lift1.stop(hold);
}

//makes lift go up or down
// its the angle to move the lift by then wait time then speed
// example lift(90,1200);  so lift 90 degrees up for 1200 msc
// example 2: lift(-90,-1,100); lift goes down 90 degrees, waits until it's done, goes at 100% speed

void mogo(double angle, int WT = -1, double speed = 100) { // WT of -1 is wait for completion
  // gear ratio is 1/5
  amogus.setVelocity(speed, percent);
  amogus.spinFor(forward, 5 * angle, degrees, WT == -1);

  if (WT >= 0)
    wait(WT, msec);
  else
    amogus.stop(hold);
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
  picasso.set(open);
}

//opens and closes thee picasso
//true is open and false is close
//examples
//picasso.set(true);     open
//picasso.set(false);    close

void inchDrive(double target, double lowerDist = 100000, double accuracy = 1) {
  leftDrive1.setPosition(0,  rev);
  leftDrive2.setPosition(0,  rev); // might only need 1 of 3 of these but im a dumbass so leave it 
  leftmiddle.setPosition(0,  rev);

  double 
    speed,
    error = target,
    olderror = error,
    Kp = 50 / 3, // about 16.667, was previously 10
    Ki = 1, // to increase speed if its taking too long. Adds a bit over 50% speed when 12 inches left.
    Kd = 40 / 3; // about 13.333, was previously 20.0

  double sum = 0;
  double decay = 0.5;

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
    sum = sum * decay + error;
    speed = Kp * error + Ki * decay + Kd * (error - olderror); // big error go fast slow error go slow 
    drive(speed, speed, 10);
    olderror = error;
    if (target - error > lowerDist) {
      amogus.spin(forward,-100,percent);
    }
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
  double Kp =  4;
  double Ki = 1;
  double Kd = 90;
  double speed;

double stopAng = 5; // stop when fabs(pitch) is at most 5°
while(fabs(pitch) > stopAng)
{
  pitch = Gyro.pitch(degrees);
  speed = Kp * pitch + Ki * 0 + Kd * (pitch - oldpitch);
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
 
  double currentDir = Gyro.rotation(degrees);
  double speed = 100;
  double error = target;
  double olderror = error;

  double lambda = 0.5; // exponential decay rate

  double sum = 0;
  
  idealDir += target;
  target = currentDir + idealDir - Gyro.rotation(degrees);
  
  while(fabs(error) > 1.25){ //fabs = absolute value while loop again
    currentDir = Gyro.rotation(degrees);
    error = target - currentDir; //error gets smaller closer you get,robot slows down
    sum = sum * lambda + error;
    speed = Kp * error + Ki * sum + Kd * (error - olderror); // big error go fast slow error go slow 
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


//wow maybe the auton code,this auton is the right side auton,work in progress but works how is 
void auton() {
  claw.set(true); // open claw
  picasso.set(false); // open picasso
  while (Gyro.isCalibrating()) {
    wait(10,msec);
  }
  Gyro.setRotation(0,degrees);

  double facing = 0;
  
  
  "AUTO NUMBER 2:";
  "MID-PICASSO-SIDE";
  "START IN THE CORNER FACING MID. CENTER OF ROBOT SHOULD BE ON CENTER OF TILE";
  double mogoStopDist = 6; // STOP THIS MANY UNITS BEFORE A MOGO. FEEL FREE TO CHANGE
  // MID
  inchDrive(84.85 - mogoStopDist, 8); // GO TO MID and LOWER MOGO LIFT.
  claw.set(false); // CLAW IT
  lift(10, 0); // RAISE LIFT BY 10° TO REDUCE FRICTION
  // PICASSO
  inchDrive(-50.91 + mogoStopDist, 0); // ALIGN WITH ALLIANCE GOAL ON Y-AXIS.
  gyroturn(-45, facing); // FACE ALLIANCE GOAL
  inchDrive(-15); // GET IT IN MOGO LIFT
  mogo(130,1000); // BRING THAT MOGO INTO PICASSO
  picasso.set(true); // PICASSO THAT MOGO
  // SIDE
  mogo(-140,0); // START LOWERING MOGO LIFT
  inchDrive(15); // GO BACK TO WHEREVER IT WAS B4 THE LINE THAT SAYS "GET IT IN MOGO LIFT" FROM SECTION "PICASSO"
  gyroturn(-90, facing); // FACE MOGO LIFT TO SIDE GOAL
  inchDrive(-36 + mogoStopDist); // GET MOGO IN LIFT
  mogo(60,500); // RAISE MOGO LIFT BY 60°
  inchDrive(50); // RETURN TO HOME ZONE
  brakeDrive();

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

    if (Controller1.ButtonY.pressing() && backHook.value() == 1) //claw down
    {
      backHook.set(false);
    }
    else if (Controller1.ButtonB.pressing() && backHook.value() == 0) //claw up or maybe the opposite 
    {
      backHook.set(true);
    }
    
    if (Controller1.ButtonX.pressing() && claw.value() == 1) //claw close
    {
      claw.set(false);
    }
    else if (Controller1.ButtonA.pressing()  && claw.value() == 0) //claw open
    {
      claw.set(true);
    }

    if (Controller1.ButtonUp.pressing() && picasso.value() == 1) //picasso open or whaever you say
    {
      picasso.set(false);
    }
    else if (Controller1.ButtonRight.pressing() && picasso.value() == 0) //picasso do opposite
    {
      picasso.set(true);
    }

    wait(10, msec); // dont waste air 
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
