// motor code, pinout: https://create.arduino.cc/projecthub/Techatronic/dc-motor-speed-control-using-arduino-0e5b2b
// ultrasonic code, pinout: https://www.instructables.com/id/Arduino-Ultrasonic-Sensor-HC-SR04/
// nrf24l01 code, pinout: https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
// led code, pinout: https://howtomechatronics.com/tutorials/arduino/how-to-control-ws2812b-individually-addressable-leds-using-arduino/
// buzzer code, pinout: https://create.arduino.cc/projecthub/munir03125344286/add-buzzer-to-arduino-bf010b

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

                      // WE, UNO
#define echoPinOne D0 // D0, D10
#define trigPinOne D8 // D8, D11

#define echoPinTwo D3 // D3, D12
#define trigPinTwo D4 // D4, D13

const long SERIAL_BAUD = 115200;
const byte RADIO_ADDRESS[6] = "00001";

int distanceForward = 0;
int distanceBackward = 0;

RF24 radio(4, 5); // CE, CSN

// -------------------------------------------------------------------------------------------------
void ultrasonicPinsSetup(){
  pinMode(trigPinOne, OUTPUT);
  pinMode(echoPinOne, INPUT);

  pinMode(trigPinTwo, OUTPUT);
  pinMode(echoPinTwo, INPUT);
}

void radioSetup(){
  radio.begin();
  radio.openReadingPipe(0, RADIO_ADDRESS);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

int getSensorDistance(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  return int(pulseIn(echoPin, HIGH) * 0.034 / 2);
}

void moveForward(){
  if (distanceForward < 25){
    Serial.println("Too close to an obstacle, move backward");
    return;
  }

  Serial.println("Moving forward 10 cm");
}
void moveBackward(){
  if (distanceForward < 25){
    Serial.println("Too close to an obstacle, move forward");
    return;
  }

  Serial.println("Moving backward 10 cm");
}
void moveLeft(){
  Serial.println("Moving left");
}
void moveRight(){
  Serial.println("Moving right");
}

// -------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println();
  Serial.println("Serial debug set up");

  ultrasonicPinsSetup();
  Serial.println("Ultrasonic pins set up");

  radioSetup();
  Serial.println("Radio set up");
}

void loop() {
  distanceForward = getSensorDistance(trigPinOne, echoPinOne);  
  distanceBackward = getSensorDistance(trigPinTwo, echoPinTwo);

  if (!radio.available()) return;
  
  char radioRequest[16] = "";
  radio.read(&radioRequest, sizeof(radioRequest));
  if (strcmp(radioRequest, "forward") == 0) moveForward();
  else if (strcmp(radioRequest, "backward") == 0) moveBackward();
  else if (strcmp(radioRequest, "left") == 0) moveLeft();
  else if (strcmp(radioRequest, "right") == 0) moveRight();
}