#ifndef _STREAM_READER_
#define _STREAM_READER_

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk.h"
#include "mem.h"
#include "string-extras.h"
#include "stream-reader.h"

void ICACHE_FLASH_ATTR StreamReader::copyBytes(unsigned char* target, unsigned char* source, size_t amount) {
  for (int i = 0; i < amount; i++) {
    target[i] = source[i];
  }
}

unsigned char* ICACHE_FLASH_ATTR StreamReader::readString() {
  int size = strlen((const char*)stream) + 1;
  unsigned char* chars = (unsigned char*) os_malloc(size);
  copyBytes(chars, stream, size);
  stream += size;

  DEBUG("Read string %s\n", chars);
  return chars;
}

char ICACHE_FLASH_ATTR StreamReader::readByte() {
  unsigned char number = (unsigned char)*stream;
  stream++;

  DEBUG("Read char %d\n", number);
  return number;
}

bool ICACHE_FLASH_ATTR StreamReader::readBool() {
  char number = (char)*stream;
  stream++;
  bool value = number == 0x01;

  DEBUG("Read bool %d\n", value);
  return value;
}

int ICACHE_FLASH_ATTR StreamReader::readLong() {
  unsigned char* bytes = (unsigned char*) os_malloc(5);
  bytes[4] = 0x00;
  copyBytes(bytes, stream, 4);
  stream += 4;
  long number = strtoul((const char*)bytes, NULL, 16);
  os_free(bytes);

  DEBUG("Read long %ld from %s\n", number, bytes);
  return number;
}

#ifdef __cplusplus
}
#endif

#endif
