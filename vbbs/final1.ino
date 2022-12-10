#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_ADXL345_U.h>
#include "Sodaq_DS3231.h"
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

File myFile;
File myFile1;
const int chipSelect = 10;

int RXPin = 2;
int TXPin = 3;
int GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()    
{
  SD.begin(chipSelect);
  Serial.begin(9600); 
  if(!accel.begin())
  {
     Serial.println("No ADXL345 sensor detected.");
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");
  gpsSerial.begin(GPSBaud);
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(1);
  }
}

void loop()
{
  
  sensors_event_t event; 
  accel.getEvent(&event);
  
  if(gpsSerial.available() > 0 && gps.encode(gpsSerial.read())){
    myFile= SD.open("readings.txt", FILE_WRITE);
    if (myFile){
      myFile.print(gps.location.lat(),4);
      myFile.print(",");
      myFile.print(gps.location.lng(),4);
      myFile.print(",");
      myFile.println(gps.altitude.meters(),4);
      Serial.println("file written1"); 
    }
    myFile.close();
    myFile1 = SD.open("data_log.txt", FILE_WRITE);
    if (myFile1){
      sensors_event_t event; 
      accel.getEvent(&event);
      myFile1.print(gps.location.lat(),6);
      myFile1.print(",");
      myFile1.print(gps.location.lng(),6);
      myFile1.print(", ");
      myFile1.print(gps.altitude.meters(),6);
      myFile1.print(",");
      myFile1.print(event.acceleration.x);
      myFile1.print(", ");
      myFile1.print(event.acceleration.y);
      myFile1.print(", ");
      myFile1.println(event.acceleration.z);
      Serial.println("file written");
    }
    myFile1.close();  
  }
}
