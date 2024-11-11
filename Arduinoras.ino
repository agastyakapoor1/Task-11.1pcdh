#include <WiFiNINA.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
const char* ssid = "Ag";            // Replace with your WiFi network name
const char* password = "Football";     // Replace with your WiFi password
const char* serverIP = "192.168.200.10";     // Replace with Raspberry Pi's IP address
const int serverPort = 12345;

const int relayPin = 7;            // Relay pin connected to the Arduino
bool bulbState = false;            // Track the bulb state (on/off)
const int rightTiltThreshold = 5000;
const int leftTiltThreshold = -5000;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Connect to the server
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection to server failed");
  }
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Check for right tilt
  if (gy > rightTiltThreshold && !bulbState) {
    bulbState = true;
    digitalWrite(relayPin, LOW); // Turn bulb ON
    Serial.println("Right tilt detected - Bulb ON");
    sendToServer("Right tilt detected - Bulb ON");
    delay(500); // Debounce delay
  }
  // Check for left tilt
  else if (gy < leftTiltThreshold && bulbState) {
    bulbState = false;
    digitalWrite(relayPin, HIGH); // Turn bulb OFF
    Serial.println("Left tilt detected - Bulb OFF");
    sendToServer("Left tilt detected - Bulb OFF");
    delay(500); // Debounce delay
  }

  delay(100); // Adjust this delay as needed for responsiveness
}

// Function to send data to server
void sendToServer(String message) {
  if (client.connected()) {
    client.println(message);  // Send the tilt message to Raspberry Pi
  } else {
    Serial.println("Disconnected from server");
  }
}
