/*
    This file is part of the KDE Libraries

    Copyright (C) 2015 Valentin Rusu (valir@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB. If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "defines.h"
#include "ksecrets_crypt.h"

#include <sys/types.h>
#include <errno.h>
#include <memory>

extern "C" {
#include <keyutils.h>
}

#define GCRPYT_NO_DEPRECATED
#include <gcrypt.h>

#define GCRYPT_REQUIRED_VERSION "1.6.0"

#define KSECRETS_SALTSIZE 56
#define KSECRETS_KEYSIZE 256

const char* get_keyname_encrypting();
const char* get_keyname_mac();

int kss_init_gcry()
{
    syslog(KSS_LOG_DEBUG, "ksecrets: setting-up grypt library");
    if (!gcry_check_version(GCRYPT_REQUIRED_VERSION)) {
        syslog(KSS_LOG_ERR, "ksecrets_store: libcrypt version is too old");
        return 0;
    }

    gcry_error_t gcryerr;
    gcryerr = gcry_control(GCRYCTL_INIT_SECMEM, 32768, 0);
    if (gcryerr != 0) {
        syslog(KSS_LOG_ERR, "ksecrets_store: cannot get secure memory: %d", gcryerr);
        return 0;
    }

    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
    syslog(KSS_LOG_DEBUG, "gcrypt library now set-up");
    return 1;
}

int kss_derive_keys(const char* salt, const char* password, char* encryption_key, char* mac_key, size_t keySize)
{
    gpg_error_t gcryerr;

    syslog(KSS_LOG_INFO, "kss_set_credentials: attempting keys generation");
    if (0 == password) {
        syslog(KSS_LOG_INFO, "NULL password given. ksecrets will not be available.");
        return FALSE;
    }

    /* generate both encryption and MAC key in one go */
    char keys[2 * keySize];
    gcryerr = gcry_kdf_derive(password, strlen(password), GCRY_KDF_ITERSALTED_S2K, GCRY_MD_SHA512, salt, 8, KSECRETS_ITERATIONS, 2 * keySize, keys);
    if (gcryerr) {
        syslog(KSS_LOG_ERR, "key derivation failed: code 0x%0x: %s/%s", gcryerr, gcry_strsource(gcryerr), gcry_strerror(gcryerr));
        return FALSE;
    }

    memcpy(encryption_key, keys, keySize);
    memcpy(mac_key, keys + keySize, keySize);
    syslog(KSS_LOG_INFO, "successuflly generated ksecrets keys from user password.");

    return TRUE;
}

int kss_store_keys(const char* encryption_key, const char* mac_key, size_t keySize)
{
    key_serial_t ks;
    const char* key_name = get_keyname_encrypting();
    ks = add_key("user", key_name, encryption_key, keySize, KEY_SPEC_SESSION_KEYRING);
    if (-1 == ks) {
        syslog(KSS_LOG_ERR, "ksecrets: cannot store encryption key in kernel "
                            "keyring: errno=%d (%m)",
            errno);
        return FALSE;
    }
    syslog(KSS_LOG_DEBUG, "ksecrets: encrpyting key now in kernel keyring "
                          "with id %d and desc %s",
        ks, key_name);

    key_name = get_keyname_mac();
    ks = add_key("user", key_name, mac_key, keySize, KEY_SPEC_SESSION_KEYRING);
    if (-1 == ks) {
        syslog(KSS_LOG_ERR, "ksecrets: cannot store mac key in kernel keyring: errno=%d (%m)", errno);
        return FALSE;
    }
    syslog(KSS_LOG_DEBUG, "ksecrets: mac key now in kernel keyring with id %d and desc %s", ks, key_name);
    return TRUE;
}

int kss_set_credentials(const std::string& password, const char* salt)
{
    // FIXME this should be adjusted on platforms where kernel keyring is not
    // available and store the keys elsewhere
    char encryption_key[KSECRETS_KEYSIZE];
    char mac_key[KSECRETS_KEYSIZE];
    auto res = kss_derive_keys(salt, password.c_str(), encryption_key, mac_key, KSECRETS_KEYSIZE);
    if (res)
        return res;

    return kss_store_keys(encryption_key, mac_key, KSECRETS_KEYSIZE);
}

int kss_keys_already_there()
{
    key_serial_t key;
    key = request_key("user", get_keyname_encrypting(), 0, KEY_SPEC_SESSION_KEYRING);
    if (-1 == key) {
        syslog(KSS_LOG_DEBUG, "request_key failed with errno %d (%m), so "
                              "assuming ksecrets not yet loaded",
            errno);
        return FALSE;
    }
    syslog(KSS_LOG_DEBUG, "ksecrets: keys already in keyring");
    return TRUE;
}

