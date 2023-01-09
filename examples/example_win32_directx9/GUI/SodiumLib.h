#pragma once
#define SODIUM_STATIC
#include <sodium.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>


class SodiumLib
{
public:
    static SodiumLib* GetSodium();
    bool InitSodium();
    static std::string writeAsHex(unsigned char* text, int text_length);
    static unsigned char* readHex(std::string input);
    void setupKeys();
    void setupNonce();
    bool decryptMessage(unsigned char* decrypted, unsigned char* cipher, int length, unsigned char* nonce, unsigned char* key);
    bool encryptMessage(unsigned char* cipher, unsigned char* message, int length, unsigned char* nonce, unsigned char* key);

    unsigned char internal_public_key[crypto_box_PUBLICKEYBYTES];
    unsigned char internal_private_key[crypto_box_SECRETKEYBYTES];
    unsigned char internal_secret_key[crypto_stream_chacha20_KEYBYTES];
    unsigned char internal_nonce[crypto_stream_chacha20_NONCEBYTES];


private:
    static SodiumLib* sodium_lib_;
};
