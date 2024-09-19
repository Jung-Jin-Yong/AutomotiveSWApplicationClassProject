#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(10);
byte md[2] = {100, 100};
int i;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  CAN0.begin(CAN_500KBPS);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    for(i = 0; i < 2; i++){
      for(i = 0; i < 2; i++) md[i] = Serial.parseInt();
      Serial.flush();
    }
  }
  Serial.print(md[0]);
  Serial.print("   ");
  Serial.println(md[1]);
  
  CAN0.sendMsgBuf(0x00, 0, 2, md);
  delay(40);
}
