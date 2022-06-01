#include <WiFi.h>
#include <MicroGear.h>
const char* ssid     = "bank_2G";  //ชื่อ wifi
const char* password = "bank10086";  //รหัสผ่าน wifi

#define APPID   "IOTwifi"                    //ใส่ appid จากเว็บ
#define KEY     "vTGG1Fp5rWnG3Ck"              //ใส่ key จากเว็บ
#define SECRET  "H5ZIwQuggdx57bKJyOzjllWFV"    //ใส่ secret จากเว็บ
#define ALIAS   "IOTwifi_1"                 //ชื่อ device ชื่อที่ตั้ง key

WiFiClient client;
MicroGear microgear(client);

unsigned long previousMilliss = 0;
const long intervall = 1000;
char data[32];
int x1, x2;

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Found new member --> ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Lost member --> ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.setAlias(ALIAS);
}

/**********************************************************************************/

#include <EasyScheduler.h>//ฟังก์ชั่น loop

//Scheduler
Schedular Task1; // สร้างงาน Task
Schedular Task2;
Schedular Task3;
Schedular Task4;
Schedular Task5;
Schedular Task6;

int Distance = 0;
char choice;

int buzzer =13; //buzzer

#include "config.h"//รวมฟังก์ชั่น

#define time_A 300 //กำหนดการทำงานใน Tasks

void setup() {

  microgear.on(MESSAGE, onMsghandler);
  microgear.on(PRESENT, onFoundgear);
  microgear.on(ABSENT, onLostgear);
  microgear.on(CONNECTED, onConnected);

  Serial.begin(115200);
  Serial.println("Starting...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  microgear.init(KEY, SECRET, ALIAS);
  microgear.connect(APPID);

  digitalWrite(2, HIGH);  //LED บนบอร์ด(GPIO 2)
  pinMode(2, OUTPUT);

/****************************************************************************/
  
//  Serial.begin(9600);
  Task1.start(); // สั่งให้งาน Task เริ่มทำงาน
  Task2.start();
  Task3.start();
  Task4.start();
  Task5.start();
  Task6.start();

   pinMode(buzzer, OUTPUT); //buzzer

  //setupจอTFT
  tft.init();
  tft.setRotation(1);
  TFT_AA();


  //setupวัดระยะ
  Wire.begin();
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    tft.setTextColor(tft.color24to16(0xFF4500));
    tft.setTextSize(2);//ขนาดข้อความ
    tft.drawString("Failed to detect and initialize sensor!", 10, 120);
    while (1) {}
  }
  sensor.startContinuous();//วัดระยะ

  //setupแกน
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

//หัวใจ
  Serial.println("Initializing...");
  while (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30102 was not found. Please check wiring/power/solder jumper at MH-ET LIVE MAX30102 board. ");
    tft.setTextColor(tft.color24to16(0xFF4500));
    tft.setTextSize(2);//ขนาดข้อความ
    tft.drawString("Sensor not found.!MAX30102", 10, 120);
    delay(1000);
  }
  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x7F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  //Options: 1 = IR only, 2 = Red + IR on MH-ET LIVE MAX30102 board
  int sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
  // Set up the wanted parameters
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  particleSensor.enableDIETEMPRDY();


//เซนเซอร์วัอุณหภูมิ  GY-906 MLX90614 
/*
Serial.println("Adafruit MLX90614 test");
mlx.begin();
*/


  
}
/*****************************************void loop*************************************************/
void loop() {

  Task1.check(t1Callback, time_A); // ทำงานฟังก์ชั่น LED1_blink ทุก 1 วินาที
  Task2.check(t2Callback, time_A); 
  Task3.check(t3Callback, time_A); 
  Task4.check(t4Callback, time_A); 
  Task5.check(t5Callback, time_A); 
  Task6.check(t6Callback, time_A); 

/*  long i = sensor.readRangeContinuousMillimeters();
  DacAudio.FillBuffer();                       // Fill the sound buffer with data
  if (i > Distance)
  {
    char c = 'A';
    switch (c) {
      case 'A':
        DacAudio.Play(&ForceWithYou);          // play it, this will cause it to repeat and repeat...
        break;                                 //การพักคำสั่ง เมื่อ case ตรงตามเงื่อนไขจะออกจาก switch ทันที
     }
   }
*/

//แสดงค่าเซนเซอร์
A_sensor();//หัวใจ
spo2_A ();

}


