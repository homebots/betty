#ifndef _STREAM_READER_H_
#define _STREAM_READER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk.h"

class StreamDecoder {
  void copyBytes(uint8_t* target, uint8_t* source, size_t amount);

  public:
    uint8_t* stream;
    int length;
    int position;

    uint8_t* readString();
    char readByte();
    bool readBool();
    uint32_t readNumber();
    bool isNotEmpty();
    void setStream(char* input, int length);
};

#ifdef __cplusplus
}
#endif

#endif
