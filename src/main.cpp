#include "ModeManager.h"
#include "KeypadManager.h"
#include "DisplayManager.h"
#include "RFIDManager.h"
#include "NetworkManager.h"
#include "EnrollmentHandler.h"

#define NETWORK_SSID "_ekefan_"
#define NETWORK_PASSWORD "88888877"
#define MQTT_SERVER_IP "192.168.149.187" // Your Windows IP
#define MQTT_SERVER_PORT 1883
ModeManager modeManager;
KeypadManager keypadManager;
DisplayManager displayManager;
RFIDManager rfid(5, 17);                                  // SDA, RST
NetworkManager network(MQTT_SERVER_IP, MQTT_SERVER_PORT); // Replace with your MQTT broker
EnrollmentHandler enrollment(rfid, network, displayManager);

void setup()
{
  Serial.begin(115200);
  displayManager.begin();
  displayManager.showMode(modeManager.getMode(), modeManager.isPrompting());
  rfid.begin();
  displayManager.showMessage("connecting to network");
  network.begin(NETWORK_SSID, NETWORK_PASSWORD);
}

void loop()
{
  keypadManager.update(modeManager);
  displayManager.showMode(modeManager.getMode(), modeManager.isPrompting());
  network.loop();

  if (modeManager.getMode() == SystemMode::ENROLLMENT)
  {
    enrollment.update();
  }
  delay(100);
}