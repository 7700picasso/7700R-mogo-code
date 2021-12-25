using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor leftDrive1;
extern motor leftDrive2;
extern motor rightDrive1;
extern motor rightDrive2;
extern motor amogus;
extern motor lift1;
extern digital_out backHook;
extern digital_out claw;
extern digital_out picasso;
extern inertial Gyro;
extern motor leftmiddle;
extern motor rightmiddle;
extern gps GPS21;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );