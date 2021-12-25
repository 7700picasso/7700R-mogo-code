#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor leftDrive1 = motor(PORT10, ratio6_1, true);
motor leftDrive2 = motor(PORT20, ratio6_1, true);
motor rightDrive1 = motor(PORT12, ratio6_1, false);
motor rightDrive2 = motor(PORT19, ratio6_1, false);
motor amogus = motor(PORT18, ratio36_1, false);
motor lift1 = motor(PORT1, ratio36_1, true);
digital_out backHook = digital_out(Brain.ThreeWirePort.H);
digital_out claw = digital_out(Brain.ThreeWirePort.G);
digital_out picasso = digital_out(Brain.ThreeWirePort.A);
inertial Gyro = inertial(PORT17);
motor leftmiddle = motor(PORT5, ratio18_1, true);
motor rightmiddle = motor(PORT4, ratio18_1, false);
gps GPS21 = gps(PORT21, 0.00, 0.00, mm, 180);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}