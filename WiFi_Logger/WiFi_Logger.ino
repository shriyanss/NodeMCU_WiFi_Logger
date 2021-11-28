#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//Define WiFi credentials. Make sure you change/remember these
const char *ssid = "ESP.Net";
const char *passwd = "masterpasswd";

//Define runtime variables
char* wifiList = "";
const int outPin = 16; //D0
const int inPin = 2; //D4
int serverStat = 0;
int hotspotStat = 0;
int state = 0;
String recNets = "";
String nets = "SSID;;Signal_Strength;;Encryption_Type<br>";

ESP8266WebServer server(80);

void serverManage(){
  if (serverStat == 0){
    server.on("/", handleRoot);
    server.begin();
    server.handleClient();
    serverStat = 1;
  } else{
    server.handleClient();
  }
}

void manageHotspot(){
  if (hotspotStat == 0){
    WiFi.softAP(ssid, passwd);
    hotspotStat = 1;
    Serial.write("Wifi on");
  } else{
    
  }
}

void handleRoot() {
  server.send(200, "text/html", nets);
}

void setup() {
  Serial.begin(115200);
  pinMode(outPin, OUTPUT);
  pinMode(inPin, INPUT);
}

void loop() {
  state = digitalRead(inPin);

  if (state == HIGH){
    int scanNetwork = WiFi.scanNetworks();
    
    if (scanNetwork == 0){
    } else if (scanNetwork > 0){
      for(int i =0; i<scanNetwork; i++){
//        byte* bssid = WiFi.BSSID(i);
        String wifiSsid = WiFi.SSID(i);
        if(recNets.indexOf(wifiSsid) >= 0){
          
        } else {
          nets = nets + String(WiFi.SSID(i)) + ";;" /*+ ((char *) bssid) + ";;"*/ + String(WiFi.RSSI(i)) + ";;" + String(WiFi.encryptionType(i)) + "<br><br>";
          recNets = recNets + ";;" + String(WiFi.SSID(i)) + ";;";
        }
      }
    }
  } else if (state == LOW) {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println(nets);
    manageHotspot();
    serverManage();
  }
}
