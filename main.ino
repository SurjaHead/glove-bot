// #include <SPI.h>
// #include <RF24.h>
// #include <Servo.h>

// // Pin Definitions
// #define CE_PIN     10    // Chip Enable pin for nRF24L01+
// #define CSN_PIN    9     // Chip Select Not pin for nRF24L01+
// #define SERVO_PIN1 3     // Servo 1 pin
// #define SERVO_PIN2 5     // Servo 2 pin
// #define SERVO_PIN3 6     // Servo 3 pin
// #define SERVO_PIN4 4    // Servo 4 pi4n
// #define SERVO_PIN5 7     // Servo 5 pin

// // Radio & Servo Objects
// RF24 radio(CE_PIN, CSN_PIN);
// Servo servo1;
// Servo servo2;
// Servo servo3;
// Servo servo4;
// Servo servo5;

// // Pipe Address (must match transmitter)
// const uint64_t radioAddress = 0xE8E8F0F0E1LL;

// // Received Data Array
// long receivedData[5];

// void setup() {
//   // Initialize Serial for debugging
//   Serial.begin(115200);

//   // Attach Servos to their respective pins
//   servo1.attach(SERVO_PIN1);
//   servo2.attach(SERVO_PIN2);
//   servo3.attach(SERVO_PIN3);
//   servo4.attach(SERVO_PIN4);
//   servo5.attach(SERVO_PIN5);

//   // Set initial position of all servos to 0 degrees
//   servo1.write(0);
//   servo2.write(0);
//   servo3.write(0);
//   servo4.write(0);
//   servo5.write(0);

//   // Optional: Add a small delay to ensure servos reach the position
//   delay(500);  // 500ms delay

//   // Initialize Radio
//   if (!radio.begin()) {
//     Serial.println("Radio hardware not responding!");
//     while (1); // Halt if radio fails
//   }

//   // Configure radio settings
//   radio.setPALevel(RF24_PA_LOW);       // Power amplifier level
//   radio.setDataRate(RF24_250KBPS);     // Data rate
//   radio.openReadingPipe(1, radioAddress); // Open pipe for receiving
//   radio.startListening();              // Set as receiver

//   Serial.println("Receiver started");
// }

// void loop() {
//   if (radio.available()) {
//     // Read incoming data
//     radio.read(receivedData, sizeof(receivedData));

//     // Map received values (0-4095) to servo angles (0-180)
//     int angle1 = map(receivedData[0], 0, 4095, 0, 180);
//     int angle2 = map(receivedData[1], 0, 4095, 0, 180);
//     int angle3 = map(receivedData[2], 0, 4095, 0, 180);
//     int angle4 = map(receivedData[3], 0, 4095, 0, 180);
//     int angle5 = map(receivedData[4], 0, 4095, 0, 180);

//     // Write angles to servos
//     servo1.write(angle1);
//     servo2.write(angle2);
//     servo3.write(angle3);
//     servo4.write(angle4);
//     servo5.write(angle5);

//     // Debug output
//     Serial.print("Received: ");
//     Serial.print(receivedData[0]);
//     Serial.print(" ");
//     Serial.print(receivedData[1]);
//     Serial.print(" ");
//     Serial.print(receivedData[2]);
//     Serial.print(" ");
//     Serial.print(receivedData[3]);
//     Serial.print(" ");
//     Serial.print(receivedData[4]);
//     Serial.print(" | Angles: ");
//     Serial.print(angle1);
//     Serial.print(", ");
//     Serial.print(angle2);
//     Serial.print(", ");
//     Serial.print(angle3);
//     Serial.print(", ");
//     Serial.print(angle4);
//     Serial.print(", ");
//     Serial.println(angle5);
//   }

//   delay(10); // Small delay for stability
// }










#include <SPI.h>
#include <RF24.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin Definitions
#define CE_PIN     10    // Chip Enable pin for nRF24L01+
#define CSN_PIN    9     // Chip Select Not pin for nRF24L01+
#define SERVO_PIN1 3     // Servo 1 pin (Thumb)
#define SERVO_PIN2 5     // Servo 2 pin (Index)
#define SERVO_PIN3 6     // Servo 3 pin (Middle)
#define SERVO_PIN4 4     // Servo 4 pin (Ring)
#define SERVO_PIN5 7     // Servo 5 pin (Pinky)

// OLED Definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1    // Reset pin not used
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Radio & Servo Objects
RF24 radio(CE_PIN, CSN_PIN);
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// Pipe Address (must match transmitter)
const uint64_t radioAddress = 0xE8E8F0F0E1LL;

