const int wavesaveinterval = 1000;   //Saveinterval of the wave
unsigned long wavetime, wavetimeold = 0;
int startcounter, counter = 0;
float saveline1[320];  //Array size = size of wave length(each pixel is a value)
long saveline2[320];  //Array size = size of wave length(each pixel is a value)
long saveline3[320];  //Array size = size of wave length(each pixel is a value)
long saveline4[320];  //Array size = size of wave length(each pixel is a value)
long line1, line2, line3, line4;
long wavescale;
int zoomlevel=1;
int pixelhighwave = 190; //high of wave in pixel
boolean wavebuilt = false;
float Altitude;
unsigned long flyhigh;
unsigned long lowspace;
unsigned long highspace;

void sendToDisplay1(String command)   //send Data to Display 1
{
  Serial2.print(command);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
} 
void sendToDisplay2(String command)   //send Data to Display 2
{
  Serial1.print(command);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
} 

void registernextionevent() //register event callback function for nextion buttons
// Format for press events: <object name>.attachPush(<object name>PushCallback);
// Format for release events: <object name>.attachPop(<object name>PopCallback);
{
  warpminus.attachPush(warpminusPushCallback); 
  warpstop.attachPush(warpstopPushCallback); 
  warpplus.attachPush(warpplusPushCallback); 
  vesselprev.attachPush(vesselprevPushCallback); 
  mapview.attachPush(mapviewPushCallback); 
  vesselnext.attachPush(vesselnextPushCallback); 
  cameraiva.attachPush(cameraivaPushCallback); 
  camerareset.attachPush(cameraresetPushCallback); 
  cameranext.attachPush(cameranextPushCallback); 
  evaboard.attachPush(evaboardPushCallback); 
  evause.attachPush(evausePushCallback); 
  evarun.attachPush(evarunPushCallback);
  evarcs.attachPush(evarcsPushCallback);
  evahelmet.attachPush(evahelmetPushCallback);
  evachute.attachPush(evachutePushCallback);
  dsoff.attachPush(dsoffPushCallback);
  dsman.attachPush(dsmanPushCallback);
  dspro.attachPush(dsproPushCallback);
  dsret.attachPush(dsretPushCallback);
  dsnorm.attachPush(dsnormPushCallback);
  dsanti.attachPush(dsantiPushCallback);
  dsradout.attachPush(dsradoutPushCallback);
  dsradin.attachPush(dsradinPushCallback);
  dstarg.attachPush(dstargPushCallback);
  dsrettarg.attachPush(dsrettargPushCallback);
  zoommin.attachPush(zoomminPushCallback);
  zoomplus.attachPush(zoomplusPushCallback);
}

// Events for button
void warpminusPushCallback(void *ptr){Keyboard.write(44);}
void warpstopPushCallback(void *ptr){Keyboard.write(47);}
void warpplusPushCallback(void *ptr){Keyboard.write(46);}
void vesselprevPushCallback(void *ptr){Keyboard.write(123);}
void mapviewPushCallback(void *ptr){Keyboard.write('M');}
void vesselnextPushCallback(void *ptr){Keyboard.write(93);}
void cameraivaPushCallback(void *ptr){Keyboard.write('C');}
void cameraresetPushCallback(void *ptr){Keyboard.write(96);}
void cameranextPushCallback(void *ptr){Keyboard.write('V');}
void evaboardPushCallback(void *ptr){Keyboard.write('B');}
void evausePushCallback(void *ptr){Keyboard.write('F');}
void evarunPushCallback(void *ptr){
  uint32_t valrun = 0;       // Create variable to store value we are going to get
  evarun.getValue(&valrun);  // Read value of dual state button to know the state (0 or 1)
  if (valrun == 1){Keyboard.press(129);}
  else {Keyboard.release(129);}
  }
void evarcsPushCallback(void *ptr){Keyboard.write('R');}
void evahelmetPushCallback(void *ptr){Keyboard.write('O');}
void evachutePushCallback(void *ptr){Keyboard.write('P');}
void dsoffPushCallback(void *ptr){
  uint32_t valrun = 0;       // Create variable to store value we are going to get
  dsoff.getValue(&valrun);  // Read value of dual state button to know the state (0 or 1)
  if (valrun == 1){SASset=1;}
  else {SASset=1;}
  }
void dsmanPushCallback(void *ptr){
  uint32_t valrun = 0;       // Create variable to store value we are going to get
  dsman.getValue(&valrun);  // Read value of dual state button to know the state (0 or 1)
  if (valrun == 1){SASset=10;}
  else {SASset=1;}
  }
void dsproPushCallback(void *ptr){
  uint32_t valrun = 0;       // Create variable to store value we are going to get
  dspro.getValue(&valrun);  // Read value of dual state button to know the state (0 or 1)
  if (valrun == 1){SASset=2;}
  else {SASset=1;}
  }
void dsretPushCallback(void *ptr){
  uint32_t valrun = 0;       // Create variable to store value we are going to get
  dsret.getValue(&valrun);  // Read value of dual state button to know the state (0 or 1)
  if (valrun == 1){SASset=3;}
  else {SASset=1;}
  }
void dsnormPushCallback(void *ptr){
  uint32_t valrun = 0;       // Create variable to store value we are going to get
  dsnorm.getValue(&valrun);  // Read value of dual state button to know the state (0 or 1)
  if (valrun == 1){SASset=4;}
  else {SASset=1;}
  }
