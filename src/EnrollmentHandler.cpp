#include "EnrollmentHandler.h"

EnrollmentHandler::EnrollmentHandler(RFIDManager &r, NetworkManager &n, DisplayManager &d)
    : rfid(r), network(n), display(d) {}

void EnrollmentHandler::update()
{
    if (Serial.available())
    {
        Serial.println("Serial is available");
        encryptedData = Serial.readStringUntil('\n');
        display.showMessage("Enrolling...");
        delay(5000);
        display.showMessage("Please place card");
    }

    if (!encryptedData.isEmpty() && rfid.isCardPresent())
    {
        String uid = rfid.readUID();
        String payload = "{\"uid\":\"" + uid + "\",\"data\":\"" + encryptedData + "\"}";
        network.publish("enrollment/topic", payload);
        display.showMessage("Success!");
        delay(500);
        display.showMessage(payload.c_str());
        encryptedData = "";
        delay(2000);
    }
}