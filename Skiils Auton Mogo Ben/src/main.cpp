/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Sean Jaehoon and Saif                                     */
/*    Created:      sometime                                                  */
/*    Description:  7700R code 2021-2022  Skills                              */
/*               Benjamin 1-0-22                                              */
/*----------------------------------------------------------------------------*/
//7700R
//Benjamin
//Sean
//Avery
//Matthew
//Jaehoon
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
#include <math.h>

using namespace vex;

// A global instance of competition, dont touch either
competition Competition;

// tile size
#define UNITSIZE 600
// converts mm to inches
#define INCHES 25.4

// define your global Variables here
const long double pi = 3.1415926535897932384626433832795028841971693993751058209749445923; // much more accurate than 3.14
const double Diameter = 3.25 * INCHES; // mm

void pre_auton(void) {
  vexcodeInit();
  Gyro.calibrate();
  GPS.calibrate();
  wait(2000, msec);
	
	picasso.set(false);
	claw.set(true);
	backhook.set(false);

  // All activities that occur before the competition starts
  // gets pistons down before match
  // dont touch this 
	
	// BOOOOOOPP
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
//use mmDrive to go forward and backwards,use gyro to turn

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

// i am bad at naming stuff
double degToTarget(double x1, double y1, double x2, double y2, bool Reverse = false) { // get the angle at which the robot needs to turn to point towards point (x,y)
  double theta = atan2((x2 - x1), (y2 - y1)) * 180 / pi - GPS.rotation(degrees); // get number of degrees robot must turn to point in a certain direction.
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

void Claw(bool open) {
  wait(50 * open, msec);
  claw.set(open);
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

void mmDrive(double target, double accuracy = 5) {
	double Kp = 0.66; // was previously 10 / 25.4
	double Ki = 0.2; // to increase speed if its taking too long. Adds a bit over 50% speed when 12 inches left.
	double Kd = 0.525; // was previously 20.0 / 25.4
	volatile double speed;
	volatile double error = target;
	volatile double olderror = error;
	 
  leftDrive1.setPosition(0, rev);
	leftDrive2.setPosition(0, rev);
  leftmiddle.setPosition(0, rev);
  rightDrive1.setPosition(0, rev);
  rightDrive2.setPosition(0, rev);
  rightmiddle.setPosition(0, rev);
	 
  volatile double sum = 0;
  volatile short t = 1;
	 
	while(fabs(error) > accuracy) {
    // did this late at night but this while is important 
    error = target - minRots() * Diameter * pi; //the error gets smaller when u reach ur target
    sum = sum * 0.993 + error;
    speed = Kp * error + Ki * sum / t++ + Kd * (error - olderror); // big error go fast slow error go slow 
    drive(speed, speed, 10);
    olderror = error;
	}
	brakeDrive();
}

//if gyro needs calibrating add a 10ms wait or something, gyro cal takes about 1.5 sec
//1 sec if your good

void balance() { // WIP
  Brain.Screen.clearScreen();
  double Kp = 4;
  double Ki = 1;
  double Kd = 110; // we need it to go backwards when it starts tipping the other way.
  volatile double speed;
	volatile double pitch = Gyro.pitch(degrees);
  volatile double oldpitch = pitch;

  volatile double sum = 0;
  volatile int i = 1;

	double stopAng = 5; // stop when fabs(pitch) is at most 5° and when its no longer tipping back and forth.
	while(fabs(pitch) > stopAng || fabs(pitch - oldpitch) > 3) {
  	pitch = Gyro.pitch(degrees);
  	sum = sum * 0.993 + pitch;
  	speed = Kp * pitch + Ki * sum / i++ + Kd * (pitch - oldpitch);
  	drive(speed, speed, 10);
  	oldpitch = pitch;
  	Brain.Screen.printAt(1, 100, "pitch=   %.3f   ",pitch);
	}
	brakeDrive();
	Brain.Screen.printAt(1, 150, "i am done ");
}

// R.I.P. gyroturn 2021 - 2022 it will live on in pointAt(x, y)

void printPos() {
  while (true) {
    Controller1.Screen.setCursor(0, 0);
    Controller1.Screen.print("(%0.3f, %0.3f), dir: %0.3f", GPS.xPosition(mm) / 600, GPS.yPosition(mm) / 600, GPS.rotation(degrees));
    this_thread::sleep_for(1000);
  }
}

vex::thread POS(printPos);

void pointAt(double x2, double y2, bool Reverse = false, double accuracy = 1.25) { 
	// point towards target
  double x1 = GPS.xPosition(mm), y1 = GPS.yPosition(mm);
  x2 *= 600, y2 *= 600;
	double target = degToTarget(x1, y1, x2, y2, Reverse); // I dont trust the gyro for finding the target, and i dont trst the gps with spinning
	
	double Kp = 2;
	double Ki = 0.5;
	double Kd = 16.0;

	volatile double sum = 0;
	volatile int t = 1;

	volatile double speed;
	volatile double error = target;
	volatile double olderror = error;
	
	target += Gyro.rotation(degrees); // I trust gyro better for turning.

	while(fabs(error) > accuracy) { // fabs = absolute value while loop again
		error = target - Gyro.rotation(degrees); //error gets smaller closer you get,robot slows down
		sum = sum * 0.993 + error;
		speed = Kp * error + Ki * sum / t++ + Kd * (error - olderror); // big error go fast slow error go slow 
		drive(speed, -speed, 10);
		olderror = error;
	}
	brakeDrive();
}

void driveTo(double x2, double y2, bool Reverse = false, double accuracy = 5) {
  // point towards target
  pointAt(x2, y2, Reverse);
	
	// get positional data
	volatile double x1 = GPS.xPosition(mm), y1 = GPS.yPosition(mm);
  x2 *= 600, y2 *= 600;

  // go to target
  volatile double speed;
  volatile double distError = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
  volatile double oldDistError = distError;
	double distKp = 0.66;
  double distKi = 0.2;
  double distKd = 0.525; 

  volatile double distSum = 0;
  volatile int t = 1; // time

  // angle correction
  volatile double rotSpeed; // like speed
  volatile double rotError = degToTarget(x1, y1, x2, y2, Reverse);
  volatile double oldRotError = rotError;
  double rotKp = 0.333;
  double rotKi = 0.667;
  double rotKd = 5;

  volatile double rotSum = 0;

  while(fabs(distError) > accuracy){
    // did this late at night but this while is important 
    // fabs = absolute value
    x1 = GPS.xPosition(mm), y1 = GPS.yPosition(mm);
    rotError = degToTarget(x1, y1, x2, y2, Reverse);
    distError = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    distSum = distSum * 0.995 + distError;
    rotSum = rotSum * 0.995 + rotError;
    speed = distKp * distError + distKi * distSum / t + distKd * (distError - oldDistError);
    rotSpeed = rotKp * rotError + rotKi * rotSum / t + rotKd * (rotError - oldRotError);
    drive(speed + rotSpeed, speed - rotSpeed, 10);
    oldDistError = distError;
    oldRotError = rotError;
  }
  brakeDrive();
}


// This auton is the skills auton, not tested
void auton() {
  backHook.set(false); // bring up back hook to prevent problems
  picasso.set(false); // un-picasso nothing
	Claw(true); // open claw

  while (Gyro.isCalibrating() || GPS.isCalibrating()) { // dont start until gyro and GPS are calibrated
    wait(10, msec);
  }

  Gyro.setRotation(GPS.rotation(degrees), degrees);

	/*
	FULL STRAT: (POV: BLUE SIDE)
	1.  GET LEFT RED WITH AMOGUS
	2.  GET RIGHT RED WITH LIFT1
	3.  SHOVE RIGHT YELLOW TO RED SIDE
	4.	PLATFORM RIGHT RED ON MIDDLE
	5. 	DROP LEFT RED WITH AMOGUS
	6. 	GET RIGHT YELLOW
	7. 	PICASSO RIGHT BLUE
	8.  PLATFORM RIGHT YELLOW ON RIGHT
	9. 	GET & PLATFORM LEFT RED ON LEFT
	10.	GET LEFT BLUE WITH AMOGUS
	11. GET & PLATFORM LEFT YELLOW NEAR MIDDLE 
	12. GET CENTER YELLOW WITH LIFT1
	13. GO TO BLUE SIDE & ALIGN TO PARK
	14. LOWER BLUE PLATFORM
	15. PARK
	16. (optional) PRAY THAT IT WORKS
	*/

  // FIRST ALLIANCE (dont picasso)
  brakeDrive(); // set motors to brake
  mogoTime(-100, 600, false); // lower amogus for 750 msec
	liftTime(-25, 150, false); // lower lift just because it might not be all the way down but not too fast bc we dont want to break it; completes the 750 msec wait
  mmDrive(-17 * INCHES); // scoop up mogo
  mogoDeg(45.0, 375);  // takes 750 for lift to move by 90°, so it should to half that to move 45°.
  driveTo(-1.5, -2.5);
  // OTHER ALLIANCE GOAL
  driveTo(-1.5, -1.5, true);
  driveTo(2, -1.5);
  Claw(false);
  // SHOVE FIRST YELLOW TO THE OTHER SIDE
  liftDeg(90.0, 20);
  driveTo(1.5, -1.5, true);
  driveTo(1.5, 1);
  // PLATFORM FIRST ALLIANCE GOAL ON MIDDLE (1st on platform)
  driveTo(1.5, 0.5, true); // back up
  // driveTo(0.5, 0.5); // ADD BACK IF RINGS GET IN THE WAY
  driveTo(0, 1.5); 
  driveTo(0, 1.75); // go into platform
  Claw(true); // drop mogo
  liftDeg(-95.0, 20); // start lowering lift
	mogoDeg(-50, 0); // start lowering the mogo in the back lift
  mmDrive(-150); // back up
  // GET FIRST YELLOW
  driveTo(1.25, 1.5);
  Claw(false);
	// PICASSO RIGHT FAR ALLIANCE GOAL
	driveTo(1.25, 2, true);
	liftDeg(90, 0); // start raising lift
	mogoDeg(100, 375);
	driveTo(1.25, 1.5); // back up
	picasso.set(true);
  // PLATFORM FIRST YELLOW ON RIGHT (2nd on platform)
	driveTo(1 / 3, 1.5);
	driveTo(1 / 3, 1.25); // go into platform
	Claw(true);
	liftDeg(-95, 20); // start lowering lift
	mmDrive(-150); // back up
  // GET FIRST ALLIANCE (WHICH WAS DROPPED PRIOR)
	pointAt(-1 / 3, 1.5); // turn so that lift doesn't hit platform while we wait.
	wait(250, msec); // pause bc you probably need to
	driveTo(-1 / 3, 1.5); 
  Claw(false);
  liftDeg(90.0, 250);
  // PLATFORM FIRST ALLIANCE ON LEFT (3rd on platform)
	driveTo(-1 / 3, 1.75);
	Claw(true); // drop it. good boi.
	liftDeg(-95, 20); // start lowering lift
	mmDrive(-150); // back up
	// GET LAST ALLIANCE WITH AMOGUS
	driveTo(-2, 1.5, true); // go backwards to mogo
	mogoDeg(50, 375); // raise by 50 degrees (mogo is already lowered)
  // GET LEFT YELLOW
	driveTo(5 / 3, 0.5);
	Claw(false)
	liftDeg(90.0, 20);
  // PLATFORM LEFT YELLOW NEAR MID
  driveTo(-1 / 6, 1.5);
	driveTo(-1 / 6, 1.75); // go into platform
	Claw(true);
	liftDeg(-95, 20); // start lowering lift
	mmDrive(-150); // back up
  // GET TALL GOAL
	driveTo(-2 / 45, 0.4); // dont to too close but also not too far
	Claw(false); // grab it.
	liftDeg(90, 20); // start raising the lift.
  // ALIGN TO PARK
	driveTo(-2, -2.5); // dont hit the platform.
	driveTo(-4 / 3, -2.5);
  liftDeg(-95, 3000); // bring down the platform. wait till it's done
	lift.setStopping(coast); // allow lift to get shoved a bit up.
  // PARK
  mmDrive(892); // goes to about the middle of the platform... I think
  balance(); // just in case. I hope this works.
}

//driver controls,dont change unless your jaehoon or sean
void driver() {
  // User control code here, inside the loop
  //2 joy sticks
  //rstick is axis 2 and lstick is axis 3,why its 2,3 and not 1,2 idk ask vex
  coastDrive(); // set drive motors to coast

  while (Gyro.isCalibrating() || GPS.isCalibrating()) { // dont start until gyro is calibrated
    wait(10, msec);
  }

  Gyro.setRotation(0, degrees);

  while (true) {
		int rstick=Controller1.Axis2.position();
		int lstick=Controller1.Axis3.position();
		drive(lstick, rstick,10);
		int8_t tmp;

		// mogo control
		tmp = 100 * (Controller1.ButtonR1.pressing() - Controller1.ButtonR2.pressing());
		if (tmp == 0) {
			amogus.stop(hold);
		}
		else {
			amogus.spin(forward, tmp, percent);
		}
		// lift control
		tmp = 100 * (Controller1.ButtonL1.pressing() - Controller1.ButtonL2.pressing());
		if (tmp == 0) {
			lift1.stop(hold);
		}
		else {
			lift1.spin(forward, tmp, percent);
		}

		if (Controller1.ButtonY.pressing()) { // hook down
			backHook.set(false);
		}
		else if (Controller1.ButtonB.pressing()) { // hook up or maybe the opposite 
			backHook.set(true);
		}

		if (Controller1.ButtonX.pressing()) { // claw close
			claw.set(false);
		}
		else if (Controller1.ButtonA.pressing()) { //claw open
			claw.set(true);
		}

		if (Controller1.ButtonUp.pressing()) { // picasso
			picasso.set(false);
		}
		else if (Controller1.ButtonRight.pressing()) { // un-picasso
			picasso.set(true);
		}

    if (Controller1.ButtonDown.pressing()) {
      driveTo(0, 0);
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
