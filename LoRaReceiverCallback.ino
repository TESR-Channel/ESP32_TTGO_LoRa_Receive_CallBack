#include <Crc16.h>
#include <math.h>
#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>

#define SCK     5
#define LoRa_MISO 19
#define LoRa_MOSI 27
#define SS      18
#define RST     14
#define irqPin  26
#define BAND    921E6//433E6 

byte DataReadfromSerial[50];
bool read_fin = false;
int count = 0;

void setup() 
{
  Serial.begin(115200);
  pinMode(irqPin,INPUT);
  SPI.begin(SCK,LoRa_MISO,LoRa_MOSI,SS);
  LoRa.setPins(SS,RST,irqPin);
  if (!LoRa.begin(BAND)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.onReceive(onReceive); // register the receive callback
  LoRa.receive(); // put the radio into receive mode

}

void loop() 
{
  LoRa.receive();
  while (Serial.available()) 
  {
    byte respone = (byte)Serial.read();
    //Serial.write(respone);
    DataReadfromSerial[count] = respone;
    count++;

    if(respone == 0x0A) // "\n"
    {
      read_fin = true;
    }
  }

  if(read_fin == true)
  {
    // send packet
    LoRa.beginPacket();
    for(byte i=0;i<count-1;i++)
    {
      LoRa.write(DataReadfromSerial[i]);
    }
    LoRa.endPacket();
    count = 0;
    memset(DataReadfromSerial,0,50);
    read_fin = false;
  }
  delay(250);
}

void onReceive(int packetSize) 
{
  byte DataIn[70];
  // read packet
  for (int i = 0; i < packetSize; i++) 
  {
    DataIn[i] = (byte)LoRa.read();
    Serial.write(DataIn[i]);
  }
  Serial.print("\n");
  //memset(DataIn,0,70);
}
