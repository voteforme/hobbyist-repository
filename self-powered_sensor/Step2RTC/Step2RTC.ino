//Created 03/04/2013 
//By Hobbyist

//Last modified 02/08/2013
//By Hobbyist


// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"
#include <OneWire.h>

#define SERIAL_DEBUG 
#define ON true
#define OFF false

#define RTC_VCC_PIN 18
#define TEMP_INT_PIN 19

#define TEMP_EXT_VCC_PIN 7
#define TEMP_EXT_PIN 6

RTC_DS1307 RTC;
OneWire temp_int(TEMP_INT_PIN); //internal temperature sensor
OneWire temp_ext(TEMP_EXT_PIN); //internal temperature sensor

float temp_int_value;
float temp_ext_value;

String textline;
String date_delimiter = "/";
String time_delimiter = ":";
String value_delimiter = ",";

void init_io()
{
  pinMode(RTC_VCC_PIN, OUTPUT);   
  pinMode(TEMP_EXT_VCC_PIN, OUTPUT);     
}
void Switch_Peripherals(bool on_off)
{
  if (on_off == ON)
  {
    //switch on the sensor on the real time clock
    digitalWrite(RTC_VCC_PIN, HIGH); 
    digitalWrite(TEMP_EXT_VCC_PIN, HIGH); 
  }
  else
  {
    digitalWrite(RTC_VCC_PIN, LOW); 
    digitalWrite(TEMP_EXT_VCC_PIN, LOW);     
  }
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
  while(temperature == -1000 || (int)temperature == 85);
  return temperature;
}  

void setup () {

    init_io();
    //supply power to the peripherals
    Switch_Peripherals(ON);
    //delay(5000);
    Serial.begin(9600);

    Wire.begin();
    RTC.begin(); 
   
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    //Reset the RTC time to the compiler time, may only need to do it once.
    //Best effect is to take RTC battery off, then on, and then load this sketch
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }    
  
}

void loop () {
  Switch_Peripherals(ON);
  delay(300);
    DateTime now = RTC.now();
    
    textline = now.year() + date_delimiter + now.month() + date_delimiter + now.day() + value_delimiter
             + now.hour() + time_delimiter + now.minute() + time_delimiter + now.second() + value_delimiter;                
             
    float temp_int_value = readValidTemp(temp_int);
    float temp_ext_value = readValidTemp(temp_ext);    
    textline = now.year() + date_delimiter + now.month() + date_delimiter + now.day() + value_delimiter
             + now.hour() + time_delimiter + now.minute() + time_delimiter + now.second() + value_delimiter;                
             
     
    Serial.println("Temperature Reading: ");
   
    Serial.print(temp_int_value);       
    Serial.println(" DegreeC");
   
    Serial.print(temp_ext_value);       
    Serial.println(" DegreeC");  
    
    Serial.print(textline);    
    Serial.println();            
    
  //Switch_Peripherals(OFF);
  delay(3000); //wait for 3sec
}
