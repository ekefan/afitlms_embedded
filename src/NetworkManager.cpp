#include "NetworkManager.h"

NetworkManager::NetworkManager(const char *server, int port)
    : mqttClient(wifiClient), mqttServer(server), mqttPort(port), connected(false) {}

void NetworkManager::begin(const char *ssid, const char *password)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.printf("Connecting to Wi-Fi (%s)...\n", ssid);

    unsigned long startTime = millis();
    const unsigned long wifiTimeout = 10000; // 10 seconds

    while (WiFi.status() != WL_CONNECTED && millis() - startTime < wifiTimeout)
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("\n❌ Wi-Fi connection failed.");
        connected = false;
        return;
    }

    Serial.println("\n✅ Wi-Fi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    mqttClient.setServer(mqttServer, mqttPort);

    // Create unique MQTT client ID using MAC
    String clientId = "ESP32Client-" + String((uint32_t)ESP.getEfuseMac(), HEX);

    Serial.print("Connecting to MQTT broker... ");
    if (mqttClient.connect(clientId.c_str()))
    {
        Serial.println("✅ connected.");
        connected = true;
    }
    else
    {
        Serial.println("❌ failed to connect to MQTT.");
        connected = false;
    }
}

void NetworkManager::loop()
{
    if (connected)
        mqttClient.loop();
}

void NetworkManager::publish(const char *topic, const String &payload)
{
    if (connected)
    {
        mqttClient.publish(topic, payload.c_str());
    }
    else
    {
        Serial.println("⚠️ MQTT not connected. Skipping publish.");
    }
}

bool NetworkManager::isConnected() const
{
    return connected;
}
