#ifndef _INSTRUCTION_RUNNER_
#define _INSTRUCTION_RUNNER_

#ifdef __cplusplus
extern "C" {
#endif

#include "instruction-runner.h"
#include "stream-reader.h"
#include "stream-encoder.h"
#include "pins.h"
#include "mem.h"

void ICACHE_FLASH_ATTR Runner::run(unsigned char* byteStream) {
  StreamReader reader(byteStream);
  uid = reader.readByte();

  switch (reader.readByte()) {
    case BiWrite:
      this->writePin(
        reader.readByte(),
        false,
        reader.readBool()
      );

    // case BiAnalogWrite:
    //   this->writePin(
    //     reader.readByte(),
    //     true,
    //     reader.readLong()
    //   );
    //   break;

    case BiRead:
      this->readPin(
        reader.readByte()
      );
      break;
  }
}

void ICACHE_FLASH_ATTR Runner::readPin(unsigned char pin) {
  StreamEncoder output(uid, 4);

  output.writeByte(BiRead);
  output.writeByte(pinRead(pin));

  sendOutput(&output);
}

void ICACHE_FLASH_ATTR Runner::writePin(unsigned char pin, bool isAnalog, long value) {
  pinWrite(pin, value > 0 ? HIGH : LOW);
}

void ICACHE_FLASH_ATTR Runner::sendOutput(StreamEncoder* output) {
  char* bytes = (char*)output->getStream();
  int length = strlen((const char*)bytes);
  os_printf("SEND %d %s\n", length, bytes);

  if (sizeof(bytes) > 0) {
    ws_send(this->socket, WS_OPCODE_BINARY, bytes, length);
    os_free(bytes);
  }
}

#ifdef __cplusplus
}
#endif

#endif
