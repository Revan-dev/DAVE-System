 //Dynamics II PD Controller Lab 

/////// Change This ////////

 // direction
  int ccw = 1; // either 0 for cw or 1 for ccw
  
  int motorRatePWM = 0; // choose one or the other
  int PWM = 0; // 10bits (0 - 1023) //motorRatePWM 0\1
  
 // #define motorRateHz 1 // choose one or the other
  float Hz = 0; // [rev/sec] //motorRateHz 0\1
  // PID Velocity Controller Gains
  float Kp = 40;
  float Kd = 30;

////////////////////////////

#include <Encoder.h>
int motorONOFF = 0;
int initialized = 0; 
int LabNum;
int printMotorEncoder = 0;


// Pin Setup ///////////////////////////////////////////

 const int PWMotor = 36; 
 const int input1  = 37; // Motor Direction
 Encoder PendEcountt(41,40);
 Encoder ecountt(22,23);
 const int LeftTrigger = 12;  
 const int RightTrigger = 38;
 static unsigned long last_interrupt_time = 0; 
 
 
// Initial States //////////////////////////////////////

 // angle control //
 float PendEcounttt = 0;
 float PendAngle = 300;
 float errorP = 0; float AvErrorP = 0;
 float errorI = 0; float AvErrorI = 0;
 float errorD = 0; float AvErrorD = 0; 
 float numeratorErrD = 0;
 float oldAngle = 0;
 
 // position control //
 float ecounttt = 0;
 float Position = 0;
 float PosErrorP = 0; float AvPosErrorP = 0;
 float PosErrorI = 0; float AvPosErrorI = 0;
 float PosErrorD = 0; float AvPosErrorD = 0;
 float numeratorPosErrD = 0;
 float oldPos = 0;

 

 // angle encoder //
 //float PendEcounttt = 0;
 //float PendAngle = 300;



 // position encoder //
 //float ecounttt = 0;
 float ecountttLast = 0;
 //float Position = 0;
 float MotorPos = 0;
 float OldMotorPos = 0;
 float PosD = 0;
 int data[1000];

 // motor //
 float PDController = 0;
 float motorController = 0;
 int motorMinPWM = 0;
 
 // code tags //
 int connectedd = 0;
 int iter = 0; 
 int TIter = 0;
 int n = 15; // readings to average over to reduce error
 
 // triggers // 
 int Lpressed = 0;
 int Rpressed = 0;

 
// Timer ///////////////////////////////////////////////

 unsigned long t = 0;
 unsigned long t_5sec = 0;
 double deltaT = 0.0005; //seconds
 IntervalTimer motorRTI; // Create an IntervalTimer object 
 unsigned long interrupt_timeR = 0;
 unsigned long interrupt_timeL = 0;

 
////////////////////////////////////////////////////////

// Accelerometer //////////////THIS GOTTA BE FIXED

#include "SparkFunLIS3DH_InvPend.h" //(code changes for the Teensy 4.1)
#include "Wire.h"
LIS3DH myIMU;

////////////////////////////////////////////////////////

// Serialport //////////////////////////////////////////

String x;

////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); 
  analogWriteResolution(10);  // analogWrite value 0 to 4095, or 4096 for high
  pinMode(input1, OUTPUT); //Initialize H-bridge Input 1 Pin
  pinMode(PWMotor, OUTPUT); // Initialize D/C motor Pin 
  pinMode(LeftTrigger, INPUT_PULLDOWN); // emergency button pin
  pinMode(RightTrigger, INPUT_PULLDOWN); // emergency button pin
  attachInterrupt(digitalPinToInterrupt(LeftTrigger), LeftTriggerFunc, RISING);
  attachInterrupt(digitalPinToInterrupt(RightTrigger), RightTriggerFunc, RISING);

//  double startTime = millis();

  delay(1000);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
//  Serial.println("Processor came out of reset.\n");
  myIMU.begin();
}

