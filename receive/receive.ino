#include <FlexCAN_T4.h>

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can0;

// ========================
// CONFIG
// ========================
#define ITERATION_TIME 50          // Run watchdog every 50ms
#define TIMEOUT        (1000 / ITERATION_TIME)
#define NODE_COUNT     2

#define WATCHDOG_PIN   23
#define IMD_PIN        19

bool ready = false;

// ========================
// NODE ENUM
// ========================
enum NODE {
  pedalBox     = 0x011F,       // 11-bit ID
  IMD          = 0x18FF01F4,   // 29-bit extended ID
  readyToDrive = 0x07FE
};

// Track what node timed out
NODE timed_out_node;

// Timers for each known node
volatile uint16_t pedalBoxTimer = 0;
volatile uint16_t IMD_Timer     = 0;

volatile uint16_t* timerArray[] = { &pedalBoxTimer, &IMD_Timer };
NODE nodeArray[]                = { pedalBox, IMD };

// ===============================
// RESET TIMER WHEN CAN RECEIVED
// ===============================
void resetTimer(const CAN_message_t &msg)
{
  Serial.print("CAN RX -> ID: 0x");
  Serial.print(msg.id, HEX);
  Serial.print(msg.flags.extended ? " (EXT)  " : " (STD)  ");
  
  Serial.print("DLC: ");
  Serial.print(msg.len);
  Serial.print("  Data: ");
  
  for (int i = 0; i < msg.len; i++) {
    Serial.print(msg.buf[i], HEX);
    Serial.print(" ");
  }
  
  Serial.print("  @ time ");
  Serial.println(millis());
  
  // Match incoming ID to known nodes
  for (int i = 0; i < NODE_COUNT; i++)
  {
    if (msg.id == nodeArray[i])
    {
      *timerArray[i] = 0;        // Reset timer
    }
  }
  
  ready = true;
}

// ===============================
// SETUP
// ===============================
void setup() {
  Serial.begin(115200);
  delay(400);
  
  pinMode(WATCHDOG_PIN, OUTPUT);
  pinMode(IMD_PIN, OUTPUT);
  
  digitalWrite(WATCHDOG_PIN, LOW);
  digitalWrite(IMD_PIN, LOW);
  
  // CAN startup
  Can0.begin();
  Can0.setBaudRate(500000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(resetTimer);
  
  Serial.println("Watchdog node online. Listening for CAN messages...");
}

// ===============================
// MAIN LOOP
// ===============================
void loop()
{
  Can0.events();
  digitalWrite(WATCHDOG_PIN, HIGH);
  digitalWrite(IMD_PIN, HIGH);
  
  long long start_time = millis();
  
  // Increment timers & report timeout
  for (uint8_t i = 0; i < NODE_COUNT; i++)
  {
    (*timerArray[i])++;
    
    if ((*timerArray[i]) > TIMEOUT) {
      timed_out_node = nodeArray[i];
      Serial.print("TIMEOUT -> Node ID: 0x");
      Serial.println(timed_out_node, HEX);
    }
  }
  
  // Maintain 50ms loop time
  while (millis() - start_time < ITERATION_TIME) {
    Can0.events();
  }
}
