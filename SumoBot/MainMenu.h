/*
 * The main menu to select the desired behaviour of the zumo bot.
 * A to select the current item, B to switch to the next item.
 */

// Variables.
mainStates nextState = stateSumo;

// Code to execute.
void mainMenu(){

  // Display something based on menu item.
  switch(nextState){
    case stateSumo:{

      if(displayFlag) updateDisplay("Press A "," To SUMO");
      break;
    }
    case stateMusicTest:{
      
      if(displayFlag) updateDisplay(" Music");
      break;
    }
    case stateBatteryTest:{

      if(displayFlag) updateDisplay("Battery"," Voltage");
      break;
    }
    case stateMotorTest:{

      if(displayFlag) updateDisplay(" Motor","  Tests");
      break;
    }
    case stateSensorTest:{

      if(displayFlag) updateDisplay(" Sensor","  Tests");
      break;
    }
    default:{

      // Loop back to begginning, by default enum starts at 0 and I want to skip stateMainMenu.
      nextState = 1;
      break;
    }
  }

  // Menu Controls.
  if(aButtonPressed){mainState = nextState; displayFlag = true; delayTimeMillis = millis();} // Trasition to next state.
  if(bButtonPressed){nextState = (int)nextState + 1; displayFlag=true;} // Scroll to next menu option.
}
