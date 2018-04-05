/*

*/
/*****************************************************************************************
* FILENAME :        rfSniffer.cpp
*
* DESCRIPTION :
* 
*   RCSwitch sniffer based on RfProcl:
* 
*  https://github.com/sui77/rc-switch/
*       
*
* NOTES :
*
*
* Copyright (c) [2018] [Stephan Wink]
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
vAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* AUTHOR :    Stephan Wink        START DATE :    13.03.2018
*
*****************************************************************************************/

/****************************************************************************************/
/* Include Interfaces */
#include <RCSwitch.h>
#include <RfProcl.h>
#include <RfDevices.h>

/****************************************************************************************/
/* Local constant defines */
#define WEMOS_PIN_D0                    16u // D0
#define WEMOS_PIN_D1                    5u  // D1
#define WEMOS_PIN_D2                    4u  // D2
#define WEMOS_PIN_D3                    0u  // D3
#define WEMOS_PIN_D4                    2u  // D4
#define WEMOS_PIN_D5                    14u // D5
#define WEMOS_PIN_D6                    12u // D6
#define WEMOS_PIN_D7                    13u // D7
#define WEMOS_PIN_D8                    15u // D8

#define RX_PIN                          WEMOS_PIN_D1

/****************************************************************************************/
/* Local function like makros */

/****************************************************************************************/
/* Local type definitions (enum, struct, union) */

/****************************************************************************************/
/* Static Data instantiation */
static RCSwitch mySwitch_s = RCSwitch();
static msg_t message_s;

/****************************************************************************************/
/* Public functions (unlimited visibility) */

/**---------------------------------------------------------------------------------------
 * @brief     base setup function
 * @author    winkste
 * @date      13. Mar. 2018
 * return     -
*//*-----------------------------------------------------------------------------------*/
void setup() 
{
  Serial.begin(115200);
  mySwitch_s.enableReceive(RX_PIN);  
}

/**---------------------------------------------------------------------------------------
 * @brief     base loop function
 * @author    winkste
 * @date      13. Mar. 2018
 * return     -
*//*-----------------------------------------------------------------------------------*/
void loop() 
{
  uint32_t rawValue_u32 = 0; 
  uint16_t bitLength_u16 = 0;
  uint16_t protocol_u16 = 0;
  
  if (mySwitch_s.available()) 
  {   
    // handle new message   
    rawValue_u32 = mySwitch_s.getReceivedValue();
    bitLength_u16 = mySwitch_s.getReceivedBitlength();    
    protocol_u16 = mySwitch_s.getReceivedProtocol();
    mySwitch_s.resetAvailable();

    // print raw message
    Serial.print("Received Raw: ");
    Serial.print(rawValue_u32);
    Serial.print(" / ");
    Serial.print(bitLength_u16);
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println(protocol_u16);
    
    // parse message according to RfProcl
    if(32 == bitLength_u16)
    {
      /*Serial.print(sizeof(msg_t));
      Serial.print(", ");
      Serial.print(sizeof(uint16_t));
      Serial.print(", ");
      Serial.println(sizeof(uint8_t));*/
      RfProcl::SetRawData(&message_s, rawValue_u32); 
      Serial.print("+++ Header: ");
      Serial.print(message_s.header_u8);
      Serial.print(", Data: ");
      Serial.print(message_s.data_u16);
      Serial.print(", CheckSum: ");
      Serial.println(message_s.chkSum_u8);
      Serial.print(">>> FromNode: ");
      Serial.print(RfProcl::GetFromNodeId(&message_s));
      Serial.print(" -> ToNode: ");
      Serial.print(RfProcl::GetToNodeId(&message_s));
      Serial.print(" [");
      Serial.print(RfProcl::GetMsgTypeId(&message_s));
      Serial.print("] ");
      Serial.print("Payload: ");
      Serial.print(RfProcl::GetMsgData(&message_s));
      Serial.print(" V: ");
      Serial.println(RfProcl::VerifyMessage(&message_s));
    }
  }
}
