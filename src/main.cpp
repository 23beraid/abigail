/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\26hovgra                                         */
/*    Created:      Tue Dec 20 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Flywheel             motor         8               
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

controller Controller1 = controller(primary);
brain  Brain;

motor Flywheel = motor(PORT8, ratio6_1, true);
motor intake = motor(PORT10, ratio6_1, false);
motor RightDrive1 = motor(PORT11, ratio6_1, false);
motor RightDrive2 = motor(PORT14, ratio6_1, false);
motor RightDrive3 = motor(PORT13, ratio6_1, false);
motor_group RightDrive = motor_group(RightDrive1, RightDrive2, RightDrive3);
motor LeftDrive1 = motor(PORT16, ratio6_1, true);
motor LeftDrive2 = motor(PORT17, ratio6_1, true);
motor LeftDrive3 = motor(PORT18, ratio6_1, true);
motor_group LeftDrive = motor_group(LeftDrive1, LeftDrive2,LeftDrive3);
digital_out Expansion = digital_out(Brain.ThreeWirePort.G);
digital_out Expansion2 = digital_out(Brain.ThreeWirePort.F);
digital_out Indexer = digital_out(Brain.ThreeWirePort.H);


bool intakestate = false;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;
bool drivestate = false;

int rc_auto_loop_function_Controller1();



void toggleintake(){
   if (intakestate){
      intake.stop();
      intakestate = false;
    }else{
        intake.spin(forward);
        intake.setVelocity(100, percent);
        intakestate = true;
      }
}


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
  Brain.Screen.print(color::cyan);

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
  RightDrive.setVelocity(100, percent);
  LeftDrive.setVelocity(100, percent);
RightDrive.rotateFor(forward, 42, rotationUnits::rev) && LeftDrive.rotateFor(forward, 42, rotationUnits::rev);
wait(1, sec);
intake.rotateFor(forward, 5, rotationUnits::raw);

 // Flywheel.spin(forward);
 // Flywheel.setVelocity(100, percent);
 //wait(10, seconds);
 //Flywheel.stop();
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
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
 
  Controller1.ButtonA.pressed(toggleintake);
  Brain.Screen.clearScreen(color::blue);
  // User control code here, inside the loop
  while (1) {
    int drivetrainLeftSideSpeed = Controller1.Axis3.position();
      int drivetrainRightSideSpeed = Controller1.Axis2.position();
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) {
          // stop the left drive motor
          LeftDrive.stop();
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
          RightDrive.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        LeftDrive.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDrive.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        RightDrive.setVelocity(drivetrainRightSideSpeed, percent);
        RightDrive.spin(forward);
      }
  
    if(Controller1.ButtonR2.pressing()){
      Flywheel.spin(forward);
      Flywheel.setVelocity(100.0, percent);
    }else{
      Flywheel.stop();
    }
    if(Controller1.ButtonR1.pressing()){
      Indexer = true;

    }else{
      Indexer = false;
    }if(Controller1.ButtonUp.pressing()){
      Expansion = true;

    }else{
      Expansion = false;
    }if(Controller1.ButtonUp.pressing()){
      Expansion2 = true;

    }else{
      Expansion2 = false;
    }
    
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

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
  while (true) {
    wait(100, msec);
  }
}
