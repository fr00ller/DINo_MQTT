/*
 MQTT - DINo
 
  - connects to an MQTT server
  - subscribes to the topic for Rele update
  - Publish data read by 4 digital Input 
  - Connect to Domotiga for remote control 

*/

#include <SPI.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

char* _Topic1 = "raw/Giardino/DINo/set/0001";
char* _Topic2 = "raw/Giardino/DINo/set/0002";
char* _Topic3 = "raw/Giardino/DINo/set/0003";
char* _Topic4 = "raw/Giardino/DINo/set/0004";

// Update these with values suitable for your network.

char* _SendTopic1 = "raw/Giardino/DINo/get/0001";
char* _SendTopic2 = "raw/Giardino/DINo/get/0002";
char* _SendTopic3 = "raw/Giardino/DINo/get/0003";
char* _SendTopic4 = "raw/Giardino/DINo/get/0004";

// Update these with values suitable for your network.

char* client_name = "";
byte mac[]    = {  0x6A, 0x2C, 0x13, 0xC4, 0xEE, 0xED };
byte server[] = { 0, 0, 0, 0 };  /// Setting Broker Server address 
IPAddress ip(0, 0, 0, 0);  /// Setting device IP --- Need DHCP autoassing !!!

/// -------------------------------------------------- ///


unsigned long lastRun = 0;  // Used to temporize sending Input value .

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

// Callback function for message return from subscription
void callback(char* topic, byte* payload, unsigned int length) {

  /// Convert Byte value to int (Truncate at first byte)
  int i = hex2int((char *)payload, 1);
  

  /// Switch by Topic and setting Rele state 
  if(String(topic) == String(_Topic1)){
    i > 0 ? digitalWrite(5, 1) : digitalWrite(5, 0);
  } 
  if(String(topic) == String(_Topic2)){
    i > 0 ? digitalWrite(6, 1) : digitalWrite(6, 0);
  } 
  if(String(topic) == String(_Topic3)){
    i > 0 ? digitalWrite(7, 1) : digitalWrite(7, 0);
  }   
  if(String(topic) == String(_Topic4)){
    i > 0 ? digitalWrite(8, 1) : digitalWrite(8, 0);
  } 
  
}

void Setup_Pins(){

	pinMode(5, OUTPUT); digitalWrite(5, 0);
	pinMode(6, OUTPUT); digitalWrite(6, 0);
	pinMode(7, OUTPUT); digitalWrite(7, 0);
	pinMode(8, OUTPUT); digitalWrite(8, 0);

}

unsigned long hex2int(char *a, unsigned int len)
{
   int i;
   unsigned long val = 0;

   for(i=0;i<len;i++)
      if(a[i] <= 57)
       val += (a[i]-48)*(1<<(4*(len-1-i)));
      else
       val += (a[i]-55)*(1<<(4*(len-1-i)));
   return val;
}

char* getPinValue(int pin){
  
  if(digitalRead(pin) == HIGH){
    return "1";
  }else{
    return "0";
  }

}

void setup()
{
  Setup_Pins();
  Ethernet.begin(mac, ip);

  /// Checking if client is connet  --- Need Try/catch retry (maybe DHCP ... ) cycle .
  if (client.connect(client_name)) {

    /// Subscibing one Topic for Rele 
    client.subscribe(_Topic1);
    client.subscribe(_Topic2);
    client.subscribe(_Topic3);
    client.subscribe(_Topic4);
  }
}

void loop()
{

   if(!client.connected()){
      client.disconnect();
      client.connect(client_name);       
      lastRun = millis();

   }else{
      
      // Sending data to Domotiga every 60s ...

     if(millis() > (lastRun + 60000)){
        lastRun = millis();
        client.publish(_SendTopic1, getPinValue(A2));
        client.publish(_SendTopic2,getPinValue(A3));
        client.publish(_SendTopic3,getPinValue(A4));
        client.publish(_SendTopic4,getPinValue(A5));  
         
        client.publish(_SendTopicRel1,getPinValue(5));
        client.publish(_SendTopicRel2,getPinValue(6));
        client.publish(_SendTopicRel3,getPinValue(7));
        client.publish(_SendTopicRel4,getPinValue(8));    
     } 
   
     client.loop();  
   
   }
    
}

