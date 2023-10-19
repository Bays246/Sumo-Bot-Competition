/*
 * B to switch between sensors.
 * When testing the accelerometer, A to re-calibrate it.
 * The green and red leds react faster, so while testing accel they show movement on the y axis.
 * If you switch sensors while an led is glowing it may just keep glowing. I don't care enough to fix this.
 * Originally planned to include gyroscope and magnetometer, but there was no time for it.
 */

// Variables.
byte sensorSelection;

// Code to execute.
void sensorTest(){

  char toPrint[9] = "        ";
  generalFlag = false;

  // Display top line and set speed.
  switch(sensorSelection){
    case 0:{

      if(displayFlag){
        topScreenBuffer = "ProxLMMR";
        proxSensors.read();
        if(proxSensors.countsLeftWithLeftLeds() > PROX_THRESHOLD) toPrint[4] = '+';
        if(proxSensors.countsFrontWithLeftLeds() > PROX_THRESHOLD) toPrint[5] = '+';
        if(proxSensors.countsFrontWithRightLeds() > PROX_THRESHOLD) toPrint[6] = '+';
        if(proxSensors.countsRightWithRightLeds() > PROX_THRESHOLD) toPrint[7] = '+';
      }

      break;
    }
    case 1:{

      if(displayFlag){
        topScreenBuffer = "Line LMR";
        lineSensors.read(lineSensorValues);
        if(lineSensorValues[0] < LINE_THRESHOLD) toPrint[5] = '+';
        if(lineSensorValues[1] < LINE_THRESHOLD) toPrint[6] = '+';
        if(lineSensorValues[2] < LINE_THRESHOLD) toPrint[7] = '+';
      }

      break;
    }
    case 2:{

      // Obtain new readings
      imuAccel();

      // Compare with reference.
      imuCompare[0] = imuAverage(0) - imuOriginal[0];
      imuCompare[1] = imuAverage(1) - imuOriginal[1];
      imuCompare[2] = imuAverage(2) - imuOriginal[2];

      // Fast refresh (y-axis).
      ledGreen(false);
      ledRed(false);
      if(imuCompare[1] > ACCEL_THRESHOLD) ledGreen(true);
      if(imuCompare[1] < -ACCEL_THRESHOLD) ledRed(true);

      // Re-calibrate.
      if(aButtonPressed){
        imuCalibrate();
      }

      // Slow refresh.
      if(displayFlag){
        topScreenBuffer = "AccelXYZ";

        if(imuCompare[0] > ACCEL_THRESHOLD) toPrint[5] = '+';
        if(imuCompare[0] < -ACCEL_THRESHOLD) toPrint[5] = '-';
        if(imuCompare[1] > ACCEL_THRESHOLD) toPrint[6] = '+';
        if(imuCompare[1] < -ACCEL_THRESHOLD) toPrint[6] = '-';
        if(imuCompare[2] > ACCEL_THRESHOLD) toPrint[7] = '+';
        if(imuCompare[2] < -ACCEL_THRESHOLD) toPrint[7] = '-';
      }

      break;
    }
    default:{

      // Loop back to begginning.
      sensorSelection = 0;

      //Don't set anything prematurely.
      generalFlag = true;
      
      break;
    }
  }

  //Don't set anything prematurely.
  if(!generalFlag){
    
    // Update Display.
    if(displayFlag) updateDisplay(topScreenBuffer, toPrint);

  }

  // Update the display every quarter second.
  if(millis() - delayTimeMillis >= 250){

    // Schedule update display.
    displayFlag = true;

    // Reset Timer.
    delayTimeMillis = millis();
  }

  // Controls.
  if(bButtonPressed){sensorSelection++; displayFlag = true;} // Something.
}
