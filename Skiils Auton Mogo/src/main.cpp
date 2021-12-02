/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Sean and Jaehoon  and Saif                                                     */
/*    Created:      sometime                                           */
/*    Description:  7700R code 2021-2022  Skills                             */
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
// Gyro                 inertial      20              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition,dont touch either
competition Competition;

// define your global Variables here
float pi=3.14;
float Diameter = 3.25;

//dont touch
//Diameter is the wheel
//pie is pie dumbass



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
//if gyro needs calibrating add a 10ms wait or something, gyro cal takes about 1.5 sec
 
void breakdrive()
{
leftDrive1.stop(brake);
leftDrive2.stop(brake);
rightDrive1.stop(brake);
rightDrive2.stop(brake);

}
void coastdrive()
{
leftDrive1.stop(coast);
leftDrive2.stop(coast);
rightDrive1.stop(coast);
rightDrive2.stop(coast);

}
void balance()
{
 float pitch=Gyro.pitch(deg);
 float oldpitch=pitch;
 inchDrive(10, 100);
     Brain.Screen.clearScreen();
     float kp=1;
      float kd = 20.0;

float d=0.5;
while(fabs(pitch)>d)
{
  float speed = kp*pitch+kd*(pitch-oldpitch);
  drive(speed, speed, 10);
  oldpitch=pitch;
    pitch = Gyro.pitch(deg);
    Brain.Screen.printAt(1, 100, "pitch=   %.3f   ",pitch);

}
breakdrive();
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


//This auton is the skills auton,not fully tested
void auton() {
  breakdrive(); // set motors to brake
  mogo(-100,1200);
  mogo(0, 0);
  inchDrive(-17, 100);
  mogo(100, 1500);
  picasso.set(true);
  mogo(0, 0);
  inchDrive(8, 100);
  gyroturn(93);
  claw.set(true);
  inchDrive(55, 100);
  claw.set(false);
  inchDrive(-10, 100);
  gyroturn(140);
  lift(100, 1700);
  inchDrive(38,100);
  lift(0,0);
  claw.set(true);
  inchDrive(-22,100);
  gyroturn(-110);
  lift(-100, 1500);
  inchDrive(27, 100);
  claw.set(false);
  inchDrive(-10, 100);
  gyroturn(123);
  lift(100,1700);
  inchDrive(34,100);
  claw.set(true);
  lift(0, 0);
  inchDrive(-30,100);
  gyroturn(100);
  mogo(-100,1200);
  mogo(0, 0);
  inchDrive( -40, 100);
  mogo(100, 1500);
  mogo(0, 0);

  
 




  
  









}

//driver controls,dont change unless your jaehoon or sean
void driver() {
  // User control code here, inside the loop
  //2 joy sticks
  //rstick is axis 2 and lstick is axis 3,why its 2,3 and not 1,2 idk ask vex
  coastdrive(); // set drive motors to coast
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
