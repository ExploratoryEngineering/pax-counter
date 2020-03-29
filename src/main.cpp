#include <Arduino.h>

#include "TelenorNBIot.h"
#include "wifi_scan.h"
#include "ble_scan.h"

void setup() 
{
  wifi_scanner_setup();
  ble_scanner_setup();
  nbiot_setup();
  Serial.begin(115200);
 }

void loop() 
{
    int wifi_count = wifi_scan();
    int bt_count = ble_scan();

    nbiot_transmit_message(bt_count, wifi_count);
}