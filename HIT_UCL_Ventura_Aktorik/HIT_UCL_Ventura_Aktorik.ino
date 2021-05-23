/* 
 * Titel: HIT_UCL_Ventura
 * 
 * Version: V1.0
 * Autor: Amor Azzabi
 * Datum: 14.05.2021
 *
 * Beschreibung:
 * Es handelt sich um ein Programm der data (Benutzer Eingabe und die Sensoren Werte) 
 * aus einem database liest und das in Umdrehungen umwandeln um die Aktoren von 
 * UCL_Ventura zu steuern damit die UCL _Ventura automatisiert wird und nicht mehr manuell
 * 
 * 
 */

#include <stdlib.h>
#include <HTTPClient.h>
#include "AccelStepper.h"
#include "ESP_Remote.h"
#include <WebSerial.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


#define FULLSTEP 4
#define HALFSTEP 8
#define PRESSURE_SENSOR "ams6915"  // Sensorname für Druck
#define O2_SENSOR       "nrcm04"   // Sensorname für Sauerstoff
#define FLOW_IN_SENSOR   "sfm3019"  // Sensorname für Durchfluss beim Einatem
#define FLOW_OUT_SENSOR  "sfm3200aw"// Sensorname für Durchfluss beim Ausatmen
#define ON_OFF_MAX_POSITION 2048 // 1 Umdrehung ' 2038
#define O2_MAX_POSITION 6144   // 3 Umdrehungen * 2038
#define FLOW_IN_MAX_POSITION 8192   // 4 Umdrehungen * 2038
#define O2_RANGE 79   // 100 -21
#define FLOW_IN_RANGE 250   // 250 -0
#define O2_MAX   100       
#define FLOW_IN_MAX   250
#define O2_MIN        21
#define FLOW_IN_MIN   0

#define STATE_ALL_OFF    0
#define STATE_ON    1



const char* ssid = "NETGEAR34"; // Wifi name
const char* password = "karlsruhe"; // Wifi password
char* ota_pass = "..."; // set ota access password

// Number of steps per internal motor revolution 
const float STEPS_PER_REV = 32; 
 
//  Amount of Gear Reduction
const float GEAR_RED = 64;
 
// Number of steps per geared output rotation
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;

// Number of Steps Required
int StepsRequired1 = 0;
int StepsRequired2 = 0;
int StepsRequired3 = 0;

float Flowin_SOLL = 0;
float O2_SOLL = 21;
float Flowin_IST = 0;
float O2_IST = 21;
float deltaO2=0;
float deltaflowin=0;
bool On_Off_SOLL = false;
bool On_Off_IST = false;

int State = 0; // state of the UCL Ventura

// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper stepper1(FULLSTEP, 2, 16, 4, 17);    // IN1, IN3, IN2, IN4
AccelStepper stepper2(FULLSTEP, 32, 25, 33, 26);  //IN1, IN3, IN2, IN4
AccelStepper stepper3(FULLSTEP, 22, 19,23 ,21 );  //IN1, IN3, IN2, IN4

//Start server
AsyncWebServer server(80);

//RemoteDebug Debug; // remote debug element

// Sensorenwerte aus dem Webserver Einlesen
bool readSensorData(float& mFloat,String mName);
// Eingabewerte aus dem Webserver Einlesen
bool readUserInput(bool& mON, float& mFlow_IN, float& mO2);
bool writeInputData(const float mFloat,const String mName);

// Die Knöpfe von UCL_Ventura drehen
bool flow_in_Button(float mFlow_IN);
bool o2_Button(float mO2);
bool on_off_Button(bool mON);

