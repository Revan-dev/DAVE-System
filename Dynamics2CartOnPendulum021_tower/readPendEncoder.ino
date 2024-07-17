void readPendEncoder() {

  PendEcounttt = PendEcountt.read();
  PendAngle = map(PendEcounttt, 2048*4, -2048*4, -360, 360); // setting error to angle in degrees
  while (PendAngle >  180) {PendAngle = PendAngle - 360;}
  while (PendAngle < -180) {PendAngle = PendAngle + 360;} 
      Serial.println("Encoder count"); 
      Serial.println("-----------"); 
      Serial.print("Ang (deg):  ");
      Serial.println(PendAngle) ; 
}
