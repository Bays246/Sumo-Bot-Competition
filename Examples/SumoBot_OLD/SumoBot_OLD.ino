/*
 * LCD Display is 8x2.
 * Paused is not a state, it is in the lowest scope of the loop function.
 * Utilize stateTimeMillis for delays, otherwise button presses don't register well.
 * Zumo bot utilizes ATmega32U4 AVR microcontroller.
 */

// ----------------------------------------------DEFINITIONS-----------------------------------------------------------------------

#include <Wire.h>
#include <Zumo32U4.h>

// Interface with the hardware attached to the Zumo bot.
Zumo32U4LCD display; // Interfaces with the display. Alternate class is Zumo32U4OLED.
Zumo32U4ButtonA buttonA; // Interfaces with button A on the bot.
Zumo32U4ButtonB buttonB; // Interfaces with button B on the bot.
Zumo32U4ButtonC buttonC; // Interfaces with button C on the bot.
Zumo32U4Buzzer buzzer; // Interfaces with buzzer located behind the screen.
Zumo32U4Motors motors; // Interfaces with both motors on the bot.
Zumo32U4LineSensors lineSensors; // Interfaces with available line sensors.
Zumo32U4ProximitySensors proxSensors; // Interfaces available proximity sensors.
Zumo32U4IMU imu; // Interfaces with the gyroscope/magnetometer/accelerometer unit.

// Sensor Sensitivity.
#define LINE_THRESHOLD 1000 // Reference number for what is considered a white line.

// Motor Speeds.
#define SPEED_MAX 400 // Max speed is 400 due to Zumo bot itself (The class "Zumo32U4Motors" only allows up to 400).
#define SPEED_HIGH 300 // High speed, woah.
#define SPEED_LOW 100 // Low speed, wooooaaaahhhh.
#define SPEED_STOP 0 // Please do not change this from zero.

// Robot States.
enum States {
  stateMenu, // Main Menu.
  stateBatteryRead, // Read battery volts.
  stateMotorTest, // Drive in a circle.
  stateSumo, // SUMO BOTS.
  stateError
};

// State Switching.
States currentState = stateMenu; // Default state when just powered on.
States lastState = stateMenu; // Used to know what to jump back to when unpausing.
States nextState = stateMenu; // Used when the rest of the state code needs to execute before switching states.
String errorCode = " NoCode "; // Typically used for messages to be displayed when switching states.
unsigned long stateTimeMillis = 0; // Used for delays inside states.

// Substates.
byte subState = 0; // Used by some states that need multiple substates, also can be used to signify the state just changed. Changing state resets this to zero. 256 possible substates. If used bitwise than can be a bunch of flags.
byte lastSubState = 0; // For no particular reason yet.

// LCD String Containers (Used for pausing).
String currentTopLCD, currentBottomLCD;

// ----------------------------------------------PRECONFIG-------------------------------------------------------------------------

void setup() {
  // Initilize motors.
  motors.flipLeftMotor(true); // I needed to flip the direction of the left motor.
  motors.flipRightMotor(false); // Just here for my OCD.
  
  // Initilize basic sensors.
  lineSensors.initThreeSensors(); // Using 3 line sensors.
  proxSensors.initThreeSensors(); // Using 3 prox sensors.

  // Initilize IMU.
  Wire.begin();
  imu.init();
  imu.enableDefault();
}

// ----------------------------------------------RUNNING LOOP----------------------------------------------------------------------

