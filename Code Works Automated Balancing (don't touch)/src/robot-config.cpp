#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor leftDrive1 = motor(PORT7, ratio18_1, true);
motor leftDrive2 = motor(PORT3, ratio18_1, true);
motor leftmiddle = motor(PORT5, ratio18_1, true);
motor rightDrive1 = motor(PORT6, ratio18_1, false);
motor rightDrive2 = motor(PORT2, ratio18_1, false);
motor rightmiddle = motor(PORT4, ratio18_1, false);
motor amogus = motor(PORT18, ratio36_1, false);
motor lift1 = motor(PORT1, ratio36_1, true);
digital_out backHook = digital_out(Brain.ThreeWirePort.H);
digital_out claw = digital_out(Brain.ThreeWirePort.G);
digital_out picasso = digital_out(Brain.ThreeWirePort.A);
inertial Gyro = inertial(PORT20);

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