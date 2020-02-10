#include <PS2X_lib.h>  //for v1.6
#include <AFMotor.h>
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
PS2X ps2x;// create PS2 Controller Class
#include <Servo.h>
Servo servo01;
Servo servo02;
int servo1PPos;
int j=120;
int i=31;
//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int error = 0; 
byte type = 0;
byte vibrate = 0;

void setup(){
  servo02.attach(A4);
  servo01.attach(A5);
 Serial.begin(57600);
  servo01.write(120);
  servo02.write(30);
  //servo02.write();
  
 //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  pinMode(A0,OUTPUT);
     pinMode(A1,INPUT);
     pinMode(A2,OUTPUT);
     pinMode(A3,INPUT);
     pinMode(A4,OUTPUT);
 error = ps2x.config_gamepad(A0,A2,A1,A3, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
 
 if(error == 0){
   Serial.println("Found Controller, configured successful");
   Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
  Serial.println("holding L1 or R1 will print out the analog stick values.");
  Serial.println("Go to www.billporter.info for updates and to report bugs.");
 }
   
  else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
  else if(error == 3)
   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
   //Serial.print(ps2x.Analog(1), HEX);
   
   type = ps2x.readType(); 
     switch(type) {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
       case 2:
         Serial.println("GuitarHero Controller Found");
       break;
     }
     
     
  
}

void loop(){
   /* You must Read Gamepad to get new values
   Read GamePad and set vibration values
   ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
   if you don't enable the rumble, use ps2x.read_gamepad(); with no values
   
   you should call this at least once a second
   */
   
   
motor1.setSpeed(150);
motor2.setSpeed(150);
motor3.setSpeed(150);
motor4.setSpeed(150);  
 if(error == 1) //skip loop if no controller found
  return; 
  
 if(type == 2){ //Guitar Hero Controller
   
   ps2x.read_gamepad();          //read controller 
   
   if(ps2x.ButtonPressed(GREEN_FRET))
     Serial.println("Green Fret Pressed");
   if(ps2x.ButtonPressed(RED_FRET))
     Serial.println("Red Fret Pressed");
   if(ps2x.ButtonPressed(YELLOW_FRET))
     Serial.println("Yellow Fret Pressed");
   if(ps2x.ButtonPressed(BLUE_FRET))
     Serial.println("Blue Fret Pressed");
   if(ps2x.ButtonPressed(ORANGE_FRET))
     Serial.println("Orange Fret Pressed");
     

    if(ps2x.ButtonPressed(STAR_POWER))
     Serial.println("Star Power Command");
    
    if(ps2x.Button(UP_STRUM))          //will be TRUE as long as button is pressed
     Serial.println("Up Strum");
    if(ps2x.Button(DOWN_STRUM))
     Serial.println("DOWN Strum");
  
 
    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");

    
    if(ps2x.Button(ORANGE_FRET)) // print stick value IF TRUE
    {
        Serial.print("Wammy Bar Position:");
        Serial.println(ps2x.Analog(WHAMMY_BAR), DEC); 
    } 
 }

 else { //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");
         
         
     if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
       Serial.print("Up held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
       backward();
      }
      else if(ps2x.Button(PSB_PAD_RIGHT)){
       Serial.print("Right held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      right();
      }
      else if(ps2x.Button(PSB_PAD_LEFT)){
       Serial.print("LEFT held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      left();
      }
      else if(ps2x.Button(PSB_PAD_DOWN)){
       Serial.print("DOWN held this hard: ");
     Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      forward();
      
      } 
      else {
        stopp();
        }  
  
    
      vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on 
                                              //how hard you press the blue (X) button    
    
    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    {
     
       
         
        if(ps2x.Button(PSB_L3))
         Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3)){
         Serial.println("R3 pressed");
          
        }
        if(ps2x.Button(PSB_L2))
         Serial.println("L2 pressed");
        if(ps2x.Button(PSB_R2)){
         Serial.print("R2 pressed");
         servo01.write(j);
         j--;
        }
        if(ps2x.Button(PSB_GREEN))
         Serial.println("Triangle pressed");
         
    }   
         
    
    if(ps2x.ButtonPressed(PSB_RED)){             //will be TRUE if button was JUST pressed
         Serial.println("Circle just pressed");
         servo02.write(i);
         i=i-5;}
    if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released
         Serial.println("Square just released");     
    
    if(ps2x.NewButtonState(PSB_BLUE)) {           //will be TRUE if button was JUST pressed OR released
         Serial.println("X just changed");    
        servo02.write(i);
         i=i+5;
    }
    
    if(ps2x.Button(PSB_R1)) // print stick values if either is TRUE
    {
      Serial.print("R1");
     servo01.write(j);
         j++;
    } 
    
    
 }
 
 
 delay(50);
     
}
void forward(){
  //forward
  motor1.run(FORWARD);
  
  motor2.run(FORWARD);
  
  motor3.run(FORWARD);
  
  motor4.run(FORWARD);

  }
 void backward(){
  //back
  motor1.run(BACKWARD);
  
  motor2.run(BACKWARD);
 
  motor3.run(BACKWARD);
  
  motor4.run(BACKWARD);

  }

 void right(){
  //right
    motor1.run(BACKWARD);
  
  motor2.run(BACKWARD);
  
  motor3.run(FORWARD);
  
  motor4.run(FORWARD);

  }
  void left(){
      //left
  motor1.run(FORWARD);
  
  motor2.run(FORWARD);
  
  motor3.run(BACKWARD);
  
  motor4.run(BACKWARD);

    }
    void stopp(){
      
//stop
  motor1.run(RELEASE);
  
  motor2.run(RELEASE);
  
  motor3.run(RELEASE);
  
  motor4.run(RELEASE);
  
      }
