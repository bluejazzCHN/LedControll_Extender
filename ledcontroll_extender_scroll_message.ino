/*
   Name:LedCtronll Extender
   Description:
   Author:bluejazzCHN
*/
#include "LedControl.h"

int dataPin = 33; // 33 is IO33 map to Pin0 on board
int clockPin = 26; //26 is IO26 map to Pin8 on board
int csPin = 32; // 32 is IO32 map to Pin1 on board
int deviceNum = 4;

LedControl lc = LedControl(dataPin, clockPin, csPin, deviceNum);

byte e[8] =     {0x7C, 0x7C, 0x60, 0x7C, 0x7C, 0x60, 0x7C, 0x7C}; //E
byte d[8] =     {0x78, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x78}; //D
byte u[8] =     {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x7E}; //U
byte c[8] =     {0x7E, 0x7E, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x7E}; //C
byte eight[8] = {0x7E, 0x7E, 0x66, 0x7E, 0x7E, 0x66, 0x7E, 0x7E}; //8
byte s[8] =     {0x7E, 0x7C, 0x60, 0x7C, 0x3E, 0x06, 0x3E, 0x7E}; //S
byte dot[8] =   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18}; //.
byte o[8] =     {0x7E, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x7E}; //O
byte m[8] =     {0xE7, 0xFF, 0xFF, 0xDB, 0xDB, 0xDB, 0xC3, 0xC3}; //M
byte smile[8] =   {0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C}; //笑脸
byte neutral[8] = {0x3C, 0x42, 0xA5, 0x81, 0xBD, 0x81, 0x42, 0x3C}; //标准脸

void lcInit() {
  for (int i = 0; i < lc.getDeviceCount(); i++) {
    lc.shutdown(i, false);      //启动时，MAX72XX处于省电模式
    lc.setIntensity(i, 2);      //将亮度设置为最大值
    lc.clearDisplay(i);         //清除显示
  }
}

void setup() {
  lcInit();

}

void loop() {
  scrollMsgOnMulti(0, 4,smile,500);
}

//base function : 1.InOut; 2.OutIn; 3.Cross
//==========================================
void scrollLeftInOutMsg(int addr, byte character[], int delayTime) {
  for (int j = 0; j < 8 + 1; j++) {
    for (int i = 0; i < 8; i++) {
      lc.setRow(addr, i, character[i] << j);
    }
    delay(delayTime);
    lc.clearDisplay(addr);
  }
}

void scrollLeftOutInMsg(int addr, byte character[], int delayTime) {
  for (int j = 8; j > -1; j--) {
    for (int i = 0; i < 8; i++) {
      lc.setRow(addr, i, character[i] >> j);
    }
    delay(delayTime);
    lc.clearDisplay(addr);
  }
}

void scrollLeftCrossMsgOnOne(int addr, byte character[], int delayTime) {
  for (int j = 0; j < 8 + 1; j++) {
    for (int i = 0; i < 8; i++) {
      lc.setRow(addr, i, character[i] << j);
      lc.setRow(addr + 1, i, character[i] >> (8 - j));
    }
    delay(delayTime);
    lc.clearDisplay(addr);
    lc.clearDisplay(addr + 1);
  }
}

void scrollLeftCrossMsgOnMulti(int addr, int deviceNumber, byte character[], int delayTime) {
  for (int n = 0; n < deviceNumber - 1; n++) {
    int baseAddr = addr + n;
    for (int j = 0; j < 8 + 1; j++) {
      for (int i = 0; i < 8; i++) {
        lc.setRow(baseAddr, i, character[i] << j);
        lc.setRow(baseAddr + 1, i, character[i] >> (8 - j));
      }
      delay(delayTime);
      lc.clearDisplay(baseAddr);
      lc.clearDisplay(baseAddr + 1);
    }
  }
}

//scroll show messeage function: 1 On one; 2.On double
//====================================================
void scrollMsgOnOne(int addr, byte character[], int delayTime) {
  scrollLeftOutInMsg(addr, character, delayTime);
  scrollLeftInOutMsg(addr, character, delayTime);
}

void scrollMsgOnMulti(int addr, int deviceNumber, byte character[], int delayTime) {
  scrollLeftOutInMsg(addr, character, delayTime);
  scrollLeftCrossMsgOnMulti(addr, deviceNumber, character, delayTime);
  scrollLeftInOutMsg(addr + deviceNumber - 1, character, delayTime);
}
