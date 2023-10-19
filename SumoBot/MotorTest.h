/*
 * This allows the motors of the Zumo bot to be tested 16 different ways.
 * A button cycles through speeds,
 * B button cycles through direction/pattern.
 * Remember that C is still pause.
 */

// Variables.
byte motorTestPattern;
byte motorTestSpeed;

// Code to execute.
void motorTest(){

  // General flag is used so that the default case in the switch statements don't mess things up.
  generalFlag = false;

  // Display top line and set speed.
  switch(motorTestSpeed){
    case 0:{

      if(displayFlag) topScreenBuffer = "Stopped";
      if(motorFlag){leftMotorBuffer = SPEED_STOP; rightMotorBuffer = SPEED_STOP;}
      break;
    }
    case 1:{

      if(displayFlag) topScreenBuffer = "Low";
      if(motorFlag){leftMotorBuffer = SPEED_LOW; rightMotorBuffer = SPEED_LOW;}
      break;
    }
    case 2:{

      if(displayFlag) topScreenBuffer = "High";
      if(motorFlag){leftMotorBuffer = SPEED_HIGH; rightMotorBuffer = SPEED_HIGH;}
      break;
    }
    case 3:{

      if(displayFlag) topScreenBuffer = "Max";
      if(motorFlag){leftMotorBuffer = SPEED_MAX; rightMotorBuffer = SPEED_MAX;}
      break;
    }
    default:{

      // Loop back to begginning.
      motorTestSpeed = 0;

      //Don't set anything prematurely.
      generalFlag = true;
      
      break;
    }
  }

  // Display bottom line and set direction.
  switch(motorTestPattern){
    case 0:{

      if(displayFlag) bottomScreenBuffer = "Forward";
      // Motors are already going forward.
      break;
    }
    case 1:{

      if(displayFlag) bottomScreenBuffer = "CW";
      if(motorFlag) rightMotorBuffer *= -1;
      break;
    }
    case 2:{

      if(displayFlag) bottomScreenBuffer = "CCW";
      if(motorFlag) leftMotorBuffer *= -1;
      break;
    }
    case 3:{

      if(displayFlag) bottomScreenBuffer = "Reverse";
      if(motorFlag){leftMotorBuffer *= -1; rightMotorBuffer *= -1;}
      break;
    }
    default:{

      // Loop back to begginning.
      motorTestPattern = 0;

      //Don't set anything prematurely.
      generalFlag = true;
      
      break;
    }
  }

  //Don't set anything prematurely.
  if(!generalFlag){
    // Both of these use their appropriate buffers.
    if(displayFlag) updateDisplay();
    if(motorFlag) updateMotors();
  }

  // Controls.
  if(aButtonPressed){motorTestSpeed++; displayFlag = true; motorFlag = true;} // Stopped, Low, High, Max.
  if(bButtonPressed){motorTestPattern++; displayFlag = true; motorFlag = true;} // Forward, Clockwise, CounterClockwise, Reverse.
}
