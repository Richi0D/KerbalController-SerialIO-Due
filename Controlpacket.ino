//Main controls uses enum above, e.g. MainControls(SAS,true);
void MainControls(byte n, boolean s) {
  if (s)
    CPacket.MainControls |= (1 << n);       // forces nth bit of x to be 1.  all other bits left alone.
  else
    CPacket.MainControls &= ~(1 << n);      // forces nth bit of x to be 0.  all other bits left alone.
}

//Control groups (action groups) uses an integer to refer to a custom action group, e.g. ControlGroup(5,true);
void ControlGroups(byte n, boolean s) {
  if (s)
    CPacket.ControlGroup |= (1 << n);       // forces nth bit of x to be 1.  all other bits left alone.
  else
    CPacket.ControlGroup &= ~(1 << n);      // forces nth bit of x to be 0.  all other bits left alone.
}

//SAS mode uses enum above, e.g. setSASMode(SMPrograde);
void setSASMode(byte m) {
  CPacket.NavballSASMode &= B11110000;
  CPacket.NavballSASMode += m;
}

//Navball mode uses enum above, e.g. setNavBallMode(NAVBallSURFACE);
void setNavballMode(byte m) {
  CPacket.NavballSASMode &= B00001111;
  CPacket.NavballSASMode += m << 4;
}


//check if it is time to send a control packet
void send_control_packet() {
  now = millis();
  controlTime = now - controlTimeOld;
  joystick_keyboard();        // send keyboard keys and get values for axis
  motorfader();               //call motorfader if button pressed
  
  if (controlTime > CONTROLREFRESH) {
    controlTimeOld = now;
    define_control_packet();
  }
}

void define_control_packet() {
  if (Connected) {
    //here we define what controls to send when which pins are manipulate

    //toggle switches
    if(!digitalRead(pSAS)){MainControls(SAS, true); setSASMode(SASset);} else {MainControls(SAS, false);}
    if(!digitalRead(pRCS)){MainControls(RCS, true);} else {MainControls(RCS, false);}
    if(digitalRead(pABORT)){MainControls(ABORT, true);} else {MainControls(ABORT, false);}
    
    if(debouncerBrakes.pressed()){MainControls(BRAKES, !brakes_on);}        
    
    

    //send the control packet to the KSPSerialIO plugin
    KSPBoardSendData(details(CPacket)); 
  }
}

