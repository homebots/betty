#ifdef __cplusplus
extern "C" {
#endif

void loop(void) {}
void setup(void);

void sendOutput();
void next();
void onReceive(struct ws_info *wsInfo, int length, char *message, int opCode);
void connectWebSocket();
void setup();

#ifdef __cplusplus
}
#endif
