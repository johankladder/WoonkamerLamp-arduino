#include <SoftwareSerial.h>
SoftwareSerial HM10(0, 1); 
String command = "";
int ledPin = 3;

void setup()
{
  Serial.begin(9600);
  Serial.println("HM10 serial started at 9600");
  HM10.begin(9600); // set HM10 serial at 9600 baud rate
  pinMode(ledPin, OUTPUT); // onboard LED
  digitalWrite(ledPin, LOW); // switch OFF LED
}

void loop()
{
  HM10.listen();  // listen the HM10 port
  while (HM10.available() > 0) {   // if HM10 sends something then read
    command = HM10.readString();
    Serial.println(command);
  }

  String preCommand = command.substring(0,3);
  
  if (preCommand == "CML") { 
      lightsState(command.charAt(3));
  }
  
  if (preCommand == "CMB") {
       lightsBlink(command.charAt(3));
  }

  if (preCommand == "CMD") {
       String brightness = command.substring(3,6);
       setBrightness(brightness);
  }
}

void lightsState(char value) {
   if (value == '1') {
      lightsOn();
   } else {
      lightsOff();
   }
}


void lightsOn() {
   digitalWrite(ledPin, HIGH); 
}

void lightsOff() {
  digitalWrite(ledPin, LOW);
}

void lightsBlink(char speedChar) {
  String s = "";
  s += speedChar;
  int intSpeed = s.toInt(); // int = 1 to 9
  
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin, HIGH);
  delay(1000 / intSpeed);
  digitalWrite(ledPin, LOW);
  delay(1000 / intSpeed);
}

void setBrightness(String brightness) {
  analogWrite(ledPin, brightness.toInt());
}
