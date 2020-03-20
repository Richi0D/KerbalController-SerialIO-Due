//get the current state of main controls and custom action groups using enumeration above, e.g. ControlStatus(AGBrakes);
byte ControlStatus(byte n){return ((VData.ActionGroups >> n) & 1) == 1;}
//get the current SAS Mode. Can be compared to enum values, e.g. if(getSASMode() == SMPrograde){//do stuff}
byte getSASMode() {return VData.NavballSASMode & B00001111;} // leaves alone the lower 4 bits of; all higher bits set to 0.
//get the current navball mode. Can be compared to enum values, e.g. if (getNavballMode() == NAVBallTARGET){//do stuff}
byte getNavballMode() {return VData.NavballSASMode >> 4;} // leaves alone the higher 4 bits of; all lower bits set to 0.


//connect to the KSPSerialIO plugin and receive data
int16_t get_vessel_data() {
  int16_t returnValue = -1;
  now = millis();

  if (KSPBoardReceiveData()){
    //data is being received
    deadtimeOld = now;   
    returnValue = id;
    switch(id) {
    case 0: //First packet is a handshake packet
      Handshake();
      PageDisplay1 = 0;
      PageDisplay2 = 0;
      sendToDisplay1(String("page ") + String(PageDisplay1));
      sendToDisplay1(String("boot.txt=\"") + String("Handshake")+String("\""));
      sendToDisplay2(String("page ") + String(PageDisplay2));
      sendToDisplay2(String("boot.txt=\"") + String("Handshake")+String("\""));
      break;
    case 1:
      //subsequent packets are data from the plugin
      if (!digitalRead(pswitch1) && PageDisplay1 != 2) {
        PageDisplay1 = 2;
        sendToDisplay1(String("page ") + String(PageDisplay1)); 
      }
      else if (!digitalRead(pswitch2) && PageDisplay1 != 3) {
        PageDisplay1 = 3;
        sendToDisplay1(String("page ") + String(PageDisplay1)); 
      }
      else if (!digitalRead(pswitch3) && PageDisplay1 != 4) {
        PageDisplay1 = 4;
        sendToDisplay1(String("page ") + String(PageDisplay1)); 
      }
      else if (!digitalRead(pswitch4) && PageDisplay1 != 5) {
        PageDisplay1 = 5;
        sendToDisplay1(String("page ") + String(PageDisplay1)); 
      }     
      if (PageDisplay2 !=2) {
        PageDisplay2 = 2;
        sendToDisplay2(String("page ") + String(PageDisplay2));   
      }
      displayTime = now - displayTimeOld;
      if (displayTime > DISPLAYREFRESH) {
        displayTimeOld = now;
        define_vessel_data_display();   //define what to do with the data below
      }
      break;
    }
    Connected = true;
  }
  else
  { //if no message received for a while, go idle
    deadtime = now - deadtimeOld; 
    if (deadtime > IDLETIMER)
    {
      deadtimeOld = now;
      Connected = false;
      if (PageDisplay1 !=1){
        PageDisplay1 = 1;
        sendToDisplay1(String("page ") + String(PageDisplay1));}
      if (PageDisplay2 !=1){
        PageDisplay2 = 1;
        sendToDisplay2(String("page ") + String(PageDisplay2));}
      AtmosphereGauge(10);   //Atmosphere gauge off
      SASset = 1;
      
  //turn off Leds
  digitalWrite(pLIGHTSLED, LOW); 
  digitalWrite(pGEARSLED, LOW);
  digitalWrite(pBRAKESLED, LOW);
  digitalWrite(pACTION1LED, LOW);
  digitalWrite(pACTION2LED, LOW);
  digitalWrite(pACTION3LED, LOW);
  digitalWrite(pACTION4LED, LOW);
  digitalWrite(pACTION5LED, LOW);
  digitalWrite(pACTION6LED, LOW);    
  digitalWrite(pLADDERLED, LOW);
  digitalWrite(pSOLARLED, LOW);
  digitalWrite(pCHUTESLED, LOW);      
    }    
  }
  return returnValue;
}


