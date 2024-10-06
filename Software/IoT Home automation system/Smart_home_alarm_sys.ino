#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <MFRC522.h>
#include <Servo.h>
#include <DHT.h>

// Pin Definitions
#define Pot A1
#define trigPin 11
#define echoPin 10
#define buzzer 12
#define LEDR 22
#define LEDG 23
#define ServoPin 49
#define SSPin 53
#define RSTPin 48
#define DHTPIN 24
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define BUTTON 47
#define RoofS A8
#define PIR 25
#define LedPIR 28 
#define Touch 46
#define red 30
#define green 26
#define MQOut A0
#define Led1 27
#define Led2 31
#define Led3 29
// Password and Screen Definitions
#define Password_Length 5
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Global Objects and Variables
byte authorizedUID[4] = {0x33, 0xD5, 0x21, 0x25};
byte authorizedUID2[4] = {0x03, 0x36, 0x13, 0x11};
MFRC522 rfid(SSPin, RSTPin);
Servo servo;
Servo Roof;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float new_delay;
boolean stop = true;
int PotValue;
int angle = 90;
int Tval;
int data;
const byte ROWS = 4;
const byte COLS = 4;
char key;
char Password[Password_Length];
char Master[Password_Length] = "1302";
byte data_count = 0;


char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
    dht.begin();
    Serial.begin(9600);
    delay(100);
    SPI.begin(); // init SPI bus
    rfid.PCD_Init(); // init MFRC522

    servo.attach(ServoPin);
    Roof.attach(RoofS);
    servo.write(angle);

    // Setting Pin Modes
    Roof.write(90);
    delay(500);
    pinMode(BUTTON,INPUT);
    pinMode(MQOut, INPUT);
    pinMode(PIR,INPUT);
    pinMode(Touch, INPUT);
    pinMode(echoPin, INPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(red, OUTPUT);
    pinMode(Led1, OUTPUT);
    pinMode(Led2, OUTPUT);
    pinMode(Led3, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(LEDR, OUTPUT);
    pinMode(LedPIR, OUTPUT);
    pinMode(trigPin, OUTPUT);
    // Initializing OLED Display
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
    }

    display.display();
    delay(2000);
    display.clearDisplay();
    display.display();
    delay(2000);
    Serial.println("Tap RFID Tag on reader");
}

void loop() {
    TempDisplay();
    RFID();
    BTHControl();
    Motion();
    key = keypad.getKey();
    Tval = digitalRead(Touch);
    PasswordFunctions();
    Clear();
    TouchS();
    Smoke();
    LedLamp();
}

void TouchS(){
  if(Tval == HIGH){
    stop = false;
    angle = 0;
    servo.write(angle);
      digitalWrite(LEDG, HIGH);
      delay(500);
      digitalWrite(LEDG, LOW);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(20, 10);
      display.print("Correct!");
      display.display(); 
  }
}

void PasswordFunctions(){

      if (key) {
        Password[data_count] = key;
        Password[data_count + 1] = '\0';

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(5, 10);
        display.print("Password: ");
        display.print(Password);
        display.display();

        data_count++;
        delay(100);
    }

    if (data_count == Password_Length - 1) {
        DisplayPass();
        clearData();
    } else {
        Buzz();
    }
  
}

void DisplayPass() {
    if (!strcmp(Password, Master)) {
        angle = 0;
        servo.write(angle);
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(20, 10);
        display.print("Correct!");
        display.display();
        delay(500);
        stop = false;
    } else {
        angle = 90;
        servo.write(angle);
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(5, 10);
        display.print("Incorrect!");
        display.display();
        digitalWrite(buzzer, HIGH);
        delay(600);
        digitalWrite(buzzer, LOW);
        stop = true;
    }
}

void Buzz() {
    if (stop == true) {
        long duration, distance;
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        distance = (duration / 2) / 29.1;
        new_delay = (distance / 3) + 30;
        if (distance > 6) {
            digitalWrite(buzzer, HIGH);
            delay(new_delay);
            digitalWrite(buzzer, LOW);
        } else {
            digitalWrite(buzzer, LOW);
        }
        delay(200);
        return;
    }
}

void LedLamp() {
    if (key == 'A') {
        digitalWrite(Led1, LOW);
        digitalWrite(Led2, LOW);
        digitalWrite(Led3, LOW);

        }else if (key == 'B'){
          digitalWrite(Led1, HIGH);
          digitalWrite(Led2, HIGH);
          digitalWrite(Led3, HIGH);
        }
    }

void Clear() {
    if (key == 'C') {
        angle = 90;
        servo.write(angle);
        display.clearDisplay();
        display.display();
        stop = true;
        clearData();
    }
}

void clearData() {
    while (data_count != 0) {
        Password[data_count--] = 0;
    }
    return;
}

