#ifndef _INSTRUCTION_RUNNER_
#define _INSTRUCTION_RUNNER_

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG

#include "serial-debug.h"
#include "sdk.h"
#include "runner.h"
#include "stream-decoder.h"
#include "stream-encoder.h"

void Runner::next() {
  delay = 0;

  switch (input->readByte()) {
    case BiWrite:
      this->writePin(
        input->readByte(),
        input->readBool()
      );
      break;

    case BiRead:
      this->readPin(input->readByte());
      break;

    case BiDelay:
      this->delayMs(input->readNumber());
      break;

    case BiPinMode:
      this->setPinMode(
        input->readByte(),
        input->readByte()
      );
      break;
  }
}

void ICACHE_FLASH_ATTR Runner::readPin(uint8_t pin) {
  bool state = pinRead(pin);
  LOG("READ %d %d\n", pin, state);

  output->writeByte(BiRead);
  output->writeByte(state);
}

void ICACHE_FLASH_ATTR Runner::delayMs(long time) {
  LOG("DELAY %d\n", time);
  delay = time;
}

void ICACHE_FLASH_ATTR Runner::writePin(uint8_t pin, long value) {
  LOG("WRITE %d %d\n", pin, value);
  pinWrite(pin, value > 0 ? HIGH : LOW);
}

void ICACHE_FLASH_ATTR Runner::setPinMode(uint8_t pin, uint8_t value) {
  LOG("PIN MODE %d %d\n", pin, value);
  pinMode(pin, value);
}

#ifdef __cplusplus
}
#endif

#endif
