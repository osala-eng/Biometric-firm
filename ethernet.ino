

void setupEthernet() {
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields
  
  // start the Ethernet connection:
  //Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      //Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      //Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } 
  else {
    //Serial.print("  DHCP assigned IP ");
    //Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  //Serial.print("connecting to ");
  //Serial.print(server);
  //Serial.println("...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 3000)) {
    //Serial.print("connected to ");
    //Serial.println(client.remoteIP());
    // Make a HTTP request:
    client.println("GET /sms HTTP/1.1");
    client.println("Host: 192.168.0.121");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    //Serial.println("connection failed");
  }
  beginMicros = micros();
}

void runEthernet() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  int len = client.available();
  if (len > 0) {
    byte buffer[80];
    if (len > 80) len = 80;
    client.read(buffer, len);
    /*if (printWebData) {
      Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    }*/
    byteCount = byteCount + len;
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    endMicros = micros();
    //Serial.println();
    //Serial.println("disconnecting.");
    client.stop();
    //Serial.print("Received ");
    //Serial.print(byteCount);
    //Serial.print(" bytes in ");
    float seconds = (float)(endMicros - beginMicros) / 1000000.0;
    //Serial.print(seconds, 4);
    float rate = (float)byteCount / seconds / 1000.0;
    //Serial.print(", rate = ");
    //Serial.print(rate);
    //Serial.print(" kbytes/second");
    //Serial.println();

    // beep if true:
    if (true) {
      beepAfter1minute();
    }
  }
}
