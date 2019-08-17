#ifndef _INSTRUCTION_RUNNER_
#define _INSTRUCTION_RUNNER_

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk.h"
#include "instruction-runner.h"
#include "stream-reader.h"
#include "stream-encoder.h"
#include "string-extras.h"

void ICACHE_FLASH_ATTR Runner::run(unsigned char* byteStream, int length) {
  StreamReader reader(byteStream, length);
  StreamEncoder output;

  output.setResponseId(reader.readByte());

  while (reader.isNotEmpty()) {
    DEBUG("NEXT \n");
    switch (reader.readByte()) {
      case BiWrite:
        this->writePin(
          reader.readByte(),
          reader.readBool()
        );
        break;

      case BiRead:
        this->readPin(
          &output,
          reader.readByte()
        );
        break;

      case BiDelay:
        this->delayMs(
          reader.readLong()
        );
        break;
    }
  }

  sendOutput(&output);
}

void ICACHE_FLASH_ATTR Runner::readPin(StreamEncoder* output, unsigned char pin) {
  bool state = pinRead(pin);
  DEBUG("READ %d %d", pin, state);

  output->writeByte(BiRead);
  output->writeByte(state);
}

void ICACHE_FLASH_ATTR Runner::delayMs(long time) {
  DEBUG("DELAY %d", time);
  delay(time);
}

void ICACHE_FLASH_ATTR Runner::writePin(unsigned char pin, long value) {
  DEBUG("WRITE %d %d", pin, value);
  pinWrite(pin, value > 0 ? HIGH : LOW);
}

void ICACHE_FLASH_ATTR Runner::sendOutput(StreamEncoder* output) {
  char* bytes = (char*)output->getStream();
  int length = strlen((const char*)bytes);

  if (length == 1) return;

  DEBUG("SEND %d\n", length);

  ws_send(this->socket, WS_OPCODE_BINARY, (char*)bytes, length);
  os_free(bytes);
}

#ifdef __cplusplus
}
#endif

#endif
