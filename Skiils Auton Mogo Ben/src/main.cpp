/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Benjamin and Sean                                         */
/*    Created:      sometime                                                  */
/*    Description:  7700R code 2021-2022  Skills                              */
/*               Benjamin 2-11-22                                             */
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
// DistFront            distance      15              
// DistBack             distance      16              
// DistClaw             distance      17              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <math.h>

using namespace vex;

// A global instance of competition, dont touch either
competition Competition;

// define your global Variables here
std::string str = "";
const long double pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825; // much more accurate than 3.14. accurate enough to go across the universe and be within an atom of error

#define Diameter 3.25
#define UNITSIZE 23.75 // tile size
#define MOGO_DIST 5
#define NOTE str = 

// for red comments

void pre_auton(void) {
  vexcodeInit();
  Gyro.calibrate();
  GPS.calibrate();
  backHook.set(false);
  //picasso.set(false);
	claw.set(false);
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

double mod(double a, double b) {
  return (a < 0) * b + a - b * (floor(a / b) + (a < 0));
}

// get the angle at which the robot needs to turn to point towards point (x,y)
double degToTarget(double x1, double y1, double x2, double y2, bool Reverse = false, double facing = Gyro.rotation(degrees)) { 
	// First formula then second formula until the % operator so we dont have to use this formula multiple times.
	// If Reverse then we'd be adding 180 then subtracting it again at the start of formula 2. This avoids that.
  return mod(atan2(x2 - x1, y2 - y1) * 180 / pi - mod(facing, 360) - 180 * !Reverse, 360) - 180;
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
  rightDrive2.stop(coast); //put notes here with examples 
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
  }                                            //more math 
}

