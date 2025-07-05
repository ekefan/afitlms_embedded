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
        delay(2000);
        display.showMessage("Place Card");
        delay(2000);

        unsigned long lastMessageTime = millis();
        const unsigned long messageInterval = 2000;
        bool messageShown = false;

        while (!rfid.isCardPresent())
        {
            unsigned long currentTime = millis();

            if (!messageShown || (currentTime - lastMessageTime) >= messageInterval)
            {
                display.showMessage("Please place card");
                lastMessageTime = currentTime;
                messageShown = true;
            }

            delay(100);
        }

        String uid = rfid.readUID();
        display.showMessage("Success!");
        delay(500);
        display.showMessage(uid.c_str());

        Serial.printf("UID: %s", uid);

        delay(2000);

        encryptedData = "";
    }
}