#include <SPI.h>
#include <MFRC522.h>    

#define RST_PIN 9     //arduino nano D9  ---> RFID-RC522 RST
#define SS_PIN  10    //arduino nano D10  ---> RFID-RC522 SDA

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);  // 初始化串口通信，波特率设置为9600
  while (!Serial);    // 等待串口就绪
  SPI.begin();     // 初始化SPI通信
  mfrc522.PCD_Init();    // 初始化MFRC522模块
  delay(5);     // 延迟5毫秒等待初始化完成
  mfrc522.PCD_DumpVersionToSerial();   // 打印MFRC522的版本信息到串口
  Serial.println(F("请放上感应卡片,获取UID."));
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;   // 如果没有检测到新的卡片，直接返回
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;  // 如果无法读取卡片的序列号，直接返回
  }
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));   // 将卡片的UID信息打印到串口
}