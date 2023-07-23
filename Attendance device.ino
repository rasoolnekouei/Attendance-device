/*
 *  Created by Rasool_Nekouei
 * ----------------------------------------------------------------------
 * Example program showing how to read new NUID from a PICC to serial.
 * ----------------------------------------------------------------------
 * https://volleyball-house.com
 * 
 * RC522 Interfacing with NodeMCU
 * 
 * Typical pin layout used:
 * ----------------------------------
 *             MFRC522      Node     
 *             Reader/PCD   MCU      
 * Signal      Pin          Pin      
 * ----------------------------------
 * RST/Reset   RST          D1 (GPIO5)        
 * SPI SS      SDA(SS)      D2 (GPIO4)       
 * SPI MOSI    MOSI         D7 (GPIO13)
 * SPI MISO    MISO         D6 (GPIO12)
 * SPI SCK     SCK          D5 (GPIO14)
 * 3.3V        3.3V         3.3V
 * GND         GND          GND
 * We use 1 nodemcu esp8266 and 1 rc522 and 1 buzzer to this project
 */

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1
#define Buzzer 0  //D3
#define LED1 2    //D4

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
const char* ssid = "Volleyball-House";
const char* password = "123456789";
//Your Domain name with URL path or IP address with path
String serverName = "http://volleyball-house.com/";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

int statuss = 0;
int out = 0;
String card_ID="";
void setup() 
{

 pinMode(Buzzer,OUTPUT); //buzzer pin D3
 pinMode(LED1,OUTPUT);
 digitalWrite(LED1,LOW);
 Serial.begin(9600); 
 startding();
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED1,LOW);
    delay(500);
    digitalWrite(LED1,HIGH);
    Serial.print(".");  
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  connding();
  digitalWrite(LED1,HIGH);
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  


  
  
  //Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  
}
void startding(){
    digitalWrite(Buzzer,HIGH);
    delay(1000);
    digitalWrite(Buzzer,LOW);
    delay(300);
    digitalWrite(Buzzer,HIGH);
    delay(80);
    digitalWrite(Buzzer,LOW);
    delay(80);
    digitalWrite(Buzzer,HIGH);
    delay(80);
    digitalWrite(Buzzer,LOW);
  }
void ding(){
  digitalWrite(Buzzer,HIGH);
  delay(80);
  digitalWrite(Buzzer,LOW);
  
  }
   
  void connding(){ //The function for melody when nodemcu connect to the wifi
    digitalWrite(Buzzer,HIGH);
    delay(80);
    digitalWrite(Buzzer,LOW);
    delay(80);
    digitalWrite(Buzzer,HIGH);
    delay(80);
    digitalWrite(Buzzer,LOW);
    
    }
  void errording(){
    digitalWrite(Buzzer,HIGH);
    delay(1000);
    digitalWrite(Buzzer,LOW);
    }
  void send_data(String id){

     if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      digitalWrite(LED1,HIGH);

      String serverPath = serverName + "cardid.php?cardid=" + id;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
  
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
        
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(http.GET());
        Serial.println(payload);
        ding(); //Melody for the Success send  
      }        
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        errording();
      }
      // Free resources
      //http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
      digitalWrite(LED1,LOW);
      errording();
    }
    lastTime = millis();
  }
    
    }

void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  for (byte i = 0; i < mfrc522.uid.size; i++) {
   card_ID += mfrc522.uid.uidByte[i];
 }
 if (card_ID){
 Serial.println(card_ID);
 send_data(card_ID);
 delay(1000);
 card_ID="";
 }
 card_ID="";
 delay(6000);
 connding();
 delay(500);
 ding();
 
 
  //Show UID on serial monitor
 /* Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "8E 39 32 50") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    Serial.println(" Welcome Mr.Circuit ");
    delay(1000);
    Serial.println(" Have FUN ");
    Serial.println();
    statuss = 1;
  }
  
  else   {
    Serial.println(" Access Denied ");
    delay(3000);
  }*/
} 
