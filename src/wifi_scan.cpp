#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <vector>
#include <string>

#include "esp_wifi.h"

const wifi_promiscuous_filter_t filt= { .filter_mask=WIFI_PROMIS_FILTER_MASK_MGMT | WIFI_PROMIS_FILTER_MASK_DATA };

std::vector<std::string> mac_addresses;

typedef struct 
{ 
  uint8_t mac[6];
} __attribute__((packed)) MacAddr;

typedef struct {
    unsigned frame_ctrl:16;
    unsigned duration_id:16;
    uint8_t receiver_address[6];
    uint8_t sender_address[6];
    uint8_t filtering_address[6];
    unsigned sequence_ctrl:16;
    uint8_t optional_address[6];
} wifi_ieee80211_mac_hdr_t;

typedef struct {
    wifi_ieee80211_mac_hdr_t hdr;
    uint8_t payload[0]; // CRC32 checksum is appended to network data
} wifi_ieee80211_packet_t;

#define MAX_CHANNELS 14

char tmp[32];

void promiscuous_callback(void* buffer, wifi_promiscuous_pkt_type_t type) 
{ 
    if (WIFI_PKT_MISC == type)
        return; // Only MGMT, CTRL & DATA contains a payload

    const wifi_promiscuous_pkt_t * promiscuous_packet = (wifi_promiscuous_pkt_t *)buffer;
    const wifi_ieee80211_packet_t *packet = (wifi_ieee80211_packet_t *)promiscuous_packet->payload;
    const wifi_ieee80211_mac_hdr_t *header = &packet->hdr;

    sprintf(tmp, "%02X:%02X:%02X:%02X:%02X:%02X", 
        header->sender_address[0],
        header->sender_address[1],
        header->sender_address[2],
        header->sender_address[3],
        header->sender_address[4],
        header->sender_address[5]
        );

    std::string address = std::string(tmp);
    if (std::find(mac_addresses.begin(), mac_addresses.end(),address)==mac_addresses.end())
    {
        mac_addresses.push_back(address);
    }
}

void wifi_scanner_setup() 
{
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_start();
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_filter(&filt);
  esp_wifi_set_promiscuous_rx_cb(&promiscuous_callback);
}

void print_mac_addresses()
{ 
    if (mac_addresses.size() != 0)
        Serial.println("-----------------");
    for (size_t i=0; i<mac_addresses.size(); i++)
    {
        Serial.println(mac_addresses[i].c_str());
    }
}

int wifi_scan() 
{
    Serial.println("Scanning wifi...");

    for (int channel=1; channel <=MAX_CHANNELS; channel++)
    {
        esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
        delay(2000);
        print_mac_addresses();
    }

    return mac_addresses.size();
}
