#include <ESP8266WiFi.h>

const int trigPin = 2;  //D4
const int echoPin = 0;  //D3

WiFiServer server(80);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("NODE_Distance", "SKL_120202");

  Serial.begin(115200);
  Serial.println("RUBBISH");
  Serial.println("NoDiMeU (Node Distance Measurement Unit) V 1.01");
  Serial.println("(c) Karoly Straub 2017");
  Serial.println("Server mode: AP...");
  Serial.println("Starting server...");
  server.begin();
  Serial.println("Server is up and running...");
  IPAddress serverIp = WiFi.softAPIP();
  Serial.print("Server ip: ");
  Serial.println(serverIp);
}

void loop() {
  WiFiClient client = server.available();
  if(!client){
    return;
  }
  Serial.println("Client connected...");
  String requestParam = client.readStringUntil('\r');
  Serial.print("Client requested: ");
  Serial.println(requestParam);
  client.flush();
  if(requestParam.indexOf("/distance") == -1){
    client.print("Unknown command...");
    Serial.println("Unknown commad...");
    Serial.println("Client disconnected...");
    return;
  }
  float distance = getDistance();
  Serial.print("Returnning distance: ");
  Serial.print(distance);
  Serial.println(" cm...");
  client.print(distance);
  delay(1);
  Serial.println("Client disconnected...");
  }

float getDistance(){
  long duration;
  float distance = 0.0f;
  float lastDistance = 0;
  float avgDistance;
  int i;
  for(i = 1; i < 50; i++){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
  
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    duration = pulseIn(echoPin, HIGH);
    if ((duration * 0.034/2 - lastDistance > 2.0f) 
    || duration * 0.034/2 - lastDistance < -2.0f)
    {
      if(lastDistance == 0.0f){
        lastDistance = duration * 0.034/2;
      }
      continue;
    }
    lastDistance = duration * 0.034/2;
    distance += lastDistance;
  }
  avgDistance = distance / i;
  Serial.print("Distance: ");
  Serial.println(avgDistance );
  return avgDistance;
}
