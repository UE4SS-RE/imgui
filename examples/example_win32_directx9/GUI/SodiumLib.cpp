#include "SodiumLib.h"

#include <iostream>
SodiumLib* SodiumLib::sodium_lib_ = nullptr;

SodiumLib* SodiumLib::GetSodium()
{
    if (!sodium_lib_)
    {
        sodium_lib_ = new SodiumLib();
    }
    return sodium_lib_;
}

bool SodiumLib::InitSodium()
{
    if (sodium_init() == 0)
    {
        return true;
    }
    if (sodium_init() == -1)
    {
        return false;
    }
    return false;
}

std::string SodiumLib::writeAsHex(unsigned char* text, int text_length)
{
    std::stringstream ss = {};
    for (int i = 0; i < text_length; ++i)
        ss << std::hex << static_cast<int>(text[i]);
    std::string mystr = ss.str();
    return mystr;
}

unsigned char* SodiumLib::readHex(std::string input)
{
    unsigned char *val=new unsigned char[input.length()+1];
    strcpy((char *)val,input.c_str());
    return val;
}


void SodiumLib::setupKeys()
{
    crypto_box_keypair(internal_public_key, internal_private_key);
    sodium_mprotect_readonly(internal_public_key);
    sodium_mprotect_readonly(internal_private_key);
#ifdef _DEBUG
    printf("pub key:  %s\npriv key: %s\n", writeAsHex(internal_public_key, sizeof internal_public_key).data(),
           writeAsHex(internal_private_key, sizeof internal_private_key).data());
#endif
}

void SodiumLib::setupNonce()
{
    crypto_secretbox_keygen(internal_secret_key);
    randombytes_buf(internal_nonce, sizeof internal_nonce);
    std::string test(std::begin(internal_secret_key), std::end(internal_secret_key));
#ifdef _DEBUG
    printf("secret key: %s\nnonce: %s\n", writeAsHex(internal_secret_key, sizeof internal_secret_key).data(),
           writeAsHex(internal_nonce, sizeof
                      internal_nonce).data());
#endif
    sodium_mprotect_readonly(internal_nonce);
    sodium_mprotect_readonly(internal_secret_key);
}

bool SodiumLib::decryptMessage(unsigned char* decrypted, unsigned char* cipher, int length, unsigned char* nonce,
                               unsigned char* key)
{
    auto errorcode = crypto_secretbox_open_easy(decrypted, cipher, length, nonce, key);
    if (errorcode != 0)
    {
        return false;
    }
    sodium_memzero(cipher, sizeof cipher);
    sodium_memzero(&cipher, sizeof cipher);
    sodium_memzero(&decrypted, sizeof decrypted);
    return true;
}

bool SodiumLib::encryptMessage(unsigned char* cipher, unsigned char* message, int length, unsigned char* nonce,
                               unsigned char* key)
{
    auto errorcode = crypto_secretbox_easy(cipher, message, length, nonce, key);
    if (errorcode != 0)
    {
        return false;
    }
    sodium_memzero(message, sizeof message);
    sodium_memzero(&cipher, sizeof cipher);
    sodium_memzero(&message, sizeof message);
    return true;
}
