#include <mcp_can.h>
#include <SPI.h>

#define IR_L A0
#define IR_M A1
#define IR_R A2

MCP_CAN CAN0(10);

int vel = 100; //정지
int dir = 100; //중립
int count = 0; //가로선 만난 횟수
bool ir_l, ir_m, ir_r;
byte cmd[2] = {100, 100};


void setup() {
  CAN0.begin(CAN_500KBPS);
  pinMode(IR_L, INPUT);
  pinMode(IR_M, INPUT);
  pinMode(IR_R, INPUT);
  Serial.begin(9600);
}

void loop() {
  //1.적외선 읽어오기
  ir_l = digitalRead(IR_L);
  ir_m = digitalRead(IR_M);
  ir_r = digitalRead(IR_R);

  Serial.print("count : ");
  Serial.println(count);

  /*
    //2. 한계값 설정
    if(dir < 3) dir =3;
    if(dir >197) dir =197;
  */
  //3. 적외선 신호에 따라 구동제어 // 0 ~ 100 : 우회전, 100 ~ 200 : 좌회전
  if (ir_l && ir_m && ir_r) {
    // OOO 가로 흰 선을 만나는 경우 가정 -> 주행 off
    count += 1;
    switch (count) {
      case 1 :
        delay(800);
      case 2 :
        {
          vel = 100;
          
          cmd[0] = vel;
          cmd[1] = dir;

          CAN0.sendMsgBuf(0x00, 0, 2, cmd);
          delay(1000);
          
          vel = 150;
          dir = 100;
          cmd[0] = vel;
          cmd[1] = dir;

          CAN0.sendMsgBuf(0x00, 0, 2, cmd);
          delay(800);
          break;
        }
      case 3 :
        {
          vel = 100;
          /*
          while(dir <= 210){  //좌회전 풀
            dir++;
            cmd[0] = vel;
            cmd[1] = dir;

            CAN0.sendMsgBuf(0x00, 0, 2, cmd);
            delay(30);
          }
          */
          for(int i = 100 ; i <= 210; i++){
            dir = i;
            cmd[0] = vel;
            cmd[1] = dir;

            CAN0.sendMsgBuf(0x00, 0, 2, cmd);
            delay(30);
          }
          
          /*
          while(dir >=5){  // 우회전 풀
            dir--;
            cmd[0] = vel;
            cmd[1] = dir;

            CAN0.sendMsgBuf(0x00, 0, 2, cmd);
            delay(30);
          }
          */

          for(int i = 210 ; i >=5; i++){
            dir = i;
            cmd[0] = vel;
            cmd[1] = dir;

            CAN0.sendMsgBuf(0x00, 0, 2, cmd);
            delay(30);
          }

          dir = 100;
          vel = 150; // 조향 중립 조정
          cmd[0] = vel;
          cmd[1] = dir;

          CAN0.sendMsgBuf(0x00, 0, 2, cmd);
          delay(1200);
          break;
        }
      case 4 :
        {
          vel = 100;
          cmd[0] = vel;
          cmd[1] = dir;

          CAN0.sendMsgBuf(0x00, 0, 2, cmd);
          break;
        }
    }
  }
  else if (!ir_l && !ir_m && !ir_r) {
    vel = 100;  // XXX
  }
  else if (ir_l && !ir_m && ir_r) {
    vel = 100; // OXO 왼쪽, 오른쪽에 흰 선을 감지
  }
  else if (!ir_l && !ir_m && ir_r) {
    dir = 5, vel = 150; //  XXO 오른 쪽에만 흰 선을 감자 -> 오른쪽으로 회전
  }
  else if (!ir_l && ir_m && ir_r) {
    dir = 50, vel = 150;  // XOO 중간, 오른쪽에 흰 선을 감지
  }
  else if (!ir_l && ir_m && !ir_r) {
    dir = 100, vel = 150; // XOX 중간만 흰 선을 감지
  }
  else if (ir_l && ir_m && !ir_r) {
    dir = 150, vel = 150;       // OOX 중간, 왼쪽에 흰 선을 감지
  }
  else if (ir_l && !ir_m && !ir_r) {
    dir = 210, vel = 150; // OXX 왼쪽에 흰 선을 감지
  }





  cmd[0] = vel;
  cmd[1] = dir;

  CAN0.sendMsgBuf(0x00, 0, 2, cmd);

  delay(10);
}
