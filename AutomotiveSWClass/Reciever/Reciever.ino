#include <Servo.h>
#include <mcp_can.h>
#include <SPI.h>

byte len;
byte rxBuf[8];
MCP_CAN CAN(10);

Servo m1; 
Servo m2;

int m1_val = 0;
int m2_val = 0;


void setup() {
  CAN.begin(CAN_500KBPS);
  pinMode(2,INPUT);
  
  
  m1.attach(5); //5번핀을 주행모터의 pwm신호 출력
  m2.attach(6); //6번핀을 서보모터의 pwm신호 출력
  m1_val=1500; //중간값 1500
  m2_val=1500; //중간값 1500 
}


void loop() {
  //1. 주행모터 정지값, 조향모터 방향값 설정
  /*
  m1_val = 1500;   // 주행모터 속도 조절
  m2_val = 1525; // 조향모터 각도 조절 // 전진최대, 전진 최소, 정지, 후진 최대, 후진 최소 조향 각도까지, 
  */
  
  // 2. 통신 mapping
  
  if(!digitalRead(2));
  {
     CAN.readMsgBuf(&len,rxBuf);

     m1_val=map(rxBuf[0], 0, 200, 1200, 1800);
     m2_val=map(rxBuf[1], 0, 200, 1200, 1800);

  }
  

  m1.writeMicroseconds(m1_val);
  m2.writeMicroseconds(m2_val);

  delay(40);
}

/*
 주행모터

1200 - 후진

1500 - 중지 ; 1480    ~ 1530

1800 - 전진


조향모터

1200 - 오른쪽 최대 

1525 - 중간

1800 - 왼쪽 최대
 */
