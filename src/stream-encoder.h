#ifndef _STREAM_WRITER_H_
#define _STREAM_WRITER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk.h"

class StreamEncoder {
  uint8_t* stream;
  int cursor;

  public:
    void setResponseId(char uid);
    void reset();
    void writeByte(uint8_t value);
    void writeBool(bool value);
    void writeNumber(int value);
    void writeString(const char* value);
    uint8_t* getStream();
    int getLength();
};

#ifdef __cplusplus
}
#endif

#endif
