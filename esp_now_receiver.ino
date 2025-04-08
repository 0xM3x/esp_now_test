#include <esp_now.h>
#include <WiFi.h>

static const char *TAG = "RECEIVER";

// Define the parameters for RSSI-to-distance conversion
float A = -40;  // Adjusted RSSI value at 1 meter (this value may need further tuning)
float n = 2.5;   // Path loss exponent (adjust depending on your environment)

// Define the distance range in meters (e.g., 30 cm = 0.3 meters)
float distance_range = 0.3;  // 30 cm

void on_receive(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
    // Get the RSSI value of the received message from ESP-NOW
    int rssi_value = recv_info->rssi;

    // Print out the RSSI value
    Serial.print("Received RSSI: ");
    Serial.println(rssi_value);

    // Calculate the distance using the RSSI-to-distance formula
    float distance = calculate_distance(rssi_value);
    
    // Print out the calculated distance
    Serial.print("Calculated distance: ");
    Serial.print(distance);
    Serial.println(" meters");

    // Check if the distance is within the specified range (30 cm)
    if (distance <= distance_range) {
        Serial.print("Detecting client with MAC address: ");
        print_mac_address(recv_info->mac_addr);  // Print the client's MAC address
        Serial.println("Client is within range!");
    } else {
        Serial.println("Client is out of range.");
    }

    // Print received data (if any)
    Serial.print("Received data: ");
    Serial.println((char *)data);

    // Send back an acknowledgment
    esp_now_send(recv_info->mac_addr, (uint8_t *)"ACK", 3);
}

// Function to calculate distance from RSSI
float calculate_distance(int rssi) {
    // If RSSI is too weak (e.g., -100 dBm or less), consider it as out of range
    if (rssi <= -100) {
        return -1;  // Indicates out of range
    }

    // Apply the RSSI-to-distance formula
    float distance = pow(10.0, (A - rssi) / (10.0 * n));

    // If the distance calculation is very small (i.e., negative or too small), return out of range
    if (distance < 0.01) {
        return -1;  // Consider this as out of range
    }

    return distance;
}

// Helper function to print MAC address
void print_mac_address(const uint8_t *mac) {
    for (int i = 0; i < 6; i++) {
        if (i > 0) {
            Serial.print(":");
        }
        Serial.print(mac[i], HEX);
    }
    Serial.println();
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
    print_mac_address(WiFi.macAddress().c_str());

    Serial.println("Receiver is ready and listening for clients...");
}

void loop() {
    // The receiver constantly waits for data, so nothing is needed here
    delay(1000);
}
