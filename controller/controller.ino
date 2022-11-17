#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <ezButton.h>

const long DEBOUNCE = 100;
const long SERIAL_BAUD = 115200;
const byte RADIO_ADDRESS[6] = "00001";

ezButton forwardButton(2);  // D4        
ezButton backwardButton(0); // D3         
ezButton leftButton(15);    // D8     
ezButton rightButton(16);   // D0  

RF24 radio(4, 5);           // CE, CSN

// -------------------------------------------------------------------------------------------------

void debouncesSetup(){
  forwardButton.setDebounceTime(DEBOUNCE);
  backwardButton.setDebounceTime(DEBOUNCE);
  leftButton.setDebounceTime(DEBOUNCE);
  rightButton.setDebounceTime(DEBOUNCE);
}

void radioSetup(){
  radio.begin();
  radio.openWritingPipe(RADIO_ADDRESS);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void buttonLoops(){
  forwardButton.loop();
  backwardButton.loop();
  leftButton.loop();
  rightButton.loop();
}

// -------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println();
  Serial.println("Serial debug set up");

  debouncesSetup();
  Serial.println("Debounces set up");

  radioSetup();
  Serial.println("Radio set up");
}

void loop() {
  buttonLoops();

  String radioData = "";
  // button.isReleased()
  if(forwardButton.isPressed()) radioData = "forward";
  if(backwardButton.isPressed()) radioData = "backward";
  if(leftButton.isPressed()) radioData = "left";
  if(rightButton.isPressed()) radioData = "right";
  if(radioData == "") return;

  radio.write(radioData.c_str(), sizeof(radioData));
}