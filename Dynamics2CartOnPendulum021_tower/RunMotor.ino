void RunMotor() {


// run this if PWM regulated
  if (motorRatePWM == 1) {
   if (motorONOFF == 1) {
    if (ccw == 1) {  
    digitalWrite(input1, HIGH);  //Write Digital value to Input 1
    analogWrite(PWMotor, PWM); }  //Write Analog value to D/C Motor
    if (ccw == 0) {
    digitalWrite(input1, LOW);  //Write Digital value to Input 1
    analogWrite(PWMotor, PWM); }  //Write Analog value to D/C Motor
   
   
    if (printMotorEncoder == 1) {
       TIter++;
       if (TIter % 10 == 0){
       MotorPos = ecountt.read();
       PosD = MotorPos - OldMotorPos; // into Delta[ticks]
       PosD = PosD/(deltaT*10); // into ticks/sec
       PosD = (PosD/400)*6.28; //into rad/sec
       data[(TIter/10)-1] = PosD;; // print out ticks
       OldMotorPos = MotorPos;
       }
    }
   }
   if (motorONOFF == 0) {
    analogWrite(PWMotor, 0);   //Write Analog value to D/C Motor
   }
  }

// run this if frequency regulated
  else {
   if (motorONOFF == 0) {
    MotorPos = ecountt.read();
    analogWrite(PWMotor,0);
   }
    
   if (motorONOFF == 1) {
    MotorPos = MotorPos + Hz*400*deltaT; // cycling 1 through 5
    ecounttt = ecountt.read(); // if it goes above realmax?
 // P //  
    PosErrorP = ( MotorPos - ecounttt);
 // D //
    numeratorPosErrD =  PosErrorP - oldAngle;
    PosErrorD = numeratorPosErrD / deltaT;       
    oldAngle = PosErrorP;
 // run motor //
    motorController = Kp*PosErrorP  + Kd*PosErrorD;
    motorController = constrain(motorController,-1023,1023);
    if (motorController < 0) {
     digitalWrite(input1, LOW);  //Write Digital value to Input 1
     analogWrite(PWMotor, -motorController*motorONOFF); //Write Analog value to D/C Motor
    }  
    else if (motorController > 0) {
     digitalWrite(input1, HIGH);  //Write Digital value to Input 1
     analogWrite(PWMotor, motorController*motorONOFF);   //Write Analog value to D/C Motor
    }
    else {
     analogWrite(PWMotor, 0);  //Write Analog value to D/C Motor
    }
   }
  }

}
