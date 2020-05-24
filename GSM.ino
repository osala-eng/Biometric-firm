

void setupGSM() {
  sim800l.begin(9600);   //Module baude rate, this is on max, it depends on the version
  while (!sim800l){digitalWrite(red,HIGH);}
  digitalWrite(red,LOW);
  delay(1000);
}

void runGSM() {
 
  //if (sim800l.available()){            //Displays on the serial monitor if there's a communication from the module
  //  Serial.write(sim800l.read()); 
  //}

}

void sendSMS()
{
  const char phone = "+254790112652";
  //Serial.println("Sending SMS...");               //Show this message on serial monitor
  sim800l.print("AT+CMGF=1\r");                   //Set the module to SMS mode
  delay(100);
  sim800l.print("AT+CMGS=\"+254790112652\"\r");  
  //Your phone number don't forget to include your country code, example +212123456789"
  delay(500);
  sim800l.print("SIM800l is working");       //This is the text to send to the phone number, don't make it too long or you have to modify the SoftwareSerial buffer
  delay(500);
  sim800l.print((char)26);// (required according to the datasheet)
  delay(500);
  sim800l.println();
  //Serial.println("Text Sent.");
  delay(500);

}
 