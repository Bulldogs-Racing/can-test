#include <ACAN_T4.h>

#define SEND_PERIOD_MS 10

ACAN_T4 can;

void setup() {
  Serial.begin(115200);
  delay(300);

  randomSeed(analogRead(A0));  // entropy

  // Configure CAN settings
  ACAN_T4_Settings settings(500 * 1000); // 500 kbps
  settings.mDriverType = ACAN_T4_Settings::DriverType::MCP2551;
  settings.mBitRatePrescaler = 1;
  settings.mPhaseSegment1 = 14;
  settings.mPhaseSegment2 = 5;
  settings.mSJW = 4;
  settings.mTripleSampling = false;

  // Initialize CAN bus
  const uint32_t errorCode = can.begin(settings);
  if (errorCode == 0) {
    Serial.println("Random CAN sender started...");
  } else {
    Serial.print("CAN initialization error: 0x");
    Serial.println(errorCode, HEX);
  }
}

void loop() {
  CANMessage msg;

  // ---------- CAN ID ----------
  msg.id = random(0x000, 0x7FF);   // standard 11-bit ID
  msg.ext = false;                 // standard frame
  msg.rtr = false;                 // data frame

  // ---------- DLC ----------
  msg.len = random(1, 9);          // 1–8 bytes

  // ---------- DATA ----------
  for (int i = 0; i < msg.len; i++) {
    msg.data8[i] = random(0, 256);
  }

  // ---------- SEND ----------
  bool ok = can.tryToSend(msg);

  // ---------- SERIAL DEBUG ----------
  Serial.print("CAN TX -> ID: 0x");
  Serial.print(msg.id, HEX);
  Serial.print(" DLC: ");
  Serial.print(msg.len);
  Serial.print(" Data: ");

  for (int i = 0; i < msg.len; i++) {
    Serial.print(msg.data8[i], HEX);
    Serial.print(" ");
  }

  Serial.print(" | ");
  Serial.println(ok ? "OK" : "FAILED");

  delay(SEND_PERIOD_MS);
}
