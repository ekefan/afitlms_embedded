#include <Arduino.h>
#include "ModeManager.h"
#include "KeypadManager.h"
#include "DisplayManager.h"
#include "RFIDManager.h"
#include "EnrollmentHandler.h"
#include "AttendanceHandler.h"
#include "DataStore.h"

#define NETWORK_SSID "_ekefan_"
#define NETWORK_PASSWORD "88888877"
#define MQTT_SERVER_IP "192.168.149.187" // Your Windows IP
#define MQTT_SERVER_PORT 1883
ModeManager modeManager;
DisplayManager displayManager;
DataStore dataStore;
RFIDManager rfid(5, 17);                                             // SDA, RST
NetworkManager network(MQTT_SERVER_IP, MQTT_SERVER_PORT, dataStore); // Replace with your MQTT broker
EnrollmentHandler enrollment(rfid, network, displayManager);
AttendanceHandler attendance(displayManager, rfid, network, dataStore, modeManager);
KeypadManager keypadManager(attendance);

void mqttCallBack(char *topic, uint8_t *payload, unsigned int length)
{
  String message;
  for (unsigned int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }

  if (String(topic) == "attendance/sheet/ekefan01")
  {
    // attendance.handleSheetPayload(message);
    Serial.println("Shitty payload.");
  }
}

void setup()
{
  Serial.begin(115200);
  displayManager.begin();
  displayManager.showMode(modeManager.getMode(), modeManager.isPrompting());
  rfid.begin();
  displayManager.setScreen(DisplayScreen::MESSAGE);
  displayManager.showMessageAtPos(0, 10, "connecting to network");
  displayManager.clear();
  network.begin(NETWORK_SSID, NETWORK_PASSWORD);
}

void loop()
{
  keypadManager.handleModeChange(modeManager);
  displayManager.showMode(modeManager.getMode(), modeManager.isPrompting());
  network.loop();
  attendance.loop();

  if ((modeManager.getMode() == SystemMode::ENROLLMENT) && !modeManager.isPrompting())
  {
    displayManager.setScreen(DisplayScreen::ENROLLMENT);
    enrollment.update();
  }
  if ((modeManager.getMode() == SystemMode::ATTENDANCE) && !modeManager.isPrompting() && !modeManager.isTakingAttendance())
  {
    displayManager.setScreen(DisplayScreen::ATTENDANCE_FILTERS);
    attendance.displayFilters();
  }
}