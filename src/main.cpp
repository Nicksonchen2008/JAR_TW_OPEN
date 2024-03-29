#include "vex.h"
#include "usercontrol.hpp"
#include "image.h"
#include "jason.h"
// #include "samson.h"
#include "autofunctions.hpp"
#include <cmath>
using namespace vex;
competition Competition;



/*---------------------------------------------------------------------------*/
/*                             VEXcode Config                                */
/*                                                                           */
/*  Before you do anything else, start by configuring your motors and        */
/*  sensors using the V5 port icon in the top right of the screen. Doing     */
/*  so will update robot-config.cpp and robot-config.h automatically, so     */
/*  you don't have to. Ensure that your motors are reversed properly. For    */
/*  the drive, spinning all motors forward should drive the robot forward.   */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                             JAR-Template Config                           */
/*                                                                           */
/*  Where all the magic happens. Follow the instructions below to input      */
/*  all the physical constants and values for your robot. You should         */
/*  already have configured your robot manually with the sidebar configurer. */
/*---------------------------------------------------------------------------*/


Drive chassis(

//Specify your drive setup below. There are seven options:
//ZERO_TRACKER_NO_ODOM, ZERO_TRACKER_ODOM, TANK_ONE_ENCODER, TANK_ONE_ROTATION, TANK_TWO_ENCODER, TANK_TWO_ROTATION, HOLONOMIC_TWO_ENCODER, and HOLONOMIC_TWO_ROTATION
//For example, if you are not using odometry, put ZERO_TRACKER_NO_ODOM below:
// ZERO_TRACKER_ODOM,
ZERO_TRACKER_NO_ODOM,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
motor_group(LF,LM,LB),

//Right Motors:
motor_group(RF,RM,RB),

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
PORT11,

//Input your wheel diameter. (4" omnis are actually closer to 4.125"):
3.25,

//External ratio, must be in decimal, in the format of input teeth/output teeth.
//If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
//If the motor drives the wheel directly, this value is 1:
0.75,

//Gyro scale, this is what your gyro reads when you spin the robot 360 degrees.
//For most cases 360 will do fine here, but this scale factor can be very helpful when precision is necessary.
353.5,

/*---------------------------------------------------------------------------*/
/*                                  PAUSE!                                   */
/*                                                                           */
/*  The rest of the drive constructor is for robots using POSITION TRACKING. */
/*  If you are not using position tracking, leave the rest of the values as  */
/*  they are.                                                                */
/*---------------------------------------------------------------------------*/

//If you are using ZERO_TRACKER_ODOM, you ONLY need to adjust the FORWARD TRACKER CENTER DISTANCE.

//FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is only necessary for holonomic drives, otherwise this section can be left alone.
//LF:      //RF:    
PORT1,     -PORT2,

//LB:      //RB: 
PORT3,     -PORT4,

//If you are using position tracking, this is the Forward Tracker port (the tracker which runs parallel to the direction of the chassis).
//If this is a rotation sensor, enter it in "PORT1" format, inputting the port below.
//If this is an encoder, enter the port as an integer. Triport A will be a "1", Triport B will be a "2", etc.
3,

//Input the Forward Tracker diameter (reverse it to make the direction switch):
2.75,

//Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
//For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
//This distance is in inches:
-2,

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
1,

//Sideways tracker diameter (reverse to make the direction switch):
-2.75,

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
5.5

);

int autonState = 0; 
// int current_auton_selection = 0;
// bool auto_started = false;

int brainimg(){
  while(1){
    Brain.Screen.drawImageFromBuffer(image, 0, 0, sizeof(image));
    wait(20,msec);
  }
}

