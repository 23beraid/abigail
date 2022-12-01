/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Clawbot Competition Template                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
             
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor LeftDriveSmart = motor(PORT10, ratio18_1, false);//false);
motor RightDriveSmart = motor(PORT20, ratio18_1, true); // true);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 1.016, 12.953999999999999, mm, 1);
controller Controller1 = controller(primary);
motor Motor1 = motor(PORT1, ratio18_1, false);
motor MotorGroup4MotorA = motor(PORT4, ratio18_1, false);
motor MotorGroup4MotorB = motor(PORT7, ratio18_1, true);
motor_group MotorGroup4 = motor_group(MotorGroup4MotorA, MotorGroup4MotorB);
motor Catapult = motor(PORT8, ratio36_1, true);
motor RollerDrive = motor(PORT6, ratio18_1, true);
// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool Controller1LeftShoulderControlMotorsStopped = true;
bool Controller1RightShoulderControlMotorsStopped = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;
bool stopCatapult = true;

// define a task that will handle monitoring inputs from Controller1

  // process the controller input every 20 milliseconds
  // update the motors based on the input values
using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  const int t = 1000;
    wait(t, msec);
    RightDriveSmart.spin(forward);
    LeftDriveSmart.spin(reverse);
    wait(2000, msec);
    RightDriveSmart.stop();
    LeftDriveSmart.stop();
    Catapult.spin(reverse);
    wait(2000, msec);
    Catapult.stop();
    LeftDriveSmart.spin(forward);
    RightDriveSmart.spin(reverse);
    wait(2000, msec);
    LeftDriveSmart.stop();
    RightDriveSmart.stop();
    Catapult.spin(forward);
    wait(4000, msec);
    Catapult.stop();
    wait(3*t, msec);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
    if(RemoteControlCodeEnabled) {
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3
      // right = Axis2
      int drivetrainLeftSideSpeed = Controller1.Axis3.position();
      int drivetrainRightSideSpeed = Controller1.Axis2.position();
      //int RollerDrive = Controller1.ButtonUp.pressing();
    
      if (Controller1.ButtonDown.pressing()){
      
        RollerDrive.spin(reverse);
        }
      else if (Controller1.ButtonUp.pressing()){

        RollerDrive.spin(forward);


          }
      // check if the value is inside of the deadband range
       if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
         if (DrivetrainLNeedsToBeStopped_Controller1) {
           // stop the left drive motor
           LeftDriveSmart.stop();
           // tell the code that the left motor has been stopped
           DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
      // check the ButtonL1/ButtonL2 status to control Motor1
      if (Controller1.ButtonL1.pressing()) {
        Motor1.spin(reverse);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonR1.pressing()) {
        Motor1.spin(forward);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (!Controller1LeftShoulderControlMotorsStopped) {
        Motor1.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1LeftShoulderControlMotorsStopped = true;
      }
      // check the ButtonR1/ButtonR2 status to control MotorGroup4
      if (Controller1.ButtonR2.pressing()) {
        MotorGroup4.spin(forward);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonR2.pressing()) {
        MotorGroup4.spin(forward);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (!Controller1RightShoulderControlMotorsStopped) {
        MotorGroup4.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }

      if (Controller1.ButtonR1.pressing()) {
        Catapult.spin(reverse);
        stopCatapult = false;
      } else if (Controller1.ButtonB.pressing()) {
        Catapult.spin(forward);
        stopCatapult = false;
      } else if (!stopCatapult) {
        Catapult.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        stopCatapult = true;
      }
    }
    // wait before repeating the process
  
  }
  
  


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) ; 
  
}

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  


//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
 
  }
