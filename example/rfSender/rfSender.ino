/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>
#include <RfProcl.h>
#include <RfDevices.h>

#define WEMOS_PIN_D0                    16u // D0
#define WEMOS_PIN_D1                    5u  // D1
#define WEMOS_PIN_D2                    4u  // D2
#define WEMOS_PIN_D3                    0u  // D3
#define WEMOS_PIN_D4                    2u  // D4
#define WEMOS_PIN_D5                    14u // D5
#define WEMOS_PIN_D6                    12u // D6
#define WEMOS_PIN_D7                    13u // D7
#define WEMOS_PIN_D8                    15u // D8

#define TX_PIN                          WEMOS_PIN_D4

RCSwitch mySwitch = RCSwitch();
static msg_t  myMessage_s;

static void printMsg();

void setup() {

  Serial.begin(115200);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(TX_PIN);

  // initialize message 
  RfProcl::InitializeMessage(&myMessage_s);
  
  Serial.println("sender initialized");
  Serial.print("msg after init:");
  Serial.println(RfProcl::GetRawData(&myMessage_s));
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() 
{

  Serial.println("send start of loop pattern:");
  mySwitch.send(1111, 32);
  delay(1000);

  // send all message with 1
  RfProcl::InitializeMessage(&myMessage_s);
  RfProcl::SetFromNodeId(&myMessage_s, FROM_NODE_ID_03);
  RfProcl::SetToNodeId(&myMessage_s, TO_NODE_ID_03);
  RfProcl::SetMsgTypeId(&myMessage_s, MSG_ID_03);
  RfProcl::SetMsgData(&myMessage_s, 0xffff);
  RfProcl::CalculateChkSum(&myMessage_s);
  mySwitch.send(RfProcl::GetRawData(&myMessage_s), 32);
  printMsg();
  delay(1000);

  // send all message with all 0
  RfProcl::InitializeMessage(&myMessage_s);
  RfProcl::SetFromNodeId(&myMessage_s, FROM_NODE_ID_00);
  RfProcl::SetToNodeId(&myMessage_s, TO_NODE_ID_00);
  RfProcl::SetMsgTypeId(&myMessage_s, MSG_ID_00);
  RfProcl::SetMsgData(&myMessage_s, 0x0000);
  RfProcl::CalculateChkSum(&myMessage_s);
  mySwitch.send(RfProcl::GetRawData(&myMessage_s), 32);
  printMsg();
  delay(1000);

  Serial.println("send end of loop pattern");
  mySwitch.send(4444, 32);
  delay(3000);
}

void printMsg()
{
    Serial.print("+++ Header: ");
    Serial.print(myMessage_s.header_u8);
    Serial.print(", Data: ");
    Serial.print(myMessage_s.data_u16);
    Serial.print(", CheckSum: ");
    Serial.println(myMessage_s.chkSum_u8);
    Serial.print(">>> FromNode: ");
    Serial.print(RfProcl::GetFromNodeId(&myMessage_s));
    Serial.print(" -> ToNode: ");
    Serial.print(RfProcl::GetToNodeId(&myMessage_s));
    Serial.print(" [");
    Serial.print(RfProcl::GetMsgTypeId(&myMessage_s));
    Serial.print("] ");
    Serial.print("Payload: ");
    Serial.print(RfProcl::GetMsgData(&myMessage_s));
    Serial.print(" V: ");
    Serial.println(RfProcl::VerifyMessage(&myMessage_s));
}
