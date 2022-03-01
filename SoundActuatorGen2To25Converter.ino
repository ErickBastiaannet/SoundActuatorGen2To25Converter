#include <mcp_can.h>                      
MCP_CAN CAN0(10); 

// Bytes for can sending
byte b0_167;
byte b1_167;
byte b2_167;
byte b3_167; 
byte b4_167;
byte b5_167;
byte b6_167;
byte b7_167;

// Bytes for can sending
byte b0_168;
byte b1_168;
byte b2_168;
byte b3_168;
byte b4_168;
byte b5_168;
byte b6_168;                                                                                          //EngineSpeed
byte b7_168;                                                                                          //Enginespeed

// Bytes for can sending
byte b0_253;
byte b1_253;
byte b2_253;
byte b3_253;
byte b4_253;                                                                                          //VehicleSpeed
byte b5_253;                                                                                          //VehicleSpeed
byte b6_253;
byte b7_253;

// outputs & variabel define 
int ledD2         =  0;                                                                               // led D2 
int ledD3         =  1;                                                                               // led D3

byte Nm;
long unsigned int Test;

// can receiving buffers
long unsigned int rxId;
unsigned char lenght = 0;
unsigned char rxBuf[8];

//Timer
class timer {
  private:
    unsigned long timer;
    void reset() {
      timer = millis();
    }
  public:
    bool check(int WantedDelay) {
      if (timer + WantedDelay <= millis()) {
        timer::reset();
        return 1;
      }
      else return 0;
    }
};

timer Timer;
timer sendCanMessages;

unsigned int uIntAddLowHighByte(byte low, byte high) 
    {
    return low + ((high << 8) & 0xFF00);
    }

void setup() 
  {    
    CAN0.begin(CAN_500KBPS);                                                                          // init can bus : baudrate = 100k  
    pinMode(ledD2, OUTPUT);                                                                           // led D2
    pinMode(ledD3, OUTPUT);                                                                           // led D3   
    Serial.begin(115200);                                                                             // Seriele poort openen met baud 115200
    Serial.println("Setup Finished");    
  }
    
void loop() 
  { 
    CAN0.readMsgBuf(&lenght, rxBuf);                                                                  // Read data: lenght = data length, buf = data byte(s)
    rxId = CAN0.getCanId();                                                                           // Get message ID      
    
    if (rxId == 261) //0x105h
    {   
      //Serial.print("Motor");  
      //b0_168 = rxBuf[0];
      //b1_168 = rxBuf[1];
      b6_168 = rxBuf[2];                                                                              //EngineSpeed
      b7_168 = rxBuf[3];                                                                              //EngineSpeed
      //b4_168 = rxBuf[4];                                                                            //Value003atNoThrottle 002atThrottle
      //b5_168 = rxBuf[5];
      //b3_167 = rxBuf[6];                                                                            //ThrottlePedal 0 Min - 250 Max
      //b7_168 = rxBuf[7];      
    }    
    if(rxId == 256) //0x100h
    {      
      //Serial.print("ESP");  
      //b0_253 = rxBuf[0];
      //b1_253 = rxBuf[1];
      //b2_253 = rxBuf[2];
      //b3_253 = rxBuf[3];
      b4_253 = rxBuf[4];                                                                              //VehicleSpeed
      b5_253 = rxBuf[5];                                                                              //VehicleSpeed
      //b6_253 = rxBuf[6];
      //b7_253 = rxBuf[7]; 
    }
    if(rxId == 128) //0x80h
    {      
      //Serial.print("OBD");  
      //b0_167 = rxBuf[0];
      //b1_167 = rxBuf[1];
      b2_167 = rxBuf[2];                                                                              //MO_Mom_Ist_Summe?
      b3_167 = rxBuf[3];                                                                              //MO_Mom_Ist_Summe?
      //b4_167 = rxBuf[4]; 
      //b5_167 = rxBuf[5];
      //b6_167 = rxBuf[6];
      //b7_167 = rxBuf[7]; 
    }   
       
    unsigned int oldValue = (b2_167 >> 6) + (((unsigned int) b3_167) << 2);                           //Select 10 bits (byte3 76543210 byte2 76)
    
    //int raisedValue = oldValue  + 509;                                                              //Raise value with offset
    int raisedValue = oldValue  + 459;                                                                //Raise value with offset with negative Load

    if (raisedValue > 1000)                                                                           //1023 is MaxValue when this is reached the sound goes off. 
    {
      raisedValue = 1000;
    }
    
    byte outByte2 = (b2_167 & 0b00111111) + ((raisedValue << 6) & 0b11000000);                        //Bitshift and bitmask the bits back to (byte2 76)
    byte outByte3 = ((raisedValue >> 2) & 0b11111111);                                                //Bitshift the bits back to (byte3 76543210)
       
    //Serial.print(oldValue, DEC);
    //Serial.print('\n');
    //Serial.print(raisedValue, DEC);
    //Serial.print('\n');
    //Serial.print(outByte2, DEC);
    //Serial.print('\n');
    //Serial.print(outByte3, DEC);
    //Serial.print('\n');   
    
    //Fill bytes
    byte motorMoment[8]   = {b0_167,b1_167,outByte2,outByte3,b4_167,b5_167,b6_167,b7_167};           //Nm from ID 0x80h (128 dec) raises with offset
    byte motorRpm[8]      = {b0_168,b1_168,b2_168,b3_168,b4_168,b5_168,b6_168,b7_168};               //Byte 6 and 7 RPM
    byte esp[8]           = {b0_253,b1_253,b2_253,b3_253,b4_253,b5_253,b6_253,b7_253};               //Byte 4 and 5 Speed Signal

    //Cycle time
    if (sendCanMessages.check(20)) 
    {           
    CAN0.sendMsgBuf(0xA7, 0, 8, motorMoment);  
    CAN0.sendMsgBuf(0xA8, 0, 8, motorRpm);
    CAN0.sendMsgBuf(0xFD, 0, 8, esp);
    }
}  
