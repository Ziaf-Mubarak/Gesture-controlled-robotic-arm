#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

#define FLEX_SENSOR_PIN 34   // ADC pin for flex sensor

// Data structure to send
typedef struct {
  int pitch;   // degrees
  int roll;    // degrees
  int flex;    // raw ADC value
} Message;

Message dataToSend;

// 🔁 Replace this with your receiver's MAC address
uint8_t receiverMACAddress[] = {0xEC, 0x62, 0x60, 0x1A, 0xEC, 0xAC};

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("ESP-NOW send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  Wire.begin();  // I2C start

  if (!mpu.begin()) {
    Serial.println("Could not find MPU6050 sensor!");
    while (1) {
      delay(1000);
    }
  }

  // Optional: basic MPU config
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    return;
  }

  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, receiverMACAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add ESP-NOW peer!");
    return;
  }

  Serial.println("Glove transmitter setup complete.");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Simple tilt-based pitch & roll from accelerometer
  float pitch_f = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;
  float roll_f  = atan2(a.acceleration.x, a.acceleration.z) * 180.0 / PI;

  // Constrain to a safe range
  pitch_f = constrain(pitch_f, -60.0, 60.0);
  roll_f  = constrain(roll_f,  -60.0, 60.0);

  // Small deadzone to avoid jitter
  if (pitch_f > -3.0 && pitch_f < 3.0) pitch_f = 0;
  if (roll_f  > -3.0 && roll_f  < 3.0) roll_f  = 0;

  int pitch = (int)pitch_f;
  int roll  = (int)roll_f;

  int flexValue = analogRead(FLEX_SENSOR_PIN);   // 0–4095 on ESP32

  dataToSend.pitch = pitch;
  dataToSend.roll  = roll;
  dataToSend.flex  = flexValue;

  // Send data via ESP-NOW
  esp_now_send(receiverMACAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));

  // Debug prints for calibration
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" | Roll: ");
  Serial.print(roll);
  Serial.print(" | Flex: ");
  Serial.println(flexValue);

  delay(40);  // ~25 Hz update rate
}