int horvert(){
  double hangpos;
  bool ver_is_pressed = false;
  bool hor_is_pressed = false;
  bool ratchet = false;
  hangrot.resetPosition(); 
  while(1){
    hangpos = std::abs(hangrot.position(rotationUnits::rev));
    Brain.Screen.printAt(200, 120, "Position:%f\n",hangpos);
    if(Controller1.ButtonA.pressing() && hor_is_pressed == false){ //Click once, set ratchet to false, lift up
      pistonratchet.set(false);
      ratchet = false;
      while(hangpos <= 1.15){
        hangpos = std::abs(hangrot.position(rotationUnits::rev));
        cataMotor.spin(forward,-100,pct);
        Brain.Screen.printAt(200, 120, "Position:%f\n",hangpos);
        wait(20,msec);
      }
      cataMotor.stop(hold);
      hor_is_pressed = true;
    }
    else if(Controller1.ButtonA.pressing() && hor_is_pressed == true){ //Click another time, set ratchet to true, lift down
      pistonratchet.set(true);
      ratchet = true;
      while(hangpos >= 0.4){
        hangpos = std::abs(hangrot.position(rotationUnits::rev));
        cataMotor.spin(forward,100,pct);
        wait(20,msec);
      }
      cataMotor.stop(hold);
    }

    if(Controller1.ButtonB.pressing() && ver_is_pressed == false){ //Click once, set ratchet to false, lift up
      pistonratchet.set(false);
      ratchet = false;
      while(hangpos <= 2.6){
        cataMotor.spin(forward,-100,pct);
        hangpos = std::abs(hangrot.position(rotationUnits::rev));
        wait(20,msec);
      }
      cataMotor.stop(hold);
      ver_is_pressed = true;
    }
    else if(Controller1.ButtonB.pressing() && ver_is_pressed == true){ //Click another time, set ratchet to true, lift down
      ratchet = true;
      pistonratchet.set(true);
      while(hangpos >= 0.4){
        cataMotor.spin(forward,100,pct);
        hangpos = std::abs(hangrot.position(rotationUnits::rev));
        wait(20,msec);
      }
      cataMotor.stop(hold);
    }

    if (Controller1.ButtonDown.pressing()){ //Manual Lift function
      pistonratchet.set(false);
      // ratchet = false;
      cataMotor.spin(forward,100,percent);
    }
    else if (Controller1.ButtonUp.pressing()){
      pistonratchet.set(true);
      // ratchet = true;
      cataMotor.spin(forward,-100,percent);
    }
    else cataMotor.stop(hold);

    if(Controller1.ButtonX.pressing()){ // Piston Ratchet function
      waitUntil(Controller1.ButtonX.pressing() == false);
      ratchet =! ratchet;
    }
    pistonratchet.set(ratchet);
  }
    wait(20,msec);
}

int current_auton_selection = 0;
bool auto_started = false;

