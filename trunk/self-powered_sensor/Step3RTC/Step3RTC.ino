// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"
#include <OneWire.h>
#include <SD.h>

#define SERIAL_DEBUG 

#define ON true
#define OFF 0false

#define TEMP_INT_PIN 19
#define TEMP_EXT_PIN 7
#define CHIP_SELECT_PIN 53 //for mega 

#define MIN_TEMP -10
#define MAX_TEMP 100

RTC_DS1307 RTC;
OneWire temp_int(TEMP_INT_PIN); //internal temperature sensor
OneWire temp_ext(TEMP_EXT_PIN); //internal temperature sensor

float temp_int_value;
float temp_ext_value;

String textline;
String date_delimiter = "/";
String time_delimiter = ":";
String value_delimiter = ",";

File LogFile;
char c;
void init_io()
{
  pinMode(CHIP_SELECT_PIN, OUTPUT);   
}

float getTemp(OneWire temp_sensor){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !temp_sensor.search(addr)) {
      //no more sensors on chain, reset search
      temp_sensor.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      //Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
//      Serial.print("Device is not recognized");
      return -1000;
  }

  temp_sensor.reset();
  temp_sensor.select(addr);
  temp_sensor.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = temp_sensor.reset();
  temp_sensor.select(addr);    
  temp_sensor.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = temp_sensor.read();
  }
  
  temp_sensor.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
  
}

float readValidTemp(OneWire temp_sensor)
{
  int max_n_attempt = 10;
  float temperature;
  do
  {
    temperature = getTemp(temp_sensor);  
  }
  while(temperature == -1000 || (int)temperature == 85 ||
  temperature < MIN_TEMP || temperature > MAX_TEMP);
  return temperature;
}  

void setup () {
    init_io();
    Serial.begin(57600);
    //enable serial for Bluetooth
    Serial2.begin(9600);
    Wire.begin();
    RTC.begin();
  //if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  //}    
  if (!SD.begin(CHIP_SELECT_PIN)) {
    #ifdef SERIAL_DEBUG
    Serial.println("* Initialization failed!");
    Serial.println(" Things to check:");
    Serial.println("* Is a card inserted?");
    Serial.println("* Is the card formated to FAT32 or FAT16?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* Did you change the chipSelect pin to match your shield or module?");    
    #endif
    return;
  }
  #ifdef SERIAL_DEBUG
  Serial.println("initialization done.");  
  #endif
  
}

void loop () {

  if (Serial2.available())
  {
    c = Serial2.read();
  }
  
  if (c == 's')
  {//sending all the data to PC
    // re-open the file for reading:
    LogFile = SD.open("LogFile.txt", FILE_READ);  
    if (LogFile)
    {
      while(LogFile.available())
      {//send thro Bluetooth
        Serial2.write(LogFile.read());        
      }
    }   
    else
    {
      Serial2.println("Can't open file");
    }
  }
  
    DateTime now = RTC.now();
    
    textline = now.year() + date_delimiter + now.month() + date_delimiter + now.day() + value_delimiter
             + now.hour() + time_delimiter + now.minute() + time_delimiter + now.second() + value_delimiter;                
             
    //if the date time from the RTC is not valid, then skip the measurement
    if ( now.month()>12)
    {
      return; //Arduino doesn't allow to use 'continue'
    }
             
    Serial.println("Temperature Reading: ");
    float temp_int_value = readValidTemp(temp_int);
    Serial.print(temp_int_value);       
    Serial.println(" DegreeC");
    float temp_ext_value = readValidTemp(temp_ext);
    Serial.print(temp_ext_value);       
    Serial.println(" DegreeC");  
    
    Serial.println("Send to PC");
    //send to C# app via Bluetooth in this formate <temp1;temp>, e.g. <19.21;16.50>
    Serial2.print("<");
    Serial2.print(temp_int_value);
    Serial2.print(";");
    Serial2.print(temp_ext_value);
    Serial2.println(">");
    
        textline = now.year() + date_delimiter + now.month() + date_delimiter + now.day() + value_delimiter
             + now.hour() + time_delimiter + now.minute() + time_delimiter + now.second() + value_delimiter;                             
    Serial.print(textline);    
    Serial.println();  

    LogFile = SD.open("LogFile.txt", FILE_WRITE);  
    if (LogFile)
    {
      
      LogFile.print(textline);
      LogFile.print(temp_int_value);
      LogFile.print(value_delimiter);
      LogFile.print(temp_ext_value);
      LogFile.println();
      
      LogFile.close();
      #ifdef SERIAL_DEBUG 
      Serial.println("Write done.");     
      #endif
    }  
    else
    {
      #ifdef SERIAL_DEBUG      
      Serial.println("error can't write file");
      #endif
    }
    
    delay(5000);
    c = '\0';
}