//define what to do with the vessel data here, e.g. turn on LED's, display text on the LCD
void define_vessel_data_display() {

  //get in-game status for updating the LED statuses on the controller 
  SAS_on = ControlStatus(AGSAS);
  RCS_on = ControlStatus(AGRCS);  
  abort_on = ControlStatus(AGAbort);  
  lights_on = ControlStatus(AGLight);
  gears_on = ControlStatus(AGGears);
  brakes_on = ControlStatus(AGBrakes);
  action1_on = ControlStatus(AGCustom01);
  action2_on = ControlStatus(AGCustom02);
  action3_on = ControlStatus(AGCustom03);
  action4_on = ControlStatus(AGCustom04);
  action5_on = ControlStatus(AGCustom05);
  action6_on = ControlStatus(AGCustom06);  
  ladder_on = ControlStatus(AGCustom07);
  chutes_on = ControlStatus(AGCustom08);  
  solar_on = ControlStatus(AGCustom09);

  //update button LEDs based on in-game status
  digitalWrite(pLIGHTSLED, lights_on); 
  digitalWrite(pGEARSLED, gears_on);
  digitalWrite(pBRAKESLED, brakes_on);
  digitalWrite(pACTION1LED, action1_on);
  digitalWrite(pACTION2LED, action2_on);
  digitalWrite(pACTION3LED, action3_on);
  digitalWrite(pACTION4LED, action4_on);
  digitalWrite(pACTION5LED, action5_on);
  digitalWrite(pACTION6LED, action6_on);    
  digitalWrite(pLADDERLED, ladder_on);
  digitalWrite(pSOLARLED, solar_on);
  digitalWrite(pCHUTESLED, chutes_on);

  AtmosphereGauge(VData.Density);   //call Atmosphere gauge
  
  //update button LEDs based on in-game status
  switch (PageDisplay1){

    case 4:  // update SAS Buttons  
     //SASval = getSASMode();
     //if (SASval <> SASset){
      switch (SASset){
        case 0: //SMOFF
          sendToDisplay1(String("dsoff.val=") + String(0));
          sendToDisplay1(String("dsman.val=") + String(0));
          sendToDisplay1(String("dspro.val=") + String(0));
          sendToDisplay1(String("dsret.val=") + String(0));
          sendToDisplay1(String("dsnorm.val=") + String(0));
          sendToDisplay1(String("dsanti.val=") + String(0));
          sendToDisplay1(String("dsradout.val=") + String(0));
          sendToDisplay1(String("dsradin.val=") + String(0));
          sendToDisplay1(String("dstarg.val=") + String(0));
          sendToDisplay1(String("dsrettarg.val=") + String(0));
        break;        
        case 1: //SMSAS
          sendToDisplay1(String("dsoff.val=") + String(1));
          sendToDisplay1(String("dsman.val=") + String(0));
          sendToDisplay1(String("dspro.val=") + String(0));
          sendToDisplay1(String("dsret.val=") + String(0));
          sendToDisplay1(String("dsnorm.val=") + String(0));
          sendToDisplay1(String("dsanti.val=") + String(0));
          sendToDisplay1(String("dsradout.val=") + String(0));
          sendToDisplay1(String("dsradin.val=") + String(0));
          sendToDisplay1(String("dstarg.val=") + String(0));
          sendToDisplay1(String("dsrettarg.val=") + String(0));
        break;
        case 2: //SMPrograde
          sendToDisplay1(String("dsoff.val=") + String(0));
          sendToDisplay1(String("dsman.val=") + String(0));
          sendToDisplay1(String("dspro.val=") + String(1));
          sendToDisplay1(String("dsret.val=") + String(0));
          sendToDisplay1(String("dsnorm.val=") + String(0));
          sendToDisplay1(String("dsanti.val=") + String(0));
          sendToDisplay1(String("dsradout.val=") + String(0));
          sendToDisplay1(String("dsradin.val=") + String(0));
          sendToDisplay1(String("dstarg.val=") + String(0));
          sendToDisplay1(String("dsrettarg.val=") + String(0));
        break;
        case 3: //SMRetroGrade
          sendToDisplay1(String("dsoff.val=") + String(0));
          sendToDisplay1(String("dsman.val=") + String(0));
          sendToDisplay1(String("dspro.val=") + String(0));
          sendToDisplay1(String("dsret.val=") + String(1));
          sendToDisplay1(String("dsnorm.val=") + String(0));
          sendToDisplay1(String("dsanti.val=") + String(0));
          sendToDisplay1(String("dsradout.val=") + String(0));
          sendToDisplay1(String("dsradin.val=") + String(0));
          sendToDisplay1(String("dstarg.val=") + String(0));
          sendToDisplay1(String("dsrettarg.val=") + String(0));
        break;
        case 4: //SMNormal
          sendToDisplay1(String("dsoff.val=") + String(0));
          sendToDisplay1(String("dsman.val=") + String(0));
          sendToDisplay1(String("dspro.val=") + String(0));
          sendToDisplay1(String("dsret.val=") + String(0));
          sendToDisplay1(String("dsnorm.val=") + String(1));
          sendToDisplay1(String("dsanti.val=") + String(0));
          sendToDisplay1(String("dsradout.val=") + String(0));
          sendToDisplay1(String("dsradin.val=") + String(0));
          sendToDisplay1(String("dstarg.val=") + String(0));
          sendToDisplay1(String("dsrettarg.val=") + String(0));
        break;
         case 5: //SMAntinormal
          sendToDisplay1(String("dsoff.val=") + String(0));
          sendToDisplay1(String("dsman.val=") + String(0));
          sendToDisplay1(String("dspro.val=") + String(0));
          sendToDisplay1(String("dsret.val=") + String(0));
          sendToDisplay1(String("dsnorm.val=") + String(0));
          sendToDisplay1(String("dsanti.val=") + String(1));
          sendToDisplay1(String("dsradout.val=") + String(0));
          sendToDisplay1(String("dsradin.val=") + String(0));
          sendToDisplay1(String("dstarg.val=") + String(0));
          sendToDisplay1(String("dsrettarg.val=") + String(0));
        break;
         case 6: //SMRadialIn
          sendToDisplay1(String("dsoff.val=") + String(0));
          sendToDisplay1(String("dsman.val=") + String(0));
          sendToDisplay1(String("dspro.val=") + String(0));
          sendToDisplay1(String("dsret.val=") + String(0));
          sendToDisplay1(String("dsnorm.val=") + String(0));
          sendToDisplay1(String("dsanti.val=") + String(0));
          sendToDisplay1(String("dsradout.val=") + String(0));
          sendToDisplay1(String("dsradin.val=") + String(1));
          sendToDisplay1(String("dstarg.val=") + String(0));
          sendToDisplay1(String("dsrettarg.val=") + String(0));
        break;
        case 7: //SMRadialOut
          sendToDisplay1(String("dsoff.val=") + String(0));
          sendToDisplay1(String("dsman.val=") + String(0));
          sendToDisplay1(String("dspro.val=") + String(0));
          sendToDisplay1(String("dsret.val=") + String(0));
          sendToDisplay1(String("dsnorm.val=") + String(0));
          sendToDisplay1(String("dsanti.val=") + String(0));
          sendToDisplay1(String("dsradout.val=") + String(1));
          sendToDisplay1(String("dsradin.val=") + String(0));
          sendToDisplay1(String("dstarg.val=") + String(0));
          sendToDisplay1(String("dsrettarg.val=") + String(0));
        break;
         case 8: //SMTarget
          sendToDisplay1(String("dsoff.val=") + String(0));
          sendToDisplay1(String("dsman.val=") + String(0));
          sendToDisplay1(String("dspro.val=") + String(0));
          sendToDisplay1(String("dsret.val=") + String(0));
          sendToDisplay1(String("dsnorm.val=") + String(0));
          sendToDisplay1(String("dsanti.val=") + String(0));
          sendToDisplay1(String("dsradout.val=") + String(0));
          sendToDisplay1(String("dsradin.val=") + String(0));
          sendToDisplay1(String("dstarg.val=") + String(1));
          sendToDisplay1(String("dsrettarg.val=") + String(0));
        break;
         case 9: //SMAntiTarget
          sendToDisplay1(String("dsoff.val=") + String(0));
          sendToDisplay1(String("dsman.val=") + String(0));
          sendToDisplay1(String("dspro.val=") + String(0));
          sendToDisplay1(String("dsret.val=") + String(0));
          sendToDisplay1(String("dsnorm.val=") + String(0));
          sendToDisplay1(String("dsanti.val=") + String(0));
          sendToDisplay1(String("dsradout.val=") + String(0));
          sendToDisplay1(String("dsradin.val=") + String(0));
          sendToDisplay1(String("dstarg.val=") + String(1));
          sendToDisplay1(String("dsrettarg.val=") + String(0));
        break;  
         case 10: //SMManeuverNode
          sendToDisplay1(String("dsoff.val=") + String(0));
          sendToDisplay1(String("dsman.val=") + String(1));
          sendToDisplay1(String("dspro.val=") + String(0));
          sendToDisplay1(String("dsret.val=") + String(0));
          sendToDisplay1(String("dsnorm.val=") + String(0));
          sendToDisplay1(String("dsanti.val=") + String(0));
          sendToDisplay1(String("dsradout.val=") + String(0));
          sendToDisplay1(String("dsradin.val=") + String(0));
          sendToDisplay1(String("dstarg.val=") + String(0));
          sendToDisplay1(String("dsrettarg.val=") + String(0));
        break;                                                                       
        }
       //}
    break;
    
    case 5:  // update Wavepage    
      //Altitude
      String strAlt = "";
      if (Altitude < 1000000 && Altitude > -1000000) {
        strAlt += String(Altitude,0);
        strAlt += "m";
      } else if ((Altitude >= 1000000 && Altitude < 1000000000) || (Altitude <= -1000000 && Altitude > -1000000000)) {
        strAlt += String((Altitude / 1000.0),0);
        strAlt += "km";
      } else if ((Altitude >= 1000000000 && Altitude < 1000000000000) || (Altitude <= -1000000000 && Altitude > -1000000000000)) {
        strAlt += String((Altitude / 1000000.0),0);
        strAlt += "Mm";
      } else {
        strAlt += String((Altitude / 1000000000.0),0);
        strAlt += "Gm";
      }      
      sendToDisplay1(String("valh.txt=\"") + String(strAlt)+String("\""));

      //Fly high
      String strflyhigh = "";
      if (flyhigh < 1000000) {
        strflyhigh += String(flyhigh);
        strflyhigh += "m";
      } else if ((flyhigh >= 1000000 && flyhigh < 1000000000)) {
        strflyhigh += String((flyhigh / 1000));
        strflyhigh += "km";
      } else if ((flyhigh >= 1000000000 && flyhigh < 1000000000000)) {
        strflyhigh += String((flyhigh / 1000000));
        strflyhigh += "Mm";
      } else {
        strflyhigh += String((flyhigh / 1000000000));
        strflyhigh += "Gm";
      }       
      sendToDisplay1(String("valfh.txt=\"") + String(strflyhigh)+String("\""));

      //Low space
      String strlowspace = "";
      if (lowspace < 1000000) {
        strlowspace += String(lowspace);
        strlowspace += "m";
      } else if ((lowspace >= 1000000 && lowspace < 1000000000) || (lowspace <= -1000000 && lowspace > -1000000000)) {
        strlowspace += String((lowspace / 1000));
        strlowspace += "km";
      } else if ((lowspace >= 1000000000 && lowspace < 1000000000000) || (lowspace <= -1000000000 && lowspace > -1000000000000)) {
        strlowspace += String((lowspace / 1000000));
        strlowspace += "Mm";
      } else {
        strlowspace += String((lowspace / 1000000000));
        strlowspace += "Gm";
      }       
      sendToDisplay1(String("valls.txt=\"") + String(strlowspace)+String("\""));

      //High space
      String strhighspace = "";
      if (highspace < 1000000) {
        strhighspace += String(highspace);
        strhighspace += "m";
      } else if ((highspace >= 1000000 && highspace < 1000000000) || (highspace <= -1000000 && highspace > -1000000000)) {
        strhighspace += String((highspace / 1000));
        strhighspace += "km";
      } else if ((highspace >= 1000000000 && highspace < 1000000000000) || (highspace <= -1000000000 && highspace > -1000000000000)) {
        strhighspace += String((highspace / 1000000));
        strhighspace += "Mm";
      } else {
        strhighspace += String((highspace / 1000000000));
        strhighspace += "Gm";
      }         
      sendToDisplay1(String("valhs.txt=\"") + String(strhighspace)+String("\""));
    break;    
    }

//Ressources Page2 Display2
  if (PageDisplay2==2){
  const unsigned int gaugeColorOK = 29927;           //Color green when full
  const unsigned int gaugeColorAlert = 59169;        //Color yellow when alert
  const unsigned int gaugeColorCritical = 63488;     //Color red when critical
    
  //Battery
  if (VData.EChargeTot <= 0){
     sendToDisplay2(String("jel.val=") + String(100));
  }else{
      //gauges
      byte vEL = 100-(100 * VData.ECharge / VData.EChargeTot);            //percentage of electric charge remaining
      sendToDisplay2(String("jel.val=") + String(vEL));
      //change color in gauges
      if (vEL >=0 && vEL < 60){
      sendToDisplay2(String("jel.bco=") + String(gaugeColorOK));
      }
      else if (vEL >=60 && vEL <= 90){
      sendToDisplay2(String("jel.bco=") + String(gaugeColorAlert));  
       }
      else {
      sendToDisplay2(String("jel.bco=") + String(gaugeColorCritical));  
      }
      //Numbers
      String strbattery = String(VData.ECharge,0);
      strbattery += "/";
      strbattery += String(VData.EChargeTot,0);
      sendToDisplay2(String("tel.txt=\"") + String(strbattery)+String("\""));
  }
  //Monopropellant
  if (VData.MonoPropTot <= 0){
     sendToDisplay2(String("jmp.val=") + String(100));  
  }else{
      //gauges
      byte vMP = 100-(100 * VData.MonoProp / VData.MonoPropTot);          //percentage of monopropellant remaining
      sendToDisplay2(String("jmp.val=") + String(vMP));       
      //change color in gauges  
      if (vMP >=0 && vMP < 60){
      sendToDisplay2(String("jmp.bco=") + String(gaugeColorOK));
      }
      else if (vMP >=60 && vMP <= 90){
      sendToDisplay2(String("jmp.bco=") + String(gaugeColorAlert));  
      }
      else {
      sendToDisplay2(String("jmp.bco=") + String(gaugeColorCritical));  
      }
      //Numbers
      String strMonopropellant = String(VData.MonoProp,0);
      strMonopropellant += "/";
      strMonopropellant += String(VData.MonoPropTot,0);
      sendToDisplay2(String("tmp.txt=\"") + String(strMonopropellant)+String("\""));
  }
  //Liquid Fuel
  if (VData.LiquidFuelTot <= 0){
     sendToDisplay2(String("jlf.val=") + String(100));  
  }else{
      //gauges
      byte vLF = 100-(100 * VData.LiquidFuel / VData.LiquidFuelTot);    //percentage of liquid fuel remaining in current stage
      sendToDisplay2(String("jlf.val=") + String(vLF));
      //change color in gauges
      if (vLF >=0 && vLF < 60){
      sendToDisplay2(String("jlf.bco=") + String(gaugeColorOK));
      }
      else if (vLF >=60 && vLF <= 90){
      sendToDisplay2(String("jlf.bco=") + String(gaugeColorAlert));  
      }
      else {
      sendToDisplay2(String("jlf.bco=") + String(gaugeColorCritical));  
      }
      //Numbers
      String strLiquidFuel = String(VData.LiquidFuel,0);
      strLiquidFuel += "/";
      strLiquidFuel += String(VData.LiquidFuelTot,0);
      sendToDisplay2(String("tlf.txt=\"") + String(strLiquidFuel)+String("\""));
  }
  //Oxidized
  if (VData.OxidizerTot <= 0){
     sendToDisplay2(String("jox.val=") + String(100));
  }else{
      //gauges
      byte vOX = 100-(100 * VData.Oxidizer / VData.OxidizerTot);        //percentage of oxidized remaining in current stage
      sendToDisplay2(String("jox.val=") + String(vOX));

      if (vOX >=0 && vOX < 60){
      sendToDisplay2(String("jox.bco=") + String(gaugeColorOK));
      }
      else if (vOX >=60 && vOX <= 90){
      sendToDisplay2(String("jox.bco=") + String(gaugeColorAlert));  
      }
      else {
      sendToDisplay2(String("jox.bco=") + String(gaugeColorCritical));  
      } 
      //Numbers
      String strOxidized = String(VData.Oxidizer,0);
      strOxidized += "/";
      strOxidized += String(VData.OxidizerTot,0);
      sendToDisplay2(String("tox.txt=\"") + String(strOxidized)+String("\""));              
  }
  //Solid Fuel
  if (VData.SolidFuelTot <= 0){
     sendToDisplay2(String("jsf.val=") + String(100));
  }else{
      //gauges
      byte vSF = 100-(100 * VData.SolidFuel / VData.SolidFuelTot);        //percentage of solid fuel remaining
      sendToDisplay2(String("jsf.val=") + String(vSF));
 
      if (vSF >=0 && vSF < 60){
      sendToDisplay2(String("jsf.bco=") + String(gaugeColorOK));
      }
      else if (vSF >=60 && vSF <= 90){
      sendToDisplay2(String("jsf.bco=") + String(gaugeColorAlert));  
      }
      else {
      sendToDisplay2(String("jsf.bco=") + String(gaugeColorCritical));  
      }  
      //Numbers
      String strSolidFuel = String(VData.SolidFuel,0);
      strSolidFuel += "/";
      strSolidFuel += String(VData.SolidFuelTot,0);
      sendToDisplay2(String("tsf.txt=\"") + String(strSolidFuel)+String("\""));        
  }
  //Xenon Gas
  if (VData.XenonGasTot <= 0){
     sendToDisplay2(String("jxe.val=") + String(100));
  }else{
      //gauges
      byte vXE = 100-(100 * VData.XenonGas / VData.XenonGasTot);          //percentage of xenon gas remaining
      sendToDisplay2(String("jxe.val=") + String(vXE));
      //change color in gauges
      if (vXE >=0 && vXE < 60){
      sendToDisplay2(String("jxe.bco=") + String(gaugeColorOK));
      }
      else if (vXE >=60 && vXE <= 90){
      sendToDisplay2(String("jxe.bco=") + String(gaugeColorAlert));  
      }
      else {
      sendToDisplay2(String("jxe.bco=") + String(gaugeColorCritical));  
      }
      //Numbers
      String strXenonGas = String(VData.XenonGas,0);
      strXenonGas += "/";
      strXenonGas += String(VData.XenonGasTot,0);
      sendToDisplay2(String("txe.txt=\"") + String(strXenonGas)+String("\""));           
  }
  //Intake Air
  if (VData.IntakeAirTot <= 0){
     sendToDisplay2(String("jair.val=") + String(100));
  }else{
      //gauges
      byte vAIR = 100-(100 * VData.IntakeAir / VData.IntakeAirTot);         //percentage of xenon gas remaining
      sendToDisplay2(String("jair.val=") + String(vAIR));  
      //change color in gauges
      if (vAIR >=0 && vAIR < 60){
      sendToDisplay2(String("jair.bco=") + String(gaugeColorOK));
      }
     else if (vAIR >=60 && vAIR <= 90){
      sendToDisplay2(String("jair.bco=") + String(gaugeColorAlert));  
      }
      else {
      sendToDisplay2(String("jair.bco=") + String(gaugeColorCritical));  
      }  
      //Numbers
      String strIntakeAir = String(VData.IntakeAir,0);
      strIntakeAir += "/";
      strIntakeAir += String(VData.IntakeAirTot,0);
      sendToDisplay2(String("tair.txt=\"") + String(strIntakeAir)+String("\""));                   
  }
  }  
    

  //Get all Data for Wavepage
  Altitude = VData.Alt;
  byte strSOINumber = VData.SOINumber;
  switch (strSOINumber){
      case 100:     //100 = SUN
          flyhigh = 500000;
          lowspace = 1600000;
          highspace = 3000000000;
      break;
      case 110:     //110 = MOHO
          flyhigh = 0;
          lowspace = 0;
          highspace = 325000;
      break;
      case 120:     //120 = EVE
          flyhigh = 27000;
          lowspace = 60000;
          highspace = 1025000;
      break;
      case 121:     //121 = GILLY
          flyhigh = 0;
          lowspace = 0;
          highspace = 20000;
      break; 
      case 130:     //130 = KERBIN
          flyhigh = 21000;
          lowspace = 85000;
          highspace = 800000;
      break;   
      case 131:     //131 = MUN
          flyhigh = 0;
          lowspace = 0;
          highspace = 200000;
      break;  
      case 132:     //132 = MINMUS
          flyhigh = 0;
          lowspace = 0;
          highspace = 80000;
      break;  
      case 140:     //140 = DUNA
          flyhigh = 10000;
          lowspace = 70000;
          highspace = 400000;
      break;  
      case 141:     //141 = IKE
          flyhigh = 0;
          lowspace = 0;
          highspace = 105000;
      break;  
      case 150:     //150 = DRES
          flyhigh = 0;
          lowspace = 0;
          highspace = 180000;
      break;  
      case 160:     //160 = JOOL
          flyhigh = 285000;
          lowspace = 700000;
          highspace = 7000000;
      break;  
      case 161:     //161 = LAYTHE
          flyhigh = 23000;
          lowspace = 75000;
          highspace = 550000;
      break;  
      case 162:     //162 = VALL
          flyhigh = 0;
          lowspace = 0;
          highspace = 275000;
      break;  
      case 163:     //163 = TYLO
          flyhigh = 11000;
          lowspace = 85000;
          highspace = 450000;
      break;  
      case 164:     //164 = BOP
          flyhigh = 0;
          lowspace = 0;
          highspace = 95000;
      break;  
      case 165:     //165 = POL
          flyhigh = 0;
          lowspace = 0;
          highspace = 65000;
      break;  
      case 170:     //170 = EELOO
          flyhigh = 10000;
          lowspace = 80000;
          highspace = 300000;
      break; 
      case 171:     //171 = CELES
          flyhigh = 0;
          lowspace = 0;
          highspace = 100000;
      break; 
      case 172:     //172 = TAM
          flyhigh = 0;
          lowspace = 0;
          highspace = 10000;
      break; 
      case 210:     //210 = EDNA
          flyhigh = 0;
          lowspace = 0;
          highspace = 130000;
      break; 
      case 211:     //211 = DAK
          flyhigh = 0;
          lowspace = 0;
          highspace = 15000;
      break; 
      case 220:     //220 = LINDOR
          flyhigh = 230000;
          lowspace = 540000;
          highspace = 4000000;
      break; 
      case 221:     //221 = KREL
          flyhigh = 0;
          lowspace = 0;
          highspace = 75000;
      break; 
      case 222:     //222 = ADEN
          flyhigh = 0;
          lowspace = 0;
          highspace = 150000;
      break; 
      case 223:     //223 = HUYGEN
          flyhigh = 75000;
          lowspace = 180000;
          highspace = 335000;
      break;            
      case 224:     //224 = RIGA
          flyhigh = 10000;
          lowspace = 90000;
          highspace = 375000;
      break;        
      case 225:     //225 = TALOS
          flyhigh = 0;
          lowspace = 0;
          highspace = 250000;
      break;        
      case 230:     //230 = HAMEK
          flyhigh = 0;
          lowspace = 0;
          highspace = 225000;
      break;        
      case 240:     //240 = NARA
          flyhigh = 85000;
          lowspace = 200000;
          highspace = 1800000;
      break;         
      case 241:     //241 = AMOS
          flyhigh = 0;
          lowspace = 0;
          highspace = 160000;
      break;         
      case 242:     //242 = ENON
          flyhigh = 0;
          lowspace = 0;
          highspace = 350000;
      break;         
      case 243:     //243 = PRAX
          flyhigh = 0;
          lowspace = 0;
          highspace = 55000;
      break;
      default:
          flyhigh = 0;
          lowspace = 0;
          highspace = 0;
      break;
     }  
}

void AtmosphereGauge (float DataAtmo){
//Atmosphere gauge

 //prepare the shift register
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
  digitalWrite(latchPin, LOW); 
  
  int vA = (12 - 6*log10(1 + 99*(DataAtmo / 6)));
  vA = (vA < 12) ? vA : 11;
  int powvA = 0.1+pow(2,vA);
    shiftOut(dataPin, clockPin, LSBFIRST, powvA<<4);
    shiftOut(dataPin, clockPin, LSBFIRST, powvA>>4);

  //latch the values in when done shifting
  digitalWrite(latchPin, HIGH); 
}
