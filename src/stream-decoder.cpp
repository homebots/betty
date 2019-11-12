#ifndef _STREAM_DECODER_
#define _STREAM_DECODER_

#ifdef __cplusplus
extern "C" {
#endif

#include "serial-debug.h"
#include "sdk.h"
#include "stream-decoder.h"
#include <stdlib.h>

void ICACHE_FLASH_ATTR StreamDecoder::copyBytes(uint8_t* target, uint8_t* source, size_t amount) {
  for (int i = 0; i < amount; i++) {
    target[i] = source[i];
  }
}

bool ICACHE_FLASH_ATTR StreamDecoder::isEmpty() {
  return length == 0 || position >= length;
}

void ICACHE_FLASH_ATTR StreamDecoder::end() {
  position = length;
}

void ICACHE_FLASH_ATTR StreamDecoder::rewind() {
  position = 0;
}

void ICACHE_FLASH_ATTR StreamDecoder::goTo(int newPosition) {
  position = newPosition;
}

void ICACHE_FLASH_ATTR StreamDecoder::setStream(char* input, int size) {
  position = 0;
  length = 0;

  if (stream) {
    os_free(stream);
  }

  LOG("COPY %d\n", size);
  length = size;
  stream = (uint8_t*)os_zalloc(size);
  copyBytes(stream, (uint8_t*)input, size);
}

uint8_t* ICACHE_FLASH_ATTR StreamDecoder::readString() {
  uint32_t size = readNumber();
  uint8_t* chars = (uint8_t*) os_zalloc(size + 1);
  copyBytes(chars, stream, size);
  position += size;

  return chars;
}

char ICACHE_FLASH_ATTR StreamDecoder::readByte() {
  uint8_t number = (uint8_t)stream[position];
  position++;

  return number;
}

bool ICACHE_FLASH_ATTR StreamDecoder::readBool() {
  char number = (char)stream[position];
  position++;

  return number & 0x01;
}

uint32_t ICACHE_FLASH_ATTR StreamDecoder::readNumber() {
  uint32_t number =
    stream[position++] << 12 |
    stream[position++] << 8 |
    stream[position++] << 4 |
    stream[position++];

  return number;
}

#ifdef __cplusplus
}
#endif

#endif
