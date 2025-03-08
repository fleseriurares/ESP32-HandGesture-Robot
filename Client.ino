#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>

const int ENB = 27;     // Pinul conectat la ENB
const int mpin10 = 25;  // Pin pentru direcție
const int mpin11 = 26;  // Pin pentru direcție

const int ENA = 18;     // Pinul conectat la ENB
const int mpin00 = 19;  // Pin pentru direcție
const int mpin01 = 21;  // Pin pentru direcție

const char* SSID = "DMP Server";
const char* PASS = "123456789";
const int port = 80;

WiFiServer server(port);


void StartMotor(int m1, int m2, int forward, int speed, int pwm) {

  if (speed == 0)  // oprire
  {
    digitalWrite(m1, 0);
    digitalWrite(m2, 0);
  } else {
    if (forward) {
      digitalWrite(m2, 0);
      digitalWrite(m1, 1);
      analogWrite(pwm, speed);
    } else {
      digitalWrite(m1, 0);
      digitalWrite(m2, 1);
      analogWrite(pwm, speed);
    }
  }
}
// Functie de siguranta
// Executa oprire motoare, urmata de delay
void delayStopped(int ms) {
  StartMotor(mpin00, mpin01, 0, 0, 0);
  StartMotor(mpin10, mpin11, 0, 0, 0);
  delay(ms);
}

void machineStart(float tx, float ty, float tz) {
  if (ty > 30) {
    if (tx > 20) {
      StartMotor(mpin10, mpin11, 1, 256 - 5 * tx, ENB);
      StartMotor(mpin00, mpin01, 1, 256, ENA);
    } else if (tx < -20) {
      StartMotor(mpin10, mpin11, 1, 256, ENB);
      StartMotor(mpin00, mpin01, 1, 256 + 5 * tx, ENA);
    } else {
      StartMotor(mpin10, mpin11, 1, 256, ENB);
      StartMotor(mpin00, mpin01, 1, 256, ENA);
    }
  } else if (ty < -50) {
    if (tx > 20) {
      StartMotor(mpin10, mpin11, 0, 256 - 3 * tx, ENB);
      StartMotor(mpin00, mpin01, 0, 256, ENA);
    } else if (tx < -20) {
      StartMotor(mpin10, mpin11, 0, 256, ENB);
      StartMotor(mpin00, mpin01, 0, 256 + 3 * tx, ENA);
    } else {
      StartMotor(mpin10, mpin11, 0, 256, ENB);
      StartMotor(mpin00, mpin01, 0, 256, ENA);
    }
  } else {
    if (tx > 60) {
      StartMotor(mpin10, mpin11, 0, 30, ENB);
      StartMotor(mpin00, mpin01, 0, 160, ENA);
    } else if (tx < -60) {
      StartMotor(mpin10, mpin11, 0, 160, ENB);
      StartMotor(mpin00, mpin01, 0, 30, ENA);
    } else {
      StartMotor(mpin10, mpin11, 0, 0, ENB);
      StartMotor(mpin00, mpin01, 0, 0, ENA);
    }
  }
}


void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(mpin00, OUTPUT);
  pinMode(mpin01, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(mpin10, OUTPUT);
  pinMode(mpin11, OUTPUT);

  Serial.begin(9600);
  if (!WiFi.softAP(SSID, PASS)) {
    Serial.println("Error starting SoftAP");
    while (1)
      ;
  }

  IPAddress accessPointIP = WiFi.softAPIP();
  Serial.print("Server started at ");
  Serial.print(accessPointIP);
  Serial.print(" on port ");
  Serial.println(port);

  server.begin();
}

void loop() {
  WiFiClient client = server.available();


  if (client) {
    Serial.println("New client connected");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        currentLine += c;
        if (c == '\n') {
          currentLine.trim();
          // Split the string to extract tiltX, tiltY, tiltZ
          int firstComma = currentLine.indexOf(',');
          int secondComma = currentLine.indexOf(',', firstComma + 1);

          float tiltX = currentLine.substring(0, firstComma).toFloat();
          float tiltY = currentLine.substring(firstComma + 1, secondComma).toFloat();
          float tiltZ = currentLine.substring(secondComma + 1).toFloat();

          Serial.print("Received tiltX: ");
          Serial.println(tiltX);
          Serial.print("Received tiltY: ");
          Serial.println(tiltY);
          Serial.print("Received tiltZ: ");
          Serial.println(tiltZ);
          //StartMotor(mpin10,mpin11,0,200,ENB);


          machineStart(tiltX + 10, tiltY, tiltZ);


          client.print("tiltX: " + String(tiltX) + ", tiltY: " + String(tiltY) + ", tiltZ: " + String(tiltZ));
          currentLine = "";
          client.stop();
          Serial.println("Client disconnected\n");
        }
      }
    }
  }
}