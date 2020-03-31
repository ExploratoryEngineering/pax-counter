#include "Arduino.h"
#include "mac_pool.h"

bool MACSighting::operator==(const MACSighting &b)
{
    return _mac == b._mac;
}


MACAddressPool::MACAddressPool(unsigned long age_limit) :
    _age_limit(age_limit)
{
}

void MACAddressPool::Add(MACSighting mac)
{
    auto it = std::find(_address_pool.begin(), _address_pool.end(), mac);
    if (it == _address_pool.end())
        _address_pool.push_back(mac);
}

void MACAddressPool::Purge()
{
    unsigned long time = millis();
    auto it = _address_pool.begin();
    while (it != _address_pool.end())
    {
        if (time - it->_time > _age_limit) 
        {
            *it = std::move(_address_pool.back());
            _address_pool.pop_back();
            continue;
        }
        ++it;
    }
}

int MACAddressPool::get_count(TARGET_T target_type)
{
    int count = 0;
    for (auto it = _address_pool.begin() ; it != _address_pool.end(); ++it)
    {
        if (it->_target_type == target_type)
            count++;
    }
    return count;
}

void MACAddressPool::Log()
{
    Serial.printf("--- DEVICES ---\r\n");
    for (auto it = _address_pool.begin() ; it != _address_pool.end(); ++it)
    {
        if (BT == it->_target_type)
            Serial.print("BT  : ");
        else
            Serial.print("WIFI: ");
        Serial.printf("%s\r\n", it->_mac.c_str());
    }
}

