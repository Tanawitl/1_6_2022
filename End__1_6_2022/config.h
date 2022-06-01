//จอTFT
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#include "TFT_A.h"

//วัดระยะ
#include <Wire.h>
#include <VL53L0X.h>
VL53L0X sensor;

//นับการทำงาน
unsigned long previousMillis = 0;         
int sec = 0, m = 0, h = 0;

//แกน
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

//ลำโพง
#include "SoundData.h"
#include "XT_DAC_Audio.h"

XT_Wav_Class ForceWithYou(Force);     // create an object of type XT_Wav_Class that is used by
XT_DAC_Audio_Class DacAudio(26, 0);   // Create the main player class object.
                                      // Use GPIO 25, one of the 2 DAC pins and timer 0

//หัวใจ
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;
#include"heartRate_A.h"

//ออกซิเจน
double avered = 0; double aveir = 0;
double sumirrms = 0;
double sumredrms = 0;
int i = 0;
int Num = 50;//calculate SpO2 by this sampling interval 100
double ESpO2 = 95.0;//initial value of estimated SpO2
double FSpO2 = 0.7; //filter factor for estimated SpO2
double frate = 0.95; //low pass filter for IR/red LED value to eliminate AC component

#define SCALE 88.0 //adjust to display heart beat and SpO2 in the same scale
#define SAMPLING 5 //if you want to see heart beat more precisely , set SAMPLING to 1
#define FINGER_ON 3000 // if red signal is lower than this , it indicates your finger is not on the sensor
//#define MINIMUM_SPO2 80.0
uint32_t ir, red , green;
#define USEFIFO

#define MINIMUM_SPO2 0.0
#define TIMETOBOOT 3000
int oxygen;

#include"spo2_A.h"







//เซนเซอร์วัอุณหภูมิ  GY-906 MLX90614 
/*
#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
*/






                                    
