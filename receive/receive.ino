#include <FlexCAN_T4.h>

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can0;

void printFrame(const CAN_message_t &msg) {
  Serial.print("RX: 0x");
  Serial.print(msg.id, HEX);
  Serial.print(" Len: ");
  Serial.print(msg.len);
  Serial.print(" Data: ");
  for (int i = 0; i < msg.len; i++) {
    Serial.print(msg.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(200);

  Can0.begin();
  Can0.setBaudRate(500000);
  
  // Standard pins for CAN2 on Teensy 4.0/4.1 are 0(RX) and 1(TX)
  // Can0.setPins(0, 1); 

  Can0.enableFIFO();
  
  // FIXED: Interrupts disabled to allow safe Serial printing
  // Can0.enableFIFOInterrupt(); 
  
  Can0.onReceive(printFrame);
  Serial.println("CAN Receiver Started");
}

void loop() {
  // polling the queue in the loop is safe for Serial
  Can0.events();
}