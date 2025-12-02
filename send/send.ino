#include <FlexCAN_T4.h>

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can0;

#define ITERATION_TIME 10 // send msg every 10 ms

// define pins
#define POT_PIN_ONE 19
#define POT_PIN_TWO 23

#define SET_MSG_ID 0x011F
#define SET_MSG_LEN 2

uint8_t curr_val[SET_MSG_LEN];

void setup(void) {
  Serial.begin(115200); delay(400);
  Can0.begin();
  Can0.setBaudRate(500000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.mailboxStatus();
}

void loop() {
  long long start_time = millis();
  
  CAN_message_t msg;
  msg.id = SET_MSG_ID;
  msg.len = SET_MSG_LEN;
  
  // mapResistanceToVal(); 
  
  for (uint8_t i = 0; i < msg.len; i++ ) {
    Serial.println(curr_val[i]);
    msg.buf[i] = curr_val[i];
  }
  Can0.write(msg);
  
  long long end_time = millis();
  if ((end_time - start_time) < ITERATION_TIME) 
  delay(ITERATION_TIME - (end_time - start_time));  // stall until iteratin time is hit
}
