#pragma once

#include <stdint.h>

class EEPROMClass
{
public:

    template< typename T > T &get(int idx, T &t )
    {
        idx = 0;
        return t;
    }

    template< typename T > const T &put(int idx, const T &t )
    {
        idx = 0;
        return t;
    }

    int length() {return 0;}
    int read(int ) {return 0;}
    void write( int , uint8_t  )   {}
    void update( int , uint8_t  )  {}

};

static EEPROMClass EEPROM;
