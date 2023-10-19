/*
 * Reads battery voltage and displays every half second.
 * readBatteryMillivolts() always gives a 4 digit number.
 */
 
// Variables.
// None.

// Code to execute.
void batteryTest(){
  // Every half second.
  if(millis() - delayTimeMillis >= 500){
  
    // This takes up 8% of programming space (float -> String conversion is what does it).
    //updateDisplay(" "+String((float)readBatteryMillivolts()*0.001, 3), "  Volts ");

    // This takes up 2% of programming space.
    //topScreenBuffer = String(readBatteryMillivolts());
    //topScreenBuffer = ' ' + topScreenBuffer[0] + '.' + topScreenBuffer.substring(1);

    // This takes up 2% of programming space (but slightly less than above).
    topScreenBuffer = String(readBatteryMillivolts());
    char toPrint[7] = {' ',topScreenBuffer[0],'.',topScreenBuffer[1],topScreenBuffer[2],topScreenBuffer[3]};
  
    // Printing String (1% of programming space).
    updateDisplay(toPrint, "  Volts ");
  
    // Reset Timer.
    delayTimeMillis = millis();
  }
}
