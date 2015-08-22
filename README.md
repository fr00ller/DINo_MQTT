# DINo_MQTT

MQTT DINo Realy Ethernet Board

Library reference :

  MQTT Arduino library : https://github.com/knolleary/pubsubclient
  
  Arduino UIP : https://github.com/ntruchsess/arduino_uip ( Used to drive ENC28j60 ethernet ic )


NB. 
  
  Need to remove every istance of Serial class, it appears to interfere with the normal program flow, i need more investigate . 
  Maybe define a Software Serial could solve the problem .
  The code is only tested whit the KMTronic DINo realy board, but it should work with any board based on ENC28J60 .
  
