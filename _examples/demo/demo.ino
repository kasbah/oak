#define DEBUG 1
// 0 = no debug
// 1 = debug
// 2 = debug + GPS info

#ifdef EINK_V2
#include "src/eink_v2/eink_v2.h"
#else
#include "src/eink/eink.h"
#endif

#include "src/debug/debug.h"
#include "src/gps/gps.h"
#include "src/led/led.h"
#include "src/lora/lora.h"

byte localAddress = LOCAL_ADDRESS;
byte destinationAddress = DESTINATION_ADDRESS;

int sendLoRaInterval = 3000;  // Send LoRa packet every 3 seconds
long lastLoRaSendTime = 0;

int displayInterval = 5000;  // display on E-Ink every 5 seconds
long lastDisplayTime = 0;

String dataFromDestinationAddress = "";

String gpsTime = "00:00";
String gpsDate = "2021-12-31";

String gpsLatLong = "waiting for fix";
// Example "1234.12345678N, 12345.12345678E"

String gpsLatLongForDisplay = "";
// Example "1 40N, 103 91E";

LatLong latlong = {0.00, 0.00};
LatLong prevLatlong = {0.00, 0.00};
LatLong peerLalong = {0.00, 0.00};

void setup() {
  #ifdef DEBUG
  SerialUSB.begin(9600);
  while (!SerialUSB) {}
  #endif

  DEBUG_PRINT_MORE("Starting Oak demo on node " + String(localAddress, HEX));

  DEBUG_PRINT_MORE("   localAddress: " + String(localAddress, HEX));
  DEBUG_PRINT_MORE("   destinationAddress: " + String(destinationAddress, HEX));

  if (!initLoRa()) {
    DEBUG_PRINT("Starting LoRa failed!");
  } else {
    DEBUG_PRINT("Starting LoRa succeeded!");
  }

  if (!initEink()) {
    DEBUG_PRINT("Starting Eink failed!");
  } else {
    DEBUG_PRINT("Starting Eink succeeded!");
  }

  displayOnEink(gpsLatLong, gpsTime, "0km");
  initGPS();
}

void loop() {
  if (millis() - lastLoRaSendTime > sendLoRaInterval) {
    if (hasNewGPSFix(&prevLatlong, &latlong)) {
      sendLoRa(gpsLatLong, localAddress, destinationAddress);

      DEBUG_PRINT_MORE("Send data "
        + gpsLatLong
        + " from 0x"
        + String(localAddress, HEX)
        + " to 0x"
        + String(destinationAddress, HEX));

      lastLoRaSendTime = millis();
      sendLoRaInterval = random(2000) + 1000;
    }
  }

  if (receiveLoRa(
    LoRa.parsePacket(), localAddress, dataFromDestinationAddress)) {
    DEBUG_PRINT_MORE("Received data "
      + dataFromDestinationAddress
      + " from 0x"
      + String(destinationAddress, HEX)
      + " to 0x"
      + String(localAddress, HEX));
  }

  if (receivedGPSfix()) {
    getGPStime(gpsTime);
    getGPSdate(gpsDate);
    getLatLong(&latlong);

    printGPSinfo();

    if (millis() - lastDisplayTime > displayInterval) {
      if (hasNewGPSFix(&prevLatlong, &latlong)) {
        // Convert peerLatlong to struct LatLong
        convertStringToLatLong(dataFromDestinationAddress, &peerLalong);

        // Calculate distance between peerLatlong and latlong
        float distance = getHaversineDistance(&latlong, &peerLalong);

        // Display on E-Ink
        convertLatLongForDisplay(&latlong, gpsLatLongForDisplay);
        displayOnEink(gpsLatLongForDisplay, gpsTime, String(distance, 0));
        prevLatlong = latlong;
      }

      lastDisplayTime = millis();
    }
  }
}

void printGPSinfo() {
  DEBUG_GPS("----------------------------------------");
  DEBUG_GPS("Time: " + gpsTime);
  DEBUG_GPS("Date: " + gpsDate);
  convertLatLongToString(&latlong, gpsLatLong);
  DEBUG_GPS("Lat/Long: " + gpsLatLong);

  DEBUG_GPS("GPS Fix? " + String(getGPSfix(), DEC));
  DEBUG_GPS("GPS Fix quality: " + String(getGPSfixquality(), DEC));
  DEBUG_GPS("Speed (knots): " + String(getGPSspeed()));
  DEBUG_GPS("Angle: " + String(getGPSangle()));
  DEBUG_GPS("Altitude: " + String(getGPSaltitude()));
  DEBUG_GPS("Satellites: " + String(getGPSsatellites()));

  DEBUG_GPS("Time [s] since last fix: "
    + String(getGPStimeSinceLastFix(), 3));
  DEBUG_GPS("    since last GPS time: "
    + String(getGPSlastTime(), 3));
  DEBUG_GPS("    since last GPS date: "
    + String(getGPSlastDate(), 3));
}
