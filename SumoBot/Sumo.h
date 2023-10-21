/*
 * SUMO.
 * There was so much else I wanted to do to this. NO FRIGGEN TIME.
 */

// States.
enum sumoStates {
  sumoWaiting,
  sumoSearching,
  sumoRam,
  sumoEvadeBot,
  sumoEvadeLine,
  sumoStall
};

// Variables
sumoStates sumoCurrentState = sumoWaiting;
bool leftBias = true; // Movement is biased twards the left direction. 
//unsigned long movementChangeTime = 0; // Used to recalibrate the accelerometer so that the bot isn't set off prematurely.

byte sequentialProx = 0;

// ----------------------------------------------FUNCTIONS-------------------------------------------------------------------------

// Function for changing state in the sumo main state.
inline void sumoState(sumoStates nextState){

  // Change the state.
  sumoCurrentState = nextState;

  // Display update.
  displayFlag = true;

  // Reset timers.
  delayTimeMillis = millis();
  //delayTimeMillis2 = millis();
}

// Function for checking lines.
inline void sumoLineCheck(){

  // If any of the sensors detect a line, switch to sumoEvadeLine.
  if(lineSensorValues[0] < LINE_THRESHOLD ||
     lineSensorValues[1] < LINE_THRESHOLD || 
     lineSensorValues[2] < LINE_THRESHOLD
     ) sumoState(sumoEvadeLine);
}

// Checks if the sumo got hit in action, if so switch to sumoEvadeBot.
inline void sumoCheckHit(){

  if(abs(imuCompare[0]) > ACCEL_THRESHOLD ||
     abs(imuCompare[1]) > ACCEL_THRESHOLD
     ) sumoState(sumoEvadeBot);
}

// ----------------------------------------------MAIN------------------------------------------------------------------------------

// Code to execute.
void sumo(){

  // Here we go.
  switch(sumoCurrentState){
    case sumoWaiting:{

      // Update the display.
      display.clear();

      // Delay and beeping.
      countdownCalibrate();

      // Next state.
      sumoState(sumoSearching);

      break;
    }
    case sumoSearching:{

      // Display.
      if(displayFlag) updateDisplay(" Search ");

      // Default.
      if(leftBias){
        leftMotorBuffer = SPEED_LOW;
        rightMotorBuffer = SPEED_MED;
      }else{
        leftMotorBuffer = SPEED_MED;
        rightMotorBuffer = SPEED_LOW;
      }

      // If a bot is detected left or right.
      if(proxSensors.countsLeftWithLeftLeds() > PROX_THRESHOLD || proxSensors.countsFrontWithLeftLeds() > PROX_THRESHOLD){
        leftMotorBuffer = -SPEED_MED;
        rightMotorBuffer = SPEED_MED;
        leftBias = true;
      }
      if(proxSensors.countsRightWithRightLeds() > PROX_THRESHOLD || proxSensors.countsFrontWithRightLeds() > PROX_THRESHOLD){
        leftMotorBuffer = SPEED_MED;
        rightMotorBuffer = -SPEED_MED;
        leftBias = false;
      }

      // If a bot is detected in front.
      if(proxSensors.countsFrontWithLeftLeds() > PROX_THRESHOLD && proxSensors.countsFrontWithRightLeds() > PROX_THRESHOLD){
        sumoState(sumoRam);
      }

      // Check for areana edge.
      sumoLineCheck();

      // Check if hit.
      //sumoCheckHit();

      // Always update movement.
      motorFlag = true;

      break;
    }
    case sumoEvadeLine:{

      // Display update does double duty in setting up the initial movement.
      if(displayFlag){
        updateDisplay("EvadeLin");

        // Remember which way to go when done moving backwards.
        if(lineSensorValues[0] < LINE_THRESHOLD){
          leftMotorBuffer = SPEED_MAX;
          rightMotorBuffer = -SPEED_MAX;
          leftBias = false;
        }else{
          leftMotorBuffer = -SPEED_MAX;
          rightMotorBuffer = SPEED_MAX;
          leftBias = true;
        }

        // Remember to turn after moving backwards.
        generalFlag = true;

        // MOVE BACK NOW.
        updateMotors(-SPEED_MAX, -SPEED_MAX);
      }

      // After a time then turn.
      if(millis() - delayTimeMillis > LINE_EVADE_REVERSE_TIME && generalFlag){motorFlag = true; generalFlag = false; delayTimeMillis = millis();}

      // After a longer time return to searching state.
      if(millis() - delayTimeMillis > LINE_EVADE_TURN_TIME && !generalFlag) sumoState(sumoSearching);

      break;
    }
    case sumoRam:{

      // Display update.
      if(displayFlag) updateDisplay(" Pursue ");

      // Full speed ahead!
      leftMotorBuffer = SPEED_MAX;
      rightMotorBuffer = SPEED_MAX;

      // Check for areana edge.
      sumoLineCheck();

      // Always update motor.
      motorFlag = true;

      break;
    }
    default:{

      // You should not be here.
      // You are stuck here now.
      updateMotors(SPEED_STOP, SPEED_STOP);
      ledRed(true);

      break;
    }
  }

  // Setting Speeds.
  if(motorFlag) updateMotors();

  // Accelerometer update.
  imuAccel();

  // Line Sensor Update.
  lineSensors.read(lineSensorValues);

  // Provimity Sensor Update.
  proxSensors.read();
}
