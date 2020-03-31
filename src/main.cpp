#include <Arduino.h>

#include "TelenorNBIot.h"
#include "wifi_scan.h"
#include "ble_scan.h"
#include "mac_pool.h"

MACAddressPool pool(300000); // Age limit: 5 minutes

void setup() 
{
  wifi_scanner_setup();
  ble_scanner_setup();
  nbiot_setup();
  Serial.begin(115200);
 }

void loop() 
{
  while (true)
  {
    wifi_scan();
    ble_scan();
    nbiot_transmit_message(pool.get_count(BT), pool.get_count(WIFI));
    pool.Purge();
    pool.Log();
  }
}