#include <RH_RF95.h> // 

#define LED_BUILTIN 16

#define CR01_FREQUENCY 433.0

uint8_t tempdata[30];

RH_RF95 CR01;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  if (!CR01.init()){
    Serial.println("init failed");
  }

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  // Failure to do that will result in extremely low transmit powers.

  //CR01.setModemConfig(CR01.Bw31_25Cr48Sf512);
  CR01.setFrequency(CR01_FREQUENCY);
  CR01.setTxPower(23, false);
}

void loop()
{
  Serial.println("Sending to CR03_server");
  // Send a message to CR03_server

  sprintf(tempdata, "%s", "Hello Server");

  CR01.send(tempdata, sizeof(tempdata));

  CR01.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (CR01.waitAvailableTimeout(3000))
  {
    // Should be a reply message for us now
    if (CR01.recv(buf, &len))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(CR01.lastRssi(), DEC);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is CR03_server running?");
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(400);
}


