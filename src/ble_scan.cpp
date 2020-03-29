#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include "ble_scan.h"

BLEScan* pBLEScan;
int scanTime = 5; //In seconds

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
{
    void onResult(BLEAdvertisedDevice advertisedDevice) 
    {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};

void ble_scanner_setup()
{
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);  // less or equal setInterval value
}

int ble_scan()
{
    int ble_device_count;
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    Serial.print("BLE Devices found: ");
    ble_device_count = foundDevices.getCount();
    Serial.println(ble_device_count);
    pBLEScan->clearResults();
    delay(2000);
    return ble_device_count;
}
