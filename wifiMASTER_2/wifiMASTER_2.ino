#include <esp_now.h>
#include <WiFi.h>

#define PIN 2
#define CHANNEL 1


#define Y analogRead(33)
#define X analogRead(32)
#define SW digitalRead(27)

long timer = 0;
int O[5] = {0,0,0,0,0};
uint8_t output = 0;


#define MASTER
uint8_t peerMacAddress[] = {0x24, 0x0A, 0xC4, 0x9B, 0xBA, 0x94};

esp_now_peer_info_t peer;

// THIS MAC ADDRESS : A4:CF:12:12:0A:C4
// 164:207:18:18:10:196

void setup(){
  Serial.begin(115200);
  pinMode(27,INPUT_PULLUP);
  pinMode(23,OUTPUT);
  modeStation();
  InitESPNow();
  addPeer(peerMacAddress);
}

void loop(){
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);
  if(output != 0){
    timer = millis();
  }
  if(output == 0 and millis()-timer > 30000){
    delay(100);
    digitalWrite(23,LOW);
    delay(1000);
    digitalWrite(23,HIGH);
    delay(100);
    digitalWrite(23,LOW);
    delay(100);
    digitalWrite(23,HIGH);
    delay(100);
    digitalWrite(23,HIGH);
    delay(100);
    digitalWrite(23,LOW);
    delay(100);
    digitalWrite(23,HIGH);
    delay(100);
    digitalWrite(23,HIGH);
    delay(100);
    digitalWrite(23,LOW);
    delay(100);
    digitalWrite(23,HIGH);
    delay(100);
    digitalWrite(23,LOW);
    delay(1000);
    esp_deep_sleep_start();
  }
  checkstatus();
  encode();
  Serial.println(output);
  ReadAndSend(output);
}

void encode(){
  if(O[0]==0 and O[1]==0 and O[2]==0 and O[3]==0 and O[4]==0 ){
    digitalWrite(23,HIGH);
    output = 0;
  }
  else if(O[0]==1 and O[1]==0 and O[2]==0 and O[3]==0 and O[4]==0 ){
    digitalWrite(23,HIGH);
    output = 16;
  }
  else if(O[0]==0 and O[1]==0 and O[2]==0 and O[3]==0 and O[4]==1 ){
    digitalWrite(23,HIGH);
    output = 2;
  }
  else if(O[0]==0 and O[1]==0 and O[2]==0 and O[3]==1 and O[4]==0 ){
    digitalWrite(23,HIGH);
    output = 1;
  }
  else if(O[0]==0 and O[1]==0 and O[2]==1 and O[3]==0 and O[4]==0 ){
    digitalWrite(23,HIGH);
    output = 4;
  }
  else if(O[0]==0 and O[1]==1 and O[2]==0 and O[3]==0 and O[4]==0 ){
    digitalWrite(23,HIGH);
    output = 8;
  }
  else if(O[0]==1 and O[1]==0 and O[2]==0 and O[3]==0 and O[4]==1 ){
    digitalWrite(23,HIGH);
    output = 18;
  }
  else if(O[0]==1 and O[1]==0 and O[2]==0 and O[3]==1 and O[4]==0 ){
    digitalWrite(23,HIGH);
    output = 17;
  }
  else if(O[0]==1 and O[1]==0 and O[2]==1 and O[3]==0 and O[4]==0 ){
    digitalWrite(23,HIGH);
    output = 20;
  }
  else if(O[0]==1 and O[1]==1 and O[2]==0 and O[3]==0 and O[4]==0 ){
    digitalWrite(23,HIGH);
    output = 24;
  }
  
}


void checkstatus(){
  if(SW==0){
    O[0] = 1;
  }else{
    O[0] = 0;
  }

  if(X==0){
    O[1] = 1;
  }else{
    O[1] = 0;
  }

  if(X==4095){
    O[2] = 1;
  }else{
    O[2] = 0;
  }

  if(Y==0){
    O[3] = 1;
  }else{
    O[3] = 0;
  }

  if(Y==4095){
    O[4] = 1;
  }else{
    O[4] = 0;
  }
  

  
}


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void ReadAndSend(uint8_t value){
  sendDATA(&value, peerMacAddress);
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
