// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"
#include <OneWire.h>
#include <SD.h>



#define RTC_VCC_PIN A0                                                                                                                                                                                                                                                                                                                                                                                                                              
#define TEMP_INT_PIN 19
#define TEMP_EXT_PIN A2
#define SD_CS_PIN 53

RTC_DS1307 RTC;
OneWire temp_int(TEMP_INT_PIN); //internal temperature sensor
//OneWire temp_ext(TEMP_EXT_PIN); //external temperature sensor
//char textbuff[50];
char textline[35];
File LogFile;

float temp_int_value;
float temp_ext_value;
char  temp_int_str[8];
char  temp_ext_str[8];
char  year_str[6];
char  month_str[4];
char  day_str[4];
char  hour_str[4];
char  minute_str[4];
char  second_str[4];
char  date_str[12];
char  time_str[12];

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
  int attempt = 0;
  float temperature;
  do
  {
    temperature = getTemp(temp_sensor);  
    attempt++;
  }
  while(temperature == -1000 || (int)temperature == 85 || attempt<max_n_attempt);
  return temperature;
}  

void floatToCharArray(float num_float, char* num_str)
{
  int int_part = (int) num_float;
  int dec_part = (int) ((num_float - int_part) * 100);
  int i =0;
  String temp_str;
  //Serial.println(int_part);
  //Serial.println(dec_part);
  temp_str = String(int_part,DEC) + "." + String(dec_part,DEC) + '\0';  
  temp_str.toCharArray(num_str, temp_str.length()+2);
}

void intToCharArray(int num_int, char* num_str)
{
  String temp_str;
  //Serial.println(num_int);
  temp_str = String(num_int,DEC) + '\0';  
  temp_str.toCharArray(num_str, temp_str.length()+2);
}
/*
void LogToFile(char *buff)
{
    
    LogFile = SD.open("LogFile.txt", FILE_WRITE);  
    if (LogFile) {
    LogFile.println(buff);
    LogFile.close();
    // print to the serial port too:
    Serial.println(buff);        
    }
}
*/
void clean(char *var){
 int i = 0;
 while(var[i] != '\0'){
      var[i] = '\0';
      i++;
 }
}



void setup () {
    //supply power to the RTC
    pinMode(RTC_VCC_PIN, OUTPUT); 
    digitalWrite(RTC_VCC_PIN, HIGH); 
    Serial.begin(57600);
    Wire.begin();
    RTC.begin();
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH); // davekw7x: If it's low, the Wiznet chip corrupts the SPI bus
  
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  //pinMode(SD_CS_PIN, OUTPUT);
   
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  //LogFile = SD.open("LogFile.txt", FILE_WRITE);  
}

void loop () {  
    DateTime now = RTC.now(); 
    
    //Serial.print("String length is: ");
    //Serial.println(strlen(textbuff));
    
    
    clean(date_str);
    clean(time_str);
    clean(textline);
    
    intToCharArray(now.year(),year_str);
    intToCharArray(now.month(),month_str);
    intToCharArray(now.day(),day_str);
    intToCharArray(now.hour(),hour_str);
    intToCharArray(now.minute(),minute_str);
    intToCharArray(now.second(),second_str);
    
    if (now.day()<31)
    {
        
      
      strcpy(date_str,year_str);    
          
      strcat(date_str,"/");
      strcat(date_str,month_str);
      strcat(date_str,"/");
      strcat(date_str,day_str);
    
      strcpy(time_str,hour_str);   
      
      strcat(time_str,":");
      strcat(time_str,minute_str);
      strcat(time_str,":");
      strcat(time_str,second_str);    
    /*
      Serial.println(date_str);
      Serial.println(time_str);    
      
      
      Serial.println();
      */
      float temp_int_value = readValidTemp(temp_int);
      /*
      Serial.print("Internal temperature : ");
      Serial.print(temp_int_value);
      Serial.println(" DegreeC");
      */
      
      //float temp_ext_value = readValidTemp(temp_ext);
      /*
      Serial.print("External temperature : ");
      Serial.print(temp_ext_value);
      Serial.println(" DegreeC");
      */
      floatToCharArray(temp_int_value, temp_int_str);
      //floatToCharArray(temp_ext_value, temp_ext_str);
      
      //Serial.println(temp_int_str);
      //Serial.println(temp_ext_str);
      
      strcat(textline, date_str);
      strcat(textline, ",");
      strcat(textline, time_str);
      strcat(textline, ",");
      strcat(textline, temp_int_str);  
      //strcat(textline, ",");
      //strcat(textline, temp_ext_str); 
   
      //LogToFile(textline);
      
      LogFile = SD.open("LogFile.csv", FILE_WRITE);  
      if (LogFile) {
      LogFile.println(textline);
      LogFile.close();
      // print to the serial port too:
      Serial.println(textline);    
      } 
    } 
    
    digitalWrite(RTC_VCC_PIN, LOW); 
    delay(3000);
    digitalWrite(RTC_VCC_PIN, HIGH);         
}
