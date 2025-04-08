#include <esp_now.h>
#include <WiFi.h>

static const char *TAG = "RECEIVER";

// Define the parameters for RSSI-to-distance conversion
float A = -50;  // RSSI value at 1 meter (adjust based on your environment)
float n = 2.0;   // Path loss exponent (adjust depending on your environment)

// Define the distance range in meters (e.g., 30 cm = 0.3 meters)
float distance_range = 0.3;  // 30 cm

void on_receive(const uint8_t *mac_addr, const uint8_t *data, int len) {
    // Get the RSSI value of the received message
    int rssi_value = WiFi.RSSI();

    // Calculate the distance using the RSSI-to-distance formula
    float distance = calculate_distance(rssi_value);
    
    // Print out the calculated distance
    Serial.print("Calculated distance: ");
    Serial.print(distance);
    Serial.println(" meters");

    // Check if the distance is within the specified range (30 cm)
    if (distance <= distance_range) {
        Serial.print("Detecting client with MAC address: ");
        Serial.println(mac_addr);  // Print the client's MAC address
        Serial.println("Client is within range!");
    } else {
        Serial.println("Client is out of range.");
    }

    // Print received data (if any)
    Serial.print("Received data: ");
    Serial.println((char *)data);

    // Send back an acknowledgment
    esp_now_send(mac_addr, (uint8_t *)"ACK", 3);
}

// Function to calculate distance from RSSI
float calculate_distance(int rssi) {
    // Apply the RSSI-to-distance formula
    return pow(10.0, (A - rssi) / (10.0 * n));
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

    // Display the receiver's MAC address
    Serial.print("Receiver MAC Address: ");
    Serial.println(WiFi.macAddress());

    Serial.println("Receiver is ready and listening for clients...");
}

void loop() {
    // The receiver constantly waits for data, so nothing is needed here
    delay(1000);
}
