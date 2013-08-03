//Created 03/04/2013 
//By Hobbyist

//Last modified 02/08/2013
//By Hobbyist

float temp_int_value;
float temp_ext_value;
void setup()
{
  Serial.begin(57600);
  //enable serial for Bluetooth
  Serial2.begin(9600);  
}


void loop()
{
    temp_int_value = random(18, 22);
    temp_ext_value = random(18,22);
    
    Serial.println("Temperature Reading: ");
    Serial.print(temp_int_value);    
    Serial.println(" DegreeC");      
    Serial.print(temp_ext_value);       
    Serial.println(" DegreeC");  
    
    //send to C# app via Bluetooth in this formate <temp1;temp>, e.g. <19.21;16.50>
    Serial2.print("<");
    Serial2.print(temp_int_value);
    Serial2.print(";");
    Serial2.print(temp_ext_value);
    Serial2.println(">");    
    
    delay(3000);
}
