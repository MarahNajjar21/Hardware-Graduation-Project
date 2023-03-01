#include <Stepper.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <IRremote.hpp>
#include <Wire.h>
#include <SeeedOLED.h>
#include <avr/pgmspace.h>
#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>

//RFID with lock
#define SS_PIN 13
#define RST_PIN 12
#define ACCESS_DELAY 3000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

//keypad with lock
const int RELAY_PIN = 26;  // the Arduino pin, which connects to the IN pin of relay
const int ROW_NUM = 4;     // four rows
const int COLUMN_NUM = 4;  // four columns
char keys[ROW_NUM][COLUMN_NUM] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte pin_rows[ROW_NUM] = { 31, 32, 33, 34 };       //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = { 27, 28, 29, 30 };  //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);
const String password_1 = "123";  // change your password here
String input_password;

//RGB led
//const int redPin = 48;    // digital output pin that controls the red element of the LED
const int greenPin = 49;  // digital output pin that controls the green element of the LED
const int bluePin = 50;   // digital output pin that controls the blue element of the LED
int counter;

//for motor
const float stepsPerRevolution = 32;
//const float step_ratio=64;
const float step_ratio = 6;
const float step_rev = stepsPerRevolution * step_ratio;
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);     //top on right from the back 4
Stepper myStepper1(stepsPerRevolution, 4, 5, 6, 7);      //bottom right 1
Stepper myStepper2(stepsPerRevolution, 16, 17, 18, 19);  //top left 3
Stepper myStepper3(stepsPerRevolution, 36, 38, 40, 42);  //bottom left 2
int steper_requier;

// coin ACCEPTOR
const int coin = 2;
boolean insert = false;
volatile int pulse = 0;

//to read from esp serial
char data;  // Initialized variable to store received data
String inputData = "";

//Keypad
#define key1 22  //connect wire 1 to pin 22
#define key2 23  //connect wire 2 to pin 23
#define key3 24  //connect wire 3 to pin 24
#define key4 25  //connect wire 4 to pin 25

void setup() {
  counter = 4;
  // Start the serial ports at 115200 baud
  Serial3.begin(115200);
  Serial.begin(115200);

  //Keypad
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);

  //OLED
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display

  //RGB led
  //pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  //analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
  delay(1000);  // wait for 1 second

  //coin
  attachInterrupt(digitalPinToInterrupt(2), coinInterrupt, RISING);
  delay(1000);

  // keypad with lock
  input_password.reserve(32);    // maximum input characters is 33, change if needed
  pinMode(RELAY_PIN, OUTPUT);    // initialize pin as an output.
  digitalWrite(RELAY_PIN, LOW);  // lock the door

  //rfid with lock
  SPI.begin();         // Initiate  SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522
  Serial.println("Put your card to the reader...");
  Serial.println();
}

void setDisplayToOriginalState() {
  delay(100);
  SeeedOled.init();  //initialze SEEED OLED display
  //SeeedOled.clearDisplay();      // clear the screen and set start position to top left corner
  SeeedOled.deactivateScroll();  // deactivete Scroll (might be activated by previous test case)
  SeeedOled.setNormalDisplay();  // Non-inverted Display
  SeeedOled.setPageMode();       // Page mode to start with
  SeeedOled.setTextXY(3, 1);     // 0 Page, 0th Column
}

