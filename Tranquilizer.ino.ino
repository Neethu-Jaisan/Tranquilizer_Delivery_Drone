#include <Servo.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin definitions
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define SERVO_PIN 9
#define HC12_TX_PIN 10
#define HC12_RX_PIN 11

Servo servoMotor;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);      // For debugging
  Serial1.begin(9600);     // For HC-12 communication

  // Initialize OLED
  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
    Serial.println("OLED initialization failed!");
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Set pin modes
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  servoMotor.attach(SERVO_PIN);

  // Initial servo position
  servoMotor.write(0);

  // Display welcome message
  display.setCursor(0, 0);
  display.println("System Ready");
  display.display();
}

void loop() {
  // Read button states
  bool button1Pressed = !digitalRead(BUTTON1_PIN);
  bool button2Pressed = !digitalRead(BUTTON2_PIN);

  // Button 1 triggers servo action
  if (button1Pressed) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Triggering Servo");
    display.display();

    servoMotor.write(90);  // Move servo to 90 degrees
    delay(1000);           // Wait for 1 second
    servoMotor.write(0);   // Return to 0 degrees

    // Send data via HC-12
    Serial1.println("Action: Servo Triggered");

    delay(500);            // Debounce delay
  }

  // Button 2 sends a custom message via HC-12
  if (button2Pressed) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sending Data");
    display.display();

    Serial1.println("Custom Message");

    delay(500);            // Debounce delay
  }

  // Receive data from HC-12 and display it
  if (Serial1.available()) {
    String receivedData = Serial1.readString();

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Received:");
    display.println(receivedData);
    display.display();
  }
}
