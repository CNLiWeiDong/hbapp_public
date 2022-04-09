#pragma once

#include <cryptopp/aes.h>
#include <cryptopp/base32.h>  // Base64Encoder
#include <cryptopp/base64.h>  // Base64Encoder
#include <cryptopp/files.h>   // FileSource, FileSink
#include <cryptopp/gcm.h>     // GCM模式支持
#include <cryptopp/hex.h>     // HexEncoder
#include <cryptopp/md5.h>
#include <cryptopp/modes.h>  // CFB_Mode
#include <cryptopp/osrng.h>  // AutoSeededRandomPool
#include <cryptopp/pem.h>
#include <cryptopp/pem_common.h>
#include <cryptopp/rsa.h>  // RSAES_OAEP_SHA_Decryptor
#include <cryptopp/sha.h>