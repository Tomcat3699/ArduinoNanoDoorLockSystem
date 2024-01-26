#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10   //定义MFRC522模块的SPI片选引脚
#define RST_PIN 9   //定义MFRC522模块的复位引脚
#define servoPin 3   //定义舵机连接的引脚

String UID = "F3 7C 67 F6";//输入感应卡片的唯一标识号
byte lock = 0;   //用于追踪锁的状态,0表示未锁定，1表示已锁定

Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);


void setup() {
  Serial.begin(9600);
  servo.write(60);     //设置舵机的初始角度位置
  servo.attach(servoPin);  //将舵机连接到指定引脚
  SPI.begin();         //初始化SPI通信
  rfid.PCD_Init();   //初始化MFRC522模块
}

void loop() {

  if ( ! rfid.PICC_IsNewCardPresent())
    return;   //检查范围内是否有感应卡片出现，如果没有，则直接返回
  if ( ! rfid.PICC_ReadCardSerial())
    return;   //读取卡片序列号，如果读取失败，则直接返回

  Serial.print("读取成功\n");   //打印 "读取成功" 到串口
  String ID = "";    //空字符串来存储卡片的UID

  // 将卡片UID的字节转换为十六进制字符串，并拼接到ID中
  for (byte i = 0; i < rfid.uid.size; i++) {
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(300);
  }
  ID.toUpperCase();  // 将UID字符串转换为大写字母


// 检查卡片的UID是否与预定义的UID匹配，并根据锁的状态控制舵机
  if (ID.substring(1) == UID && lock == 0 ) {
    delay(100);
    servo.write(20); 
    delay(1500);  // 延迟1.5秒
    lock = 1;     // 锁定状态设置为1，表示已锁定
    Serial.print("锁定状态\n"); 
  } else if (ID.substring(1) == UID && lock == 1 ) {
    delay(100);
    servo.write(100);
    delay(1500);
    lock = 0;
    Serial.print("打开状态\n"); 
  } else {
    delay(1500);   // 如果卡片UID不匹配，延迟1.5秒
    Serial.print("UID不匹配\n"); 
  }
}