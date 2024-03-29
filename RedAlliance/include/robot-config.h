using namespace vex;

using signature = vision::signature;

extern brain Brain;

// VEXcode devices
extern signature Vision5__BLUEBOX;
extern signature Vision5__SIG_4;
extern signature Vision5__SIG_5;
extern signature Vision5__SIG_6;
extern signature Vision5__SIG_7;

extern competition Competition;
extern drivetrain Drivetrain;
extern controller Controller1;
extern motor Flywheel;
extern motor intake;
extern motor RightDrive1;
extern motor RightDrive2;
extern motor RightDrive3;
extern motor_group RightDrive;
extern motor LeftDrive1;
extern motor LeftDrive2;
extern motor LeftDrive3;
extern motor_group LeftDrive;
extern digital_out Solenoid;
extern digital_out Expansion;
extern digital_out Expansion2;
extern vision DiscSensor;
extern vision Vision5;
/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );