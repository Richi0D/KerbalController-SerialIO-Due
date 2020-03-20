#include <Button.h>
#include <Keyboard.h>
#include <Joystick.h>
#include <Nextion.h>

//analog pins
const byte pTHROTTLE = A0; //slide pot
const byte pTX = A2;       //translation x-axis
const byte pTY = A1;       //translation y-axis
const byte pTZ = A3;       //translation z-axis
const byte pRX = A5;       //rotation x-axis
const byte pRY = A4;       //rotation y-axis
const byte pRZ = A6;       //rotation z-axis

//digital pins
const byte dataPin = 2;      //DS - blue
const byte clockPin = 3;     //SH_CP - yellow
const byte latchPin = 4;     //ST_CP - pink
const byte motoron = 5;      //Motor on pin (L293D pin 1)
const byte motorup = 6;      //Motor up pin
const byte motordown = 7;   //Motor down pin 
const byte maxthrottle = 9;  //Max Throttle button
const byte minthrottle = 8;  //Min Throttle button
const byte pMODE = 51;       //mode switch (used for debug mode)
//Display 2 RX1  blue
//Display 2 TX1  yellow
//Display 1 RX2  blue
//Display 1 TX2  yellow
const byte pABORT = 47;      //Abort switch (safety switch, active high)
const byte pARM = 49;        //Arm switch (safety switch, active high)
const byte pSTAGE = 10;      //Stage button
const byte pSAS = 46;        //SAS switch
const byte pRCS = 48;        //RCS switch
const byte pprecision = 50;  //Precision Control Switch
const byte pLIGHTS = 45;     //Lights button
const byte pLIGHTSLED = 44;  //Lights button LED
const byte pBRAKES = 43;     //Brakes button
const byte pBRAKESLED = 42;  //Brakes button LED
const byte pGEARS = 41;      //Gears button
const byte pGEARSLED = 40;   //Gears button LED
const byte pSOLAR = 39;      //Solar button (action group 9)
const byte pSOLARLED = 38;   //Solar button LED
const byte pLADDER = 35;     //Ladder button (action group 7)
const byte pLADDERLED = 34;  //Ladder button LED
const byte pCHUTES = 37;     //Chutes button (action group 8)
const byte pCHUTESLED = 36;  //Chutes button LED
const byte pACTION6 = 33;    //Action Group 6 button
const byte pACTION6LED = 32; //Action Group 6 button LED
const byte pACTION5 = 31;    //Action Group 5 button
const byte pACTION5LED = 30; //Action Group 5 button LED
const byte pACTION4 = 29;    //Action Group 4 button
const byte pACTION4LED = 28; //Action Group 4 button LED
const byte pACTION3 = 27;    //Action Group 3 button  
const byte pACTION3LED = 26; //Action Group 3 button LED
const byte pACTION2 = 25;    //Action Group 2 button
const byte pACTION2LED = 24; //Action Group 2 button LED
const byte pACTION1 = 23;    //Action Group 1 button
const byte pACTION1LED = 22; //Action Group 1 button LED
const byte pmodeswitch = 52; //rocket mode or plane mode switch
const byte pswitch1 = 62;    //A8 Switch position 1, display mode orbit
const byte pswitch2 = 63;    //A9 Switch position 2, display mode surface
const byte pswitch3 = 64;    //A10 Switch position 3, display mode target/node
const byte pswitch4 = 65;    //A11 Switch position 4, display mode Body Info

//variables used for detect bounce buttons
Button debouncerStage(pSTAGE);
Button debouncerLights(pLIGHTS);
Button debouncerBrakes(pBRAKES);
Button debouncerGears(pGEARS);
Button debouncerSolar(pSOLAR);
Button debouncerChutes(pCHUTES);
Button debouncerLadder(pLADDER);
Button debouncerA6(pACTION6);
Button debouncerA5(pACTION5);
Button debouncerA4(pACTION4);
Button debouncerA3(pACTION3);
Button debouncerA2(pACTION2);
Button debouncerA1(pACTION1);

// Create a joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,
  0, 0,                  // Button Count, Hat Switch Count
  true, true, true,      // X, Y, Z Axis
  true, true, true,      // Rx, Ry, Rz Axis
  false, true,           // rudder, throttle
  false, false, false);  // accelerator, brake, steering

//Enumeration of MainControls
#define SAS       7
#define RCS       6
#define LIGHTS    5
#define GEARS     4
#define BRAKES    3
#define PRECISION 2
#define ABORT     1
#define STAGE     0
//Enumeration of ActionGroups (includes main controls and custom action groups)
#define AGSAS      0
#define AGRCS      1
#define AGLight    2
#define AGGears    3
#define AGBrakes   4
#define AGAbort    5
#define AGCustom01 6
#define AGCustom02 7
#define AGCustom03 8
#define AGCustom04 9
#define AGCustom05 10
#define AGCustom06 11
#define AGCustom07 12
#define AGCustom08 13
#define AGCustom09 14
#define AGCustom10 15
//Enumeration of SAS Modes
#define SMOFF           0
#define SMSAS           1
#define SMPrograde      2
#define SMRetroGrade    3
#define SMNormal        4
#define SMAntinormal    5
#define SMRadialIn      6
#define SMRadialOut     7
#define SMTarget        8
#define SMAntiTarget    9
#define SMManeuverNode  10
//Enumeration of Navball Target Modes
#define NAVBallIGNORE   0
#define NAVBallORBIT    1
#define NAVBallSURFACE  2
#define NAVBallTARGET   3

//macro used to generate the control packet (also used for the handshake packet)
#define details(name) (uint8_t*)&name,sizeof(name)

//debug variable
bool debug = false;
int chooseled = 0;

//variables used in timing
const int IDLETIMER = 4000;        //if no message received from KSP for more than 5s, go idle (default 2000)
const int CONTROLREFRESH = 100;      //send control packet every 20 ms (default 25)
const int DISPLAYREFRESH = 100;      //refresh Display every 50 ms = 20fps
unsigned long deadtime, deadtimeOld, controlTime, controlTimeOld, displayTime, displayTimeOld = 0;
unsigned long debugtime, debugtimeOld =0, atmotime, atmotimeOld =0;
unsigned long now = 0;

//variables used in serial communication
boolean Connected = false;
byte id =0;
//variables used for display status
byte PageDisplay1 = 0;              //Page display1
byte PageDisplay2 = 0;              //Page display2 
//variables used for motorfader
int throttletarget;
int theThreshold = 10;


struct __attribute__((__packed__)) HandShakePacket
{
  byte id;
  byte M1;
  byte M2;
  byte M3;
};
//define the structure of a VesseData packet
struct __attribute__((__packed__)) VesselData
{
    byte id;                //1
    float AP;               //2
    float PE;               //3
    float SemiMajorAxis;    //4
    float SemiMinorAxis;    //5
    float VVI;              //6
    float e;                //7
    float inc;              //8
    float G;                //9
    long TAp;               //10
    long TPe;               //11
    float TrueAnomaly;      //12
    float Density;          //13
    long period;            //14
    float RAlt;             //15
    float Alt;              //16
    float Vsurf;            //17
    float Lat;              //18
    float Lon;              //19
    float LiquidFuelTot;    //20
    float LiquidFuel;       //21
    float OxidizerTot;      //22
    float Oxidizer;         //23
    float EChargeTot;       //24
    float ECharge;          //25
    float MonoPropTot;      //26
    float MonoProp;         //27
    float IntakeAirTot;     //28
    float IntakeAir;        //29
    float SolidFuelTot;     //30
    float SolidFuel;        //31
    float XenonGasTot;      //32
    float XenonGas;         //33
    float LiquidFuelTotS;   //34
    float LiquidFuelS;      //35
    float OxidizerTotS;     //36
    float OxidizerS;        //37
    uint32_t MissionTime;   //38
    float deltaTime;        //39
    float VOrbit;           //40
    uint32_t MNTime;        //41
    float MNDeltaV;         //42
    float Pitch;            //43
    float Roll;             //44
    float Heading;          //45
    uint16_t ActionGroups;  //46  status bit order:SAS, RCS, Light, Gear, Brakes, Abort, Custom01 - 10
    byte SOINumber;         //47  SOI Number (decimal format: sun-planet-moon e.g. 130 = kerbin, 131 = mun)
    byte MaxOverHeat;       //48  Max part overheat (% percent)
    float MachNumber;       //49
    float IAS;              //50  Indicated Air Speed
    byte CurrentStage;      //51  Current stage number
    byte TotalStage;        //52  TotalNumber of stages
    float TargetDist;       //53  Distance to targeted vessel (m)
    float TargetV;          //54  Target vessel relative velocity
    byte NavballSASMode;    //55  Combined byte for navball target mode and SAS mode
                                    // First four bits indicate AutoPilot mode:
                                    // 0 SAS is off  //1 = Regular Stability Assist //2 = Prograde
                                    // 3 = RetroGrade //4 = Normal //5 = Antinormal //6 = Radial In
                                    // 7 = Radial Out //8 = Target //9 = Anti-Target //10 = Maneuver node
                                    // Last 4 bits set navball mode. (0=ignore,1=ORBIT,2=SURFACE,3=TARGET)
    int16_t ProgradePitch;  //56 Pitch   Of the Prograde Vector;  int_16 ***Changed: now fix point, actual angle = angle/50*** used to be (-0x8000(-360 degrees) to 0x7FFF(359.99ish degrees)); 
    int16_t ProgradeHeading;//57 Heading Of the Prograde Vector;  see above for range   (Prograde vector depends on navball mode, eg Surface/Orbit/Target)
    int16_t ManeuverPitch;  //58 Pitch   Of the Maneuver Vector;  see above for range;  (0 if no Maneuver node)
    int16_t ManeuverHeading;//59 Heading Of the Maneuver Vector;  see above for range;  (0 if no Maneuver node)
    int16_t TargetPitch;    //60 Pitch   Of the Target   Vector;  see above for range;  (0 if no Target)
    int16_t TargetHeading;  //61 Heading Of the Target   Vector;  see above for range;  (0 if no Target)
    int16_t NormalHeading;  //62 Heading Of the Prograde Vector;  see above for range;  (Pitch of the Heading Vector is always 0)                                 
};

//define the structure of a control packet
struct __attribute__((__packed__)) ControlPacket {
  //the following controls can be sent to the KSPSerialIO plugin (defined by the plugin)
  byte id;
  byte MainControls;                  //SAS RCS Lights Gears Brakes Precision Abort Stage (see enum)
  byte Mode;                          //0 = stage, 1 = docking, 2 = map
  uint16_t ControlGroup;              //action groups 1-10
  byte NavballSASMode;                //AutoPilot mode
  byte AdditionalControlByte1;
  int16_t Pitch;                          //-1000 -> 1000
  int16_t Roll;                           //-1000 -> 1000
  int16_t Yaw;                            //-1000 -> 1000
  int16_t TX;                             //-1000 -> 1000
  int16_t TY;                             //-1000 -> 1000
  int16_t TZ;                             //-1000 -> 1000
  int16_t WheelSteer;                     //-1000 -> 1000
  int16_t Throttle;                       //    0 -> 1000
  int16_t WheelThrottle;                  //    0 -> 1000
};

//Create an instance of a handshake packet
HandShakePacket HPacket;
//create an instance of a VesselData object
VesselData VData;
//Create an instance of a control packet
ControlPacket CPacket;

//in-game state used to update button LEDs
byte SASval;
byte SASset;
bool SAS_on = false;
bool RCS_on = false;
bool abort_on = false;
bool lights_on = false;
bool ladder_on = false;
bool solar_on = false;
bool gears_on = false;
bool brakes_on = false;
bool chutes_on = false;
bool stage_on = false;
bool action1_on = false;
bool action2_on = false;
bool action3_on = false;
bool action4_on = false;
bool action5_on = false;
bool action6_on = false;

//input value variables
int throttle_value;
int tx_value;
int ty_value;
int tz_value;
int rx_value;
int ry_value;
int rz_value;
int flymode = 0;

// Declare objects that we are going to read from the Nextion display1.
// Format: <type of object> <object name> = <type of object>(<page id>, <object id>, "<object name>");
NexButton warpminus = NexButton(2, 2, "warpminus");
NexButton warpstop = NexButton(2, 3, "warpstop");
NexButton warpplus = NexButton(2, 4, "warpplus");
NexButton vesselprev = NexButton(2, 7, "vesselprev");
NexButton mapview = NexButton(2, 6, "mapview");
NexButton vesselnext = NexButton(2, 8, "vesselnext");
NexButton cameraiva = NexButton(2, 10, "cameraiva");
NexButton camerareset = NexButton(2, 11, "camerareset");
NexButton cameranext = NexButton(2, 12, "cameranext");
NexButton evaboard = NexButton(3, 2, "evaboard");
NexButton evause = NexButton(3, 3, "evause");
NexDSButton evarun = NexDSButton(3, 7, "evarun");
NexButton evarcs = NexButton(3, 4, "evarcs");
NexButton evahelmet = NexButton(3, 5, "evahelmet");
NexButton evachute = NexButton(3, 6, "evachute");
NexDSButton dsoff = NexDSButton(4, 2, "dsoff");
NexDSButton dsman = NexDSButton(4, 7, "dsman");
NexDSButton dspro = NexDSButton(4, 3, "dspro");
NexDSButton dsret = NexDSButton(4, 8, "dspro");
NexDSButton dsnorm = NexDSButton(4, 4, "dsnorm");
NexDSButton dsanti = NexDSButton(4, 9, "dsanti");
NexDSButton dsradout = NexDSButton(4, 5, "dsradout");
NexDSButton dsradin = NexDSButton(4, 10, "dsradin");
NexDSButton dstarg = NexDSButton(4, 6, "dstarg");
NexDSButton dsrettarg = NexDSButton(4, 11, "dsrettarg");
NexHotspot zoommin = NexHotspot(5, 11, "zoommin");
NexHotspot zoomplus = NexHotspot(5, 12, "zoomplus");

// Declare touch event objects to the touch event list: 
// Format: &<object name>,
NexTouch *nex_listen_list[] = 
{
  &warpminus,
  &warpstop,
  &warpplus,
  &vesselprev,
  &mapview,
  &vesselnext,      
  &cameraiva,
  &camerareset,
  &cameranext,
  &evaboard,
  &evause,
  &evarun,
  &evarcs,
  &evahelmet,
  &evachute,  
  &dsoff,
  &dsman,
  &dspro,
  &dsret,
  &dsnorm,
  &dsanti,
  &dsradout,
  &dsradin,
  &dstarg,
  &dsrettarg,
  &zoommin,
  &zoomplus,  
  NULL  // String terminated
};



void setup() {  // put your setup code here, to run once:

  Serial.begin(115200);   //KSPSerialIO connection
  Serial1.begin(115200);  //Display1 connection
  Serial2.begin(115200);  //Display2 connection
      PageDisplay1 = 0;
      PageDisplay2 = 0; 
      sendToDisplay1(String("page ") + String(PageDisplay1));
      sendToDisplay1(String("boot.txt=\"") + String("booting")+String("\""));
      sendToDisplay2(String("page ") + String(PageDisplay2));
      sendToDisplay2(String("boot.txt=\"") + String("booting")+String("\""));

//Initialize  
  controlsInit();         //set all pin modes
  testLEDS(50);         //blink every LED once to test (with a delay of xx ms)
  InitTxPackets();        //initialize the serial communication
  registernextionevent(); //register event callback function for nextion buttons  
  Keyboard.begin();       // initialize control over the keyboard: 
  Joystick.begin(false);  // Initialize Joystick Library   
    // Set Range Values
    Joystick.setXAxisRange(0, 1023);
    Joystick.setYAxisRange(0, 1023);
    Joystick.setZAxisRange(0, 1023);
    Joystick.setRxAxisRange(0, 1023);
    Joystick.setRyAxisRange(0, 1023);
    Joystick.setRzAxisRange(0, 1023);
    Joystick.setThrottleRange(0, 1023);
 
      PageDisplay1 = 0;
      PageDisplay2 = 0; 
      sendToDisplay1(String("page ") + String(PageDisplay1));
      sendToDisplay1(String("boot.txt=\"") + String("booting complete")+String("\""));
      sendToDisplay2(String("page ") + String(PageDisplay2));
      sendToDisplay2(String("boot.txt=\"") + String("booting complete")+String("\""));
      SASset = 1;
}

void loop() {  // put your main code here, to run repeatedly:

  //check if debug is on 
  while(!digitalRead(pMODE)){debugfunction();} 

  //KSP mode: send and receive data  

  send_control_packet();      // set up controls
  get_vessel_data();          // update Displays and get Data  
  Joystick.sendState();       // send Joystick inputs
  nexLoop(nex_listen_list);   // Check for any touch event  
  wave();                     // save Data and build wave
  }
