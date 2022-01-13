/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Benjamin                                                  */
/*    Created:      sometime                                                  */
/*    Description:  7700R code 2021-2022  Skills                              */
/*               Benjamin 1-10-22                                             */
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

// define your global Variables here
const long double pi = 3.14159265358979323846264338327950288419716939937510582097494459230; // much more accurate than 3.14
#define Diameter 3.25
// tile size
#define UNITSIZE 23.75

void pre_auton(void) {
  vexcodeInit();
  Gyro.calibrate();
  GPS.calibrate();
  wait(2000, msec);

  // All activities that occur before the competition starts
  // gets pistons down before match
  // dont touch this 
	
	// BOOOOOOP whoops i touched it
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

// get the angle at which the robot needs to turn to point towards point (x,y)
double degToTarget(double x1, double y1, double x2, double y2, bool Reverse = false, double facing = Gyro.rotation(degrees)) { 
	// First formula then second formula until the % operator so we dont have to use this formula multiple times.
	// If Reverse then we'd be adding 180 then subtracting it again at the start of formula 2. This avoids that.
  double theta = atan2(x2 - x1, y2 - y1) * 180 / pi - facing - 180 * !Reverse;
	// the rest of the angle correction formula with our modulo formula.
  return (theta < 0) * 360 + theta - 360 * (floor(theta / 360) + (theta < 0)) - 180;
	/*
	EXPLAINATION:
	Formula for some value of error when current direction is 'facing' (assume atan2 is in radians so we must convert it to degrees):
	error(facing) = atan2(x2 - x1, y2 - y1) * 180 / pi + 180 * Reverse - facing. (formula 1)
	This formula can return a value greater than 180, and we don't want the robot turning more than it needs to.
	So we covert it to an angle between -180 and 180 with the formula (which I derived myself):
	angle(theta) = (theta - 180) % 360 - 180. (formula 2)
	Unfortunately, C++ doesn't allow % operator for non-int types, so we have our own formula for a % b:
	a % b = (a < 0) * b + a - b * int(a / b).
	Idk if int(x) will work the way it needs to, so we'll make our own:
	INT(x) = floor(x) + (x < 0).
	Noting that a < 0 iff a / b < 0 for all a and b ≠ 0, our modulo formula becomes:
	a % b = (a < 0) * b + a - b * (floor(a / b) + (a < 0)). (modulo formula)
	*/
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

void liftTime(int speed, int duration, bool stop = false) {
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

void mogoTime(int speed, int duration, bool stop = false) {
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

void unitDrive(double target, bool endClaw = false, double accuracy = 0.25) {
	double Kp = 50 / 3; // was previously 10
	double Ki = 5; // to increase speed if its taking too long.
	double Kd = 40 / 3; // was previously 20.0
	double decay = 0.8; integral decay
	
	target *= UNITSIZE; // convert UNITS to inches
	
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
	 
  while(fabs(error) > accuracy) {
    // did this late at night but this while is important 
    error = target - minRots() * Diameter * pi; //the error gets smaller when u reach ur target
    sum = sum * decay + error;
    speed = Kp * error + Ki * sum + Kd * (error - olderror); // big error go fast slow error go slow 
    drive(speed, speed, 10);
    olderror = error;
    if (endClaw && error < 0 && claw.value()) { // close claw b4 it goes backwards.
	Claw(false);
    }
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
	double decay = 0.8; // integral decay
  volatile double speed;
  volatile double pitch = Gyro.pitch(degrees);
  volatile double oldpitch = pitch;

  volatile double sum = 0;

  double stopAng = 5; // stop when fabs(pitch) is at most 5° and when its no longer tipping back and forth.
  while(fabs(pitch) > stopAng || fabs(pitch - oldpitch) > 3) {
  	pitch = Gyro.pitch(degrees);
  	sum = sum * decay + pitch;
  	speed = Kp * pitch + Ki * sum + Kd * (pitch - oldpitch);
  	drive(speed, speed, 10);
  	oldpitch = pitch;
  	Brain.Screen.printAt(1, 100, "pitch=   %.3f   ",pitch);
	}
	brakeDrive();
	Brain.Screen.printAt(1, 150, "i am done ");
}

void gyroturn(double target, double accuracy = 1.25) { // idk maybe turns the robot with the gyro,so dont use the drive function use the gyro
  double Kp = 2;
  double Ki = 1;
  double Kd = 16;
	double decay = 0.8; // integral decay
	
  volatile double sum = 0;

  volatile double speed;
  volatile double error = target;
  volatile double olderror = error;

  target += Gyro.rotation(degrees);

  while(fabs(error) > accuracy) { //fabs = absolute value while loop again
    error = target - Gyro.rotation(degrees);; //error gets smaller closer you get,robot slows down
    sum = sum * decay + error; // some testing tells me that 0.8 is a good decay rate
    speed = Kp * error + Ki * sum + Kd * (error - olderror); // big error go fast slow error go slow 
    drive(speed, -speed, 10);
    Brain.Screen.printAt(1, 60, "speed = %0.2f    degrees", speed);
    olderror = error;
  }
}

void pointAt(double x2, double y2, bool Reverse = false, double accuracy = 1.25) { 
	// point towards target
  double x1 = -GPS.yPosition(inches), y1 = GPS.xPosition(inches);
  x2 *= UNITSIZE, y2 *= UNITSIZE;
	double target = degToTarget(x1, y1, x2, y2, Reverse); // I dont trust the gyro for finding the target, and i dont trst the gps with spinning
	
	// using old values bc they faster
	double Kp = 2;
	double Ki = 1;
	double Kd = 16.0;
	double decay = 0.8; // integral sum decay.

	volatile double sum = 0;

	volatile double speed;
	volatile double error = target;
	volatile double olderror = error;
	
	target += Gyro.rotation(degrees); // I trust gyro better for turning.

	while(fabs(error) > accuracy) { // fabs = absolute value while loop again
		error = target - Gyro.rotation(degrees); //error gets smaller closer you get,robot slows down
		sum = sum * decay + error;
		speed = Kp * error + Ki * sum + Kd * (error - olderror); // big error go fast slow error go slow 
		drive(speed, -speed, 10);
		olderror = error;
	}
	brakeDrive();
}
bool runningAuto = 0;
void printPos() {
  while (true) {
    //Controller1.Screen.clearLine();
    if (!runningAuto) {
      Controller1.Screen.setCursor(0, 0);
      Controller1.Screen.print("(%0.2f, %0.2f), %0.2f", GPS.yPosition(inches) / -UNITSIZE, GPS.xPosition(inches) / UNITSIZE, GPS.rotation(degrees) - 90);
    }
    this_thread::sleep_for(1000);
  }
}
vex::thread POS(printPos);


void driveTo(double x2, double y2, bool Reverse = false, bool endClaw = false, double accuracy = 0.25) {
  // point towards target
  pointAt(x2, y2, Reverse);

	// get positional data
	/*volatile*/ double x1 = -GPS.yPosition(inches), y1 = GPS.xPosition(inches);
  x2 *= UNITSIZE, y2 *= UNITSIZE;

  // go to target
  //volatile double distSpeed = 100;
  volatile double distError = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
  /*volatile double oldDistError = distError;
	double distKp = 50 / 3;
	double distKi = 5;
	double distKd = 40 / 3;
	volatile double distSum = 0;*/

  // angle correction
  /*volatile double rotSpeed; // like speed
  volatile double rotError = degToTarget(x1, y1, x2, y2, Reverse);
  volatile double oldRotError = rotError;
  double rotKp = 1 / 3 * 0;
  double rotKi = 2 / 3 * 0;
  double rotKd = 2 * 0;*/
  //volatile double rotSum = 0;
	
unitDrive((!Reverse ? 1 : -1) * distError / UNITSIZE, endClaw);
 /* while(fabs(distError) > accuracy){
    // did this late at night but this while is important 
    // fabs = absolute value
    x1 = -GPS.yPosition(inches), y1 = GPS.xPosition(inches);
    distError = (fabs(degToTarget(x1, y1, x2, y2, Reverse)) < 160 ? 1 : -1) * sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    rotError = degToTarget(x1, y1, x2, y2, (!Reverse && distSpeed > 0) || (Reverse && distSpeed < 0) ? false : true); // so that it doesn't output something like 180 if it barely goes past the target.
    distSum = distSum * 0.8 + distError;
    rotSum = rotSum * 0.8 + rotError;
    distSpeed = distKp * distError + distKi * distSum + distKd * (distError - oldDistError);
    rotSpeed = rotKp * rotError + rotKi * rotSum + rotKd * (rotError - oldRotError);
    if (endClaw && claw.value() && ((distSpeed < 0 && !Reverse) || (distSpeed > 0 && Reverse))) {
      claw.set(false);
    }
    drive(-distSpeed + rotSpeed, -distSpeed - rotSpeed, 10);
    oldDistError = distError;
    oldRotError = rotError;
  }
  brakeDrive();*/
}


// This auton is the skills auton, not tested
void auton() {
  backHook.set(false); // bring up back hook to prevent problems
  picasso.set(false); // un-picasso nothing
	Claw(true); // open claw

  //runningAuto = true;

  while (Gyro.isCalibrating() || GPS.isCalibrating()) { // dont start until gyro and GPS are calibrated
    wait(10, msec);
  }

  Gyro.setRotation(GPS.rotation(degrees) - 90, degrees);
  //printPos();
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
  amogus.setPosition(0,degrees);
  lift1.setPosition(0,degrees);
  unitDrive(-17 / UNITSIZE); // scoop up mogo
  mogoDeg(45.0, 375);  // takes 750 for lift to move by 90°, so it should to half that to move 45°.
  unitDrive(6 / UNITSIZE);
  // OTHER ALLIANCE GOAL
  gyroturn(-90);
  unitDrive(1);
  driveTo(2, -1.5, false, true); // go there and close the claw
  //Claw(false);
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
  unitDrive(-UNITSIZE / 4); // back up
  // GET FIRST YELLOW
  driveTo(1.25, 1.5, false, true); // go there and close the claw
  //Claw(false);
	// PICASSO RIGHT FAR ALLIANCE GOAL
	driveTo(1.25, 2, true);
	liftDeg(90, 0); // start raising lift
	mogoDeg(100, 375);
	driveTo(1.25, 1.5); // back up
  while (amogus.position(degrees) < 490) { // mack sure it acutally picassos
    wait(10, msec);
  }
	picasso.set(true);
  // PLATFORM FIRST YELLOW ON RIGHT (2nd on platform)
	driveTo(1 / 3, 1.5);
	driveTo(1 / 3, 1.25); // go into platform
	Claw(true);
	liftDeg(-95, 20); // start lowering lift
	unitDrive(-UNITSIZE / 4); // back up
  // GET FIRST ALLIANCE (WHICH WAS DROPPED PRIOR)
	pointAt(-1 / 3, 1.5); // turn so that lift doesn't hit platform while we wait.
	wait(250, msec); // pause bc you probably need to
	driveTo(-1 / 3, 1.5, false, true); // go there close claw
  //Claw(false);
  liftDeg(90.0, 250);
  // PLATFORM FIRST ALLIANCE ON LEFT (3rd on platform)
	driveTo(-1 / 3, 1.75);
	Claw(true); // drop it. good boi.
	liftDeg(-95, 20); // start lowering lift
	unitDrive(-UNITSIZE / 4); // back up
	// GET LAST ALLIANCE WITH AMOGUS
	driveTo(-2, 1.5, true); // go backwards to mogo
	mogoDeg(50, 0); // raise by 50 degrees (mogo is already lowered)
  while (amogus.position(degrees) < 45 * 5) { // mack sure it goes up far enough b4 it continues
    wait(10, msec);
  }
  // GET LEFT YELLOW
	driveTo(5 / 3, 0.5, false, true); // go there and close claw
	//Claw(false);
	liftDeg(90.0, 20);
  // PLATFORM LEFT YELLOW NEAR MID
  driveTo(-1 / 6, 1.5);
	driveTo(-1 / 6, 1.75); // go into platform
	Claw(true);
	liftDeg(-95, 20); // start lowering lift
	unitDrive(-UNITSIZE / 4); // back up
  // GET TALL GOAL
	driveTo(-2 / 45, 0.4, false, true); // dont to too close but also not too far. also closes claw after
	//Claw(false); // grab it.
	liftDeg(90, 20); // start raising the lift.
  // ALIGN TO PARK
	driveTo(-2, -2.5); // dont hit the platform.
	driveTo(-4 / 3, -2.5);
  liftDeg(-95, 0); // bring down the platform. wait till it's done
  while (lift1.position(degrees) > 5) { // wait until lift is all the way down. but dont wait for too long or too short.
    wait(10, msec);
  }
	lift1.setStopping(coast); // allow lift to get shoved a bit up.
  // PARK
  unitDrive(30 / UNITSIZE); // goes to about the middle of the platform... I think
  balance(); // just in case its not balanced. I hope this works.
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

  Gyro.setRotation(GPS.rotation(degrees) - 90, degrees);
  //Controller1.Screen.print("%0.3f", Gyro.rotation(deg));

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
      driveTo(1, 1);
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
