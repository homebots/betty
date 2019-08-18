#ifndef _INSTRUCTION_RUNNER_H_
#define _INSTRUCTION_RUNNER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "c_types.h"
#include "stream-decoder.h"
#include "stream-encoder.h"
#include "websocket.h"

typedef enum {
  BiWrite         = 10,
  BiRead          = 11,
  BiDelay         = 12,
} InstructionCode;

class Runner {
  public:
    uint32_t delay;
    StreamDecoder* input;
    StreamEncoder* output;

    void next();
    void readPin(unsigned char pin);
    void writePin(unsigned char pin, long value);
    void delayMs(long value);
};

#ifdef __cplusplus
}
#endif

#endif