void liftTo(double angle, int8_t WT = -1, int8_t speed = 100) {
  lift1.setVelocity(speed, percent);
  lift1.setStopping(hold);

  lift1.spinFor(forward, 9 * angle - lift1.position(degrees), degrees, WT == -1);
  
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

void mogoTo(double angle, int WT = -1, int speed = 100) {
  amogus.setVelocity(speed, percent);
  amogus.setStopping(hold);

  amogus.spinFor(forward, 5 * angle - amogus.position(degrees), degrees, WT == -1);
  
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
  //wait(50 * open, msec);
  claw.set(open);
  //wait(50 * !open, msec);
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

void unitDrive(double target, bool endClaw = false, double clawDist = 1, uint32_t maxTime = 4294967295, double accuracy = 0.25) {
	double Kp = 10; // was previously 50/3
	double Ki = 2; // to increase speed if its taking too long.
	double Kd = 20; // was previously 40/3
	double decay = 0.5; // integral decay
	
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
  uint32_t startTime = vex::timer::system();
	 
  while((fabs(error) > accuracy || fabs(speed) > 10) && vex::timer::system() - startTime < maxTime) {
    // did this late at night but this while is important 
    error = target - minRots() * Diameter * pi; //the error gets smaller when u reach ur target
    sum = sum * decay + error;
    speed = Kp * error + Ki * sum + Kd * (error - olderror); // big error go fast slow error go slow 
    drive(speed, speed, 10);
    olderror = error;
    if (endClaw && claw.value() && (/*DistClaw.objectDistance(inches) < 2 ||*/ error <= clawDist)) { // close claw b4 it goes backwards.
	    Claw(false);
    }
  }
	brakeDrive();
  if (endClaw && claw.value()) {
    Claw(false);
  }
}

//if gyro needs calibrating add a 10ms wait or something, gyro cal takes about 1.5 sec
//1 sec if your good

int8_t sgn(double x) {
	return (x > 0 ? 1 : x < 0 ? -1 : 0);
}

void balance() { // WIP
  Brain.Screen.clearScreen();
  double Kp = 1.2;
  volatile double speed;
  volatile double pitch = Gyro.pitch(degrees);
  volatile double oldpitch = pitch;

  double stopAng = 20; // stop when fabs(pitch) is at most 5° and when its no longer tipping back and forth.
  while (true) { //(fabs(pitch) > stopAng || fabs(pitch - oldpitch) > 0.005) {
  	pitch = Gyro.pitch(degrees);
  	speed = Kp * pitch;
    bool sameDir = sgn(pitch) == sgn(oldpitch);
		if (fabs(pitch) > stopAng || (sameDir && sgn(pitch - oldpitch) == sgn(pitch) && fabs(pitch) > 3)) { // ensure that pitch, oldpitch, and pitch - oldpitch are both + or both - to make sure it only climbs if its gaining pitch
			drive(speed, speed, 30);
		}
		else if (fabs(pitch) > 12 && sameDir) {
			drive(-speed / 2, -speed / 2, 30); // Dont back up too fast. This is basically 6% speed for like 1 second
		}
		else {
			brakeDrive();
			wait(30, msec);
  		oldpitch = pitch;
		}
	}

	Brain.Screen.printAt(1, 150, "i am done ");
}


void gyroturn(double target, double accuracy = 1) { // idk maybe turns the robot with the gyro,so dont use the drive function use the gyro
  double Kp = 1.1;
  double Ki = 0.2;
  double Kd = 1.25;
	double decay = 0.5; // integral decay
	
  volatile double sum = 0;

  volatile double speed;
  volatile double error = target;
  volatile double olderror = error;

  target += Gyro.rotation(degrees);

  while(fabs(error) > accuracy || fabs(speed) > 1) { //fabs = absolute value while loop again
    error = target - Gyro.rotation(degrees);; //error gets smaller closer you get,robot slows down
    sum = sum * decay + error; 
    speed = Kp * error + Ki * sum + Kd * (error - olderror); // big error go fast slow error go slow 
    drive(speed, -speed, 10);
    Brain.Screen.printAt(1, 60, "speed = %0.2f    degrees", speed);
    olderror = error;
  }
}

void pointAt(double x2, double y2, bool Reverse = false, double x1 = -GPS.yPosition(inches), double y1 = GPS.xPosition(inches), uint32_t maxTime = 4294967295, double accuracy = 1) { 
	// point towards targetnss 
  x2 *= UNITSIZE, y2 *= UNITSIZE;
	double target = degToTarget(x1, y1, x2, y2, Reverse, Gyro.rotation(degrees)); // I dont trust the gyro for finding the target, and i dont trst the gps with spinning

	// using old values bc they faster
	double Kp = 1.1;
	double Ki = 0.2;
	double Kd = 1.25;
	double decay = 0.5; // integral sum decay.

	volatile double sum = 0;

	volatile double speed;
	volatile double error = target;
	volatile double olderror = error;
  uint32_t startTime = vex::timer::system();
	
	target += Gyro.rotation(degrees); // I trust gyro better for turning.

	while((fabs(error) > accuracy || fabs(error - olderror) > 0.05) && vex::timer::system() - startTime < maxTime) { // fabs = absolute value while loop again
		error = target - Gyro.rotation(degrees); //error gets smaller closer you get,robot slows down
		sum = sum * decay + error;
		speed = Kp * error + Ki * sum + Kd * (error - olderror); // big error go fast slow error go slow 
		drive(speed, -speed, 10);
		olderror = error;
	}
	brakeDrive();
}
bool runningAuto = 0;
/*void printPos() {
  while (true) {
    //Controller1.Screen.clearLine();
    if (!runningAuto) {
      Controller1.Screen.setCursor(0, 0);
      Controller1.Screen.print("(%0.2f, %0.2f), %0.2f", GPS.yPosition(inches) / -UNITSIZE, GPS.xPosition(inches) / UNITSIZE, GPS.rotation(degrees) - 90, Gyro.rotation(degrees));
    }
    this_thread::sleep_for(500);
  }
}
vex::thread POS(printPos);*/
//                                                        if this runs for 4.3 billion msecs, then skills is broken, and our battery is magical v
void driveTo(double x2, double y2, bool Reverse = false, bool endClaw = false, double offset = 0, double clawDist = 6, uint32_t maxTime = 4294967295, double accuracy = 0.25) {
  // point towards target
  wait(200, msec);
	// get positional data
  double x1 = -GPS.yPosition(inches), y1 = GPS.xPosition(inches);
  pointAt(x2, y2, Reverse, x1, y1, maxTime / 2);
  x2 *= UNITSIZE, y2 *= UNITSIZE;
  x1 = -GPS.yPosition(inches), y1 = GPS.xPosition(inches);

  // go to target
  // volatile double distSpeed = 100;
  double target = (1 - Reverse * 2) * (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) - offset);
  unitDrive(target / UNITSIZE, endClaw, clawDist, maxTime, accuracy);

  /*return; // end cuz everything after this is experimental.

  // experimental

  double Kp = 10; // was previously 10
  double Ki = 2; // to increase speed if its taking too long.
  double Kd = 20; // was previously 20.0
  double decay = 0.5; // integral decay
  
  volatile double sum = 0;
          
  volatile double speed;
  volatile double error = target;
  volatile double olderror = error;

  volatile double dirError = 0;
  volatile double oldDirError = dirError;
  volatile double dirSpeed = 0;
  volatile double dirSum = 0;
  volatile double oldDir = Gyro.rotation(degrees);
  volatile double oldL = leftmiddle.position(rev) * pi * Diameter;
  volatile double oldR = rightmiddle.position(rev) * pi * Diameter;

  double dirKp = 1.1; // was previously 10
  double dirKi = 0.2; // to increase speed if its taking too long.
  double dirKd = 1.25; // was previously 20.0
  double dirDecay = 0.5; // integral decay
     
  leftDrive1.setPosition(0, rev);
  leftDrive2.setPosition(0, rev);
  leftmiddle.setPosition(0, rev);
  rightDrive1.setPosition(0, rev);
  rightDrive2.setPosition(0, rev);
  rightmiddle.setPosition(0, rev);
     
  while(fabs(error) > accuracy || fabs(speed) > 10) {
    bool overShot = fabs(degToTarget(x1, y1, x2, y2, Reverse)) > 100;
    target = -((Reverse + overShot) % 2) * sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); // the error gets smaller when u reach ur target
    dirError = degToTarget(x1, y1, x2, y2, (Reverse + overShot) % 2);
    sum = sum * decay + error;
    dirSum = dirSum * dirDecay + dirError;

    speed = Kp * error + Ki * sum + Kd * (error - olderror); // big error go fast slow error go slow 
    dirSpeed = dirKp * dirError + dirKi * dirSum + dirKd * (dirError - oldDirError);
    drive(speed + dirSpeed, speed - dirSpeed, 10);
    olderror = error;
    oldDirError = dirError;
    
    // r = (∆L + ∆R) / 2(θ - θ')
    // P' = (x + r * ( sin (θ') - sin (θ) ), y + r * ( cos (θ') - cos (θ) ) )

    // to ensure nothing is undefined and stuff

    double L = leftmiddle.position(rev) * pi * Diameter;
    double R = rightmiddle.position(rev) * pi * Diameter;
    double dir = Gyro.rotation(degrees);
    if (dir == oldDir) {
      x1 += (olderror - error) * cos(dir),
      y1 += (olderror - error) * sin(dir);
    }
    else {
      double r = (L - oldL + R - oldR) / (oldDir - dir) / 2;
      x1 += r * (sin(dir) - sin(oldDir));
      y1 += r * (cos(dir) - cos(oldDir));
    }
    oldDir = dir;
    oldL = L;
    oldR = R;

    if (endClaw && error < 0 && claw.value()) { // close claw b4 it goes backwards.
      Claw(false);
    }
  }
  brakeDrive();*/
}

void auton() {
	NOTE"            R             RRRR            RRRR  RRRRRRRRRRRRRRRRRR       RRRRRRRR       ";
	NOTE"           RRR            RRRR            RRRR  RRRRRRRRRRRRRRRRRR    RRRRRRRRRRRRRR    ";
	NOTE"          RRRRR           RRRR            RRRR         RRRR          RRRRR      RRRRR   ";
	NOTE"         RRRRRRR          RRRR            RRRR         RRRR         RRRR          RRRR  ";
	NOTE"        RRRR RRRR         RRRR            RRRR         RRRR         RRRR          RRRR  ";
	NOTE"       RRRR   RRRR        RRRR            RRRR         RRRR         RRRR          RRRR  ";
	NOTE"      RRRR     RRRR       RRRR            RRRR         RRRR         RRRR          RRRR  ";
	NOTE"     RRRRRRRRRRRRRRR      RRRR            RRRR         RRRR         RRRR          RRRR  ";
	NOTE"    RRRRRRRRRRRRRRRRR     RRRR            RRRR         RRRR         RRRR          RRRR  ";
	NOTE"   RRRR           RRRR     RRRRR        RRRRR          RRRR          RRRRR      RRRRR   ";
	NOTE"  RRRR             RRRR      RRRRRRRRRRRRRR            RRRR           RRRRRRRRRRRRRR    ";
	NOTE" RRRR               RRRR        RRRRRRRR               RRRR              RRRRRRRR       ";

  backHook.set(false); // bring up back hook to prevent problems
  picasso.set(false); // un-picasso nothing
	claw.set(true); // open claw

	//runningAuto = true;

	while (Gyro.isCalibrating() || GPS.isCalibrating()) { // dont start until gyro and GPS are calibrated
		wait(10, msec);
	}
	Gyro.setRotation(GPS.rotation(degrees), degrees);
	GPS.setRotation(GPS.rotation(degrees) - 90, degrees);

	NOTE "AUTO PLAN:";
	NOTE "START ON RED SIDE LEFT";
	/*
	  1)  PICASSO LEFT BLUE
	  2)  CLAW LEFT YELLOW
	  3)  MOGO LIFT LEFT RED 
	  4)  PLATFORM LEFT YELLOW 
	  5)  DROP LEFT RED ON RED SIDE
	  6)  SHOVE TALL YELLOW TO BLUE WITH MOGO LIFT
	  7)  CLAW + PLATFORM RIGHT YELLOW
	  8)  CLAW RIGHT BLUE
	  9)  MOGO LIFT RIGHT RED + BRING IT TO RED SIDE 
  	10) PARK ON BLUE SIDE
	*/

  // LEFT BLUE
 	brakeDrive(); // set motors to brake
  mogoTime(-100, 600, false); // lower amogus for 750 msec
  liftTime(-50, 150, false); // lower lift just because it might not be all the way down but not too fast bc we dont want to break it; completes the 750 msec wait
  amogus.setPosition(0, degrees);
  lift1.setPosition(0, degrees);
  unitDrive(-17 / UNITSIZE); // scoop up mogo
  mogoDeg(120, 375);
  unitDrive(6 / UNITSIZE);
	// LEFT YELLOW
	driveTo(-1.31,0,false,true,5); // grab it
	picasso.set(true); // picasso that mogo
	mogoDeg(-150, 0);
	liftTo(75,0);
	// LEFT RED
	driveTo(-1.4,1.6);
  driveTo(-2.4,1.7,true,false,0,0,2000);
	mogoTo(60, 375);
	// PLATFORM LEFT YELLOW
  driveTo(0.2,-1.8, false, false,0,0,3500); // first value must be experimented witH
	Claw(true); // drop it
	// SHOVE TALL MOGO TO OTHER SIDE
  unitDrive(-0.5); // back up to turn
	liftTo(-10,0); // lower lift
  driveTo(-0.425, -1.35); // drop left red
  gyroturn(-90 - mod(Gyro.rotation(degrees) - 180, 360));
  mogoDeg(-127, 400);
  driveTo(0, -1.35);
	mogoTo(90,0);
	driveTo(-0.125, 1, true);
	// RIGHT YELLOW + PLATFORM
	driveTo(1.5,0.15,false,true,-6,6); // get right yellow
	liftTo(80, 0); // raise lift
	driveTo(0.9,-2,false, false, 0, 0, 3000); // go to platform
	Claw(true); // drop it
  lift1.stop(hold);
	// RIGHT BLUE
  unitDrive(-0.5);
	liftTo(-10, 0); // lower lift
	driveTo(2.45,-1.3, false, true, 3, 3, 4000); // get it. It should not take longer than 5 seconds
  liftTo(70, 0); // raise lift. Less friction
	// RIGHT RED 
	driveTo(1.8,1.667,4000);
	mogoDeg(-130,375); // lower amogus
	driveTo(1.41, 2.63, true, false, 10, 0, 2000); // get it
	mogoTo(45, 375); // lift amogus
	unitDrive(0.5); // back up from the back so go forward.
	driveTo(1.667,-1,true); // bring to other side
	mogoDeg(-60, 375); // lower amogus to drop it
	// ALIGN FOR PARKING
  driveTo(2, 1.5, false, false, 0, 0, 3000); // get close. Next thing must be very precise
	mogoTo(90,0);
	driveTo(2, 2.7, false, false, 0, 0, 3000); // dont hit the platform.
	unitDrive(-3.5 / UNITSIZE); // use wall to align
	gyroturn(90 - mod(Gyro.rotation(degrees) - 180, 360), 1); // point STRAIGHT (I added back gyroturn just for this line xD)
 	NOTE"🅸🆂🆂🆄🅴🆂 🆂🆃🅰🆁🆃 🅷🅴🆁🅴";
	unitDrive(0.4);
  liftTo(-10, 0); // bring down the platform. wait till it's done
  uint32_t startTime = vex::timer::system();
  while (lift1.position(degrees) > 45 && vex::timer::system() - startTime < 2667) { // wait until lift is all the way down. but dont wait for too long or too short.
    wait(10, msec);
  }
	lift1.spin(forward, 0, percent); // allow lift to get shoved a bit up.
  // PARK
  unitDrive(45 / UNITSIZE); // hopefully goes to the middle
  wait(750,msec); // wait before continuing
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
      driveTo(0, -1.5);
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
