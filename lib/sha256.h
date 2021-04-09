// http://www.zedwood.com/article/cpp-sha256-function
/*
 * Updated to C++, zedwood.com 2012
 * Based on Olivier Gay's version
 * See Modified BSD License below: 
 *
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Issue date:  04/30/2005
 * http://www.ouah.org/ogay/sha2/
 *
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#pragma once
#include <string>

class SHA256 {
protected:
  typedef unsigned char uint8;
  typedef unsigned int uint32;
  typedef unsigned long long uint64;

  const static uint32 sha256_k[];
  static const unsigned int SHA224_256_BLOCK_SIZE = (512 / 8);

public:
  void init();
  void update(const unsigned char *message, unsigned int len);
  void final(unsigned char *digest);
  static const unsigned int DIGEST_SIZE = (256 / 8);

protected:
  void transform(const unsigned char *message, unsigned int block_nb);
  unsigned int m_tot_len;
  unsigned int m_len;
  unsigned char m_block[2 * SHA224_256_BLOCK_SIZE];
  uint32 m_h[8];
};

string sha256(string input);

#define SHFR(x, n) (x >> n)
#define ROTR(x, n) ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define ROTL(x, n) ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define CH(x, y, z) ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_F1(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SHA256_F2(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SHA256_F3(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ SHFR(x, 3))
#define SHA256_F4(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHFR(x, 10))
#define UNPACK32(x, str)                                                       \
  {                                                                            \
    *((str) + 3) = (uint8)((x));                                               \
    *((str) + 2) = (uint8)((x) >> 8);                                          \
    *((str) + 1) = (uint8)((x) >> 16);                                         \
    *((str) + 0) = (uint8)((x) >> 24);                                         \
  }
#define PACK32(str, x)                                                         \
  {                                                                            \
    *(x) = ((uint32) * ((str) + 3)) | ((uint32) * ((str) + 2) << 8) |          \
           ((uint32) * ((str) + 1) << 16) | ((uint32) * ((str) + 0) << 24);    \
  }

// sha256.cpp
// #include <cstring>
// #include <fstream>

const unsigned int SHA256::sha256_k[64] = // UL = uint32
    {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
     0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
     0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
     0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
     0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
     0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
     0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
     0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
     0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
     0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
     0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void SHA256::transform(const unsigned char *message, unsigned int block_nb) {
  uint32 w[64];
  uint32 wv[8];
  uint32 t1, t2;
  const unsigned char *sub_block;
  int i;
  int j;
  for (i = 0; i < (int)block_nb; i++) {
    sub_block = message + (i << 6);
    for (j = 0; j < 16; j++) {
      PACK32(&sub_block[j << 2], &w[j]);
    }
    for (j = 16; j < 64; j++) {
      w[j] = SHA256_F4(w[j - 2]) + w[j - 7] + SHA256_F3(w[j - 15]) + w[j - 16];
    }
    for (j = 0; j < 8; j++) {
      wv[j] = m_h[j];
    }
    for (j = 0; j < 64; j++) {
      t1 = wv[7] + SHA256_F2(wv[4]) + CH(wv[4], wv[5], wv[6]) +
           sha256_k[j] + w[j];
      t2 = SHA256_F1(wv[0]) + MAJ(wv[0], wv[1], wv[2]);
      wv[7] = wv[6];
      wv[6] = wv[5];
      wv[5] = wv[4];
      wv[4] = wv[3] + t1;
      wv[3] = wv[2];
      wv[2] = wv[1];
      wv[1] = wv[0];
      wv[0] = t1 + t2;
    }
    for (j = 0; j < 8; j++) {
      m_h[j] += wv[j];
    }
  }
}

void SHA256::init() {
  m_h[0] = 0x6a09e667;
  m_h[1] = 0xbb67ae85;
  m_h[2] = 0x3c6ef372;
  m_h[3] = 0xa54ff53a;
  m_h[4] = 0x510e527f;
  m_h[5] = 0x9b05688c;
  m_h[6] = 0x1f83d9ab;
  m_h[7] = 0x5be0cd19;
  m_len = 0;
  m_tot_len = 0;
}

void SHA256::update(const unsigned char *message, unsigned int len) {
  unsigned int block_nb;
  unsigned int new_len, rem_len, tmp_len;
  const unsigned char *shifted_message;
  tmp_len = SHA224_256_BLOCK_SIZE - m_len;
  rem_len = len < tmp_len ? len : tmp_len;
  memcpy(&m_block[m_len], message, rem_len);
  if (m_len + len < SHA224_256_BLOCK_SIZE) {
    m_len += len;
    return;
  }
  new_len = len - rem_len;
  block_nb = new_len / SHA224_256_BLOCK_SIZE;
  shifted_message = message + rem_len;
  transform(m_block, 1);
  transform(shifted_message, block_nb);
  rem_len = new_len % SHA224_256_BLOCK_SIZE;
  memcpy(m_block, &shifted_message[block_nb << 6], rem_len);
  m_len = rem_len;
  m_tot_len += (block_nb + 1) << 6;
}

void SHA256::final(unsigned char *digest) {
  unsigned int block_nb;
  unsigned int pm_len;
  unsigned int len_b;
  int i;
  block_nb =
      (1 + ((SHA224_256_BLOCK_SIZE - 9) < (m_len % SHA224_256_BLOCK_SIZE)));
  len_b = (m_tot_len + m_len) << 3;
  pm_len = block_nb << 6;
  memset(m_block + m_len, 0, pm_len - m_len);
  m_block[m_len] = 0x80;
  UNPACK32(len_b, m_block + pm_len - 4);
  transform(m_block, block_nb);
  for (i = 0; i < 8; i++) {
    UNPACK32(m_h[i], &digest[i << 2]);
  }
}

string sha256(string input) {
  unsigned char digest[SHA256::DIGEST_SIZE];
  memset(digest, 0, SHA256::DIGEST_SIZE);

  SHA256 ctx = SHA256();
  ctx.init();
  ctx.update((unsigned char *)input.c_str(), input.length());
  ctx.final(digest);

  char buf[2 * SHA256::DIGEST_SIZE + 1];
  buf[2 * SHA256::DIGEST_SIZE] = 0;
  for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
    sprintf(buf + i * 2, "%02x", digest[i]);
  return string(buf);
}
 
TEST(sha256){
  CHECK(sha256("grape") ==
    "0f78fcc486f5315418fbf095e71c0675ee07d318e5ac4d150050cd8e57966496");
  CHECK(sha256("0") ==
    "5feceb66ffc86f38d952786c6d696c79c2dbc239dd4e91b46729d73a27fb57e9");
}