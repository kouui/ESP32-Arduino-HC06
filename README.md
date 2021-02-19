# ESP32-Arduino-HC06
ESP-WROOM-32 和 Arduino+HC-06 之间的蓝牙匹配连接与通信。

## 实际实验步骤

### 配置Arduino+HC-06并测试

1. 连接HC-06和Arduino uno: 
  - HC-06 VCC -- Arduino 5V
  - HC-06 GND -- Arduino GND
  - HC-06 TXD -- Arduino GPIO 10
  - HC-06 RXD -- Arduino GPIO 11 (我的HC-06上没有写高电平为3.3V，所以我直接连了GPIO口，如果HC-06上写了3.3V则需要一个分压电路把GPIO的高电平5V输出降到3.3~3.5V)
2. 将HC06.ino烧录进Arduino，断开USB与电脑的连接，然后把USB连到任意一个5V电源供电。这时候可以发现HC-06的红色LED闪得很快，意味着HC-06再工作但是还没与任何设备连接
3. 打开MACOS的蓝牙，发现了“DSD TECH HC-06”设备，用默认密码(pin)"1234"连接后，HC-06的红色LED变为常亮(不再闪)
4. 打开MACOS的终端，用终端确认是否成功收到HC-06发出的数据
  ```
  $ ls -l /dev/tty.*
  crw-rw-rw-  1 root  wheel   21,   2 Feb 18 18:03 /dev/tty.Bluetooth-Incoming-Port
  crw-rw-rw-  1 root  wheel   21,  26 Feb 19 08:41 /dev/tty.DSDTECHHC-06-DevB
  crw-rw-rw-  1 root  wheel   21,   0 Feb 18 18:03 /dev/tty.VDF-washboard-SerialPor-3
  $ screen /dev/tty.DSDTECHHC-06-DevB 9600
  This is a message
  This is a message
  ```
5. 关闭MACOS的蓝牙，HC-06的红色LED恢复闪烁(等待匹配)

### 匹配ESP32与HC-06

1. 电脑USB连接ESP32，打开ArduinoIDE(会需要下载ESP32用的USB驱动和ArduinoIDE里头的board驱动)，打开ArduinoIDE的串口监视器(Serial Monitor)
2. ArduinoIDE里的ESP32驱动选择版本1.0.4，将ESP32.ino烧录进ESP32，串口监视器显示"connected"，但是HC-06仍然处于红色LED闪烁状态，也就是实际没有匹配上
3. 将ArduinoIDE里的ESP32驱动选择版本替换成1.0.2，将`1.0.2/libraries/BluetoothSerial/src/BluetoothSerial.*`替换成这个repository里头的`src/BluetoothSerial.*`后，重新将ESP32.ino烧录进ESP32，串口监视器显示"connected"并且能连续接收到"This is a message"的数据，HC-06的红色LED变为常亮


## 注意

1. HC-06和电脑匹配时候，bound rate选择115200，电脑显示乱码，改成9600后，显示正常。显示乱码的原因可能是bound rate匹配不一致？
2. ESP32.ino里头上下用`/* */`括起来的代码是用于清除ESP32记忆的匹配地址，不知道清除ESP32记忆的匹配地址对于ESP32和HC-06的成功匹配是否重要
