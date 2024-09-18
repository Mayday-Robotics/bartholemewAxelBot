/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       nikkasouza                                                */
/*    Created:      9/13/2024, 3:41:03 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
brain Brain;

motor leftMotor(PORT4, ratio18_1, false);
motor rightMotor(PORT2, ratio18_1, true);

motor armMotor(PORT3, ratio18_1, false);
motor beltMotor(PORT7, ratio18_1, false);

inertial imu(PORT10);

smartdrive drive = smartdrive(leftMotor, rightMotor, imu, 319.19, 320, 320, mm, 1);

controller Controller = controller(primary);
controller Partner = controller(partner);

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

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  imu.calibrate();
  return;
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

int position(controller::axis axis) { // dead zone calibration
  if (axis.position() < 5 && axis.position() > -5) {
    return 0;
  }

  return axis.position();
}

void usercontrol(void) {
  // User control code here, inside the loop

  leftMotor.setBrake(hold);
  rightMotor.setBrake(hold);
  armMotor.setBrake(hold);
  beltMotor.setBrake(hold);

  armMotor.setMaxTorque(50, percent);
  armMotor.setVelocity(50, percent);
  beltMotor.setVelocity(100, percent);

  while (1) {
    
    int axis2Pos = position(Controller.Axis2);
    rightMotor.setVelocity(axis2Pos, percent);

    int axis3Pos = position(Controller.Axis3);
    leftMotor.setVelocity(axis3Pos, percent);

    rightMotor.spin(forward);
    leftMotor.spin(forward);

    int axis2PosPartner = position(Partner.Axis2);
    armMotor.setVelocity(axis2PosPartner, percent);

    int axis3PosPartner = position(Partner.Axis3);
    beltMotor.setVelocity(axis3PosPartner, percent);

    armMotor.spin(forward);
    beltMotor.spin(forward);

    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Arm Motor Temp: %d", armMotor.temperature());
    

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