void pre_auton(void) {
  vexcodeInit();
  default_constants();
  // Inertial100.calibrate();
  printf("Battery: %f \n",std::round(Brain.Battery.capacity()));
  intakeMotor.setStopping(hold);
  back_wings.set(false);
  back_wings2.set(false);
  hangrot.resetPosition(); 
  hangrot.setPosition(0, rotationUnits::rev); 
  // bool limitval = false;
  Brain.Screen.clearScreen();
  while(1){
    if(Brain.Battery.capacity() > 50 && Brain.Battery.capacity() < 100){
      wait(10,msec);
    }

    // while(auto_started == false){            //Changing the names below will only change their names on the
    //   // Brain.Screen.clearScreen();            //brain screen for auton selection.
    //   switch(current_auton_selection){       //Tap the brain screen to cycle through autons.
    //     case 0:
    //       Brain.Screen.drawImageFromBuffer(image, 0, 0, sizeof(image));
    //       Brain.Screen.printAt(200, 220, "Skills");
    //       break;
    //     case 1:
    //       Brain.Screen.drawImageFromBuffer(image, 0, 0, sizeof(image));
    //       Brain.Screen.drawCircle(240, 15, 13); 
    //       Brain.Screen.drawCircle(455, 15, 13); 
    //       Brain.Screen.printAt(200, 220, "AWP");
    //       break;
    //     case 2:
    //       Brain.Screen.drawImageFromBuffer(image, 0, 0, sizeof(image));
    //       Brain.Screen.drawCircle(320, 120, 13);
    //       Brain.Screen.drawCircle(258, 120, 13); 
    //       Brain.Screen.drawCircle(240, 15, 13); 
    //       Brain.Screen.drawCircle(455, 15, 13); 
    //       Brain.Screen.printAt(200, 220, "Steal AWP");
    //       break;
    //     case 3:
    //       Brain.Screen.drawImageFromBuffer(image, 0, 0, sizeof(image));
    //       Brain.Screen.drawCircle(160, 120, 13); 
    //       Brain.Screen.drawCircle(222, 120, 13); 
    //       Brain.Screen.drawCircle(240, 15, 13); 
    //       Brain.Screen.drawCircle(222, 80, 13); 
    //       Brain.Screen.drawCircle(25, 15, 13); 
    //       Brain.Screen.printAt(200, 220, "Safe sixball");
    //       break;
    //     case 4:
    //       Brain.Screen.drawImageFromBuffer(image, 0, 0, sizeof(image));
    //       // Brain.Screen.drawRectangle(120, 20, 45, 55); 
    //       Brain.Screen.drawCircle(160, 120, 13); 
    //       Brain.Screen.drawCircle(222, 120, 13); 
    //       Brain.Screen.drawCircle(222, 80, 13); 
    //       Brain.Screen.drawCircle(25, 15, 13); 
    //       Brain.Screen.printAt(200, 220, "Fiveball");
    //       break;
    //   }
    //   if(Brain.Screen.pressing()){
    //     while(Brain.Screen.pressing()) {}
    //     current_auton_selection ++;
    //   } else if (current_auton_selection == 8){
    //     current_auton_selection = 0;
    //   }
    //   task::sleep(10);
    // }
//   }
// }
    switch(autonState){
      case 0: 
          Brain.Screen.drawImageFromBuffer(jason, 0, 0, sizeof(jason));
          // Brain.Screen.drawImageFromBuffer(jason, 244, 0, sizeof(samson));
          // Brain.Screen.drawImageFromBuffer(jason, 425, 0, sizeof(ni));
          Brain.Screen.printAt(200, 220, "Skills1");
          break;
      case 1: 
          Brain.Screen.drawImageFromBuffer(image, 0, 0, sizeof(image));
          Brain.Screen.drawCircle(240, 15, 13); 
          Brain.Screen.drawCircle(455, 15, 13); 
          Brain.Screen.printAt(200, 220, "AWP");
          break;
      case 2: 
          Brain.Screen.drawImageFromBuffer(image, 0, 0, sizeof(image));
          Brain.Screen.drawCircle(320, 120, 13); 
          Brain.Screen.drawCircle(258, 120, 13); 
          Brain.Screen.drawCircle(240, 15, 13); 
          Brain.Screen.drawCircle(455, 15, 13); 
          Brain.Screen.printAt(200, 220, "Steal AWP");
          break;
      case 3: 
          Brain.Screen.drawImageFromBuffer(image, 0, 0, sizeof(image));
          Brain.Screen.drawCircle(160, 120, 13); 
          Brain.Screen.drawCircle(222, 120, 13); 
          Brain.Screen.drawCircle(240, 15, 13);  
          Brain.Screen.drawCircle(222, 80, 13); 
          Brain.Screen.drawCircle(25, 15, 13); 
          Brain.Screen.printAt(200, 220, "Safe sixball");
          break;
      case 4: 
          Brain.Screen.drawImageFromBuffer(image, 0, 0, sizeof(image));
          // Brain.Screen.drawRectangle(120, 20, 45, 55); 
          Brain.Screen.drawCircle(160, 120, 13); 
          Brain.Screen.drawCircle(222, 120, 13); 
          Brain.Screen.drawCircle(222, 80, 13); 
          Brain.Screen.drawCircle(25, 15, 13); 
          Brain.Screen.printAt(200, 220, "Fiveball");
          break;
      case 5: 
          Brain.Screen.drawImageFromBuffer(image, 0, 0, sizeof(image));
          Brain.Screen.printAt(200, 220, "Testing");
          break;
    }
    if(limitselect.pressing()){
      while(limitselect.pressing()) {}
      autonState++; 
    if(autonState>5){
      autonState = 0; 
    }
    }
  }
}

