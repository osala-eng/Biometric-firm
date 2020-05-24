/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

//Modified by Osala

#include "Adafruit_Fingerprint.h"
#include "SoftwareSerial.h"
#include "Wire.h"
#include "SPI.h"
#include "Ethernet.h"

//----------------------- Fingerprint Global -----------------------
boolean trans = false;
int sensorid = 0;
const int red = 8;
const int green = 4;
const int txon = 6;
void transmit(); 
void blinkred();
void blinkgreen();
void beepAfter1minute(); unsigned long beepNow,beepPrev,beepT = 150;
unsigned long minNow,minPrev,minT = 1000*60; bool minState;
void buzz2();
void buzz3();
void enrollnow();
unsigned long now, prev, period = 100;
unsigned long nowg, prevg, periodg = 100;
const int buzzer = 9;
void buzz();
int sendx = 0;
const int switchButton = 7;
int mode;
uint8_t id;

//SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial);
//----------------------Fingerprint Global End----------------------

//------------------------ Ethenet Global --------------------------
void setupEthernet();
void runEthernet();
byte mac[] = { 0xDE, 0xED, 0xCE, 0xEF, 0xFE, 0xFF };

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(192,168,0,121);  // numeric IP for Google (no DNS)
char server[] = "192.168.0.121";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;  // set to false for better speed measurement
//---------------------- Ethenet Global End ------------------------

//---------------------------GSM Global-----------------------------
// Include the GSM library
SoftwareSerial sim800l(2, 3); 
// RX,TX for Arduino and for the module it's TXD RXD, 
//they should be inverted
void sendSMS();

//------------------------GSM Global End----------------------------

void setup()
{
  Wire.begin();
  //mySerial.begin(57600);
  //while(!mySerial);
  Serial.begin(57600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  pinMode(buzzer, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(txon, OUTPUT);
  pinMode(switchButton, INPUT);
  digitalWrite(switchButton, HIGH);
  digitalWrite(txon, HIGH);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(buzzer, LOW);

  //setupEthernet();
  //setupGSM();
  //Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(1000);
  if (finger.verifyPassword()) {
    //Serial.println("Found fingerprint sensor!");
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(5000);
  } else {
    //Serial.println("Did not find fingerprint sensor :(");
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  //Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  //Serial.println("Waiting for valid finger...");
}



void loop()                     // run over and over again
{
  // sensorid = 1000 + (finger.fingerID);
  mode = digitalRead(switchButton);
  sensorid = 300;
  sendx = 0;
  digitalWrite(buzzer, LOW);
  digitalWrite(txon, HIGH);
  switch (mode) {
    case 0: {
        getFingerprintIDez();
        blinkred();
        digitalWrite(green, HIGH);
        //runEthernet();
        // mode = digitalRead(switchButton);
        break;
      }
    case 1: {
        blinkgreen();
        digitalWrite(red, HIGH);
        enrollnow();
        // mode = digitalRead(switchButton);
        break;
      }
    default: {
        getFingerprintIDez();
        blinkred();
        digitalWrite(green, HIGH);
        //runEthernet();
        // mode = digitalRead(switchButton);
        break;
      }
  }
  //transmit();
  //delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      // Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      // Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      // Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      // Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      // Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      // Serial.println("Could not find fingerprint features");
      return p;
    default:
      // Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    // Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    // Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //buzz3();
    // Serial.println("Did not find a match");
    return p;
  } else {
    // Serial.println("Unknown error");
    return p;
  }

  // found a match!
  // Serial.print("Found ID #"); Serial.print(finger.fingerID);
  // Serial.print(" with confidence of "); Serial.println(finger.confidence);
  //sensorid = finger.fingerID ;
  //transmit();


  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  {
    // buzz3();
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    //buzz3();
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    //buzz3();
    return -1;
  }
  ///if(finger.image2Tz() == FINGERPRINT_OK){

  //if(sendx==2){
  // do{

  //  }
  //  if(sendx == 3)sendx = 0;
  // }

  // found a match!
  // Serial.print("Found ID #"); Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  // sensorid=finger.fingerID ;
  //transmit();
  do {
    if (finger.confidence > 20) {

      sensorid = finger.fingerID ;
      transmit();
      delay(20);
      digitalWrite(txon, LOW);
      buzz();
      digitalWrite(txon, HIGH);
      //sendSMS();
    }
    else {
      buzz2();
    }
  } while (sendx == 1);

  return finger.fingerID;
}

///////////////////////////////////////////////////////////////////////////////////////////enroll

void enrollnow()                     // run over and over again
{
  blinkgreen();
  finger.getTemplateCount();
  id = finger.templateCount + 1;
  while (!  getFingerprintEnroll() ) {
    periodg = 200;
    blinkgreen();
  }
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  //blinkred();
  periodg = 500;
  //Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        periodg = 200;
        //Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        //Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //Serial.println("Imaging error");
        break;
      default:
        //Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  //Serial.println("Remove finger");
  buzz();
  digitalWrite(green, LOW);
  periodg = 1000;
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  //Serial.print("ID "); Serial.println(id);
  p = -1;
  //Serial.println("Place same finger again");
  //digitalWrite(red, LOW);
  periodg = 200;
  blinkgreen();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        //Serial.println("Image taken");
        buzz();
        periodg = 500;
        blinkgreen();
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.print(".");
        blinkgreen();
        periodg = 200;
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        //Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //Serial.println("Imaging error");
        break;
      default:
        //Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  //Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    buzz();
    ///Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    //Serial.println("Fingerprints did not match");
    buzz2();
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }

  //Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    //Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }
}
///////////////////////////////////////////////////////////////enroll



//Ethernet

//Ethernet

//GSM

//GSM
