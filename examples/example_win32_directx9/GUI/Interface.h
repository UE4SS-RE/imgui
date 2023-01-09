#pragma once
#include "SodiumLib.h"
#include "Fonts/Roboto.hpp"

class Interface
{
public:
    static Interface* GetInterface();
    void DrawInterface();
    void SetupFonts();

    char test[256]="";
    unsigned char cipher[crypto_secretbox_MACBYTES + sizeof test]="";
    unsigned char decrypted[crypto_secretbox_MACBYTES + sizeof test]{};
private:
    static Interface* ifx;
};