void dsantiPushCallback(void *ptr){
  uint32_t valrun = 0;       // Create variable to store value we are going to get
  dsanti.getValue(&valrun);  // Read value of dual state button to know the state (0 or 1)
  if (valrun == 1){SASset=5;}
  else {SASset=1;}
  }
void dsradoutPushCallback(void *ptr){
  uint32_t valrun = 0;       // Create variable to store value we are going to get
  dsradout.getValue(&valrun);  // Read value of dual state button to know the state (0 or 1)
  if (valrun == 1){SASset=7;}
  else {SASset=1;}
  }
void dsradinPushCallback(void *ptr){
  uint32_t valrun = 0;       // Create variable to store value we are going to get
  dsradin.getValue(&valrun);  // Read value of dual state button to know the state (0 or 1)
  if (valrun == 1){SASset=6;}
  else {SASset=1;}
  }
void dstargPushCallback(void *ptr){
  uint32_t valrun = 0;       // Create variable to store value we are going to get
  dstarg.getValue(&valrun);  // Read value of dual state button to know the state (0 or 1)
  if (valrun == 1){SASset=8;}
  else {SASset=1;}
  }
void dsrettargPushCallback(void *ptr){
  uint32_t valrun = 0;       // Create variable to store value we are going to get
  dsrettarg.getValue(&valrun);  // Read value of dual state button to know the state (0 or 1)
  if (valrun == 1){SASset=9;}
  else {SASset=1;}
  }
void zoomminPushCallback(void *ptr){if (zoomlevel > 0){zoomlevel--; buildwave();}}
void zoomplusPushCallback(void *ptr){if (zoomlevel < 3){zoomlevel++; buildwave();}}


//build the wave according to the zoomlevel
void buildwave(){

    switch(zoomlevel){
    case 0: //Zoomlevel Fly High
      if (flyhigh == 0){wavescale = 30000;}
      else {wavescale = flyhigh*1.5;}
    break;
    case 1: //Zoomlevel Low space
      if (lowspace == 0){wavescale = 100000;}
      else {wavescale = lowspace*1.5;}
    break;
    case 2: //Zoomlevel High space
      if (highspace <= 300000){wavescale = 500000;}
      else {wavescale = highspace*1.5;}
    break;
    case 3: //Zoomlevel High++ space
      if (highspace <= 300000){wavescale = 2000000;}
      else {wavescale = highspace*3;}
    break;    
    }
    
/*    //nicht gut, füllt zu viel den Buffer.
    startcounter = counter;       
    for (int u = 0; u <= 319; u++){ //build wave, u = size of wave length -1 in pixel 
      if (startcounter>319){startcounter = 0;}

        line1 = map(saveline1[startcounter],0,wavescale,0,pixelhighwave);
        line1 = constrain(line1,0,pixelhighwave);
        sendToDisplay1(String("add 5,0,") + String(line1));   //Waveform: [objectID],[Channel],[Value]        

          line2 = map(saveline2[startcounter],0,wavescale,0,pixelhighwave);
          line2 = constrain(line2,0,pixelhighwave);
          sendToDisplay1(String("add 5,1,") + String(line2));        

          line3 = map(saveline3[startcounter],0,wavescale,0,pixelhighwave);
          line3 = constrain(line3,0,pixelhighwave);
          sendToDisplay1(String("add 5,2,") + String(line3));        

          line4 = map(saveline4[startcounter],0,wavescale,0,pixelhighwave);
          line4 = constrain(line4,0,pixelhighwave);
          sendToDisplay1(String("add 5,3,") + String(line4));        
      
      startcounter++;                
      } 
      */    
}

//Save values while display not active, if display active build wave.
void wave(){
now = millis();
wavetime = now - wavetimeold; 
  if (wavetime > wavesaveinterval) {
    wavetimeold = now; 
   
    if (counter>319){counter=0;} 
    saveline1[counter]=Altitude;     //Save values from Altidude
    saveline2[counter]=flyhigh; 
    saveline3[counter]=lowspace;
    saveline4[counter]=highspace;
      
    if (PageDisplay1==5)
    {
      if (not wavebuilt){buildwave();} //Build wave when first time loaded
      wavebuilt = true;

      line1 = map(Altitude,0,wavescale,0,pixelhighwave);
      line1 = constrain(line1,0,pixelhighwave);
      sendToDisplay1(String("add 5,0,") + String(line1));   //Waveform: [objectID],[Channel],[Value]

   
          line2 = map(flyhigh,0,wavescale,0,pixelhighwave);
          line2 = constrain(line2,0,pixelhighwave);
          sendToDisplay1(String("add 5,1,") + String(line2));
         
          line3 = map(lowspace,0,wavescale,0,pixelhighwave);
          line3 = constrain(line3,0,pixelhighwave);
          sendToDisplay1(String("add 5,2,") + String(line3));
        
          line4 = map(highspace,0,wavescale,0,pixelhighwave);
          line4 = constrain(line4,0,pixelhighwave);     
          sendToDisplay1(String("add 5,3,") + String(line4));
 
    } 
    else{wavebuilt = false;}
    counter++; //add +1 to counter 
  }
}
