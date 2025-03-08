#include <WiFi.h>
#include <Wire.h>
#include <math.h>

#define MPU9250_ADDRESS 0x68

#define ACC_FULL_SCALE_4_G 0x08

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data) {
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  Wire.requestFrom(Address, Nbytes); 
  uint8_t index = 0;
  while (Wire.available()) {
    Data[index++] = Wire.read();
  }
}

void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data) {
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

const char* ssid = "DMP Server";
const char* password = "123456789";
const char* host = "192.168.4.1";
const int port = 80;

long int ti;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  Wire.begin(21, 22); // SDA=21, SCL=22
  Wire.beginTransmission(MPU9250_ADDRESS);
  if (Wire.endTransmission() == 0) {
    Serial.println("MPU9250 is connected.");
  } else {
    Serial.println("MPU9250 not found.");
  }

  I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_4_G);
  ti = millis();
}

void loop() {
  if (millis() - ti >= 100) {
    ti = millis();

    uint8_t Buf[14];
    I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);

    int16_t ax = -(Buf[0] << 8 | Buf[1]);
    int16_t ay = -(Buf[2] << 8 | Buf[3]);
    int16_t az = Buf[4] << 8 | Buf[5];

    float totalAccel = sqrt(ax * ax + ay * ay + az * az);
    float tiltX = asin(ax / totalAccel) * 180.0 / 3.14159265;
    float tiltY = asin(ay / totalAccel) * 180.0 / 3.14159265;
    float tiltZ = asin(az / totalAccel) * 180.0 / 3.14159265;

    WiFiClient client;
    if (!client.connect(host, port)) {
      Serial.println("Cannot connect to server");
      delay(2000);
      return;
    }

    String message = String(tiltX) + "," + String(tiltY) + "," + String(tiltZ);
    client.println(message);
    Serial.print("Sent: ");
    Serial.println(message);

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    String line = "";
    while (client.available()) {
      char c = client.read();
      line += c;
      if (c == '\n') {
        Serial.print("Server response: ");
        Serial.println(line);
      }
    }
  }
  delay(50);
}