/*
  This example code control the Terminator bot using serial interface from the computer.
  Commands are given to Terminator bot using the serial monitor in the Arduino IDE
  Motor direction may be wrong depending the polarity of the DC motor connection.
 */

 
int OUT1 = 4;
int OUT2 = 5;
int OUT3 = 6;
int OUT4 = 7;
char command = '\0';
int pwm_speed = 200; //from 0 - 255
char last_command; //remember the last command for speed adjustment
void setup() {                
  // initialize the digital pin as an output.
   // pinMode(M1, OUTPUT); 
    //pinMode(M2, OUTPUT);   
    Serial.begin(9600);   
    
    //digitalWrite(M1,LOW); 
    //digitalWrite(M2, HIGH);                
}

void loop() {
  
  char *help_menu = "\'e\' go forward\n\'d\' stop\n\'s\' left\n\'f\' right\n\'c\' go backward\n";       
  
  if (Serial.available()>0)
  {
    command = Serial.read();
  }

  switch (command)
  {
    case 'e': //go forward
    Serial.println("Go!!\r\n");       
    analogWrite(OUT1, pwm_speed);   //PWM speed control
    analogWrite(OUT2, 0);   
    analogWrite(OUT3, 0);   
    analogWrite(OUT4, pwm_speed);   //PWM speed control
    last_command = command;    
    command = '\0'; //reset command
    break;
    case 'c': //backward
    Serial.println("Back!!\r\n");     
    analogWrite(OUT1, 0);   
    analogWrite(OUT2, pwm_speed);   //PWM speed control    
    analogWrite(OUT3, pwm_speed);   //PWM speed control
    analogWrite(OUT4, 0);   
    last_command = command;    
    command = '\0'; //reset command
    break;   
    case 's': //left
    Serial.println("Left!!\r\n");         
    analogWrite(OUT1, 0);   
    analogWrite(OUT2, pwm_speed);   //PWM speed control    
    analogWrite(OUT3, 0);   //PWM speed control
    analogWrite(OUT4, pwm_speed);           
    last_command = command;    
    command = '\0'; //reset command
    break;    
    case 'f': //right
    Serial.println("Right!!\r\n"); 
    analogWrite(OUT1, pwm_speed);   
    analogWrite(OUT2, 0);   //PWM speed control    
    analogWrite(OUT3, pwm_speed);   //PWM speed control
    analogWrite(OUT4, 0);  
    last_command = command;    
    command = '\0'; //reset command
    break;           
    case 'd': //stop
    Serial.println("Stop!\r\n"); 
    analogWrite(OUT1, 0);   
    analogWrite(OUT2, 0);   
    analogWrite(OUT3, 0);   
    analogWrite(OUT4, 0);  
    last_command = command;    
    command = '\0'; //reset command
    break;    
    case 'j': //reduce speed
    if (pwm_speed >= 5)
    {
      pwm_speed = pwm_speed - 5;      
      command = last_command;
    }
    else
    {
      command = '\0'; //reset command    
    }
    Serial.println("Slow down!\r\n");         
    break;     
    case 'k': //increase speed
    if (pwm_speed <=250 )
    {
      pwm_speed = pwm_speed + 5;
      command = last_command;
    }
    else
    {
      command = '\0'; //reset command              
    }
    Serial.println("go faster!\r\n");     
    break;         
    case '\0':
    //do nothing
    break;    
    default:
    Serial.println(help_menu);
    command = '\0'; //reset command
    break; 
  }
 
}

