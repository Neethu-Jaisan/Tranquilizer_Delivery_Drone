#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define SCREEN_WIDTH 128  // OLED display width
#define SCREEN_HEIGHT 64  // OLED display height
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

SoftwareSerial HC12(16, 17);  // HC-12 TX to ESP32 RX, RX to ESP32 TX

// Motor driver pins (L298N)
#define MOTOR_IN1 5  // Motor forward
#define MOTOR_IN2 18  // Motor backward

void setup() {
  Serial.begin(115200);
  HC12.begin(9600);

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
    Serial.println(F("OLED initialization failed!"));
    while (1);
  }
  
  displayMessage("System Ready!");
}

void loop() {
  if (HC12.available()) {
    String command = HC12.readString();
    Serial.println("Received: " + command);

    if (command.indexOf("FIRE") >= 0) {
      displayMessage("Injecting...");
      activateMotor();
      displayMessage("Ready!");
    }
  }
}

void activateMotor() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  delay(4000);  // Motor runs for 4 seconds (Injection)
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
}

void displayMessage(String message) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 30);
  display.println(message);
  display.display();
}