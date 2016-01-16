#pragma once

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

};

static EEPROMClass EEPROM;
