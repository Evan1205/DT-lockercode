  #include <SoftwareSerial.h>
  #include <Adafruit_Fingerprint.h>
  #include <Servo.h>
  #include <Adafruit_SSD1306.h>
  #include <Adafruit_GFX.h>

  #define OLED_RESET 4
  Adafruit_SSD1306 display(OLED_RESET);



  #define servoPin 9

  SoftwareSerial mySerial(2, 3);  
  Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);



  Servo myservo;

  void setup() {

      display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("Place Your   Finger");
    display.display();
    Serial.begin(9600);
    mySerial.begin(9600);
    finger.begin(57600);
    
    if (finger.verifyPassword()) {
      Serial.println("Fingerprint sensor is OK");
    } else {
      Serial.println("Fingerprint sensor not found");
      while (1) { delay(1); }
    }
    
    myservo.attach(servoPin);
  }

  void loop() {
    getFingerprintID();
  }

  uint8_t getFingerprintID() {
    uint8_t p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println("No finger detected");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        return p;
      default:
        Serial.println("Unknown error");
        return p;
    }
    
    p = finger.image2Tz();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
      default:
        Serial.println("Unknown error");
        return p; 
    }


    
    
  p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK) {
      Serial.println("Fingerprint found");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.setCursor(0,0);
      display.println("Locker has Opened");
      display.display();
      myservo.write(90);
      delay(2000);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.setCursor(0,0);
      display.println("Locker    Closing");
      display.display();
      myservo.write(90);
      myservo.write(0);
      delay(1500);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.setCursor(0,0);
      display.println("Place Your   Finger");
      display.display();
      return p;
    } else {
      Serial.println("Fingerprint not found");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.setCursor(0,0);
      display.println("Incorrect! Try again");
      display.display();

      delay(3000);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.setCursor(0,0);
      display.println("Place Your   Finger");
      display.display();
      return p;
    }
  }