void loop() {
  // Button Capture.
  bool aButtonPressed = buttonA.getSingleDebouncedPress();
  bool bButtonPressed = buttonB.getSingleDebouncedPress();
  bool cButtonPressed = buttonC.getSingleDebouncedPress();
  
  // Robot is basically a state machine.
  switch(currentState){
    case stateMenu:{
      /*
       * The main menu to select the desired behaviour of the zumo bot.
       * A to select the current item, B to switch to the next item.
       */

      // Display something based on menu item, odd states are used so display is only updated once.
      switch(subState){
        case 0:{

          nextState = stateSumo;
          updateDisplay("Press A "," To SUMO");
          subState++;
        
          break;
        }
        case 2:{

          nextState = stateBatteryRead;
          updateDisplay("Battery "," Voltage");
          subState++;
          
          break;
        }
        case 4:{

          nextState = stateMotorTest;
          updateDisplay(" Motor", "   Test");
          subState++;
          
          break;
        }
        case 6:{

          nextState = stateError;
          updateDisplay("ErrorSim");
          subState++;
          
          break;
        }
        case 8:{

          // Loop back to begginning.
          subState = 0;
          
          break;
        }
      }

      // Menu Controlls.
      if(aButtonPressed) changeState(nextState); // nextState is automatically changed to the first in the list.
      if(bButtonPressed) subState++; // Scroll to next menu option.
      
      break;
    }
    case stateBatteryRead:{
      /*
       * Reads battery voltage and displays every half second.
       * A goes back to menu.
       */

      // Every half second.
      if(millis() - stateTimeMillis >= 500){

        // This takes up 10% of programming space (float -> String conversion is what does it).
        //updateDisplay(" "+String((float)readBatteryMillivolts()*0.001, 3), "  Volts ");

        // Building String. This takes up 2% of programming space.
        String voltString = String(readBatteryMillivolts()); // Always 4 characters.
        char strToPrint[9] = {' ',voltString[0],'.',voltString[1],voltString[2],voltString[3],' ',' '};

        // Printing String.
        updateDisplay(strToPrint, "  Volts ");

        // Reset Timer.
        stateTimeMillis = millis();
      }

      if(aButtonPressed) changeState(stateMenu);
      
      break;
    }
    case stateMotorTest:{
      /*
       * The robot will run in a circle.
       * A starts/stops the zumo bot.
       * B selects the pattern.
       */

      // Break subState into parts.
      bool flagMotorsUpdate = subState & 0b10000000;
      bool flagDisplayUpdate = subState & 0b01000000;
      byte subStateNumber = subState & 0b00111111;

      // subStateNumber sets testing pattern.
      switch(subStateNumber){
        case 0:{
          
          if(flagDisplayUpdate) updateDisplay("Forward", " High");
          if(flagMotorsUpdate) motors.setSpeeds(SPEED_HIGH, SPEED_HIGH);
          subStateNumber++;
          
          break;
        }
        case 1:{

          if(flagDisplayUpdate) updateDisplay("Forward", " Low");
          if(flagMotorsUpdate) motors.setSpeeds(SPEED_LOW, SPEED_LOW);
          subStateNumber++;
          
          break;
        }
        case 2:{

          if(flagDisplayUpdate) updateDisplay("Spin In", " Place R");
          if(flagMotorsUpdate) motors.setSpeeds(SPEED_LOW, -SPEED_LOW);
          subStateNumber++;
          
          break;
        }
        case 3:{

          if(flagDisplayUpdate) updateDisplay("Spin In", " Place L");
          if(flagMotorsUpdate) motors.setSpeeds(-SPEED_LOW, SPEED_LOW);
          subStateNumber++;
          
          break;
        }
        default:{

          // Back to the beginning.
          subStateNumber = 0;
          
          break;
        }
      }

      // Save updated parts of subState.
      subState = subStateNumber;
      if(flagMotorsUpdate) subState |= 0b10000000;
      if(flagDisplayUpdate) subState |= 0b01000000;
      
      break;
    }
    case stateError:{
      /*
       * The error state stops all movement and buzzing, and displays an error code.
       * There is no way out of this state besides resetting.
       */
       
      // Stop moving.
      motors.setSpeeds(SPEED_STOP, SPEED_STOP);

      // Stop buzzing.
      buzzer.stopPlaying();

      // Display based on delay, odd states default to waiting three seconds then continuing.
      switch(subState){
        case 0:{

          // Display Update 1
          updateDisplay(" ERROR: ", errorCode);
          subState++;
          
          break;
        }
        case 2:{

          // Display Update 2
          updateDisplay(" Please ", "Reset Me");
          subState++;
          
          break;
        }
        case 4:{

          // Reset to first substate.
          subState = 0;

          break;
        }
        default:{

          // After 3.5 seconds: reset timer, goto next state.
          if(millis() - stateTimeMillis >= 3500){
            stateTimeMillis = millis();
            subState++;
          }
          
          break;
        }
      }
      break;
    }
    default:{
      /*
       * You should not be here.
       */
    
      // Banished to the error realm.
      changeState(stateError, "InvState");
      
      break;
    }
  }
  
  //Pausing Feature
  if(cButtonPressed){

    // Save current time.
    unsigned long pauseTimeMillis = millis();

    // Save current display.
    String lastTopLCD = currentTopLCD;
    String lastBottomLCD = currentBottomLCD;

    // TODO: Save current movement. (Zumo's setLeftSpeed uses OCR1B and setRightSpeed uses OCR1A, DIR_L 16, DIR_R 15)
    //int leftMotorSpeed = OCR1B;
    //int rightMotorSpeed = OCR1A;

    // Update display.
    updateDisplay(" Paused ");

    // TODO: Pause blinking. (If I ever do it)
    
    // Make sure button is not still pressed (exits prematurely if this is not here).
    buttonC.waitForRelease();

    // Wait for button press.
    buttonC.waitForButton();

    // Restore display
    updateDisplay(lastTopLCD, lastBottomLCD);

    // TODO: Restore Movement.

    // Restore time (by adding the difference to the original time).
    stateTimeMillis += millis() - pauseTimeMillis;
  }
}

// ----------------------------------------------FUNCTIONS-------------------------------------------------------------------------

// Function for changing the current state and cleaning up after the last one.
void changeState(States newState){

  // For... some reason.
  lastState = currentState;
  lastSubState = subState;
  
  // Change the currentState
  currentState = newState;
  
  // Evey state starts at subState 0.
  subState = 0;

  // Clean up stuff.
  ledRed(0);
  ledYellow(0);
  ledGreen(0);
  display.clear();

  // Used for delays.
  stateTimeMillis = millis();
}

// Overload, allows setting the errorCode with setting the state.
void changeState(States newState, String newErrorCode){
  errorCode = newErrorCode;
  changeState(newState);
}

// Clears and prints on the LCD display.
void updateDisplay(String topLine){
  
  // Update Display.
  display.clear();
  display.print(topLine);
  
  // For pausing purposes.
  currentTopLCD = topLine;
  currentBottomLCD = "";
}

// Overload, prints on both the top and bottom lines of the LCD display.
void updateDisplay(String topLine, String bottomLine){

  // Update Display.
  display.clear();
  display.print(topLine);
  display.gotoXY(0, 1);
  display.print(bottomLine);

  // For pausing purposes.
  currentTopLCD = topLine;
  currentBottomLCD = bottomLine;
}

// Displays and sounds a countdown, so that a person can set the robot down.
void audibleCountdown(){
  // TODO:
}
