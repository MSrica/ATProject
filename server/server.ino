// https://randomnerdtutorials.com/esp8266-nodemcu-client-server-wi-fi/
// https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

#define echoPinOne 5
#define trigPinOne 4

#define echoPinTwo 0
#define trigPinTwo 2

const char *SERVER_SSID = "ESP8266-Access-Point";
const char *SERVER_PASSWORD = "123456789";

const long BAUD = 115200;

int distanceForward = 0;
int distanceBackward = 0;

AsyncWebServer server(80);

String moveForward(){
  if (distanceForward < 25){
    Serial.println("Too close to an obstacle, move backward");
    return "stay";
  }

  Serial.println("Moving forward 10 cm");
  return "forward";
}
String moveBackward(){
  if (distanceForward < 25){
    Serial.println("Too close to an obstacle, move forward");
    return "stay";
  }

  Serial.println("Moving backward 10 cm");
  return "backward";
}
String moveLeft(){
  Serial.println("Moving left");
  return "left";
}
String moveRight(){
  Serial.println("Moving right");
  return "right";
}

void setupServerRequests(){
  server.on("/forward", HTTP_GET, [](AsyncWebServerRequest *request){
    request -> send_P(200, "text/plain", moveForward().c_str());
  });
  server.on("/backward", HTTP_GET, [](AsyncWebServerRequest *request){
    request -> send_P(200, "text/plain", moveBackward().c_str());
  });
  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request){
    request -> send_P(200, "text/plain", moveLeft().c_str());
  });
  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request){
    request -> send_P(200, "text/plain", moveRight().c_str());
  });
}

void setupUltrasonicPins(){
  pinMode(trigPinOne, OUTPUT);
  pinMode(echoPinOne, INPUT);

  pinMode(trigPinTwo, OUTPUT);
  pinMode(echoPinTwo, INPUT);
}

void setup(){
  Serial.begin(BAUD);
  Serial.println();
  Serial.println("Serial debug set up");

  setupUltrasonicPins();
  Serial.println("Ultrasonic pins set up");
  
  WiFi.softAP(SERVER_SSID, SERVER_PASSWORD);
  Serial.println("Access point set up");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access point IP: ");
  Serial.println(IP);

  setupServerRequests();
  
  server.begin();
  Serial.print("Server started");
}

int getSensorDistance(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  return int(pulseIn(echoPin, HIGH) * 0.034 / 2);
}

void loop() {
  distanceForward = getSensorDistance(trigPinOne, echoPinOne);  
  distanceBackward = getSensorDistance(trigPinTwo, echoPinTwo);
}