// https://randomnerdtutorials.com/esp8266-nodemcu-client-server-wi-fi/
// https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/

#include <ezButton.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

const char *SERVER_SSID = "ESP8266-Access-Point";
const char *SERVER_PASSWORD = "123456789";

const String SERVER_NAME = "http://192.168.4.1/";

const int DELAY = 500;
const long BAUD = 115200;
const long INTERVAL = 5000; 
const long DEBOUNCE = 100;

ezButton forwardButton(2);    // D4        
ezButton backwardButton(0);   // D3         
ezButton leftButton(4);       // D2     
ezButton rightButton(5);      // D1  

ESP8266WiFiMulti WiFiMulti;

void setupDebounces(){
  forwardButton.setDebounceTime(DEBOUNCE);
  backwardButton.setDebounceTime(DEBOUNCE);
  leftButton.setDebounceTime(DEBOUNCE);
  rightButton.setDebounceTime(DEBOUNCE);
}

void buttonLoops(){
  forwardButton.loop();
  backwardButton.loop();
  leftButton.loop();
  rightButton.loop();
}

void connectToWifi(){
  Serial.print("Connecting to ");
  Serial.print(SERVER_SSID);
  WiFi.begin(SERVER_SSID, SERVER_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(DELAY);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");
}

String httpGETRequest(const String serverName) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverName.c_str());
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "-"; 
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}


void setup() {
  Serial.println();

  Serial.begin(BAUD);
  Serial.println("Serial debug set up");

  setupDebounces();
 
  connectToWifi();
}

void loop() {
  buttonLoops();
  
  String httpRequestData = "";
  // button.isReleased()
  if(forwardButton.isPressed()) httpRequestData = "forward";
  if(backwardButton.isPressed()) httpRequestData = "backward";
  if(leftButton.isPressed()) httpRequestData = "left";
  if(rightButton.isPressed()) httpRequestData = "right";

  if(httpRequestData == "") return;

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("WiFi Disconnected");
    return;  
  }
  
  // GET
  Serial.println(httpGETRequest(SERVER_NAME + httpRequestData));

  // POST
  //  WiFiClient client;
  //  HTTPClient http;
  //  
  //  http.begin(client, SERVER_NAME);
  //  
  //  http.addHeader("Content-Type", "text/plain");
  //  //http.addHeader("Content-Type", "application/json");
  //  //http.addHeader("Content-Type", "application/x-www-form-urlencoded");          
  //  
  //  int httpResponseCode = http.POST(httpRequestData);
  //  Serial.print("HTTP Response code: ");
  //  Serial.println(httpResponseCode);
  //
  //  http.end();   
}