void loop() {
  /*analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
    delay(1000); */
  // wait for 1 second

  //coin acceptor and keypad
  if (insert == true && !digitalRead(22)) {
    insert = false;
    Serial.println("button 1 was clicked");
    Serial.println("DETECTED");

    //OLED
    SeeedOled.clearDisplay();
    SeeedOled.putString(" key 1 is pressed");

    delay(100);
    steper_requier = step_rev * -1;
    myStepper1.setSpeed(160);
    myStepper1.step(steper_requier);
    delay(50);
    myStepper1.step(0);
    counter--;
    delay(50);
    SeeedOled.clearDisplay();
    SeeedOled.putString(" BON APPÉTIT!");
    //inputData = "test";
  }

  if (insert == true && !digitalRead(23)) {
    insert = false;
    Serial.println("button 2 was clicked");
    Serial.println("DETECTED");
    //OLED
    SeeedOled.clearDisplay();
    SeeedOled.putString(" key 2 is pressed");

    delay(100);
    steper_requier = step_rev * 1;
    myStepper3.setSpeed(160);
    myStepper3.step(steper_requier);
    delay(50);
    myStepper3.step(0);

    delay(50);
    SeeedOled.clearDisplay();
    SeeedOled.putString(" BON APPÉTIT!");
    //inputData = "test";
  }

  if (insert == true && !digitalRead(24)) {
    insert = false;
    Serial.println("button 3 was clicked");
    Serial.println("DETECTED");
    //OLED
    SeeedOled.clearDisplay();
    SeeedOled.putString(" key 3 is pressed");

    delay(100);
    steper_requier = step_rev * -1;
    myStepper2.setSpeed(160);
    myStepper2.step(steper_requier);
    delay(50);
    myStepper2.step(0);

    delay(50);
    SeeedOled.clearDisplay();
    SeeedOled.putString(" BON APPÉTIT!");
    //inputData = "test";
  }

  if (insert == true && !digitalRead(25)) {
    insert = false;
    Serial.println("button 4 was clicked");
    Serial.println("DETECTED");
    //OLED
    SeeedOled.clearDisplay();
    SeeedOled.putString(" key 4 is pressed");

    delay(1000);
    steper_requier = step_rev * -1;
    myStepper.setSpeed(160);
    myStepper.step(steper_requier);
    delay(5000);
    myStepper.step(0);

    delay(50);
    SeeedOled.clearDisplay();
    SeeedOled.putString(" BON APPÉTIT!");
    //inputData = "test";
  }

  //esp
  if (Serial3.available() && insert) {
    // Read a single character from the Arduino Mega serial port
    data = Serial3.read();

    // Write the character to the ESP8266 serial port
    Serial.println(data);

    //OLED
    delay(200);
    setDisplayToOriginalState();
    //SeeedOled.clearDisplay();             //clear the screen and set start position to top left corner
    SeeedOled.setNormalDisplay();  //Set display to normal mode (i.e non-inverse mode)
    SeeedOled.setPageMode();       //Set addressing mode to Page Mode
    SeeedOled.setTextXY(2, 1);     //Set the cursor to Xth Page, Yth Column

    if (data == 'o') {
      Serial.println("button 1 was clicked");
      insert = false;
      //OLED
      SeeedOled.clearDisplay();
      SeeedOled.putString(" key 1 is pressed");

      delay(100);
      steper_requier = step_rev * -1;
      myStepper1.setSpeed(160);
      myStepper1.step(steper_requier);
      delay(50);
      myStepper1.step(0);

      delay(50);
      SeeedOled.clearDisplay();
      SeeedOled.putString(" BON APPÉTIT!");
      //inputData = "test";
    }
    if (data == 'w') {
      Serial.println("button 2 was clicked");
      insert = false;
      //OLED
      SeeedOled.clearDisplay();
      SeeedOled.putString(" key 2 is pressed");

      delay(100);
      steper_requier = step_rev * -1;
      myStepper3.setSpeed(160);
      myStepper3.step(steper_requier);
      delay(50);
      myStepper3.step(0);

      delay(50);
      SeeedOled.clearDisplay();
      SeeedOled.putString("BON APPÉTIT!");
    }
    if (data == 't') {
      Serial.println("button 3 was clicked");
      insert = false;
      //OLED
      SeeedOled.clearDisplay();
      SeeedOled.putString(" key 3 is pressed");

      delay(100);
      steper_requier = step_rev * -1;
      myStepper2.setSpeed(160);
      myStepper2.step(steper_requier);
      delay(50);
      myStepper2.step(0);

      delay(50);
      SeeedOled.clearDisplay();
      SeeedOled.putString(" BON APPÉTIT!");
    }
    if (data == 'f') {
      Serial.println("button 4 was clicked");
      insert = false;
      //OLED
      SeeedOled.clearDisplay();
      SeeedOled.putString(" key 4 is pressed");

      delay(100);
      steper_requier = step_rev * -1;
      myStepper.setSpeed(160);
      myStepper.step(steper_requier);
      delay(50);
      myStepper.step(0);
      delay(50);
      SeeedOled.clearDisplay();
      SeeedOled.putString(" BON APPÉTIT!");
    }
  }

  // admin
  //keypad with lock
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key == '*') {
      input_password = "";  // reset the input password
    } else if (key == '#') {
      if (input_password == password_1) {
        Serial.println("The password is correct, unlocking the door in 20 seconds");
        digitalWrite(RELAY_PIN, HIGH);  // unlock the door for 20 seconds
        delay(3000);
        digitalWrite(RELAY_PIN, LOW);  // lock the door
      } else {
        Serial.println("The password is incorrect, try again");
      }

      input_password = "";  // reset the input password
    } else {
      input_password += key;  // append new character to input password string
    }
  }

  //rfid with lock
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "62 07 5C 1C") {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(RELAY_PIN, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY_PIN, LOW);
    delay(100);
    //return;
  } else {
    Serial.println(" Access denied");
    delay(DENIED_DELAY);
  }

  // Add a delay to prevent the loop from running too quickly
  delay(500);
}
//interrupt
void coinInterrupt() {
  pulse++;
  insert = true;
}
