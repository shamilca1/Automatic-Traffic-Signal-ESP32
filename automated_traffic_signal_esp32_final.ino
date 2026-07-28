// Blynk virtual pins for vehicle counts
/************ Blynk Template Setup ************/
#define BLYNK_TEMPLATE_ID "TMPL3GnQVwTrG"
#define BLYNK_TEMPLATE_NAME "Traffic Light Controller"
#define BLYNK_AUTH_TOKEN "QJ_6vlGm8STBL-so4dI5SBbgKU5m8LhV"

/************ WiFi Credentials ************/
char ssid[] = "ASWIN's A55";
char pass[] = "duwo23122";


/************ Include Libraries ************/
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

/************ Ultrasonic Sensor Pins ************/
#define TRIG_PIN1 13
#define ECHO_PIN1 12

#define TRIG_PIN2 27
#define ECHO_PIN2 14

/************ Traffic Light Pins ************/
#define GREEN1 33
#define YELLOW1 25
#define RED1 26

#define GREEN2 5
#define YELLOW2 18
#define RED2 19

/************ Blynk Virtual Pins ************/
// Vehicle counts
#define V_COUNT1 V1
#define V_COUNT2 V2
// Lane 1 lights
#define V_GREEN1 V3
#define V_YELLOW1 V4
#define V_RED1 V5
// Lane 2 lights
#define V_GREEN2 V6
#define V_YELLOW2 V7
#define V_RED2 V8

/************ Global Variables ************/
long distance1, distance2;
int vehicleCount1 = 0;
int vehicleCount2 = 0;

BLYNK_WRITE(V9) {
  int buttonState = param.asInt(); // Get button value (0 or 1)
  if(buttonState == 1){
    vehicleCount1 = 0;
    vehicleCount2 = 0;

    // Update the Blynk Value Displays immediately
    Blynk.virtualWrite(V_COUNT1, vehicleCount1);
    Blynk.virtualWrite(V_COUNT2, vehicleCount2);

    Serial.println("Vehicle counters reset!");
  }
}

// emergency for lane 1
BLYNK_WRITE(V10) {
  int buttonState = param.asInt(); // Get button value (0 or 1)
  if(digitalRead(GREEN1) == LOW && buttonState == 1){
    digitalWrite(GREEN1, HIGH);
    digitalWrite(GREEN2, LOW);
    digitalWrite(RED1, LOW);
    digitalWrite(RED2, HIGH);

    Blynk.virtualWrite(V_GREEN1, digitalRead(GREEN1) ? 255 : 0);
    Blynk.virtualWrite(V_RED1, digitalRead(RED1) ? 255 : 0);
    Blynk.virtualWrite(V_GREEN2, digitalRead(GREEN2) ? 255 : 0);
    Blynk.virtualWrite(V_RED2, digitalRead(RED2) ? 255 : 0);

    delay(5000);
  }
}

// emergency for lane 2 
BLYNK_WRITE(V11) {
  int buttonState = param.asInt(); // Get button value (0 or 1)
  if(digitalRead(GREEN2) == LOW && buttonState == 1){
    digitalWrite(GREEN2, HIGH);
    digitalWrite(GREEN1, LOW);
    digitalWrite(RED2, LOW);
    digitalWrite(RED1, HIGH);

    Blynk.virtualWrite(V_GREEN1, digitalRead(GREEN1) ? 255 : 0);
    Blynk.virtualWrite(V_RED1, digitalRead(RED1) ? 255 : 0);
    Blynk.virtualWrite(V_GREEN2, digitalRead(GREEN2) ? 255 : 0);
    Blynk.virtualWrite(V_RED2, digitalRead(RED2) ? 255 : 0);

    delay(5000);
  }
}


/************ Setup Function ************/
void setup() {
  Serial.begin(115200);

  // Ultrasonic pins
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);

  // Traffic light pins
  pinMode(GREEN1, OUTPUT);
  pinMode(YELLOW1, OUTPUT);
  pinMode(RED1, OUTPUT);
  pinMode(GREEN2, OUTPUT);
  pinMode(YELLOW2, OUTPUT);
  pinMode(RED2, OUTPUT);

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

/************ Function to read ultrasonic sensor ************/
long readUltrasonicCM(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  long distance = duration * 0.034 / 2; // in cm
  return distance;
}

/************ Count Vehicles ************/
void countVehicles(){
  distance1 = readUltrasonicCM(TRIG_PIN1, ECHO_PIN1);
  distance2 = readUltrasonicCM(TRIG_PIN2, ECHO_PIN2);

  // Vehicle detected within 50 cm
  if(distance1 > 0 && distance1 < 10){
    vehicleCount1++;
    delay(500); // debounce
  }
  if(distance2 > 0 && distance2 < 10){
    vehicleCount2++;
    delay(500); // debounce
  }

  // Send counts to Blynk
  Blynk.virtualWrite(V_COUNT1, vehicleCount1);
  Blynk.virtualWrite(V_COUNT2, vehicleCount2);

  // Print counts to Serial Monitor
  Serial.print("Lane 1 Count: ");
  Serial.print(vehicleCount1);
  Serial.print("\tLane 1 dist: ");
  Serial.print(distance1);
  Serial.print("\tLane 2 Count: ");
  Serial.println(vehicleCount2);
  Serial.print("\tLane 2 dist: ");
  Serial.print(distance2);
}

/************ Control Traffic Lights ************/
void controlTraffic(){
  // Reset all lights
  digitalWrite(GREEN1, LOW);
  
  digitalWrite(RED1, LOW);
  digitalWrite(GREEN2, LOW);

  digitalWrite(RED2, LOW);

  // Lane with more vehicles gets GREEN, other RED
  if(vehicleCount1 >= vehicleCount2){
    digitalWrite(GREEN1, HIGH);
    digitalWrite(RED2, HIGH);
  } else {
    digitalWrite(GREEN2, HIGH);
    digitalWrite(RED1, HIGH);
  }

  // Send light states to Blynk (0=OFF, 255=ON)
  Blynk.virtualWrite(V_GREEN1, digitalRead(GREEN1) ? 255 : 0);
 
  Blynk.virtualWrite(V_RED1, digitalRead(RED1) ? 255 : 0);
  Blynk.virtualWrite(V_GREEN2, digitalRead(GREEN2) ? 255 : 0);

  Blynk.virtualWrite(V_RED2, digitalRead(RED2) ? 255 : 0);
}

/************ Main Loop ************/
void loop() {
  Blynk.run();
  countVehicles();
  controlTraffic();
  
  delay(1000); // Update every 1 sec
}