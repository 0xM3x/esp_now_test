#include <esp_now.h>
#include <WiFi.h>

// Define the broadcast MAC address
static const uint8_t broadcast_mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

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

    // Send initial message to broadcast address
    esp_now_add_peer(broadcast_mac, ESP_NOW_ROLE_CONTROLLER, 1, NULL, 0);
    
    Serial.println("Client is sending data...");
}

void loop() {
    const char *message = "Hello from client!";
    
    // Send data to the broadcast MAC address
    esp_now_send(broadcast_mac, (uint8_t *)message, strlen(message));

    // Wait before sending the next message
    delay(2000);
}
