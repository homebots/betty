#ifndef _STREAM_READER_H_
#define _STREAM_READER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk.h"

class StreamReader {
  int position;
  void copyBytes(unsigned char* target, unsigned char* source, size_t amount);

  public:
    unsigned char* stream;
    int length;

    StreamReader(unsigned char *_stream, int _length): stream(_stream), length(_length) {
      position = 0;
    }
    unsigned char* readString();
    char readByte();
    bool readBool();
    int readLong() ;
    bool isNotEmpty();
};

#ifdef __cplusplus
}
#endif

#endif