// Received Data Array
long receivedData[5];

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  while (!Serial);  // Wait for Serial to initialize

  // Attach Servos to their respective pins
  servo1.attach(SERVO_PIN1);
  servo2.attach(SERVO_PIN2);
  servo3.attach(SERVO_PIN3);
  servo4.attach(SERVO_PIN4);
  servo5.attach(SERVO_PIN5);

  // Set initial position of all servos to 0 degrees
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);
  servo5.write(0);
  delay(500);  // Allow time for servos to reach position

  // Initialize Radio
  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
    while (1); // Halt if radio fails
  }
  radio.setPALevel(RF24_PA_LOW);       // Power amplifier level
  radio.setDataRate(RF24_250KBPS);     // Data rate
  radio.openReadingPipe(1, radioAddress); // Open pipe for receiving
  radio.startListening();              // Set as receiver

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C is a common I2C address
    Serial.println("SSD1306 allocation failed");
    while (1); // Halt if display fails
  }
  display.clearDisplay();
  display.setTextSize(1);         // Text size 1 (small)
  display.setTextColor(SSD1306_WHITE); // White text on black background
  display.setCursor(0, 0);        // Start at top-left
  display.println("Waiting for data...");
  display.display();              // Show initial message

  Serial.println("Receiver started");
}

