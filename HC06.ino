#include <SoftwareSerial.h>

SoftwareSerial SerialBT(10, 11); // RX, TX

void setup() {
  // put your setup code here, to run once:

  SerialBT.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  SerialBT.println("send start!");
}

void loop() {
  // put your main code here, to run repeatedly:

  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(3000);
  //digitalWrite(LED_BUILTIN, LOW);
  //delay(3000);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  SerialBT.write("This is a message\n");

  //delay(1000);

}
