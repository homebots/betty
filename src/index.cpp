#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG

#include "index.h"
#include "homebots.h"
#include "stream-decoder.h"
#include "stream-encoder.h"
#include "eagle_soc.h"

os_timer_t webSocketCheck;
os_timer_t delayTimer;
Wifi wifiConnection;
ws_info webSocket;
StreamDecoder input;
StreamEncoder output;

void sendOutput() {
  int length = output.getLength();

  if (length > 1) {
    char* bytes = (char*)output.getStream();
    LOG("SEND %d\n", length);
    ws_send(&webSocket, WS_OPCODE_BINARY, (char*)bytes, length);
  }
}

void error(uint8_t command, uint8_t errorCode) {
  input.end();
  output.writeByte(BiError);
  output.writeByte(command);
  output.writeByte(errorCode);
}

void onInterrupt(int gpioStatus) {
  os_timer_arm(&delayTimer, 0, 1);
}

void next() {
  bool halt = false;
  uint32_t delay = 0;
  uint32_t number;
  uint8_t byte;
  uint8_t* bytes;

  if (input.isEmpty()) {
    LOG("QUEUE EMPTY\n");
    sendOutput();
    return;
  }

  uint8_t command = input.readByte();
  LOG("NEXT %d\n", command);

  switch (command) {
    case BiLoop:
      input.rewind();
      break;

    case BiGoTo:
      input.goTo(input.readNumber());
      break;

    case BiWrite:
      pinWrite(input.readByte(), input.readBool());
      break;

    case BiRead:
      output.writeByte(BiRead);
      output.writeByte(pinRead(input.readByte()));
      break;

    case BiPinMode:
      pinMode(input.readByte(), (PinMode)input.readByte());
      break;

    case BiPinType:
      pinType(input.readByte(), input.readByte());
      break;

    case BiDelay:
      delay = input.readNumber();
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
      byte = input.readByte();
      i2c_writeByte(byte);
      break;

    case BiI2CWriteAndAck:
      number = input.readNumber();
      while (number--) {
        i2c_writeByteAndAck(input.readByte());
      }
      break;

    case BiI2CRead:
      byte = i2c_readByte();
      output.writeByte(BiI2CRead);
      output.writeByte(byte);
      break;

    case BiI2CSetAck:
      i2c_setAck(input.readByte());
      break;

    case BiI2CGetAck:
      output.writeByte(BiI2CGetAck);
      output.writeByte(i2c_getAck());
      break;

    case BiI2CFindDevice:
      byte = i2c_findDevice();

      if (!byte) {
        error(command, EDeviceNotFound);
        break;
      }

      output.writeByte(BiI2CFindDevice);
      output.writeByte(byte);
      break;

    case BiI2CList:
      bytes = (uint8_t*)os_zalloc(255);
      i2c_findDevices(bytes);
      output.writeByte(BiI2CList);
      output.writeBytes(bytes, 255);
      os_free(bytes);
      break;

    case BiReadRegister:
      output.writeNumber((*(volatile uint32_t*)(input.readNumber())));
      //output.writeNumber(READ_PERI_REG(input.readNumber()));
      break;

    case BiWriteRegister:
      WRITE_PERI_REG(input.readNumber(), input.readNumber());
      output.writeByte(BiWriteRegister);
      output.writeByte(1);
      break;

    case BiInterrupt:
      attachInterrupt(input.readByte(), (InterruptCallback*)&onInterrupt, GPIO_PIN_INTR_NEGEDGE);
      break;

    default:
      LOG("ERR invalid command %d\n", byte);
      error(command, EInvalidCommand);
  }

  if (!halt) {
    os_timer_arm(&delayTimer, 0, 1);
  }
}

#ifdef DEBUG
void printBytes(char* bytes, int length) {
  int i;
  while(i < length) {
    LOG("0x%.2x ", bytes[i++]);
  }

  LOG("\n");
}
#endif

void onReceive(struct ws_info *wsInfo, int length, char *message, int opCode) {
  switch (opCode) {
    case WS_OPCODE_BINARY:
    case WS_OPCODE_TEXT:
      input.setStream(message, length);
      uint8_t requestId = input.readByte();

      #ifdef DEBUG
      LOG("RECV [%d] %d bytes\n", requestId, length);
      printBytes(message, length);
      #endif

      output.reset();
      output.writeByte(requestId);
      next();
      break;
  }
}

void ICACHE_FLASH_ATTR connectWebSocket() {
  if (!wifiConnection.isConnected()) {
    wifiConnection.printStatus();
    return;
  }

  if (webSocket.connectionState != CS_CONNECTED && webSocket.connectionState != CS_CONNECTING) {
    ws_connect(&webSocket, "ws://hub.homebots.io/hub");
    return;
  }

  os_timer_arm(&webSocketCheck, 5000, 0);
}

void ICACHE_FLASH_ATTR setup() {
  #ifndef DEBUG
  system_uart_swap();
  #endif

  system_update_cpu_freq(SYS_CPU_160MHZ);

  wifiConnection.connectTo("HomeBots", "HomeBots");
  webSocket.onReceive = onReceive;

  os_timer_setfn(&webSocketCheck, (os_timer_func_t *)connectWebSocket, NULL);
  os_timer_setfn(&delayTimer, (os_timer_func_t *)next, NULL);

  os_timer_arm(&webSocketCheck, 1000, 1);
}

#ifdef __cplusplus
}
#endif