void BTHControl(){
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (Serial.available() > 0) {
        data = Serial.read();
        Serial.println(data);

        if (data == '4'){
        digitalWrite(Led2, HIGH);
        digitalWrite(Led3, HIGH);
        digitalWrite(Led1, LOW);
        }else if(data == '5'){
          digitalWrite(Led1, HIGH);
          digitalWrite(Led3, HIGH);
          digitalWrite(Led2, LOW);
        }else if(data == 'D'){
          digitalWrite(Led1, HIGH);
          digitalWrite(Led2, HIGH);
          digitalWrite(Led3, LOW);
        }

        if (data == '3'){
        digitalWrite(Led1, LOW);
        digitalWrite(Led2, LOW);
        digitalWrite(Led3, LOW);

        }else if (data == 'C'){
          digitalWrite(Led1, HIGH);
          digitalWrite(Led2, HIGH);
          digitalWrite(Led3, HIGH);
        }

        
        if (data == '2'){
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(5, 10);
        display.print("Temperature: ");
        display.print(temperature);
        display.println(" C");
        display.print("Humidity: ");
        display.print(humidity);
        display.println(" %");
        display.display(); 
        delay(600);
        display.clearDisplay();
        display.display();    
        delay(100);
        }

        if (data == '1') {
            angle = 0;
            servo.write(angle);
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(20, 10);
            display.print("Correct!");
            digitalWrite(LEDG, HIGH);
            display.display();
            delay(500);
            digitalWrite(buzzer, LOW);
            digitalWrite(LEDG, LOW);
            stop = false;
        } else if (data == 'A'){
            angle = 90;
            servo.write(angle);
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(5, 10);
            display.print("Incorrect!");
            digitalWrite(LEDR, HIGH);
            display.display();
            digitalWrite(buzzer, HIGH);
            delay(600);
            digitalWrite(buzzer, LOW);
            digitalWrite(LEDR, LOW);
            stop = true;
        }
    }
}

void RFID(){
    if (rfid.PICC_IsNewCardPresent()) { // new tag is available
        if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
            MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

            if (rfid.uid.uidByte[0] == authorizedUID[0] &&
                rfid.uid.uidByte[1] == authorizedUID[1] &&
                rfid.uid.uidByte[2] == authorizedUID[2] &&
                rfid.uid.uidByte[3] == authorizedUID[3] 
                ||
                rfid.uid.uidByte[0] == authorizedUID2[0] &&
                rfid.uid.uidByte[1] == authorizedUID2[1] &&
                rfid.uid.uidByte[2] == authorizedUID2[2] &&
                rfid.uid.uidByte[3] == authorizedUID2[3] ) {
                Serial.println("Authorized Tag");

                // change angle of servo motor
                angle = 0;

                // control servo motor arccoding to the angle
                digitalWrite(LEDG, HIGH);
                delay(500);
                digitalWrite(LEDG, LOW);
                delay(500);
                servo.write(angle);
                display.clearDisplay();
                display.setTextSize(2);
                display.setTextColor(SSD1306_WHITE);
                display.setCursor(20, 10);
                display.print("Correct!");
                display.display();
                digitalWrite(buzzer, HIGH);
                delay(600);
                digitalWrite(buzzer, LOW);
                Serial.print("Rotate Servo Motor to ");
                Serial.print(angle);
                Serial.println("°");
                stop = false;
            } else {
                stop = true;
                Serial.print("Unauthorized Tag with UID:");

                digitalWrite(LEDR, HIGH);
                delay(500);
                digitalWrite(LEDR, LOW);
                delay(500);

                angle = 90;
                servo.write(angle);
                display.clearDisplay();
                display.setTextSize(2);
                display.setTextColor(SSD1306_WHITE);
                display.setCursor(5, 10);
                display.print("Incorrect!");
                display.display();
                digitalWrite(buzzer, HIGH);
                delay(600);
                digitalWrite(buzzer, LOW);

                for (int i = 0; i < rfid.uid.size; i++) {
                    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
                    Serial.print(rfid.uid.uidByte[i], HEX);
                }
                Serial.println();
            }

            rfid.PICC_HaltA(); // halt PICC
            rfid.PCD_StopCrypto1(); // stop encryption on PCD
        }
    }
}


void TempDisplay() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if the button is pressed
  if (digitalRead(BUTTON) == LOW) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(5, 10);
    display.print("Temperature: ");
    display.print(temperature);
    display.println(" C");
    display.print("Humidity: ");
    display.print(humidity);
    display.println(" %");
    display.display();
    
    // Wait for the button to be released
    while (digitalRead(BUTTON) == LOW) {
      // Do nothing, just wait
    }

    display.clearDisplay();
    display.display();
  }
}

void Motion(){
  if (digitalRead(PIR)) {
  
    digitalWrite(LedPIR, HIGH);
   
  }
  else {
   
    digitalWrite(LedPIR, LOW);
  }
  delay(100);
  
}

void Smoke(){

  int analogSensor = analogRead(MQOut);
  PotValue = analogRead(Pot);
  // Checks if it has reached the threshold value
  if (PotValue >= 500 && PotValue <= 720)
  {

    digitalWrite(red, HIGH);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(15, 10);
    display.print("Warning! ");
    display.display();
    Roof.write(180);  // Full speed in one direction
    delay(100);
    // Wait for the button to be released
    AlarmBuzz();


  }else if(PotValue > 720) {

    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 10);
    display.print("Evacuate! ");
    display.display();
    Roof.write(180);  // Full speed in one direction
    delay(100);
    AlarmBuzz2();



  }else{

    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    delay(500);
    display.clearDisplay();
    display.display();
    Roof.write(90);   // Stop
    delay(100);

  }
  
}



void AlarmBuzz(){
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);   
}

void AlarmBuzz2(){
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);   
}
