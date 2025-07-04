#pragma once

#include <WiFi.h>
#include <PubSubClient.h>
#include "DataStore.h"

class NetworkManager
{
public:
    NetworkManager(const char *server, int port, DataStore &ds);
    void begin(const char *ssid, const char *password);
    void loop();
    void publish(const char *topic, const String &payload);
    bool isConnected() const;
    void subscribe(const char *topic);

private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    const char *mqttServer;
    int mqttPort;
    DataStore &data;
    bool connected;
};
