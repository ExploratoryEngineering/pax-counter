#ifndef MAC_POOL_H
#define MAC_POOL_H

#include <string>
#include <vector>

typedef enum TARGET
{
    BT,
    WIFI
} TARGET_T;

class MACSighting
{
    public:
        MACSighting( TARGET_T target_type, std::string mac) :
            _target_type(target_type),
            _mac(mac),
            _time(millis()) {}

        TARGET_T        _target_type;
        std::string     _mac;
        unsigned long   _time;

    bool operator==(const MACSighting &b);
        
};

class MACAddressPool
{
    public:
         MACAddressPool(unsigned long age_limit /*in milliseconds */);
         void Add(MACSighting mac);
         void Purge();
         void Log();
         int  get_count(TARGET_T target_type);

    private:
        std::vector<MACSighting> _address_pool;
        unsigned long _age_limit;
};


#endif // MAC_POOL_H