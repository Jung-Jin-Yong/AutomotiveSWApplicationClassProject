//https://m.blog.naver.com/roboholic84/221743487231 CAN 라이브러리
#include <Servo.h>
#include <mcp_can.h>
#include <SPI.h>

byte len;
byte rxBuf[8];
MCP_CAN CAN0(10);

Servo m1;
Servo m2;

int m1_val;
int m2_val;
//int m1_span = 1;
//int m2_span = 2;

void setup() {
  CAN0.begin(CAN_500KBPS);
  pinMode(2, INPUT);
  
  // val < 90 : 우회전, val > 90 : 좌회전
  m1.attach(5);  //5번핀을 주행모터의 pwm 신호 출력
  m2.attach(6);  // 6번핀을 조향모터의 pwm 신호 출력
  m1_val = 1500;  // 중간값, 1500us
  m2_val = 1500;  // 중간값, 1500us
}

void loop() {
  if(!digitalRead(2)){
    CAN0.readMsgBuf(&len, rxBuf);
    //byte : 0 ~ 100 ~ 200 -> 1200 ~ 1500~ 1800us
    m1_val = map(rxBuf[0], 0, 200, 1200, 1800);  //1800, 1200 바꿀수 있음
    m2_val = map(rxBuf[1], 0, 200, 1200, 1800);
  }

  m1.writeMicroseconds(m1_val);
  m2.writeMicroseconds(m2_val);

  delay(10);
  /* dc, 서보모터 구동 테스트 코드입니다.
  // put your main code here, to run repeatedly:
  m1.writeMicroseconds(m1_val);
  m2.writeMicroseconds(m2_val);

  m1_val = m1_val + m1_span;
  m2_val += m2_span;

  if(m1_val > 1800) m1_span = -1;
  if(m2_val > 1800) m2_span = -1;

  if(m1_val < 1200) m1_span = 1;
  if(m2_val < 1200) m2_span = 1;
  
  delay(20);

  */ 
}
