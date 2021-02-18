#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

/* */
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include"esp_gap_bt_api.h"
#include "esp_err.h"

#define REMOVE_BONDED_DEVICES 1   // <- Set to 0 to view all bonded devices addresses, set to 1 to remove

#define PAIR_MAX_DEVICES 20
uint8_t pairedDeviceBtAddr[PAIR_MAX_DEVICES][6];
char bda_str[18];

bool initBluetooth()
{
  if(!btStart()) {
    Serial.println("Failed to initialize controller");
    return false;
  }

  if(esp_bluedroid_init() != ESP_OK) {
    Serial.println("Failed to initialize bluedroid");
    return false;
  }

  if(esp_bluedroid_enable() != ESP_OK) {
    Serial.println("Failed to enable bluedroid");
    return false;
  }
  return true;
}

char *bda2str(const uint8_t* bda, char *str, size_t size)
{
  if (bda == NULL || str == NULL || size < 18) {
    return NULL;
  }
  sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
          bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
  return str;
}


/* */

uint8_t address_hc06[6] = {0x00, 0x14, 0x03, 0x05, 0x5B, 0xC2}; // HC-06的地址
String name_hc06 = "DSD TECH HC-06";  // HC-06的名字
char *pin = "1234";     // HC-06的密码
bool connected;

void setup() {
  Serial.begin(115200);  // 用物理串行接口来输出字符串看是否连接成功

  /* */
  initBluetooth();
  Serial.print("ESP32 bluetooth address: "); Serial.println(bda2str(esp_bt_dev_get_address(), bda_str, 18));

  // Get the numbers of bonded/paired devices in the BT module
  int count = esp_bt_gap_get_bond_device_num();
  if(!count) {
    Serial.println("No bonded device found.");
  } else {
    Serial.print("Bonded device count: "); Serial.println(count);
    if(PAIR_MAX_DEVICES < count) {
      count = PAIR_MAX_DEVICES;
      Serial.print("Reset bonded device count: "); Serial.println(count);
    }
    esp_err_t tError =  esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
    if(ESP_OK == tError) {
      for(int i = 0; i < count; i++) {
        Serial.print("Found bonded device # "); Serial.print(i); Serial.print(" -> ");
        Serial.println(bda2str(pairedDeviceBtAddr[i], bda_str, 18));
        if(REMOVE_BONDED_DEVICES) {
          esp_err_t tError = esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
          if(ESP_OK == tError) {
            Serial.print("Removed bonded device # ");
          } else {
            Serial.print("Failed to remove bonded device # ");
          }
          Serial.println(i);
        }
      }
    }
  }
  /* */

  SerialBT.begin("ESP32_bluetooth", true);   // 蓝牙名字任意
  SerialBT.setPin(pin);                      // 匹配HC-06的密码
  Serial.println("ESP32 started in master mode!");
  connected = SerialBT.connect(address_hc06); // 通过地址匹配HC-06
  //connected = SerialBT.connect(name_hc06);  // 也可以通过名称匹配HC-06

  if(connected) {
    Serial.println("Connected!");
  } else {
    while(!SerialBT.connected(10000)) {         // 10秒的timeout时间
      Serial.println("Failed to connect.");
    }
  }
/*
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected!");
  }

  SerialBT.connect(); //重新连接 name_hc06 或者 address_hc06
*/
}

/*
  如果显示 "Failed to connect."
  有可能是 BluetoothSerial.cpp 里头的bug
  可以试试替换发给你的 BluetoothSerial.cpp 看能不能连接成功
  替换前记得备份.
*/


void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
}
