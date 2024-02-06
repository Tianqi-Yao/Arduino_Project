#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int sensorPin = 2; // 红外中断传感器连接的Arduino引脚
const int ledPin = 13;


int counter = 0; // 计数器
bool is_interrupted = false; // 是否中断


void setup() {
  pinMode(sensorPin, INPUT_PULLUP); // 使用内部上拉电阻
  pinMode(ledPin, OUTPUT);
  lcd.init(); // 初始化LCD
  lcd.backlight(); // 打开背光
  Serial.begin(9600); // 初始化串口
}

void loop() {
  // 读取传感器状态
  int sensorState = digitalRead(sensorPin);
  Serial.println(sensorState);

  // 清除LCD显示
  lcd.clear();

  // 第一行, 根据传感器状态显示信息
  if (sensorState == HIGH) {
    // 连接中
    lcd.setCursor(0, 0); // 设置光标位置
    lcd.print("Beam: Not Broken"); // 显示未中断信息
    digitalWrite(ledPin, LOW); // 关闭LED

    if (is_interrupted) {
      counter++; // 计数器加1
    }
    is_interrupted = false; // 重置中断状态
  } else {
    // 中断
    lcd.setCursor(0, 0); // 设置光标位置
    lcd.print("Beam: Broken"); // 显示中断信息
    digitalWrite(ledPin, HIGH); // 点亮LED

    is_interrupted = true; // 设置中断状态
  }

  // 第二行, 显示计数器数值
  lcd.setCursor(0, 1); // 设置光标位置
  lcd.print("Counter: "); // 显示计数器信息
  lcd.print(counter); // 显示计数器数值

  // 短暂延迟，减少显示刷新频率
  delay(500);
}