void joystick_keyboard(){
  //check flymode 
  if((digitalRead(pprecision)) and (!digitalRead(pmodeswitch))){
     flymode = 0;} // rocket mode
   else if((digitalRead(pprecision)) and (digitalRead(pmodeswitch))){
     flymode = 1;} // plane mode
   else if((!digitalRead(pprecision)) and (!digitalRead(pmodeswitch))){
     flymode = 2;} // rocket mode + precision      
   else if((!digitalRead(pprecision)) and (digitalRead(pmodeswitch))){
     flymode = 3;} // plane mode + precision
   else {flymode = 0;} 
   
  const int deadzone = 35;
  const int deadzonethrottle = 25;

  switch(flymode){
    case 0:
      if (analogRead(pTHROTTLE)>= deadzonethrottle || analogRead(pTHROTTLE)<= 1023-deadzonethrottle){
        Joystick.setThrottle(analogRead(pTHROTTLE));}
      else if (analogRead(pTHROTTLE)>= 1023-deadzonethrottle){
        Joystick.setThrottle(1023);}
      else {Joystick.setThrottle(0);}
      
      if (analogRead(pTX) >= 512+deadzone || analogRead(pTX) <= 512-deadzone) {Joystick.setXAxis(analogRead(pTX));}
      else {Joystick.setXAxis(512);}
      if (analogRead(pTY) >= 512+deadzone || analogRead(pTY) <= 512-deadzone) {Joystick.setYAxis(analogRead(pTY));}
      else {Joystick.setYAxis(512);}
      if (analogRead(pTZ) >= 512+deadzone || analogRead(pTZ) <= 512-deadzone) {Joystick.setZAxis(analogRead(pTZ));}
      else {Joystick.setZAxis(512);}

      if (analogRead(pRX) >= 512+deadzone || analogRead(pRX) <= 512-deadzone) {Joystick.setRxAxis(analogRead(pRX));}
      else {Joystick.setRxAxis(512);}
      if (analogRead(pRY) >= 512+deadzone || analogRead(pRY) <= 512-deadzone) {Joystick.setRyAxis(analogRead(pRY));}
      else {Joystick.setRyAxis(512);}
      if (analogRead(pRZ) >= 512+deadzone || analogRead(pRZ) <= 512-deadzone) {Joystick.setRzAxis(analogRead(pRZ));}
      else {Joystick.setRzAxis(512);}

      break;
    case 1:
      if (analogRead(pTHROTTLE)>= deadzonethrottle || analogRead(pTHROTTLE)<= 1023-deadzonethrottle){
        Joystick.setThrottle(analogRead(pTHROTTLE));}
      else if (analogRead(pTHROTTLE)>= 1023-deadzonethrottle){
        Joystick.setThrottle(1023);}
      else {Joystick.setThrottle(0);}

      if (analogRead(pTX) >= 512+deadzone || analogRead(pTX) <= 512-deadzone) {Joystick.setZAxis(analogRead(pTX));}
      else {Joystick.setZAxis(512);}
      if (analogRead(pTY) >= 512+deadzone || analogRead(pTY) <= 512-deadzone) {Joystick.setYAxis(analogRead(pTY));}
      else {Joystick.setYAxis(512);}
      if (analogRead(pTZ) >= 512+deadzone || analogRead(pTZ) <= 512-deadzone) {Joystick.setXAxis(analogRead(pTZ));}
      else {Joystick.setXAxis(512);}

      if (analogRead(pRX) >= 512+deadzone || analogRead(pRX) <= 512-deadzone) {Joystick.setRxAxis(analogRead(pRX));}
      else {Joystick.setRxAxis(512);}
      if (analogRead(pRY) >= 512+deadzone || analogRead(pRY) <= 512-deadzone) {Joystick.setRyAxis(analogRead(pRY));}
      else {Joystick.setRyAxis(512);}
      if (analogRead(pRZ) >= 512+deadzone || analogRead(pRZ) <= 512-deadzone) {Joystick.setRzAxis(analogRead(pRZ));}
      else {Joystick.setRzAxis(512);}
            
      break;
    case 2:
      if (analogRead(pTHROTTLE)>= deadzonethrottle || analogRead(pTHROTTLE)<= 1023-deadzonethrottle){
        Joystick.setThrottle(analogRead(pTHROTTLE));}
      else if (analogRead(pTHROTTLE)>= 1023-deadzonethrottle){
        Joystick.setThrottle(1023);}
      else {Joystick.setThrottle(0);}
      
      if (analogRead(pTX) >= 512+deadzone) {Joystick.setXAxis(map(analogRead(pTX),512,1023,512,768));}
      else if (analogRead(pTX) <= 512-deadzone) {Joystick.setXAxis(map(analogRead(pTX),0,512,256,512));}
      else {Joystick.setXAxis(512);}
      if (analogRead(pTY) >= 512+deadzone) {Joystick.setYAxis(map(analogRead(pTY),512,1023,512,768));}
      else if (analogRead(pTY) <= 512-deadzone) {Joystick.setYAxis(map(analogRead(pTY),0,512,256,512));}
      else {Joystick.setYAxis(512);}
      if (analogRead(pTZ) >= 512+deadzone) {Joystick.setZAxis(map(analogRead(pTZ),512,1023,512,768));}
      else if (analogRead(pTZ) <= 512-deadzone) {Joystick.setZAxis(map(analogRead(pTZ),0,512,256,512));}      
      else {Joystick.setZAxis(512);}

      if (analogRead(pRX) >= 512+deadzone) {Joystick.setRxAxis(map(analogRead(pRX),512,1023,512,768));}
      else if (analogRead(pRX) <= 512-deadzone) {Joystick.setRxAxis(map(analogRead(pRX),0,512,256,512));}       
      else {Joystick.setRxAxis(512);}
      if (analogRead(pRY) >= 512+deadzone) {Joystick.setRyAxis(map(analogRead(pRY),512,1023,512,768));}
      else if (analogRead(pRY) <= 512-deadzone) {Joystick.setRyAxis(map(analogRead(pRY),0,512,256,512));}       
      else {Joystick.setRyAxis(512);}
      if (analogRead(pRZ) >= 512+deadzone) {Joystick.setRzAxis(map(analogRead(pRZ),512,1023,512,768));}
      else if (analogRead(pRZ) <= 512-deadzone) {Joystick.setRzAxis(map(analogRead(pRZ),0,512,256,512));}       
      else {Joystick.setRzAxis(512);}
      
      break;
    case 3:
      if (analogRead(pTHROTTLE)>= deadzonethrottle || analogRead(pTHROTTLE)<= 1023-deadzonethrottle){
        Joystick.setThrottle(analogRead(pTHROTTLE));}
      else if (analogRead(pTHROTTLE)>= 1023-deadzonethrottle){
        Joystick.setThrottle(1023);}
      else {Joystick.setThrottle(0);}
      
      if (analogRead(pTX) >= 512+deadzone) {Joystick.setZAxis(map(analogRead(pTX),512,1023,512,768));}
      else if (analogRead(pTX) <= 512-deadzone) {Joystick.setZAxis(map(analogRead(pTX),0,512,256,512));}
      else {Joystick.setZAxis(512);}
      if (analogRead(pTY) >= 512+deadzone) {Joystick.setYAxis(map(analogRead(pTY),512,1023,512,768));}
      else if (analogRead(pTY) <= 512-deadzone) {Joystick.setYAxis(map(analogRead(pTY),0,512,256,512));}
      else {Joystick.setYAxis(512);}
      if (analogRead(pTZ) >= 512+deadzone) {Joystick.setXAxis(map(analogRead(pTZ),512,1023,512,768));}
      else if (analogRead(pTZ) <= 512-deadzone) {Joystick.setXAxis(map(analogRead(pTZ),0,512,256,512));}      
      else {Joystick.setXAxis(512);}

      if (analogRead(pRX) >= 512+deadzone) {Joystick.setRxAxis(map(analogRead(pRX),512,1023,512,768));}
      else if (analogRead(pRX) <= 512-deadzone) {Joystick.setRxAxis(map(analogRead(pRX),0,512,256,512));}       
      else {Joystick.setRxAxis(512);}
      if (analogRead(pRY) >= 512+deadzone) {Joystick.setRyAxis(map(analogRead(pRY),512,1023,512,768));}
      else if (analogRead(pRY) <= 512-deadzone) {Joystick.setRyAxis(map(analogRead(pRY),0,512,256,512));}       
      else {Joystick.setRyAxis(512);}
      if (analogRead(pRZ) >= 512+deadzone) {Joystick.setRzAxis(map(analogRead(pRZ),512,1023,512,768));}
      else if (analogRead(pRZ) <= 512-deadzone) {Joystick.setRzAxis(map(analogRead(pRZ),0,512,256,512));}       
      else {Joystick.setRzAxis(512);}
      
      break;            
  } 
/*  //Switches (SAS, RCS, ABORT)
  if(!digitalRead(pSAS) && !SAS_on){Keyboard.write('T');}
  else if (digitalRead(pSAS) && SAS_on){Keyboard.write('T');}
  if(!digitalRead(pRCS) && !RCS_on){Keyboard.write('R');}
  else if (digitalRead(pRCS) && RCS_on){Keyboard.write('R');}
  if(digitalRead(pABORT) && !abort_on){Keyboard.write(178);}
  else if (!digitalRead(pABORT) && abort_on){Keyboard.write(178);}
*/  
  //momentary stage button
  if(debouncerStage.pressed() && digitalRead(pARM)){Keyboard.write(32);}
  
  //toggle buttons
    if(debouncerLights.pressed()){Keyboard.write('U');}
    if(debouncerGears.pressed()){Keyboard.write('G');}
    //if(debouncerBrakes.pressed()){Keyboard.write('B');}
    if(debouncerA1.pressed()){Keyboard.write('1');}
    if(debouncerA2.pressed()){Keyboard.write('2');}
    if(debouncerA3.pressed()){Keyboard.write('3');}
    if(debouncerA4.pressed()){Keyboard.write('4');}
    if(debouncerA5.pressed()){Keyboard.write('5');} 
    if(debouncerA6.pressed()){Keyboard.write('6');}      
    if(debouncerLadder.pressed()){Keyboard.write('7');}
    if(debouncerChutes.pressed()){Keyboard.write('8');}    
    if(debouncerSolar.pressed()){Keyboard.write('9');}  
}


//Control of motorfader
void motorfader() {
    if(!digitalRead(maxthrottle)){
      throttletarget = 1023;
      digitalWrite(motoron, HIGH);
        if (analogRead(pTHROTTLE) < (throttletarget - theThreshold) ) {
          analogWrite(motordown, 0);
          analogWrite(motorup, 255);     
        }
        else {
          analogWrite(motordown, 0);
          analogWrite(motorup, 0);                 
        } 
    }
    else if(!digitalRead(minthrottle)){
      throttletarget = 0;
      digitalWrite(motoron, HIGH);
        if (analogRead(pTHROTTLE) > (throttletarget + theThreshold) ) {
          analogWrite(motordown, 255);
          analogWrite(motorup, 0);
          }
        else {
          analogWrite(motordown, 0);
          analogWrite(motorup, 0);                 
        }  
      }
    else {
      throttletarget = pTHROTTLE;
      digitalWrite(motoron, LOW);
    }
}
