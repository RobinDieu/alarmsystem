#include <NewPing.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define GREEN 3
#define MAX_DISTANCE 200
#define RED 2
#define TRIGGER_PIN 4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 5
#define RST_PIN 9
#define BUTTON 6

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(RED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(GREEN, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);

}
void loop()
{
  delay(250);
  unsigned int uS = sonar.ping();
  int result = sonar.ping() / US_ROUNDTRIP_CM;
  Serial.print("Ping: ");
  Serial.print(result); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.println("cm");

  if (digitalRead(BUTTON) == LOW)
  {
    digitalWrite(RED, HIGH);
  }
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "90 1F E9 2B") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    delay(2000);
    digitalWrite(GREEN, LOW);
    delay(3000);
  }

 else   {
    Serial.println(" Access denied");
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
    delay(500);
    digitalWrite(RED, HIGH);
    delay(3000);
  }
}
