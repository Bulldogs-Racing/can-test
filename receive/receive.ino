#include <ACAN_T4.h>

ACAN_T4 can;

void printFrame(const CANMessage &msg) {
  Serial.print("RX: 0x");
  Serial.print(msg.id, HEX);
  Serial.print(" Len: ");
  Serial.print(msg.len);
  Serial.print(" Data: ");
  for (int i = 0; i < msg.len; i++) {
    Serial.print(msg.data8[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(200);

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
    Serial.println("CAN Receiver Started");
  } else {
    Serial.print("CAN initialization error: 0x");
    Serial.println(errorCode, HEX);
  }
}

void loop() {
  CANMessage msg;
  if (can.receive(msg)) {
    printFrame(msg);
  }
}
