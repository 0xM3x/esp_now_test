#include <esp_now.h>
#include <WiFi.h>

static const char *TAG = "RECEIVER";

// Callback when data is received
void on_receive(const uint8_t *mac_addr, const uint8_t *data, int len) {
    char mac_str[18];
    snprintf(mac_str, sizeof(mac_str), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac_addr[0], mac_addr[1], mac_addr[2],
             mac_addr[3], mac_addr[4], mac_addr[5]);

    Serial.printf("Received data from %s: %.*s\n", mac_str, len, data);

    // Send back an acknowledgment
    esp_now_send(mac_addr, (uint8_t *)"ACK", 3);
}

void setup() {
    // Start Serial Communication
    Serial.begin(115200);
    
    // Initialize Wi-Fi in STA mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); // Disconnect to save power
    
    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register the receive callback
    esp_now_register_recv_cb(on_receive);

    Serial.println("Receiver ready, waiting for data...");
}

void loop() {
    // Nothing to do in loop, we wait for ESP-NOW data
}

