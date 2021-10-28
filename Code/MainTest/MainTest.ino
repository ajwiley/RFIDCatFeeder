/*
 * 
 * All the resources for this project: https://randomnerdtutorials.com/
 * Modified by Rui Santos
 * 
 * Created by FILIPEFLOP
 * 
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myservo;
int pos = 0;
 
void setup() {
  myservo.attach(4); // Attach the servo to the pin
  myservo.write(0); // Move to closed position
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
}

void loop() {
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
  
  // Parse the info
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  
  if (content.substring(1) == "AC 2B 44 64") { //We have a match!
    Serial.println("Authorized access");
    Serial.println();

    // Move to the open position
    if (pos != 180) {
      for (pos; pos < 180; pos++) {
        myservo.write(pos);  
      }
    }

    // Wait 2 minutes until we close
    delay(120000);
    //delay(3000);

    // Close the lip
    Serial.println("Moving back to 0");
    for (pos; pos != 0; pos--) {
        myservo.write(pos);  
    }
    pos = 0;
  }
  else { // Stay in the closed position
    Serial.println(" Access denied");
    for (pos; pos != 0; pos--) {
        myservo.write(pos);  
    }
    pos = 0;
    delay(1000);
  }
}