////////////////////////////////////////////////////////
void loop () {  
// This is a brancing choice tree taking inputs from the serial port
      
      // this is for connecting the serial port
        while (connectedd == 0) { // run this until serial port is connected
          if(Serial.available() > 0) {
            x = Serial.readStringUntil('\n'); //you have 0x31 in the variable x
            if (x.equals("serialConnectNow")){
              Serial.println("amHere");
              connectedd = 1;
            }
          }
          delay(10);
        }
    
      // this is for the lab choice and its specific setup 
        while (connectedd == 1) { //run this until lab selected
          if(Serial.available() > 0) {
            x = Serial.readStringUntil('\n'); //you have 0x31 in the variable x
            if (x.equals("OffSetMass")) { //LabNum 4
              Serial.println("osmHere");
              LabNum = 4;
              motorRTI.begin(RunMotor, deltaT*1000000);  // RunMotor to run every 0.0005 seconds
              connectedd = 2;
            }
            else if (x.equals("StallTorque")) { //LabNum 5
              Serial.println("stHere");
              LabNum = 5;
              motorRTI.begin(RunMotor, deltaT*1000000);  // RunMotor to run every 0.0005 seconds
              motorRatePWM = 1;
              connectedd = 2;
              printMotorEncoder = 1;
            }
            else if (x.equals("NoLoadSpeed")) { //LabNum 6
              Serial.println("nlsHere");
              LabNum = 5;
              motorRTI.begin(RunMotor, deltaT*1000000);  // RunMotor to run every 0.0005 seconds
              motorRatePWM = 1;
              connectedd = 2;
              printMotorEncoder = 1;
            }
          }
          delay(10);
        }

      // this is for running the code of each lab
        while (connectedd == 2) {
          
           switch(LabNum) {
////-------------------------------------------------------------////
            case 1: // Lab 1: Motor Spin Down
              break;
////-------------------------------------------------------------////
            case 2: // Lab 2:
              break;
////-------------------------------------------------------------////
            case 3: // Lab 3:
              break;
////-------------------------------------------------------------////
            case 4: // Lab 4: Off Set Mass
              if(Serial.available() > 0) {
//                 x = Serial.readStringUntil('\n'); 
//                 if (x.equals("MOO")) { 
                   Hz = Serial.readStringUntil('\n').toFloat();
                   Serial.println(Hz);                  
                   if (motorONOFF == 1) {motorONOFF = 0;}
                   else if (motorONOFF == 0) {motorONOFF = 1;}
//                 }
                 //Serial.flush();
              }
              break;
////-------------------------------------------------------------////
            case 5: // Lab 5: Stall Torque & No Load Speed
             iter ++;
             if(Serial.available() > 0) {
                   PWM = Serial.readStringUntil('\n').toInt();
                   Serial.println(PWM);
                   if (motorONOFF == 1) {
                    motorONOFF = 0;
                    printEcounts();
                    }
                   else if (motorONOFF == 0) {
                    delay(500);
                    motorONOFF = 1;
                    memset(data,0,sizeof(data));
                    t = millis();
                    TIter = 0;
                   }
                   t_5sec = millis();
//                   while(Serial.available()) { //purge serial line
//                    String temp= Serial.readStringUntil('\n');
//                   }
             }
             
             if ((millis() > t_5sec + 5000) && (motorONOFF == 1)) {
              motorONOFF = 0;
              printEcounts();
             }
               
              break;
////-------------------------------------------------------------////


            default: // if nothing else matches, do the default
              delay(10);  
              break;
           }
////-------------------------------------------------------------////
        }
}


////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////

void printAccels(){
  Serial.println(myIMU.readFloatAccelY()*980.665, 5); // cm/sec
  Serial.println(millis()-t);
}

void printMotorEncoderFnc(){
  Serial.println(ecountt.read()); // print out ticks
  Serial.println(millis()-t);
}

void printEcounts(){
    for(int ii = 0; ii < 1000; ii++) {
      Serial.println(data[ii]);
    }
}
//////////////////////////////////////////////////////// 

void LeftTriggerFunc () {
  
   interrupt_timeL = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore

  if (interrupt_timeL - last_interrupt_time > 250){
    last_interrupt_time = interrupt_timeL;
    
//    if (digitalRead(input1) == HIGH) {digitalWrite(input1, LOW);}
//    else if (digitalRead(input1) == LOW) {digitalWrite(input1, HIGH);}
      Lpressed += 1;
      motorONOFF = 0;
      analogWrite(PWMotor, 0); 
          // if (motorONOFF == 1) {motorONOFF = 0;}
          // else if (motorONOFF == 0) {motorONOFF = 1;}
//      Serial.println("-----------"); 
//      Serial.println(Lpressed);
//      Serial.println(digitalRead(input1));
  }  
  
  if (interrupt_timeR - last_interrupt_time < 500) {
    EmergencyStop();
  }
}


////////////////////////////////////////////////////////

void RightTriggerFunc () {
  interrupt_timeR = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_timeR - last_interrupt_time > 250) {
    last_interrupt_time = interrupt_timeR;
    Rpressed += 1; 
    motorONOFF = 0; 
    analogWrite(PWMotor, 0);
      //Serial.println("-----------"); 
     // Serial.println(Rpressed);
      //Serial.println(motorONOFF);

  }

  if (interrupt_timeL - last_interrupt_time < 500) {
    EmergencyStop();
  }
}

 
////////////////////////////////////////////////////////

void EmergencyStop () {
  
  // stop motor //

  // reset //
  initialized = 0; 
  iter = 0;
  
  motorONOFF = 0;

  // warning //
  Serial.println("Emergency Stop"); 
  digitalWrite(LED_BUILTIN, LOW);  delay(500);
  digitalWrite(LED_BUILTIN, HIGH); delay(500); 
  digitalWrite(LED_BUILTIN, LOW);  delay(500);
  digitalWrite(LED_BUILTIN, HIGH); delay(500); 
  digitalWrite(LED_BUILTIN, LOW);  delay(500);
  digitalWrite(LED_BUILTIN, HIGH); delay(500);  
  digitalWrite(LED_BUILTIN, LOW);  delay(5000000);  
 

}
