/*
  This example code control the RoboTank using serial interface from the computer.
  Commands are given to RoboTank using the serial monitor in the Arduino IDE
  On the motor controller, moving the jumper to PWRIN to supply power to the Arduino through the 6-battery pack.
 */

int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;
char command = '\0';
int pwm_speed; //from 0 - 255
void setup() {                
  // initialize the digital pin as an output.
    pinMode(M1, OUTPUT); 
    pinMode(M2, OUTPUT);   
    Serial.begin(9600);   
     pinMode(13, OUTPUT);      
    
    digitalWrite(M1,LOW); 
    digitalWrite(M2, HIGH);       
}

void loop() {
  
  
  if (Serial.available()>0)
  {
    command = Serial.read();
  }

  switch (command)
  {
    case 'e': //go forward
    Serial.println("Go!!\r\n"); 
    digitalWrite(M1,LOW); 
    digitalWrite(M2, HIGH);        
    analogWrite(E1, 255);   //PWM speed control
    analogWrite(E2, 255);   //PWM speed control    
    command = '\0'; //reset command
    break;
    case 'c': //backward
    Serial.println("Back!!\r\n");     
    digitalWrite(M1,HIGH); 
    digitalWrite(M2, LOW);        
    analogWrite(E1, 255);   //PWM speed control
    analogWrite(E2, 255);   //PWM speed control    
    command = '\0'; //reset command
    break;   
    case 's': //left
    Serial.println("Left!!\r\n");     
    digitalWrite(M1,LOW); 
    digitalWrite(M2, LOW);         
    analogWrite(E1, 255);   //PWM speed control
    analogWrite(E2, 255);   //PWM speed control    
    command = '\0'; //reset command
    break;    
    case 'f': //right
    Serial.println("right!!\r\n");     
    digitalWrite(M1,HIGH); 
    digitalWrite(M2, HIGH);          
    analogWrite(E1, 255);   //PWM speed control
    analogWrite(E2, 255);   //PWM speed control    
    command = '\0'; //reset command
    break;           
    case 'd': //stop
    analogWrite(E1, 0);   //PWM speed control
    analogWrite(E2, 0);   //PWM speed control 
    command = '\0'; //reset command
    break;    
    case '\0':
    //do nothing
    break; 
  }
 
}
