#include <FlexCAN_T4.h>

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can0;

// ===============================
// PRINT EVERY RECEIVED CAN FRAME
// ===============================
void printFrame(const CAN_message_t &msg)
{
  Serial.print("CAN RX -> ID: 0x");
  Serial.print(msg.id, HEX);

  if (msg.flags.extended)
    Serial.print(" (EXT)  ");
  else
    Serial.print(" (STD)  ");

  Serial.print("DLC: ");
  Serial.print(msg.len);
  Serial.print("  Data: ");

  for (int i = 0; i < msg.len; i++) {
    Serial.print(msg.buf[i], HEX);
    Serial.print(" ");
  }

  Serial.print("  @ ");
  Serial.print(millis());
  Serial.println(" ms");
}

// ===============================
// SETUP
// ===============================
void setup() {
  Serial.begin(115200);
  delay(300);

  // Start CAN
  Can0.begin();
  Can0.setBaudRate(500000);
  Can0.setMaxMB(16);

  // FIFO + interrupt listener
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(printFrame);

  Serial.println("Simple CAN listener started...");
}

// ===============================
// MAIN LOOP
// ===============================
void loop() {
  // Process CAN events
  Can0.events();
}
