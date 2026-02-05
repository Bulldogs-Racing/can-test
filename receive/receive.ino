// Demo for Teensy 4.x CAN1, CAN2 and CAN3

// Just connect together all CAN pins:
//   - CRX1 (#23), CTX1 (#22), CRX2 (#0), CTX2 (#1), CRX3 (#30), CTX3 (#31)

//-----------------------------------------------------------------
// CAN1, CAN2 and CAN3 are configured in CAN2.0B.
//-----------------------------------------------------------------

#ifndef __IMXRT1062__
  #error "This sketch should be compiled for Teensy 4.x"
#endif

//-----------------------------------------------------------------

#include <ACAN_T4.h>

//-----------------------------------------------------------------

void setup () {
  pinMode (LED_BUILTIN, OUTPUT) ;
  Serial.begin (9600) ;
  while (!Serial) {
    delay (50) ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  Serial.println ("CAN1-CAN2-CAN3 test") ;
  ACAN_T4_Settings settings (1000 * 1000) ;
  settings.mTxPinIsOpenCollector = true ;
  settings.mRxPinConfiguration = ACAN_T4_Settings::PULLUP_22k ;
  Serial.print ("CAN Root Clock frequency: ") ;
  Serial.print (getCANRootClockFrequency ()) ;
  Serial.println (" Hz") ;
  Serial.print ("CAN Root Clock divisor: ") ;
  Serial.println (getCANRootClockDivisor ()) ;
  Serial.print ("Bitrate prescaler: ") ;
  Serial.println (settings.mBitRatePrescaler) ;
  Serial.print ("Propagation Segment: ") ;
  Serial.println (settings.mPropagationSegment) ;
  Serial.print ("Phase segment 1: ") ;
  Serial.println (settings.mPhaseSegment1) ;
  Serial.print ("Phase segment 2: ") ;
  Serial.println (settings.mPhaseSegment2) ;
  Serial.print ("RJW: ") ;
  Serial.println (settings.mRJW) ;
  Serial.print ("Triple Sampling: ") ;
  Serial.println (settings.mTripleSampling ? "yes" : "no") ;
  Serial.print ("Actual bitrate: ") ;
  Serial.print (settings.actualBitRate ()) ;
  Serial.println (" bit/s") ;
  Serial.print ("Exact bitrate ? ") ;
  Serial.println (settings.exactBitRate () ? "yes" : "no") ;
  Serial.print ("Distance from wished bitrate: ") ;
  Serial.print (settings.ppmFromWishedBitRate ()) ;
  Serial.println (" ppm") ;
  Serial.print ("Sample point: ") ;
  Serial.print (settings.samplePointFromBitStart ()) ;
  Serial.println ("%") ;
  uint32_t errorCode = ACAN_T4::can2.begin (settings) ;
  if (0 == errorCode) {
    Serial.println ("can2 ok") ;
  }else{
    Serial.print ("Error can2: 0x") ;
    Serial.println (errorCode, HEX) ;
  }
  delay (2) ;
}

//-----------------------------------------------------------------

static uint32_t gBlinkDate = 0 ;
// static uint32_t gSentCount1 = 0 ;
// static uint32_t gReceivedCount1 = 0 ;
// static uint32_t gSentCount2 = 0 ;
static uint32_t gReceivedCount2 = 0 ;
// static uint32_t gSentCount3 = 0 ;
// static uint32_t gReceivedCount3 = 0 ;

static const uint32_t SEND_COUNT = 50 * 1000 ;

//-----------------------------------------------------------------

void loop () {
  if (gBlinkDate <= millis ()) {
    gBlinkDate += 2000 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    
    Serial.print (", CAN2: ") ;
    // Serial.print (gSentCount2) ;
    Serial.print (" / ") ;
    Serial.print (gReceivedCount2) ;
    Serial.print (" / 0x") ;
    Serial.print (ACAN_T4::can2.globalStatus (), HEX) ;
    Serial.print (" / ") ;
    Serial.print (ACAN_T4::can2.receiveBufferPeakCount ()) ;
    Serial.print (" \n ");
  }
  CANMessage message ;

//--- CAN2 receive
  if (ACAN_T4::can2.receive (message)) {
    gReceivedCount2 += 1 ;
    // Serial.print(message);
  }
}