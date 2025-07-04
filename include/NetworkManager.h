#pragma once
#include <WiFi.h>
#include <PubSubClient.h>

class NetworkManager
{
public:
    NetworkManager(const char *server, int port);
    void begin(const char *ssid, const char *password);
    void loop();
    void publish(const char *topic, const String &payload);
    bool isConnected() const;

private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    const char *mqttServer;
    int mqttPort;
    bool connected;
};
