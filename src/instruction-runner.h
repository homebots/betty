#ifndef _INSTRUCTION_RUNNER_H_
#define _INSTRUCTION_RUNNER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stream-reader.h"
#include "stream-encoder.h"
#include "websocket.h"

typedef enum {
  BiWrite         = 10,
  BiRead          = 11,
} InstructionCode;

class Runner {
  char uid;
  public:
    ws_info* socket;

    void run(unsigned char* byteStream, int length);
    void readPin(StreamEncoder* output, unsigned char pin);
    void writePin(unsigned char pin, long value);

  private:
    void sendOutput(StreamEncoder* output);
};

#ifdef __cplusplus
}
#endif

#endif
