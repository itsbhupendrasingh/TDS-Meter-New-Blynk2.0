
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "write your Blynk template ID here"
#define BLYNK_DEVICE_NAME "Write your B Device ID here"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#define USE_WROVER_BOARD
//#define BLYNK_DEBUG
//#define APP_DEBUG

#include "BlynkEdgent.h"

int DSPIN = 13; //  connect Dallas Temperature Sensor to 13 no pin of ESP32 board
int TDS_Sensor = 34; // connect TDS Sensor to 34 no pin of ESP32 board
float Aref = 3.3;

float ec_Val = 0;
unsigned int tds_value = 0;
float ecCal = 1;
BlynkTimer timer;

void sendSensor()
{
  double wTemp = TempRead()* 0.0625;  // conversion accuracy is 0.0625 / LSB
  float V_level= Aref / 1024.0;
  float rawEc = analogRead(TDS_Sensor) * V_level;  // Raw  data of EC
  float T_Cof = 1.0 + 0.02 * (wTemp - 25.0);  // Temperature Coefficient
  
  ec_Val = (rawEc / T_Cof) * ecCal;// temperature and calibration compensation
  tds_value = (133.42 * pow(ec_Val, 3) - 255.86 * ec_Val * ec_Val + 857.39 * ec_Val) * 0.5; 
  double Far= (((wTemp *9)/5) + 32); // Temp in Far*
  
  Serial.print("TDS:"); Serial.println(tds_value);
  Serial.print("EC:"); Serial.println(ec_Val, 2);
  Serial.print("Temperature (oC):"); Serial.println(wTemp,2);   
  Serial.print("Temperature (oF):"); Serial.println(Far,2);
  Serial.print("");
 
  Blynk.virtualWrite(V0, wTemp);
  Blynk.virtualWrite(V1, Far);
  Blynk.virtualWrite(V2, tds_value);
  Blynk.virtualWrite(V3, ec_Val);
}

void setup()
{
  Serial.begin(9600);
  delay(100);
  BlynkEdgent.begin();
  delay(2000); 
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  BlynkEdgent.run();
  timer.run();
}
