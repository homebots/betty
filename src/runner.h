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
  BiPinMode       = 13,
  BiI2CSetup      = 19,
  BiI2CStart      = 20,
  BiI2CStop       = 21,
  BiI2CWrite      = 22,
  BiI2CRead       = 23,
  BiI2CSetAck     = 24,
  BiI2CGetAck     = 25,
  BiI2CList       = 26,
  BiReadRegister  = 30,
  BiWriteRegister = 31,
} InstructionCode;

class Runner {
  public:
    uint32_t delay;
    StreamDecoder* input;
    StreamEncoder* output;

    void next();
};

#ifdef __cplusplus
}
#endif

#endif
