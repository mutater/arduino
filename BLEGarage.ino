#include <ArduinoBLE.h>

BLEService openService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Open Service
BLEService closeService("19B10000-E8F2-537E-4F6C-D104768A1215"); // Close Service
BLEService stopService("19B10000-E8F2-537E-4F6C-D104768A1216"); // Stop Service

BLEByteCharacteristic openChar("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic closeChar("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);
BLEByteCharacteristic stopChar("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN;

const int openPin = 13;
const int closePin = 12;
const int stopPin = 11;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(ledPin, OUTPUT);
  pinMode(openPin, OUTPUT);
  pinMode(closePin, OUTPUT);
  pinMode(stopPin, OUTPUT);

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Garage");
  BLE.setAdvertisedService(openService);

  // add the characteristic to the service
  openService.addCharacteristic(openChar);
  closeService.addCharacteristic(closeChar);
  stopService.addCharacteristic(stopChar);

  // add service
  BLE.addService(openService);
  BLE.addService(closeService);
  BLE.addService(stopService);

  // set the initial value for the characeristic:
  openChar.writeValue(0);
  closeChar.writeValue(0);
  stopChar.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE Garage Door Opener");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      if (openChar.written()) {
        if (openChar.value() > 0) {
          Serial.println("OPEN");
          digitalWrite(openPin, HIGH);
          delay(100);
          digitalWrite(openPin, LOW);
        }
      }
      if (closeChar.written()) {
        if (closeChar.value() > 0) {
          Serial.println("CLOSE");
          digitalWrite(closePin, HIGH);
          delay(100);
          digitalWrite(closePin, LOW);
        }
      }
      if (stopChar.written()) {
        if (stopChar.value() > 0) {
          Serial.println("STOP");
          digitalWrite(stopPin, HIGH);
          delay(100);
          digitalWrite(stopPin, LOW);
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
