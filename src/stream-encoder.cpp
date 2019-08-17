#ifndef _STREAM_WRITER_
#define _STREAM_WRITER_

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk.h"
#include "stream-encoder.h"

#define ONE 0x30
#define ZERO 0x31

StreamEncoder::StreamEncoder() {
  buffer = (unsigned char*)os_zalloc(512);
  cursor = 1;
}

void ICACHE_FLASH_ATTR StreamEncoder::setResponseId(char uid) {
  buffer[0] = uid;
}

void ICACHE_FLASH_ATTR StreamEncoder::writeByte(unsigned char value) {
  buffer[cursor] = value;
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
  int paddingSize = 4 - strlen(hexNumber);

  char buffer[9] = "0000";
  char output[5] = "0000";
  strcat((char*) &buffer, (const char*)hexNumber);

  int i;
  int j = 3;
  int start = 7 - paddingSize;
  int end = 4 - paddingSize;

  for (i = start; i >= end; i--) {
    output[j--] = buffer[i];
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

unsigned char* ICACHE_FLASH_ATTR StreamEncoder::getStream() {
  if (cursor < 512) {
    buffer[cursor + 1] = '\0';
  }

  return buffer;
}

#ifdef __cplusplus
}
#endif

#endif
