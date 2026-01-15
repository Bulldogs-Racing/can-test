#include <FlexCAN_T4.h>

// CHANGE CAN1 / CAN2 IF NEEDED
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can0;

#define SEND_PERIOD_MS 10

void setup() {
  Serial.begin(115200);
  delay(300);

  randomSeed(analogRead(A0));  // entropy

  Can0.begin();
  // Can0.setPins(1, 0);
  Can0.setBaudRate(500000);
  Can0.setMaxMB(16);
  Can0.mailboxStatus();


  Serial.println("Random CAN sender started...");
}

void loop() {
  CAN_message_t msg;

  // ---------- CAN ID ----------
  msg.id = random(0x000, 0x7FF);   // standard 11-bit ID
  msg.flags.extended = 0;          // standard frame
  msg.flags.remote = 0;

  // ---------- DLC ----------
  msg.len = random(1, 9);          // 1–8 bytes

  // ---------- DATA ----------
  for (int i = 0; i < msg.len; i++) {
    msg.buf[i] = random(0, 256);
  }

  // ---------- SEND ----------
  bool ok = Can0.write(msg);

  // ---------- SERIAL DEBUG ----------
  Serial.print("CAN TX -> ID: 0x");
  Serial.print(msg.id, HEX);
  Serial.print(" DLC: ");
  Serial.print(msg.len);
  Serial.print(" Data: ");

  for (int i = 0; i < msg.len; i++) {
    Serial.print(msg.buf[i], HEX);
    Serial.print(" ");
  }

  Serial.print(" | ");
  Serial.println(ok ? "OK" : "FAILED");

  delay(SEND_PERIOD_MS);
}