void t1Callback() {

  //แสดงระยะ
  long phase = sensor.readRangeContinuousMillimeters();
  Serial.print("phase : ");
  Serial.println(phase);
  tft.fillRect(200, 50, 80, 20, tft.color24to16(0x6699FF));
  tft.setTextColor(tft.color24to16(0xFF0066));//สีข้อความ
  tft.setTextSize(3);//ขนาดข้อความ
  tft.setCursor(200, 50);
  tft.print(phase);

  long pp_1 = 0;
  pp_1 = sensor.readRangeContinuousMillimeters();
  if (pp_1 <= Distance)
  {
    tft.fillRect(0, 85, 480, 95, tft.color24to16(0xFFFFFF));//ปิดข้อความแจ้งเตือนตรงกลาง
    tft.setTextColor(tft.color24to16(0xFF4500));
    tft.setTextSize(5);//ขนาดข้อความ
    tft.drawString("Be Careful", 100, 120);
    digitalWrite(buzzer, HIGH);
    delay(60);
    digitalWrite(buzzer, LOW); }
  else
  {
    tft.fillRect(0, 85, 480, 95, tft.color24to16(0xFFFFFF));//ปิดข้อความแจ้งเตือนตรงกลาง
    tft.setTextColor(tft.color24to16(0xFF4500));
    tft.setTextSize(5);//ขนาดข้อความ
    tft.drawString("****safe****", 70, 120);
  }

}
void t2Callback() {

  //แสดงแกน
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L) //Tmp/340.00+36.53
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.print("ACC X:");
  Serial.print(AcX);
  Serial.print("  ACC Y:");
  Serial.print(AcY);
  Serial.print("  ACC Z:");
  Serial.print(AcZ);
  Serial.println();
  //แสดงแกนบนจอ
  tft.fillRect(0, 20, 130, 50, tft.color24to16(0xFFFFFF));//ปิดให้ค่าลัน
  tft.setTextColor(tft.color24to16(0xFF0066));//สีข้อความ
  tft.setTextSize(1);//ขนาดข้อความ
  tft.drawString("ACC X: ", 10, 20);//ข้อความ
  tft.setCursor(45, 20);
  tft.print(AcX);

  tft.setTextColor(tft.color24to16(0xFF0066));//สีข้อความ
  tft.setTextSize(1);//ขนาดข้อความ
  tft.drawString("ACC Y: ", 10, 30);//ข้อความ
  tft.setCursor(45, 30);
  tft.print(AcY);

  tft.setTextColor(tft.color24to16(0xFF0066));//สีข้อความ
  tft.setTextSize(1);//ขนาดข้อความ
  tft.drawString("ACC Z: ", 10, 40);//ข้อความ
  tft.setCursor(45, 40);
  tft.print(AcZ);

}
void t3Callback() {

  //จำลองแสดงค่า Tmp
  tft.fillRect(360, 250, 90, 20, tft.color24to16(0x6699FF));
  tft.setTextColor(tft.color24to16(0xFF0066));//สีข้อความ
  tft.setTextSize(3);//ขนาดข้อความ
  tft.setCursor(360, 250);
  tft.print(Tmp/340.00+36.53);


}
void t4Callback() {
//การนำค่าขึ้น iot

    unsigned long currentMilliss = millis();
    if (currentMilliss - previousMilliss >= intervall) { //ทำงานทุกๆ 1 วินาที(interval)
      previousMilliss = currentMilliss;

    x1++;               //บวกเลขที่ละ 1
      x2 = random(100);   //สุ่มตัวเลข 0-100

      Serial.println("Publish...");

      sprintf(data, "%d,%d", x1, x2);   //แปรง int รวมกันใน char ชื่อ data
      Serial.println(data);
      microgear.publish("/x", data);      //ส่งค่าขึ้น NETPIE

      microgear.publish("/x1", String(x1));     //หรือจะส่งทีละค่าก็ได้

     // microgear.publish("/x2", String(x2));


     int aa = sensor.readRangeContinuousMillimeters();
     microgear.publish("/aa", String(aa));


     int ab = ESpO2;
     microgear.publish("/ab", String(ab));


     int ac = beatAvg;
     microgear.publish("/ac", String(ac));


     int ad = AcX;
     microgear.publish("/ad", String(ad));

 }


     
//เอาไว้แสดงค่าอุณหภูมิ
//เซนเซอร์วัอุณหภูมิ  GY-906 MLX90614 
/*Serial.print("Ambient = ");
Serial.print(mlx.readAmbientTempC());
Serial.print("*C\tObject = ");
Serial.print(mlx.readObjectTempC()); Serial.println("*C");
Serial.print("Ambient = ");
Serial.print(mlx.readAmbientTempF());
Serial.print("*F\tObject = ");
Serial.print(mlx.readObjectTempF()); Serial.println("*F");
Serial.println();*/



}
void t5Callback() {
  
//นับเวลาการทำงาน
unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= 1000) {
    sec++;
    if(sec >= 60){
      m++;
      sec = 0;
    }
    if(m >= 60){
      h++;
      m = 0;
    }
    if(h >= 24){
      h = 0;
    }
    previousMillis = currentMillis;
  }
//  Serial.print(h);Serial.print(":");
//  Serial.print(m);Serial.print(":");
//  Serial.println(sec);
  
  tft.fillRect(0, 0, 150, 20, tft.color24to16(0xFFFFFF));
  tft.setTextColor(tft.color24to16(0xFF0066));//สีข้อความ
  tft.setTextSize(1);//ขนาดข้อความ
  tft.drawString("Time: ", 10, 10);//ข้อความ
  tft.setCursor(40, 10);
  tft.print(h);
  tft.drawString(" : ", 50, 10);//ข้อความ
  tft.setCursor(60, 10);
  tft.print(m);
  tft.drawString(" : ", 70, 10);//ข้อความ
  tft.setCursor(80, 10);
  tft.print(sec);

}
void t6Callback() {

 Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers  
  AcX = Wire.read() << 8 | Wire.read(); 
  if (AcX > 8000)
  {
    Serial.print("AcX tt = ");
    Serial.println(AcX);
    char g = 'B';
    switch (g) {
      case 'B':
        digitalWrite(buzzer, HIGH);
         delay(100);
        digitalWrite(buzzer, LOW);
      break;                                 //การพักคำสั่ง เมื่อ case ตรงตามเงื่อนไขจะออกจาก switch ทันที
    }
  }
}
