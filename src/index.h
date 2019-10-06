#ifdef __cplusplus
extern "C" {
#endif

void loop(void) {}
void setup(void);

#define MAX_DELAY     6871000
#define MAX_DELAY_US  MAX_DELAY * 1000

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

void sendOutput();
void next();
void onReceive(struct ws_info *wsInfo, int length, char *message, int opCode);
void connectWebSocket();
void setup();

#ifdef __cplusplus
}
#endif
