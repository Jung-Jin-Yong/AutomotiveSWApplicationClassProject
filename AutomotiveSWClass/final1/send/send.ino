#include <mcp_can.h>
#include <SPI.h>

#define IR_L A0
#define IR_M A1
#define IR_R A2

MCP_CAN CAN0(10);

int vel =100;  //정지
int dir =100;  //중립
bool ir_l, ir_m, ir_r;
byte cmd[2] = {100, 100};


void setup() {
  CAN0.begin(CAN_500KBPS);
  pinMode(IR_L, INPUT);
  pinMode(IR_M, INPUT);
  pinMode(IR_R, INPUT);
}

void loop() {
  //1.적외선 읽어오기
  ir_l=digitalRead(IR_L);
  ir_m=digitalRead(IR_M);
  ir_r=digitalRead(IR_R);

  /*
  //2. 한계값 설정
  if(dir < 3) dir =3;
  if(dir >197) dir =197;
  */
  //3. 적외선 신호에 따라 구동제어 // 0 ~ 100 : 우회전, 100 ~ 200 : 좌회전
  if (ir_l && ir_m && ir_r) {
    vel = 100; // OOO 가로 흰 선을 만나는 경우 가정 -> 주행 off
  }
  else if(!ir_l && !ir_m && !ir_r) {
    vel=100; // XXX
  }
  else if(ir_l && !ir_m && ir_r) {
    vel = 100; // OXO 왼쪽, 오른쪽에 흰 선을 감지
  }
  else if(!ir_l && !ir_m && ir_r) {
    dir = 5, vel = 120; //  XXO 오른 쪽에만 흰 선을 감자 -> 오른쪽으로 회전
  }
  else if(!ir_l && ir_m && ir_r) {
    dir = 40, vel = 120;  // XOO 중간, 오른쪽에 흰 선을 감지
  }
  else if(!ir_l && ir_m && !ir_r) {
    dir = 100, vel = 120; // XOX 중간만 흰 선을 감지
  }
  else if(ir_l && ir_m && !ir_r) {
    dir = 160, vel = 120;       // OOX 중간, 왼쪽에 흰 선을 감지
  }
  else if(ir_l && !ir_m && !ir_r) {
    dir = 210, vel = 120; // OXX 왼쪽에 흰 선을 감지
  }

  cmd[0] = vel;
  cmd[1] = dir;

  CAN0.sendMsgBuf(0x00, 0, 2, cmd);

  delay(10);
} 
