#ifdef __cplusplus
extern "C" {
#endif

void loop(void) {}
void setup(void);

#define MAX_DELAY     6871000
#define MAX_DELAY_US  MAX_DELAY * 1000

typedef enum {
  BiError           = 0x00,
  BiWrite           = 0x0a,
  BiRead            = 0x0b,
  BiDelay           = 0x0c,
  BiPinMode         = 0x0d,
  // BiIoSetup         = 0x0e,

  BiI2CSetup        = 0x13,
  BiI2CStart        = 0x14,
  BiI2CStop         = 0x15,
  BiI2CWrite        = 0x16,
  BiI2CRead         = 0x17,
  BiI2CSetAck       = 0x18,
  BiI2CGetAck       = 0x19,
  BiI2CList         = 0x1a,
  BiI2CFindDevice   = 0x1b,
  BiI2CWriteAndAck  = 0x1c,
  BiPinType         = 0x1d,

  BiReadRegister    = 0x1e,
  BiWriteRegister   = 0x1f,
} InstructionCode;

typedef enum {
  EInvalidCommand         = 1,
  EDeviceNotFound         = 2,
} ErrorCode;

void sendOutput();
void next();
void onReceive(struct ws_info *wsInfo, int length, char *message, int opCode);
void connectWebSocket();
void setup();

#ifdef __cplusplus
}
#endif
