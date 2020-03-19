void debugfunction (){ 
    now = millis();
    const int debuginterval = 50;
    debugtime = now - debugtimeOld;       
    if (debugtime > debuginterval) {
    debugtimeOld = now; 
    
    //Debug mode does not communicate with KSPSerialIO, but simply displays the button states on the LCD.

    //switch lcd to debug
    if (PageDisplay1 !=6){
            PageDisplay1 = 6;
            sendToDisplay1(String("page ") + String(PageDisplay1));
    }
    if (PageDisplay2 !=3){
      PageDisplay2 = 3; 
      sendToDisplay2(String("page ") + String(PageDisplay2));
    }
    
    //read analog input
    tx_value = analogRead(pTX);
          sendToDisplay2(String("transxV.txt=\"") + String(tx_value)+String("\""));
    ty_value = analogRead(pTY);
          sendToDisplay2(String("transyV.txt=\"") + String(ty_value)+String("\""));
    tz_value = analogRead(pTZ);
          sendToDisplay2(String("trasnzV.txt=\"") + String(tz_value)+String("\""));    
    rx_value = analogRead(pRX);
          sendToDisplay2(String("rotxV.txt=\"") + String(rx_value)+String("\""));      
    ry_value = analogRead(pRY);
          sendToDisplay2(String("rotyV.txt=\"") + String(ry_value)+String("\""));     
    rz_value = analogRead(pRZ);
          sendToDisplay2(String("rotzV.txt=\"") + String(rz_value)+String("\""));
    throttle_value = analogRead(pTHROTTLE);
          sendToDisplay2(String("throttleV.txt=\"") + String(throttle_value)+String("\""));                
                    
    //check rotation switch status
    if (!digitalRead(pswitch1)) {
        sendToDisplay2(String("dispswV.txt=\"") + String("1")+String("\""));
      }
      else if (!digitalRead(pswitch2)) {
        sendToDisplay2(String("dispswV.txt=\"") + String("2")+String("\""));
      }
      else if (!digitalRead(pswitch3)) {
        sendToDisplay2(String("dispswV.txt=\"") + String("3")+String("\""));
      }
      else if (!digitalRead(pswitch4)) {
        sendToDisplay2(String("dispswV.txt=\"") + String("4")+String("\""));
      }
      else{
        sendToDisplay2(String("dispswV.txt=\"") + String("none")+String("\""));
      }

    //Throttle buttons
      if (!digitalRead(maxthrottle)){
        sendToDisplay1(String("maxV.txt=\"") + String("1")+String("\""));  
      }
      else {
        sendToDisplay1(String("maxV.txt=\"") + String("0")+String("\""));
      }
      if (!digitalRead(minthrottle)){
        sendToDisplay1(String("minV.txt=\"") + String("1")+String("\""));  
      }
      else {
        sendToDisplay1(String("minV.txt=\"") + String("0")+String("\""));
      }

    //Action buttons
      if (!digitalRead(pSTAGE)){
        sendToDisplay1(String("stageV.txt=\"") + String("1")+String("\""));  
      }
      else {
        sendToDisplay1(String("stageV.txt=\"") + String("0")+String("\""));
      } 
      if (!digitalRead(pACTION1)){
        sendToDisplay1(String("A1V.txt=\"") + String("1")+String("\""));
        digitalWrite(pACTION1LED, HIGH);  
      }
      else {
        sendToDisplay1(String("A1V.txt=\"") + String("0")+String("\""));
        digitalWrite(pACTION1LED, LOW);
      }
      if (!digitalRead(pACTION2)){
        sendToDisplay1(String("A2V.txt=\"") + String("1")+String("\""));
        digitalWrite(pACTION2LED, HIGH);  
      }
      else {
        sendToDisplay1(String("A2V.txt=\"") + String("0")+String("\""));
        digitalWrite(pACTION2LED, LOW);
      }               
      if (!digitalRead(pACTION3)){
        sendToDisplay1(String("A3V.txt=\"") + String("1")+String("\""));
        digitalWrite(pACTION3LED, HIGH);  
      }
      else {
        sendToDisplay1(String("A3V.txt=\"") + String("0")+String("\""));
        digitalWrite(pACTION3LED, LOW);
      }    
      if (!digitalRead(pACTION4)){
        sendToDisplay1(String("A4V.txt=\"") + String("1")+String("\""));
        digitalWrite(pACTION4LED, HIGH);  
      }
      else {
        sendToDisplay1(String("A4V.txt=\"") + String("0")+String("\""));
        digitalWrite(pACTION4LED, LOW);
      }    
      if (!digitalRead(pACTION5)){
        sendToDisplay1(String("A5V.txt=\"") + String("1")+String("\""));
        digitalWrite(pACTION5LED, HIGH);  
      }
      else {
        sendToDisplay1(String("A5V.txt=\"") + String("0")+String("\""));
        digitalWrite(pACTION5LED, LOW);
      }    
      if (!digitalRead(pACTION6)){
        sendToDisplay1(String("A6V.txt=\"") + String("1")+String("\""));
        digitalWrite(pACTION6LED, HIGH);  
      }
      else {
        sendToDisplay1(String("A6V.txt=\"") + String("0")+String("\""));
        digitalWrite(pACTION6LED, LOW);
      }    
       if (!digitalRead(pCHUTES)){
        sendToDisplay1(String("chutesV.txt=\"") + String("1")+String("\""));
        digitalWrite(pCHUTESLED, HIGH);  
      }
      else {
        sendToDisplay1(String("chutesV.txt=\"") + String("0")+String("\""));
        digitalWrite(pCHUTESLED, LOW);
      }    
       if (!digitalRead(pLADDER)){
        sendToDisplay1(String("ladderV.txt=\"") + String("1")+String("\""));
        digitalWrite(pLADDERLED, HIGH);  
      }
      else {
        sendToDisplay1(String("ladderV.txt=\"") + String("0")+String("\""));
        digitalWrite(pLADDERLED, LOW);
      } 
       if (!digitalRead(pSOLAR)){
        sendToDisplay1(String("solarV.txt=\"") + String("1")+String("\""));
        digitalWrite(pSOLARLED, HIGH);  
      }
      else {
        sendToDisplay1(String("solarV.txt=\"") + String("0")+String("\""));
        digitalWrite(pSOLARLED, LOW);
      } 
       if (!digitalRead(pGEARS)){
        sendToDisplay1(String("gearsV.txt=\"") + String("1")+String("\""));
        digitalWrite(pGEARSLED, HIGH);  
      }
      else {
        sendToDisplay1(String("gearsV.txt=\"") + String("0")+String("\""));
        digitalWrite(pGEARSLED, LOW);
      } 
       if (!digitalRead(pBRAKES)){
        sendToDisplay2(String("brakesV.txt=\"") + String("1")+String("\""));
        digitalWrite(pBRAKESLED, HIGH);  
      }
      else {
        sendToDisplay2(String("brakesV.txt=\"") + String("0")+String("\""));
        digitalWrite(pBRAKESLED, LOW);
      } 
       if (!digitalRead(pLIGHTS)){
        sendToDisplay2(String("lightsV.txt=\"") + String("1")+String("\""));
        digitalWrite(pLIGHTSLED, HIGH);  
      }
      else {
        sendToDisplay2(String("lightsV.txt=\"") + String("0")+String("\""));
        digitalWrite(pLIGHTSLED, LOW);
      }
                                                    
      
    //Switches
      if (!digitalRead(pMODE)){
        sendToDisplay1(String("debugV.txt=\"") + String("1")+String("\""));  
      }
      else {
        sendToDisplay1(String("debugV.txt=\"") + String("0")+String("\""));
      }    
      if (digitalRead(pABORT)){
        sendToDisplay1(String("abortV.txt=\"") + String("1")+String("\""));  
      }
      else {
        sendToDisplay1(String("abortV.txt=\"") + String("0")+String("\""));
      }        
      if (digitalRead(pARM)){
        sendToDisplay1(String("armV.txt=\"") + String("1")+String("\""));  
      }
      else {
        sendToDisplay1(String("armV.txt=\"") + String("0")+String("\""));
      } 
      if (!digitalRead(pSAS)){
        sendToDisplay2(String("sasV.txt=\"") + String("1")+String("\""));  
      }
      else {
        sendToDisplay2(String("sasV.txt=\"") + String("0")+String("\""));
      } 
      if (!digitalRead(pRCS)){
        sendToDisplay2(String("rcsV.txt=\"") + String("1")+String("\""));  
      }
      else {
        sendToDisplay2(String("rcsV.txt=\"") + String("0")+String("\""));
      } 
      if (!digitalRead(pprecision)){
        sendToDisplay2(String("preccoV.txt=\"") + String("1")+String("\""));  
      }
      else {
        sendToDisplay2(String("preccoV.txt=\"") + String("0")+String("\""));
      } 
       if (!digitalRead(pmodeswitch)){
        sendToDisplay2(String("extra1V.txt=\"") + String("1")+String("\""));
        sendToDisplay2(String("extra1T.txt=\"") + String("Flymode")+String("\""));    
      }
      else {
        sendToDisplay2(String("extra1V.txt=\"") + String("0")+String("\""));
        sendToDisplay2(String("extra1T.txt=\"") + String("Flymode")+String("\"")); 
      }            
      
    //Atmospheregauge
    const int ledinterval = 1500; 
    atmotime = now - atmotimeOld;       
    if (atmotime > ledinterval) {
    atmotimeOld = now;    
    chooseled = chooseled+1;    
    if (chooseled==12){
      chooseled = 0;
    }
    switch(chooseled){
    case 0:
      AtmosphereGauge(5);   //LED1 
      break;
    case 1:
      AtmosphereGauge(4);   //LED2   
      break;
    case 2:
      AtmosphereGauge(2.5);   //LED3  
      break;
    case 3:
      AtmosphereGauge(1.5);   //LED4  
      break;
    case 4:
      AtmosphereGauge(1);   //LED5 
      break;
    case 5:
      AtmosphereGauge(0.8); //LED6  
      break;
    case 6:
      AtmosphereGauge(0.5); //LED7 
      break;
    case 7:
      AtmosphereGauge(0.3); //LED8 
      break;
    case 8:
      AtmosphereGauge(0.2); //LED9 
      break;            
     case 9:
      AtmosphereGauge(0.1); //LED10 
      break; 
    case 10:
      AtmosphereGauge(0.05); //LED11
      break;
    case 11:
      AtmosphereGauge(0.02); //LED12  
      break;                                  
    }
  }
  }
  motorfader(); //call motorfader if button pressed
   }
