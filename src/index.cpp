#ifdef __cplusplus
extern "C" {
#endif

#include "index.h"
#include "wifi.h"
#include "ntp.h"
#include "homebots.h"
#include "websocket.h"
#include "instruction-runner.h"

static os_timer_t webSocketCheck;
static Wifi wifiConnection;
static ws_info webSocket;

void ICACHE_FLASH_ATTR onReceive(struct ws_info *wsInfo, int length, char *message, int opCode) {
  Runner instruction;
  instruction.socket = wsInfo;

  switch (opCode) {
    case WS_OPCODE_BINARY:
    case WS_OPCODE_TEXT:
    //   ws_send(wsInfo, WS_OPCODE_TEXT, "OK\0", 3);
    //   break;
      os_printf("RECV %d\n", length);
      // message += 8;
      instruction.run((unsigned char*)message);
      break;
  }
}

void loop() {}

void ICACHE_FLASH_ATTR connectWebSocket() {
  if (!wifiConnection.isConnected()) {
    wifiConnection.printStatus();
    return;
  }

  if (webSocket.connectionState != CS_CONNECTED) {
    ws_connect(&webSocket, "ws://hub.homebots.io/hub");
    return;
  }

  os_timer_arm(&webSocketCheck, 5000, 1);
}

void ICACHE_FLASH_ATTR setup() {
  os_timer_setfn(&webSocketCheck, (os_timer_func_t *)connectWebSocket, NULL);
  os_timer_arm(&webSocketCheck, 1000, 1);

  wifiConnection.connectTo("HomeBots", "HomeBots");
  webSocket.onReceive = onReceive;

  system_update_cpu_freq(SYS_CPU_160MHZ);
}

#ifdef __cplusplus
}
#endif
