#include <SPI.h>
#include <LoRa.h>

#define SCK     5
#define LoRa_MISO 19
#define LoRa_MOSI 27
#define SS      18
#define RST     14
#define irqPin  26
#define BAND    921E6//433E6 

#define RXD2 16
#define TXD2 17

void setup() 
{
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(RXD2,INPUT_PULLUP);
  pinMode(irqPin,INPUT);
  SPI.begin(SCK,LoRa_MISO,LoRa_MOSI,SS);
  LoRa.setPins(SS,RST,irqPin);
  if (!LoRa.begin(BAND)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.onReceive(onReceive); // register the receive callback
  LoRa.receive();
}

void loop() 
{
  while (Serial2.available()) 
  {
    byte respone = (byte)Serial2.read();
    LoRa.write(respone);
    Serial.write(respone);
  }
  delay(100);
}

void onReceive(int packetSize) 
{
  for (int i = 0; i < packetSize; i++) 
  {
    byte LoRaData = (byte)LoRa.read();
    Serial.write(LoRaData);
    Serial2.write(LoRaData);
  }
  Serial.print("\n");
  Serial2.print("\n");
}
