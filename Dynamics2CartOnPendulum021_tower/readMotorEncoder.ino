void readMotorEncoder() {

  ecounttt = ecountt.read();
  Position = map(ecounttt, -330, 330, -152.5, 152.5); // setting P. encoder val. to position in mm
//      Serial.println("-----------");
//      Serial.print("Pos (mm):  ");    

   if (Lpressed%2 == 0) {
      Serial.println(Position); 
   }
      
}
