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
      pinWrite(input->readByte(), input->readBool());
      break;

    case BiRead:
      output->writeByte(BiRead);
      output->writeByte(pinRead(input->readByte()));
      break;

    case BiDelay:
      delay = input->readNumber();
      break;

    case BiPinMode:
      pinMode(input->readByte(), input->readByte());
      break;

    case BiI2CSetup:
      i2c_gpio_init();
      break;

    case BiI2CStart:
      i2c_start();
      break;

    case BiI2CStop:
      i2c_stop();
      break;

    case BiI2CWrite:
      i2c_writeByte(input->readByte());
      break;

    case BiI2CRead:
      output->writeByte(BiI2CRead);
      output->writeByte(i2c_readByte());
      break;

    case BiI2CSetAck:
      i2c_setAck(input->readByte());
      break;

    case BiI2CGetAck:
      output->writeByte(BiI2CGetAck);
      output->writeByte(i2c_getAck());
      break;

    case BiI2CList:
      uint8_t devices[0xff];
      i2c_findDevices(devices);
      output->writeByte(BiI2CList);
      output->writeBytes(devices, 0xff);
      break;

    case BiReadRegister:
      output->writeNumber(READ_PERI_REG(input->readNumber()));
      break;

    case BiWriteRegister:
      WRITE_PERI_REG(input->readNumber(), input->readNumber());
      output->writeByte(1);
      break;

    default:
      input->end();
  }
}

#ifdef __cplusplus
}
#endif

#endif
