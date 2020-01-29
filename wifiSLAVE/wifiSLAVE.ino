#include <esp_now.h>
#include <WiFi.h>

#define DATA1 13
#define LATCH1 12
#define CLOCK1 14

#define DATA2 27
#define LATCH2 26
#define CLOCK2 25

#define PIN 2
#define CHANNEL 1

bool player1 = false;
bool player2 = false;
uint8_t dataP1 = 0;
uint8_t dataP2 = 0;

byte out1,out2;

uint8_t peerMacAddress[2][6] = {
  {0x3C, 0x71, 0xBF, 0x78, 0xCF, 0x78},
  {0xA4, 0xCF, 0x12, 0x12, 0x0A, 0xC4}
};

esp_now_peer_info_t peer;

// THIS MAC ADDRESS : 24:0A:C4:9B:BA:94

void setup(){
  Serial.begin(115200);
  modeStation();
  InitESPNow();
  addPeer(*peerMacAddress);
  esp_now_register_recv_cb(onDataRecv);
  pinMode(DATA1,OUTPUT);pinMode(CLOCK1,OUTPUT);pinMode(LATCH1,OUTPUT);
  pinMode(DATA2,OUTPUT);pinMode(CLOCK2,OUTPUT);pinMode(LATCH2,OUTPUT);
}

void loop(){
  decodeDATA();
  transmitDATA();
}

void transmitDATA(){
  digitalWrite(LATCH1, LOW);
  shiftOut(DATA1, CLOCK1, MSBFIRST, out1);
  digitalWrite(LATCH1, HIGH);
  digitalWrite(LATCH2, LOW);
  shiftOut(DATA2, CLOCK2, MSBFIRST, out2);
  digitalWrite(LATCH2, HIGH);
  delay(10);
}


void decodeDATA(){
  if(dataP1==0)       out1=0x00;
  else if(dataP1==1)  out1=0x01;
  else if(dataP1==2)  out1=0x02;
  else if(dataP1==4)  out1=0x04;
  else if(dataP1==8)  out1=0x08;
  else if(dataP1==16)  out1=0x10;
  else if(dataP1==17)  out1=0x11;
  else if(dataP1==18)  out1=0x12;
  else if(dataP1==20)  out1=0x14;
  else if(dataP1==24)  out1=0x18;

  if(dataP2==0)       out2=0x00;
  else if(dataP2==1)  out2=0x01;
  else if(dataP2==2)  out2=0x02;
  else if(dataP2==4)  out2=0x04;
  else if(dataP2==8)  out2=0x08;
  else if(dataP2==16)  out2=0x10;
  else if(dataP2==17)  out2=0x11;
  else if(dataP2==18)  out2=0x12;
  else if(dataP2==20)  out2=0x14;
  else if(dataP2==24)  out2=0x18;
}


void modeStation(){
  WiFi.mode(WIFI_STA);
  Serial.print("MAC ADDRESS : ");
  Serial.println(WiFi.macAddress());
}

void InitESPNow(){
  if(esp_now_init() == ESP_OK){
    Serial.println("ESP init success");
  }
  else{
    Serial.println("ESP init failed");
    ESP.restart();
  }
}

void addPeer(uint8_t *peerMacAddress){
  peer.channel = CHANNEL;
  peer.encrypt = 0;
  memcpy(peer.peer_addr, peerMacAddress, 6);
  esp_now_add_peer(&peer);
}

void sendDATA(const uint8_t *value, uint8_t *peerMacAddress){
  esp_err_t result = esp_now_send(peerMacAddress, value, sizeof(value));
  Serial.print("Send : ");
  Serial.print(*value);
  Serial.print("  <<< ");
  if (result == ESP_OK){
    Serial.println(" SUCCESS ");
  }else{
    Serial.println(" FAILED ");
  }
}

void onDataRecv(const uint8_t *mac_addr, const uint8_t *value, int lne){
    if(mac_addr[0]==60 &&  mac_addr[1]==113 && mac_addr[2]==191 && mac_addr[3]==120 && mac_addr[4]==207 && mac_addr[5]==120){
        player1 = true;
    }else{
        player1 = false;
    }
     
    if(mac_addr[0]==164 &&  mac_addr[1]==207 && mac_addr[2]==18 && mac_addr[3]==18 && mac_addr[4]==10 && mac_addr[5]==196){
        player2 = true;
    }else{
        player2 = false;
    }

    if(player1==true){
      dataP1 = *value;
    }
    
    if(player2==true){
      dataP2 = *value;
    }
    
}
