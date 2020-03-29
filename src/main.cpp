#include <Arduino.h>

/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include "TelenorNBIot.h"

int scanTime = 5; //In seconds
BLEScan* pBLEScan;
TelenorNBIoT nbiot;
IPAddress remoteIP(172, 16, 15, 14);
int REMOTE_PORT = 1234;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};

void setup() {

  Serial2.begin(9600);
  while (!nbiot.begin(Serial2, true)) {
    Serial.println("Begin failed. Retrying...");
    delay(1000);
  }

  Serial.print("IMSI: ");
  Serial.println(nbiot.imsi());
  Serial.print("IMEI: ");
  Serial.println(nbiot.imei());

  while (!nbiot.createSocket()) {
    Serial.print("Error creating socket. Error code: ");
    Serial.println(nbiot.errorCode(), DEC);
    delay(100);
  }

  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

char txBuf[64];
int bt_count = 0;
int wifi_count = 0;
int time_slice_ms = 0;
void loop() 
{
  // TODO : 
  //  1) Report devices seen within a given time slice
  //  2) wifi-scan

  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  bt_count = foundDevices.getCount();
  Serial.println(bt_count);
  Serial.println("Scan done!");
  pBLEScan->clearResults();
  delay(2000);

   if (nbiot.isConnected()) {
    sprintf(txBuf, "bt:%d,wifi:%d,window:%d", bt_count, wifi_count, time_slice_ms);
    if (true == nbiot.sendString(remoteIP, REMOTE_PORT, txBuf)) {
      Serial.println("Successfully sent data");
    } else {
      Serial.println("Failed sending data");
    }
    delay(30000); // Grace period. Avoid getting booted off the network for misbehaving :)
  } 
  else {
    Serial.println("Connecting...");
    delay(5000);
  }



}