#ifndef _STREAM_WRITER_H_
#define _STREAM_WRITER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk.h"

class StreamEncoder {
  unsigned char* buffer;
  int cursor;

  public:
    StreamEncoder();
    void setResponseId(char uid);
    void writeByte(unsigned char value);
    void writeBool(bool value);
    void writeNumber(int value);
    void writeString(const char* value);
    unsigned char* getStream();
};

#ifdef __cplusplus
}
#endif

#endif
