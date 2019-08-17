#ifndef _STREAM_READER_H_
#define _STREAM_READER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk.h"

class StreamReader {
  void copyBytes(unsigned char* target, unsigned char* source, size_t amount);

  public:
    unsigned char* stream;

    StreamReader(unsigned char *_stream): stream(_stream) {}
    unsigned char* readString();
    char readByte();
    bool readBool();
    int readLong() ;
};

#ifdef __cplusplus
}
#endif

#endif