void autonomous(void) {
  // chassis.set_heading(90);
  // intakeMotor.spin(reverse,30,percent);
  // chassis.diff(-51, -100, 1400, 300);
  // chassis.set_heading(180);
  // chassis.right_swing_to_angle(63);
  // back_wings.set(true);
  // chassis.drive_distance(-2.5);
  // intakeMotor.stop(hold);

  
  // skills3();

  auto_started = true;
    switch(current_auton_selection){  
      case 0:
      testing();
        // PID_Test();
        // skills3(); //This is the default auton, if you don't select from the brain.
        break;        //Change these to be your own auton functions in order to use the auton selector.
      case 1:         //Tap the screen to cycle through autons.
        AWP();
        break;
      case 2:
        skills4();
        break;
      case 3:
        sixball_safe();
        break;
      case 4:
        sixball();
        break;
      case 5:
        testing();
  }
  // switch(autonState){
  //   case 0: 
  //     sixball();
  //     break;
  //   case 1: 
  //     AWP();  
  //     break;
  //   case 2: 
  //     RushAWP(); 
  //     break;
  //   case 3: 
  //     sixball_safe(); 
  //     break;
  //   case 4: 
  //     skills3();
  //     break;
  //   case 5:
  //     testing();
  //     break;
  // }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*----------------------------------------------- ----------------------------*/

void usercontrol(void) {
  task Suck1task(UC_Intake);
  task Launchtask(UC_Slapper);
  task Extendtask(UC_Hang);
  task Releasetask(UC_frontwings);
  task Extask(UC_backwings);
  // task horizontaltask(UC_horizontalhang);
  // task verticaltask(UC_verticalhang);
  // task hormac(UC_horhangmac);
  // task vermac(UC_verhangmac);

  task verhormac(horvert);

  // task Endgametask(UC_distance);
  // task pistonratchettask(UC_stick);
  // task destroytask(UC_destroy);
  // task killtask(UC_destroy);
    // Brain.Screen.clearScreen();
    // // bool setup_io = false;
    // placement.setLight(ledState::on);
    // placement.brightness(100);
    // while(1){
    //   if(recoil.objectDistance(mm) < 75){
    //     cataMotor.stop(brake);
    //     kicker.stop(brake);
    //     if(placement.isNearObject()){
    //       // wait(0.3,sec);
    //       cataMotor.spin(forward,100,pct);
    //       kicker.spin(forward,100,pct); 
    //       wait(0.13,sec);
    //       cataMotor.stop(coast);
    //       kicker.stop(coast);
    //     }
    //   }    
    //   else{
    //     cataMotor.spin(forward,100,pct);
    //     kicker.spin(forward,100,pct);
    //   }
    //   wait(10,msec);
    // }
  while (1) {
    // Brain.Screen.printAt(200, 120, "Position:%f",hangrot.position(rotationUnits::rev));
    // printf("Position:%f",hangrot.position(rotationUnits::rev));
    chassis.control_arcade();
    if(Controller1.ButtonY.pressing()){
      chassis.set_drive_exit_conditions(0.3, 10, 600);
      chassis.set_swing_exit_conditions(1.2, 10, 1300);
      chassis.set_heading(90);
      intakeMotor.spin(reverse,30,percent);
      chassis.diff(-40, -80, 1400, 300);
      // chassis.set_heading(180);
      chassis.right_swing_to_angle(70.7);
      // back_wings.set(true);
      chassis.drive_distance(-7);
      intakeMotor.stop(hold);

      // cataMotor.spin(forward,68,percent);
      // kicker.spin(forward,68,percent);

      // // wait(3,sec);
      // wait(20.7,sec);
      // cataMotor.stop(brake);
      // kicker.stop(brake);

      // chassis.set_drive_exit_conditions(0.3, 10, 600);;
      // chassis.set_heading(315);
      // intakeMotor.spin(reverse,60,percent);
      // chassis.drive_distance(17);
      // chassis.turn_to_angle(62);
      // chassis.drive_distance(-2);
      // back_wings.set(true);
      // intakeMotor.stop(hold);

      
      // cataMotor.spin(forward,67,percent);
      // kicker.spin(forward,67,percent);
      // // wait(3,sec);
      // wait(21,sec);
      // cataMotor.stop(brake);
      // kicker.stop(brake);
      // back_wings.set(false);
      
    }
    // if(Controller1.ButtonUp.pressing()){
    //   setup_io = !setup_io;
    // }
    // if(setup_io==true){
    //   cataMotor.spin(forward,67,percent);
    //   kicker.spin(forward,67,percent);
    // }
    // else{
    //   cataMotor.stop(coast);
    //   kicker.stop(coast);
    // }
    // else if(setup_io==false){
    //   cataMotor.stop(coast);
    //   kicker.stop(coast);
    // }
    wait(20, msec);
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
