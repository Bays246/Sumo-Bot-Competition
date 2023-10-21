// Clears and prints on the LCD display manually.
void updateDisplay(String topLine){
  
  // Update Display.
  display.clear();
  display.print(topLine);

  // Display was updated.
  displayFlag = false;
}

// Overload, prints on both the top and bottom lines of the LCD display.
void updateDisplay(String topLine, String bottomLine){

  // Update Display.
  display.clear();
  display.print(topLine);
  display.gotoXY(0, 1);
  display.print(bottomLine);

  // Display was updated
  displayFlag = false;
}

// Overload, clears and prints on LCD display using buffer content.
void updateDisplay(){

  // Call with parameters
  updateDisplay(topScreenBuffer, bottomScreenBuffer);

}

// Sets motor speed and clears flag.
void updateMotors(int leftSpeed, int rightSpeed){

  // Set speeds.
  motors.setSpeeds(leftSpeed, rightSpeed);

  // Motor was updated.
  motorFlag = false;

  // Is the bot moving?
  movingFlag = !(leftMotorBuffer == SPEED_STOP && rightMotorBuffer == SPEED_STOP);
}

// Overload, sets motor speed and clears flags based on buffers.
void updateMotors(){

  // Call with parameters.
  updateMotors(leftMotorBuffer, rightMotorBuffer);
  
}

// Returns the average of the imu values for index x.
int imuAverage(byte x){

  // Initialize.
  int tmp = 0;

  // Add.
  for(int number : imuValues[x]){
    tmp += number;
  }

  // Divide.
  tmp /= IMU_SAMPLES;

  // Return
  return tmp;
}

// Updates imu variables with accelerometer readings.
void imuAccel(){

  // Get new readings.
  imu.readAcc();

  // Add them to the array, overwrite in order added.
  imuValues[0][imuIndex] = imu.a.x;
  imuValues[1][imuIndex] = imu.a.y;
  imuValues[2][imuIndex] = imu.a.z;
  imuIndex = (imuIndex + 1) % IMU_SAMPLES;

  // Compare with reference.
  imuCompare[0] = imuAverage(0) - imuOriginal[0];
  imuCompare[1] = imuAverage(1) - imuOriginal[1];
  imuCompare[2] = imuAverage(2) - imuOriginal[2];
}

// Sets the imu reference to the current averages.
void imuCalibrate(){
  imuOriginal[0] = imuAverage(0);
  imuOriginal[1] = imuAverage(1);
  imuOriginal[2] = imuAverage(2);
}

// Displays and sounds a countdown, so that a person can set the robot down.
// Also performs a quick calibration for the accelerometer.
void countdownCalibrate(){

  // Audible countdown.
  for(int i = 0; i < COUNTDOWN_SECONDS; i++){
    delay(1000);
    buzzer.playNote(NOTE_G(3), 50, 12);
  }

  // Get readings.
  for(int i = 0; i < IMU_SAMPLES; i++){
    imuAccel();
  }

  // Save results.
  imuCalibrate();
}