//function for Webserial message Callback
void recvMsg(uint8_t *data, size_t len)
{
    WebSerial.println("Received Data...");
    String d = "";
    for(int i=0; i < len; i++)
    {
      d += char(data[i]);
    }
    WebSerial.println(d);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
   /*******************************Remote Setup*************************/
  Easy_OTA_Setup(ssid, password, ota_pass);
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
  /**********************************my Setup code*********************/
  
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.println("Wifi connected...");
  Serial.println("initializing motors...");
  
  //Easy_OTA_Setup(ssid, password, ota_pass);

  // set the maximum speed, acceleration factor,
  // initial speed and the target position for motor 1
  //StepsRequired1  =  STEPS_PER_OUT_REV;
  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(50.0);
  stepper1.setSpeed(200);
  // Set Position to 0 : at start valve closed
  stepper1.setCurrentPosition(0);
  
  // set the same for motor 2 
  //StepsRequired2  =  STEPS_PER_OUT_REV;
  stepper2.setMaxSpeed(1000.0);
  stepper2.setAcceleration(50.0);
  stepper2.setSpeed(200);
  // Set Position to 0 : at start valve closed
  stepper2.setCurrentPosition(0);

  // set the same for motor 3
  //StepsRequired3  =  STEPS_PER_OUT_REV;
  stepper3.setMaxSpeed(1000.0);
  stepper3.setAcceleration(50.0);
  stepper3.setSpeed(200);
  // Set Position to 0 : at start valve closed
  stepper3.setCurrentPosition(0);

  Serial.println("Motors initialized!");
  //writeInputData(40,"o2_input");

}

// Da wir kein Gerät zum Ansatz haben, werden wir triviale Werte 
// für die Umdrehungen annehmen
// Wir nehmen an dass den ON/OFF Knopf eine ganze Umdrehung braucht, um dass 
// Gerät zu öffnen
// O2 Knopf: 3 Umdrehungen von min bis max
// Durchfluss Knopf: 4 Umdrehungen
void loop() 
{
  
   /*******************************Remote Handle************************/
  ArduinoOTA.handle();
  
  switch(State)
  {
    case STATE_ALL_OFF:
    readUserInput(On_Off_SOLL ,Flowin_SOLL, O2_SOLL);
    if(On_Off_SOLL)
    {
      State = STATE_ON;
      break;
    }
    if(On_Off_IST)
    {
      On_Off_IST = on_off_Button(On_Off_SOLL);
      o2_Button(O2_MIN);
      flow_in_Button(FLOW_IN_MIN);
    }
    break;

    case STATE_ON:
    readUserInput(On_Off_SOLL ,Flowin_SOLL, O2_SOLL);
    readSensorData(Flowin_IST, FLOW_IN_SENSOR);
    readSensorData(O2_IST, O2_SENSOR);
    if(!On_Off_SOLL)
    {
      State = STATE_ALL_OFF;
      break;
    }
    if(!On_Off_IST)
    {
      On_Off_IST = on_off_Button(On_Off_SOLL);
    }
    else
    {
      o2_Button(O2_SOLL);
      flow_in_Button(Flowin_SOLL);
    }
    break;

    default:
    State = STATE_ALL_OFF;
    break;

  }

  /*webserial monitor -> type in browser: <esp32 ip-adresss>/webserial*/
  WebSerial.println("Debug");
  
  delay(20);
}

// Funktion zum Lesen von Sensor Werte aus dem Database

bool readSensorData(float& mFloat,String mName)
{
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi not connected!");
    return false;
  }

  String url = "http://192.168.0.108:80/Readdata.php?Sensor=" + mName;
  HTTPClient http;
  http.begin(url);
  // Send HTTP get request
  int response = http.GET();

  // Check response (200 == OK)
  if(response > 0)
  {
    Serial.print("Sensor data read successfully: ");
    mFloat = (http.getString()).toFloat(); 
    Serial.print(mName);
    Serial.print(" : ");
    Serial.println(mFloat);
  }
  else
  {
    Serial.println("Error receiving Sensor data!");
    return false;
  }

  return true;
}

// Funktion zum Lesen von Benutzer Eingabe aus dem Database

