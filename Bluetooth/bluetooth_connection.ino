#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define LED_BUILTIN 2

// onConnect: add something that turns on the led
// onDisconnect: add something that turns off the led
class BLECallback: public BLECharacteristicCallbacks {
    // when the client writes something to the server
    void onWrite(BLECharacteristic *pCharacteristic) {
      // note: the value should be sent as UTF8
      std::string value = pCharacteristic->getValue();

      // make sure we received something
      if (value.length() == 1) {
        Serial.print("Value written:");
        for (int i = 0; i < value.length(); i++) {
          Serial.print(value[i]);
        }
        Serial.println();

        if (value[0] == 'L') {
          Serial.println("SENT L FOR LEFT");
          digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
          delay(1000);                      // wait for a second
          digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
          delay(1000);                      // wait for a second

        } else if (value[0] == 'R') {
          Serial.println("SENT R FOR RIGHT");
          digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
          delay(1000);                      // wait for a second
          digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
          delay(1000);                      // wait for a second
          digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
          delay(1000);                      // wait for a second
          digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
          delay(1000);                      // wait for a second
        }

      }

    }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  BLEDevice::init("BLE_Test");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  
  pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

  pCharacteristic->setCallbacks(new BLECallback());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}