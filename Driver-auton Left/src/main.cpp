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

using namespace vex;

// A global instance of competition,dont touch either
competition Competition;

// define your global Variables here
const long double pi = 3.1415926535897932384626433832795028841971693993751058209749445923;
const double Diameter = 3.25;

//dont touch
//Diameter is the wheel
//pie is pie dumbass
bool isDriver = false;


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

volatile int mogoSpeed = 0, mogoDuration = 0; 
volatile bool mogoStop = false, done = true;

void mogoTime() {//int speed, int duration, bool stop = false) {
  while (true) {
    if (!done) {
      amogus.spin(forward, mogoSpeed, percent);
      wait(mogoDuration, msec);

      if (mogoDuration) {
        amogus.stop(hold);
      }
      done = true;
    }
  }
}

/*void startMogoTime(int speed, int duration, bool stop = false) {
  if (!isDriver) {
    mogoSpeed = speed, 
    mogoDuration = duration; 
    mogoStop = stop;
    done = false;
  }
}

thread liftControl(mogoTime);
//makes mogo go up or down
// its the lift speed then wait time
//example mogo(-100,1200);  so mogo 100% for 1200 msc
// 100 is up and -100 is down,I know this */

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
}

//if gyro needs calibrating add a 10ms wait or something
 
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



//wow maybe the auton code,this auton is the left side auton,works well
void auton() {
  // THIS STUFF BEFORE THE LINE THAT SAYS "AUTO 1" NEEDS TO GO BEFORE BOTH AUTOS.
  claw.set(true); // open claw
  picasso.set(false); // open picasso
    backhooks(false); // raise hook so it doesn't get in the way
  
  while (Gyro.isCalibrating()) {
    wait(10,msec);
  }
  Gyro.setRotation(0,degrees);

  double facing = 0;
  double mogoStopDist = 6; // STOP THIS MANY UNITS BEFORE A MOGO. FEEL FREE TO CHANGE
  "AUTO 1";
  "SIDE-PICASSO";
  "START AT THE HALFWAY LINE NEXT TO THE ALLIANCE GOAL, FACING IT WITH THE CLAW. SEE https://cdn.discordapp.com/attachments/875890646714576906/944007778526175282/Left_auto_SIDE-PICASSO.png FOR REFERENCE";
  // SIDE
  inchDrive(61.19 - mogoStopDist); // GO TO THE MOGO
  claw.set(false); // CLAW IT
  // PICASSO
  // WE WILL APPROACH THIS FROM THE DIAGONAL.
  inchDrive(-36.714 + mogoStopDist,0) // GO TO THE DIAGONAL OF APPROACH. LOWER MOGO LIFT.
  gyroturn(-30-11.3, facing); // POINT MOGO LIFT AT THE ALLIANCE GOAL. change the 30.
  claw.set(true); // DROP SIDE
  inchDrive(-20, 0); // GET MOGO INTO MOGO LIFT.
  mogoDeg(130,1000); // BRING GOAL INTO PICASSO.
  picasso.set(true); // PICASSO IT.
  // GET SIDE BUT WITH MOGO LIFT
  gyroturn(180,facing); // FACE IT
  mogoDeg(-140,750); // LOWER MOGO LIFT
  inchDrive(-24); // GET MOGO INTO LIFT
  mogoDeg(60,500); // RAISE LIFT
  gyroturn(-90,facing); // FACE MID
  break; // END
  "AUTO 2:";
  "MID-SIDE";
  "START IN THE CORNER, CLAW FACING MID. CENTER OF ROBOT SHOULD BE IN THE CENTER OF THE TILE";
  // MID
  inchDrive(84.85, 6); // GO TO MID and LOWER MOGO LIFT
  claw.set(false); // CLAW IT
  liftDeg(10,0) // RAISE LIFT BY 10° TO REDUCE FRICTION
  // SIDE
  gyroturn(45, facing); // FACE SIDE WITH MOGO LIFT
  inchDrive(-36); // GET MOGO INTO MOGO LIFT
  mogoDeg(60, 500); // RAISE MOGO LIFT
  // GO HOME & DROP SIDE
  gyroturn(90, facing); // FACE HOMEZONE
  inchDrive(36); // GO INTO HOMEZONE
  gyroturn(-90,facing); // TURN TO DUMP SIDE GOAL
  mogoDeg(-90,250); // LOWER MOGO LIFT
  inchDrive(-10); // BACK UP TO DROP MOGO IN A BETTER PLACE
  inchDrive(10); // FINISH DROPPING MOGO
  gyroturn(-90,facing); // point kinda towards alliance goal
}

//driver controls,dont change unless your jaehoon or sean
void driver() {
  // User control code here, inside the loop
  //2 joy sticks
  //rstick is axis 2 and lstick is axis 3,why its 2,3 and not 1,2 idk ask vex
  isDriver = true;
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
