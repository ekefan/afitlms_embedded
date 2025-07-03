#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <U8g2lib.h>

// ---------------- OLED SETUP (SH1106 I2C) ---------------- //
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// ---------------- RFID PINS (SPI) ---------------- //
#define SS_PIN   5
#define RST_PIN  17
MFRC522 rfid(SS_PIN, RST_PIN);

// ---------------- SPI Pin Custom Setup ---------------- //
// Optional but useful if SPI isn’t default on your board
#define SCK_PIN   18
#define MISO_PIN  19
#define MOSI_PIN  23

void setup() {
  Serial.begin(115200);

  // Start OLED
  oled.begin();
  oled.setFont(u8g2_font_ncenB08_tr);
  oled.clearBuffer();
  oled.drawStr(0, 24, "System Booted");
  oled.sendBuffer();

  // Start SPI for RFID
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  rfid.PCD_Init();

  delay(1000);
  oled.clearBuffer();
  oled.drawStr(0, 24, "Scan your card");
  oled.sendBuffer();
}

void loop() {
  // Check for new card
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Read UID
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += (rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  Serial.println("Card UID: " + uid);

  // Display UID on OLED
  oled.clearBuffer();
  oled.setFont(u8g2_font_ncenB08_tr);
  oled.drawStr(0, 12, "Card UID:");
  oled.drawStr(0, 30, uid.c_str());
  oled.drawStr(0, 50, "Access Granted");
  oled.sendBuffer();

  delay(2000);  // Show message for 2s

  // Cleanup
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  // Show ready prompt
  oled.clearBuffer();
  oled.drawStr(0, 24, "Scan your card");
  oled.sendBuffer();
}
