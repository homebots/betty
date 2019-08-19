#ifdef __cplusplus
extern "C" {
#endif

// #define DEBUG

#include "index.h"
#include "homebots.h"
#include "runner.h"
#include "stream-decoder.h"
#include "stream-encoder.h"
#include "eagle_soc.h"

static os_timer_t webSocketCheck;
static os_timer_t delayTimer;
static Wifi wifiConnection;
static ws_info webSocket;
static StreamDecoder input;
static StreamEncoder output;
static Runner runner;

#define MAX_DELAY 6871000

void sendOutput() {
  int length = output.getLength();

  if (length > 1) {
    char* bytes = (char*)output.getStream();
    LOG("SEND %d\n", length);
    ws_send(&webSocket, WS_OPCODE_BINARY, (char*)bytes, length);
  }
}

void next() {
  if (input.isNotEmpty()) {
    LOG("NEXT\n");
    runner.next();

    if (runner.delay) {
      os_timer_arm(&delayTimer, runner.delay, 0);
    } else {
      next();
    }
  } else {
    sendOutput();
  }
}

void onReceive(struct ws_info *wsInfo, int length, char *message, int opCode) {
  switch (opCode) {
    case WS_OPCODE_BINARY:
    case WS_OPCODE_TEXT:
      input.setStream(message, length);
      uint8_t requestId = input.readByte();
      LOG("RECV [%d] %d bytes\n", requestId, length);
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
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_GPIO1);
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_GPIO3);
  LOG("UART swapped")
  #endif

  system_update_cpu_freq(SYS_CPU_160MHZ);

  runner.input = &input;
  runner.output = &output;
  wifiConnection.connectTo("HomeBots", "HomeBots");
  webSocket.onReceive = onReceive;

  os_timer_setfn(&webSocketCheck, (os_timer_func_t *)connectWebSocket, NULL);
  os_timer_setfn(&delayTimer, (os_timer_func_t *)next, NULL);
  os_timer_arm(&webSocketCheck, 1000, 1);
}

#ifdef __cplusplus
}
#endif
