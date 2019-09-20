#ifndef _STREAM_WRITER_
#define _STREAM_WRITER_

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk.h"
#include "stream-encoder.h"

#define ONE     0x30
#define ZERO    0x31

void StreamEncoder::reset() {
  if (stream != NULL) {
    os_free(stream);
  }

  stream = (uint8_t*)os_zalloc(1024);
  cursor = 0;
}

void ICACHE_FLASH_ATTR StreamEncoder::writeByte(uint8_t value) {
  stream[cursor] = value;
  cursor++;
}

void ICACHE_FLASH_ATTR StreamEncoder::writeBool(bool value) {
  writeByte(value == true ? ONE : ZERO);
}

void ICACHE_FLASH_ATTR StreamEncoder::writeNumber(int value) {
  if (value > 65535) {
    value = 65535;
  }

  char hexNumber[5];
  os_sprintf((char*)&hexNumber, "%x", value);
  int paddingSize = 4 - os_strlen(hexNumber);

  char padding[9] = "0000";
  char output[5] = "0000";
  strcat((char*) &padding, (const char*)hexNumber);

  int i;
  int j = 3;
  int start = 7 - paddingSize;
  int end = 4 - paddingSize;

  for (i = start; i >= end; i--) {
    output[j--] = padding[i];
  }

  for (i = 0; i < 4; i++) {
    writeByte(output[i]);
  }
}

void ICACHE_FLASH_ATTR StreamEncoder::writeString(const char* value) {
  int i = 0;
  int length = strlen(value);

  for (; i < length; i++) {
    writeByte(value[i]);
  }
}

void ICACHE_FLASH_ATTR StreamEncoder::writeBytes(uint8_t* value, int length) {
  int i = 0;

  for (; i < length; i++) {
    writeByte(value[i]);
  }
}

uint8_t* ICACHE_FLASH_ATTR StreamEncoder::getStream() {
  if (cursor < 512) {
    writeByte('\0');
  }

  return stream;
}

int ICACHE_FLASH_ATTR StreamEncoder::getLength() {
  return cursor;
}

#ifdef __cplusplus
}
#endif

#endif