void loop() {
  if (radio.available()) {
    // Read incoming data
    radio.read(receivedData, sizeof(receivedData));

    // Map received values (0-4095) to servo angles (0-180)
    int angle1 = map(receivedData[0], 0, 4095, 0, 180);
    int angle2 = map(receivedData[1], 0, 4095, 0, 180);
    int angle3 = map(receivedData[2], 0, 4095, 0, 180);
    int angle4 = map(receivedData[3], 0, 4095, 0, 180);
    int angle5 = map(receivedData[4], 0, 4095, 0, 180);

    // Write angles to servos
    servo1.write(angle1);
    servo2.write(angle2);
    servo3.write(angle3);
    servo4.write(angle4);
    servo5.write(angle5);

    // Determine finger states using const char*
    const char* state1 = (angle1 > 90) ? "Closed" : "Open";
    const char* state2 = (angle2 > 90) ? "Closed" : "Open";
    const char* state3 = (angle3 > 90) ? "Closed" : "Open";
    const char* state4 = (angle4 > 90) ? "Closed" : "Open";
    const char* state5 = (angle5 > 90) ? "Closed" : "Open";

    // Debug output
    Serial.print("Raw Received Data: ");
    for (int i = 0; i < 5; i++) {
      Serial.print(receivedData[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    Serial.print("Angles: ");
    Serial.print(angle1); Serial.print(", ");
    Serial.print(angle2); Serial.print(", ");
    Serial.print(angle3); Serial.print(", ");
    Serial.print(angle4); Serial.print(", ");
    Serial.print(angle5); Serial.println();

    Serial.print("States: ");
    Serial.print(state1); Serial.print(", ");
    Serial.print(state2); Serial.print(", ");
    Serial.print(state3); Serial.print(", ");
    Serial.print(state4); Serial.print(", ");
    Serial.print(state5); Serial.println();

    // Update OLED display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Pinky:  "); display.println(state1);
    display.print("Ring:  "); display.println(state2);
    display.print("Middle: "); display.println(state3);
    display.print("Index:   "); display.println(state4);
    display.print("Thumb:  "); display.println(state5);
    display.display();
  }

  delay(10); // Small delay for stability
}







// #include <SPI.h>
// #include <RF24.h>
// #include <Servo.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// // Pin Definitions
// #define CE_PIN     10    // Chip Enable pin for nRF24L01+
// #define CSN_PIN    9     // Chip Select Not pin for nRF24L01+
// #define SERVO_PIN1 3     // Servo 1 pin (Thumb)
// #define SERVO_PIN2 5     // Servo 2 pin (Index)
// #define SERVO_PIN3 6     // Servo 3 pin (Middle)
// #define SERVO_PIN4 4     // Servo 4 pin (Ring)
// #define SERVO_PIN5 7     // Servo 5 pin (Pinky)
// #define BUTTON_PIN 2     // Button to stop system

// // OLED Definitions
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #define OLED_RESET    -1    // Reset pin not used
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// // Radio & Servo Objects
// RF24 radio(CE_PIN, CSN_PIN);
// Servo servo1;
// Servo servo2;
// Servo servo3;
// Servo servo4;
// Servo servo5;

// // Pipe Address (must match transmitter)
// const uint64_t radioAddress = 0xE8E8F0F0E1LL;

// // Received Data Array
// long receivedData[5];

// void setup() {
//   // Initialize Serial for debugging
//   Serial.begin(115200);
//   while (!Serial);  // Wait for Serial to initialize

//   // Initialize the button pin
//   pinMode(BUTTON_PIN, INPUT_PULLUP); // Using internal pull-up resistor

//   // Attach Servos to their respective pins
//   servo1.attach(SERVO_PIN1);
//   servo2.attach(SERVO_PIN2);
//   servo3.attach(SERVO_PIN3);
//   servo4.attach(SERVO_PIN4);
//   servo5.attach(SERVO_PIN5);

//   // Set initial position of all servos to 0 degrees
//   servo1.write(0);
//   servo2.write(0);
//   servo3.write(0);
//   servo4.write(0);
//   servo5.write(0);
//   delay(500);  // Allow time for servos to reach position

//   // Initialize Radio
//   if (!radio.begin()) {
//     Serial.println("Radio hardware not responding!");
//     while (1); // Halt if radio fails
//   }
//   radio.setPALevel(RF24_PA_LOW);       // Power amplifier level
//   radio.setDataRate(RF24_250KBPS);       // Data rate
//   radio.openReadingPipe(1, radioAddress); // Open pipe for receiving
//   radio.startListening();              // Set as receiver

//   // Initialize OLED display
//   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C is a common I2C address
//     Serial.println("SSD1306 allocation failed");
//     while (1); // Halt if display fails
//   }
//   display.clearDisplay();
//   display.setTextSize(1);         // Text size 1 (small)
//   display.setTextColor(SSD1306_WHITE); // White text on black background
//   display.setCursor(0, 0);        // Start at top-left
//   display.println("Waiting for data...");
//   display.display();              // Show initial message

//   Serial.println("Receiver started");
// }

// void loop() {
//   // Check if the stop button is pressed
//   if (digitalRead(BUTTON_PIN) == LOW) { // Assuming active LOW when pressed
//     // Detach all servos so they stop receiving control signals
//     servo1.detach();
//     servo2.detach();
//     servo3.detach();
//     servo4.detach();
//     servo5.detach();
    
//     // Stop the radio listener if desired (this prevents receiving further data)
//     radio.stopListening();

//     // Optionally, display a stopped message on the OLED
//     display.clearDisplay();
//     display.setCursor(0, 0);
//     display.println("System Stopped!");
//     display.display();

//     // Print to Serial for debugging
//     Serial.println("Stop button pressed. System halted.");
    
//     // Enter an infinite loop to effectively halt further execution.
//     while (true) {
//       delay(100); // Keep the system in a halted state
//     }
//   }

//   // Only process radio data if the system is not stopped
//   if (radio.available()) {
//     // Read incoming data
//     radio.read(receivedData, sizeof(receivedData));

//     // Map received values (0-4095) to servo angles (0-180)
//     int angle1 = map(receivedData[0], 0, 4095, 0, 180);
//     int angle2 = map(receivedData[1], 0, 4095, 0, 180);
//     int angle3 = map(receivedData[2], 0, 4095, 0, 180);
//     int angle4 = map(receivedData[3], 0, 4095, 0, 180);
//     int angle5 = map(receivedData[4], 0, 4095, 0, 180);

//     // Write angles to servos
//     servo1.write(angle1);
//     servo2.write(angle2);
//     servo3.write(angle3);
//     servo4.write(angle4);
//     servo5.write(angle5);

//     // Determine finger states using const char*
//     const char* state1 = (angle1 > 90) ? "Closed" : "Open";
//     const char* state2 = (angle2 > 90) ? "Closed" : "Open";
//     const char* state3 = (angle3 > 90) ? "Closed" : "Open";
//     const char* state4 = (angle4 > 90) ? "Closed" : "Open";
//     const char* state5 = (angle5 > 90) ? "Closed" : "Open";

//     // Debug output
//     Serial.print("Raw Received Data: ");
//     for (int i = 0; i < 5; i++) {
//       Serial.print(receivedData[i], HEX);
//       Serial.print(" ");
//     }
//     Serial.println();

//     Serial.print("Angles: ");
//     Serial.print(angle1); Serial.print(", ");
//     Serial.print(angle2); Serial.print(", ");
//     Serial.print(angle3); Serial.print(", ");
//     Serial.print(angle4); Serial.print(", ");
//     Serial.print(angle5); Serial.println();

//     Serial.print("States: ");
//     Serial.print(state1); Serial.print(", ");
//     Serial.print(state2); Serial.print(", ");
//     Serial.print(state3); Serial.print(", ");
//     Serial.print(state4); Serial.print(", ");
//     Serial.print(state5); Serial.println();

//     // Update OLED display
//     display.clearDisplay();
//     display.setCursor(0, 0);
//     display.print("Pinky:  "); display.println(state1);
//     display.print("Ring:  "); display.println(state2);
//     display.print("Middle: "); display.println(state3);
//     display.print("Index:   "); display.println(state4);
//     display.print("Thumb:  "); display.println(state5);
//     display.display();
//   }

//   delay(10); // Small delay for stability
// }
