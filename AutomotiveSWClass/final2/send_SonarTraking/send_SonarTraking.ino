#include <mcp_can.h>
#include <SPI.h>

#define IR_L A0
#define IR_M A1
#define IR_R A2

#include <NewPing.h>

#define FRONTTRIG 2
#define FRONTECHO 3

#define LEFTTRIG 4
#define LEFTECHO 5

#define RIGHTTRIG 6
#define RIGHTECHO 7

#define MAX_DISTANCE 200

NewPing sonar_f(FRONTTRIG,FRONTECHO);
NewPing sonar_l(LEFTTRIG,LEFTECHO,MAX_DISTANCE);
NewPing sonar_r(RIGHTTRIG,RIGHTECHO,MAX_DISTANCE);

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

  Serial.begin(115200);
}

void loop() {
  /*
  //1.적외선 읽어오기
  ir_l=digitalRead(IR_L);
  ir_m=digitalRead(IR_M);
  ir_r=digitalRead(IR_R);

 
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
    dir = 5, vel = 160; //  XXO 오른 쪽에만 흰 선을 감자 -> 오른쪽으로 회전
  }
  else if(!ir_l && ir_m && ir_r) {
    dir = 50, vel = 160;  // XOO 중간, 오른쪽에 흰 선을 감지
  }
  else if(!ir_l && ir_m && !ir_r) {
    dir = 100, vel = 160; // XOX 중간만 흰 선을 감지
  }
  else if(ir_l && ir_m && !ir_r) {
    dir = 150, vel = 160;       // OOX 중간, 왼쪽에 흰 선을 감지
  }
  else if(ir_l && !ir_m && !ir_r) {
    dir = 210, vel = 160; // OXX 왼쪽에 흰 선을 감지
  }
*/

  cmd[0] = vel;
  cmd[1] = dir;

  CAN0.sendMsgBuf(0x00, 0, 2, cmd);

  int front_distance = sonar_f.ping_cm();
  int left_distance = sonar_l.ping_cm();
  int right_distance = sonar_r.ping_cm();

  Serial.print("전방 : ");
  Serial.print(front_distance);
  Serial.print(" Cm ");

  Serial.print("우측 : ");
  Serial.print(right_distance);
  Serial.print(" Cm ");

  Serial.print("좌측 : ");
  Serial.print(left_distance);
  Serial.println(" Cm");

/*
  if(sonar_r.ping_cm() <= 15){
    dir = 210, vel = 150; //오른쪽 센서 15cm 이하 감지 시, 좌회전
  } 
  else if(sonar_r.ping_cm() <= 17){
    dir = 100, vel = 150;
  }else if({
    dir = 210,
  }

  if(sonar_l.ping_cm() <= 15){
    dir = 10, vel = 150; //왼쪽 센서 15cm 이하 감지 시, 우회전
  } 
  else if(sonar_l.ping_cm() <= 17){
    dir = 100, vel = 150;
  }
  
  if(sonar_f.ping_cm() <= 10){
    vel=100; //전방 10cm 이하 정지
  }
*/

  
  
  if(front_distance <= 10){
    vel = 100;   // 정지
  }
  else{
    if((left_distance <= 17 && left_distance >=6 ) && (right_distance <= 17 && right_distance >= 6)){
      dir = 100, vel = 160;   // 전진
    }
    else if(left_distance <= 5){
      dir = 50, vel = 160;     // 직진 유지용 우회전
    }
    else if(right_distance <= 5){
      dir = 150, vel = 160;    // 직진 유지용 좌회전
    }
    else if(left_distance >= 17){
      dir = 210, vel = 160;    // 코너에서 좌회전
    }
    else if(right_distance >= 17){
      dir = 5, vel = 160;     // 코너에서 우회전
    }
  }
  
  delay(10);
} 