bool readUserInput(bool& mON, float& mFlow_IN, float& mO2)
{
   if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi not connected!");
    return false;
  }

  String url = "http://192.168.0.108:80/Inputdata.php?User_Input=on_off";
  HTTPClient http;
  http.begin(url);
  // Send HTTP get request
  int response = http.GET();

  // Check response (200 == OK)
  if(response > 0)
  {
    Serial.print("on_off Input read successfully: ");
    mON = bool((http.getString()).toInt()); 
    Serial.println(mON);
  }
  else
  {
    Serial.println("Error receiving on_off Input data!");
    return false;
  }


  url = "http://192.168.0.108:80/Inputdata.php?User_Input=flow_in_input";
  http.begin(url);
  // Send HTTP get request
  response = http.GET();

  // Check response (200 == OK)
  if(response > 0)
  {
    Serial.print("flow_in Input read successfully: ");
    mFlow_IN = (http.getString()).toFloat(); 
    Serial.println(mFlow_IN);
  }
  else
  {
    Serial.println("Error receiving flow_in Input data!");
    return false;
  }

  url = "http://192.168.0.108:80/Inputdata.php?User_Input=o2_input";
  http.begin(url);
  // Send HTTP get request
  response = http.GET();

  // Check response (200 == OK)
  if(response > 0)
  {
    Serial.print("o2 Input read successfully: ");
    mO2 = (http.getString()).toFloat(); 
    Serial.println(mO2);
  }
  else
  {
    Serial.println("Error receiving o2 Input data!");
    return false;
  }

  return true;
}

// Fubktion zum Schreiben von benutzer Eingabe im Database

bool writeInputData(const float mFloat,const String mName)

{
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi not connected!");
    return false;
  }

  String url = "http://192.168.0.108:80/Inputdata.php?" + mName+"=" + mFloat;
  HTTPClient http;
  http.begin(url);
  // Send HTTP get request
  int response = http.GET();

  // Check response (200 == OK)
  if(response > 0)
  {
    Serial.print("Input data sent successfully: "); 
    Serial.println(http.getString());
  }
  else
  {
    Serial.println("Error sending Input data!");
    return false;
  }

  return true;
}

// Funktion um die Eingabe von Benutzer (On oder Off) in Steps (Umdrehungen)  Umzuwandeln

bool on_off_Button(bool mON)
{
 if (mON ==1)
  {
   StepsRequired1 = ON_OFF_MAX_POSITION;
   stepper1.moveTo(StepsRequired1);
   while (stepper1.distanceToGo() != 0) 
   {stepper1.run();}
   return true;
  }
 else
 {
   StepsRequired1 = 0;
   stepper1.moveTo(StepsRequired1);
   while (stepper1.distanceToGo() != 0 && stepper1.run()) 
    {;}
   return false;
 }
}

// Funktion um die Eingabe von Benutzer (O2 % ) in Steps (Umdrehungen)  Umzuwandeln

bool o2_Button(float mO2)
{
  
    StepsRequired2 = (O2_MAX_POSITION  * (mO2 - O2_MIN) / O2_RANGE);
    //StepsRequired2 = 2038;
    
   /* if (StepsRequired2 = stepper2.currentPosition() )
     {
      
       stepper2.move (0);
       while (stepper2.distanceToGo() != 0) 
       {stepper2.run();}
        
     
     }  
  
     else
      {*/
       //StepsRequired2 = StepsRequired2 - stepper2.currentPosition();
       stepper2.moveTo(StepsRequired2);
        while (stepper2.distanceToGo() != 0 && stepper2.run()) 
       {;}
     
      //}
       return true;
 }   

// Funktion um die Eingabe von Benutzer (Flow (slm^3) ) in Steps (Umdrehungen)  Umzuwandeln

bool flow_in_Button(float mFlow_IN)
{
  
    StepsRequired3 =  (FLOW_IN_MAX_POSITION * (mFlow_IN - FLOW_IN_MIN) / FLOW_IN_RANGE) ;
    //StepsRequired3 = 2038;
    
    /*if (StepsRequired3 = stepper3.currentPosition() )
     {
       stepper3.move (0);
       while (stepper3.distanceToGo() != 0) 
       {stepper3.run();}
     }  
    else 
     {*/
       //StepsRequired3 = StepsRequired3 - stepper3.currentPosition();
       stepper3.moveTo(StepsRequired3);
       while (stepper3.distanceToGo() != 0 && stepper3.run()) 
       {;}
     //}
     return true;
  
}