long kss_read_key(const char* keyName, char* buffer, size_t bufferSize)
{
    key_serial_t key;
    key = request_key("user", keyName, 0, KEY_SPEC_SESSION_KEYRING);
    if (-1 == key) {
        syslog(KSS_LOG_DEBUG, "request_key failed with errno %d (%m) when reading MAC key %s", errno, keyName);
        return -1;
    }
    auto bytes = keyctl_read(key, buffer, bufferSize);
    if (bytes == -1) {
        syslog(KSS_LOG_ERR, "error reading key %s contents from the keyring", keyName);
        return -1;
    }
    if ((size_t)bytes > bufferSize) {
        return bytes;
    }
    return 0; // key contents correctly transffered into the buffer
}

long kss_read_mac_key(char* buffer, size_t bufferSize) { return kss_read_key(get_keyname_mac(), buffer, bufferSize); }

long kss_read_encrypting_key(char* buffer, size_t bufferSize) { return kss_read_key(get_keyname_encrypting(), buffer, bufferSize); }

#define ERRNO(cryres) gcry_err_code_to_errno(gcry_err_code(cryres))

long kss_cipher_setup(gcry_cipher_hd_t* hd, const void* iv, size_t liv)
{
    // FIXME perhaps all this initialization stuff could only be done once,
    // when password is setup
    auto cryres = gcry_cipher_open(hd, GCRY_CIPHER_BLOWFISH, GCRY_CIPHER_MODE_CBC, 0);
    if (cryres) {
        syslog(KSS_LOG_ERR, "ksecrets: gcry_cipher_open returned error %d", cryres);
        return ERRNO(cryres);
    }
    cryres = gcry_cipher_setiv(*hd, iv, liv);
    if (cryres) {
        syslog(KSS_LOG_ERR, "ksecrets: gcry_cipher_setif returned error %d", cryres);
        return ERRNO(cryres);
    }
    char encryptingKey[KSECRETS_KEYSIZE];
    auto keyres = kss_read_encrypting_key(encryptingKey, sizeof(encryptingKey) / sizeof(encryptingKey[0]));
    if (!keyres) {
        syslog(KSS_LOG_ERR, "ksecrets: encrypting key not found in the keyring");
        return keyres;
    }
    cryres = gcry_cipher_setkey(*hd, encryptingKey, sizeof(encryptingKey) / sizeof(encryptingKey[0]));
    if (cryres) {
        syslog(KSS_LOG_ERR, "ksecrets: gcry_cipher_setkey returned %d", cryres);
        return ERRNO(cryres);
    }
    return 0;
}

long kss_encrypt_buffer(unsigned char* out, size_t lout, const void* iv, size_t liv, const unsigned char* in, size_t lin)
{
    gcry_cipher_hd_t hd;
    auto cryres = kss_cipher_setup(&hd, iv, liv);
    if (cryres)
        return cryres; // error already logged
    cryres = gcry_cipher_encrypt(hd, out, lout, in, lin);
    if (cryres) {
        syslog(KSS_LOG_ERR, "ksecrets: gcry_cipher_encrypt returned %ld", cryres);
        return ERRNO(cryres);
    }
    return 0;
}

long kss_decrypt_buffer(unsigned char* out, size_t lout, const void* iv, size_t liv, const unsigned char* in, size_t lin)
{
    gcry_cipher_hd_t hd;
    auto cryres = kss_cipher_setup(&hd, iv, liv);
    if (cryres)
        return cryres; // error already logged
    cryres = gcry_cipher_decrypt(hd, out, lout, in, lin);
    if (cryres) {
        syslog(KSS_LOG_ERR, "ksecrets: gcry_cipher_decrypt returned %ld", cryres);
        return ERRNO(cryres);
    }
    return 0;
}

CryptBuffer::~CryptBuffer() { delete[] data_; }

bool CryptBuffer::resize(size_t rlen)
{
    if (rlen <= len_) {
        return false; // if less bytes, we cannot copy; if same, why resize?
    }
    CryptBuffer oldBuffer(std::move(*this));
    len_ = (rlen / cipherBlockLen_ + 1) * cipherBlockLen_;
    data_ = new unsigned char[len_];
    if (data_ && oldBuffer.len_) {
        memmove(data_, oldBuffer.data_, oldBuffer.len_);
        return true;
    }
    return false;
}

void CryptBuffer::empty()
{
    delete[] data_, data_ = nullptr;
    len_ = 0;
}

bool CryptBuffer::allocate(size_t rlen)
{
    empty();
    data_ = new unsigned char[rlen];
    if (data_ == nullptr) {
        return false;
    }
    len_ = rlen;
    return true;
}
// vim: tw=220:ts=4
