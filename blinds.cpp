#include <WiFi.h>
#include <HTTPClient.h>

int photoRes = 34;
int sensorValue = 0;
int in1 = 27;
int in2 = 26;
int enA = 14;

const char* ssid = "NAME_OF_NETWORK";
const char* password = "PASSWORD
const char* server = "api.thingspeak.com";

// Domain Name with full URL Path for HTTP POST Request
const char* serverName = "http://api.thingspeak.com/update";
// Service API Key
String apiKey = "API_KEY"
WiFiClient client;

void setup()
{
  Serial.begin(115200);

  //connect to thinspeak
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(photoRes,INPUT);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}
void loop()
{
  sensorValue = analogRead(photoRes);
  sendData();
  Serial.println(sensorValue);
//  Serial.print(sensorValue, DEC); // prints the value read
//  Serial.print(" \n"); // prints a space between the numbers
  delay(1000); // wait 100ms for next reading
//Control motor:
  directionControl();
}

void sendData(){
  if (client.connect(server,80))                                 //   "184.106.153.149" or api.thingspeak.com
    {                  
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(sensorValue);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
}
}

void directionControl() {
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  if (sensorValue > 3000){ //set threshold to turn on motor
    digitalWrite(enA, HIGH);
    digitalWrite(in1, HIGH);
    } else if (sensorValue <=3000){
      digitalWrite(enA, LOW);
    }
}
