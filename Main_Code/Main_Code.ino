//Suhu
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// Setup a oneWire instance to communicate with any OneWire devices


//GSM
#include <SoftwareSerial.h>
SoftwareSerial sim808(3,4);   //3 to TX , 4 to RX

//pH
#define SensorPin A3            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;



//DO
#include <Arduino.h>
#define VREF 5000    //VREF (mv)
#define ADC_RES 1024 //ADC Resolution

//Single-point calibration Mode=0
//Two-point calibration Mode=1
#define TWO_POINT_CALIBRATION 0

#define READ_TEMP (nilaiTempr()) //Current water temperature ℃, Or temperature sensor function


//Single point calibration needs to be filled CAL1_V and CAL1_T
#define CAL1_V (1490) //mv
#define CAL1_T (25)   //℃

const uint16_t DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};

uint8_t Temperaturet;
uint16_t ADC_Raw;
uint16_t DO;

int16_t readDO(uint16_t raw, uint8_t temperature)
{
#if TWO_POINT_CALIBRATION == 0
  uint16_t V_saturation = (uint32_t)CAL1_V + (uint32_t)35 * temperature - (uint32_t)CAL1_T * 35;
  return (uint64_t(VREF) * DO_Table[temperature] * raw) / (uint32_t(ADC_RES) * V_saturation);
#else
  uint16_t V_saturation = (int16_t)((int8_t)temperature - CAL2_T) * ((uint16_t)CAL1_V - CAL2_V) / ((uint8_t)CAL1_T - CAL2_T)+CAL2_V;
  return (uint64_t(VREF) * DO_Table[temperature] * raw) / (uint32_t(ADC_RES) * V_saturation);
#endif
}

//OLED
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 

int Powerkey = 9; //To turn on the GSM Module

String  teks;
String kirimData;

//Init Sensors' Variable
int valDO;
float valPH;
float valTEMPR;


void setup() {
  Serial.begin(9600);
  
  //init oled
  initOLED();
  
  //init gsm
  pinMode(Powerkey, OUTPUT);   // initialize the digital pin as an output.  
  power();                     //power on the sim808 or power down the sim808

  //init sensors
  sensors.begin();
  //ph.begin();

  //init sim808
  sim808.begin(9600);

  //init connection
  Serial.println("Establishing Connection");
  delay(2000);
  Serial.println("Connecting");
  sendData("AT",2000);
  sendData("AT",2000);
  sendData("AT+CGATT=1",2000);
  sendData("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"",2000);
  sendData("AT+SAPBR=3,1,\"APN\",\"internet\"",2000);
  sendData("AT+SAPBR=2,1",2000);
  sendData("AT+SAPBR=1,1",2000);
  Serial.println("Connection Established");
    
}


void sendData (String command , const int timeout){
  String response = "";
  sim808.println(command);
  long int time = millis();
  int i = 0;
  while ( (time+timeout ) > millis()){
    while (sim808.available()){
      char c = sim808.read();
      response +=c;
    }

  }

  Serial.println(response);
}


void power(void)
{
  digitalWrite(Powerkey, LOW); 
  delay(4000);               // wait for 1 second
  digitalWrite(Powerkey, HIGH);
}



void loop() {
  //gathering sensors value
  Serial.println("Collecting data....");
  delay(500);
  Serial.println("");
  sensors.requestTemperatures();
  valDO = nilaiDO();
  valPH = nilaiPH();
  valTEMPR = nilaiTempr(); 
  
  //live update on oled
  exOLED();

  //sending sensors' value to database
  Serial.println("Sending data....");
  kirimData = String("{") + String("\"val_do\":") + valDO + String(",\"val_ph\":") + valPH + String(",\"val_tempr\":") + valTEMPR + String('}');
  sendData("AT+HTTPINIT",2000);
  sendData("AT+HTTPPARA=\"CID\",1",2000);
  sendData("AT+HTTPPARA=\"URL\",\"https://smartpond-dashboard.firebaseio.com/smartpond-dashboard.json\"",2000);
  sendData("AT+HTTPSSL=1",2000);
  sendData("AT+HTTPPARA=\"CONTENT\",\"application/json\"",2000);
  sendData("AT+HTTPDATA="+String(kirimData.length())+",10000",5000);
  sendData(kirimData, 5000);
  sendData("AT+HTTPACTION=1",2000);
  sendData("AT+HTTPREAD=0,20",2000);
  sendData("AT+HTTPTERM",2000);
  Serial.println("Data sent!");
  Serial.println(kirimData);
  delay(15000);

}
