using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor leftDrive1;
extern motor leftDrive2;
extern motor leftmiddle;
extern motor rightDrive1;
extern motor rightDrive2;
extern motor rightmiddle;
extern motor amogus;
extern motor lift1;
extern digital_out backHook;
extern digital_out claw;
extern inertial Inertial21;
extern digital_out picasso;
extern inertial Gyro;
extern gps GPS;
extern distance DistFront;
extern distance DistBack;
extern distance DistClaw;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );