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



vision DiscSensor (PORT5);
motor Flywheel = motor(PORT8, ratio6_1, true);
motor intake = motor(PORT10, ratio18_1, false);
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
digital_in Switch = digital_in(Brain.ThreeWirePort.E);
//I have no idea what these four lines actually do, but it won't work without them
signature Vision5__SIG_4 = signature(4, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision5__SIG_5 = signature(5, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision5__SIG_6 = signature(6, 0, 0, 0, 0, 0, 0, 2.5, 0);
signature Vision5__SIG_7 = signature(7, 0, 0, 0, 0, 0, 0, 2.5, 0);
//Define the blue scanner
signature Vision5__BLUEBOX = signature(1, -3441, -2785, -3113, 8975, 10355, 9665, 2.5, 0);
signature Vision5__REDBOX = signature(3, 8099, 8893, 8496, -1505, -949, -1227, 2.5, 0);
//call the vision sensor
vision Vision5 = vision(PORT5, 50, Vision5__BLUEBOX, Vision5__REDBOX, Vision5__SIG_4, Vision5__SIG_5, Vision5__SIG_6, Vision5__SIG_7);

event checkBlue = event();

bool intakestate = false;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;
bool drivestate = false;
float speedMultiplier = 1;
int visionCountdown = 2;

int rc_auto_loop_function_Controller1();


void ToggleDriveDirection(){
  if(drivestate){
    speedMultiplier = 1;
    drivestate = false;
  }else{
    speedMultiplier = -1;
    drivestate = true;
  }
}
void ToggleIntake(){
   if (intakestate){
      intake.stop();
      intakestate = false;
    }else{
        intake.spin(forward);
        intake.setVelocity(100, percent);
        intakestate = true;
      }
}

void hasBlueCallback(){
  Vision5.takeSnapshot(Vision5__BLUEBOX);
  if (Vision5.objectCount > 0) {
    visionCountdown = 4;
    Brain.Screen.print("Object Found");
    intake.spin(forward);
  } else {
    visionCountdown -= 1;
    if(visionCountdown < 1){
      intake.stop();
    }
    Brain.Screen.print("No Object");
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
  checkBlue(hasBlueCallback);

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
  motor_group Drive = motor_group(LeftDrive1, LeftDrive2, LeftDrive3, RightDrive1, RightDrive2, RightDrive3);
  Brain.Screen.clearScreen(color::green);
  Drive.setVelocity(25, percent);
  LeftDrive.setVelocity(25, percent);
  RightDrive.setVelocity(25, percent);
  //Drive.spinFor(reverse, 3, rotationUnits::rev, false);
  intake.setVelocity(50, percent);
  while(visionCountdown > 0){
    checkBlue.broadcastAndWait();}
  Drive.stop();
  //Drive.spinFor(forward, 0.5, rotationUnits::rev);
  RightDrive.spinFor(reverse, 1, rotationUnits::rev, false);
  LeftDrive.spinFor(forward, 1, rotationUnits::rev, true);
  //RightDrive.spinFor(forward, 3, rotationUnits::rev, true);
  //wait(1, sec);
  //Expansion = true;

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
  Controller1.ButtonA.pressed(ToggleIntake);
  Controller1.ButtonL2.pressed(ToggleDriveDirection);
  Brain.Screen.clearScreen(color::red);
  // User control code here, inside the loop
  while (1) {
      int drivetrainLeftSideSpeed = Controller1.Axis2.position() * speedMultiplier;
      int drivetrainRightSideSpeed = Controller1.Axis3.position() * speedMultiplier;
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
      Flywheel.spin(forward, 100, percentUnits::pct);
    }else{
      Flywheel.stop();
    }
    if(Controller1.ButtonR1.pressing()){
      Indexer = true;

    }else{
      Indexer = false;

    }if(Controller1.ButtonUp.pressing()){
      Expansion = true;
      Expansion2 = true;

    }else{
      Expansion = false;
      Expansion2 = false;
    }if(Controller1.ButtonL1.pressing()){
      intake.setVelocity(100, percent);
      intake.spin(reverse);

    }
    if(Controller1.ButtonB.pressing()){
      checkBlue.broadcastAndWait();
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