#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "Name of the wifi ";
const char* password = "Password ";
String  command =""; // Command received from Android device
 
// Set Relay Pins
int relay1 = 5;
int relay2 = 4;

 
void setup()
{
  Serial.begin(115200);
 
  pinMode(relay1, OUTPUT); 
  pinMode(relay2, OUTPUT);  
  
 
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,HIGH);
 
      
  connectWiFi();
  server.begin();
}
 
void loop()
{
    client = server.available();
    if (!client) return; 
    command = checkClient ();
 
         if (command == "r1on"  || command == "turn on relay 1"    || command == "relay 1 on")        digitalWrite(relay1,0);
    else if (command == "r1off" || command == "turn off relay 1" || command == "relay 1 off")     digitalWrite(relay1,1);
    else if (command == "r2on"  || command == "turn on relay 2"    || command == "relay 2 on")      digitalWrite(relay2,0);     
    else if (command == "r2off" || command == "turn off relay 2" || command == "relay 2 off")   digitalWrite(relay2,1);
    else if (command == "allon" || command == "Turn on all devices" || command == "all on") 
    {
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
    }
    else if (command == "alloff" || command == "Turn off all devices" || command == "all off") 
    {
      digitalWrite(relay1,HIGH);
      digitalWrite(relay2,HIGH);
    }
    sendBackEcho(command); // send command echo back to android device
    command = "";
}
 
/* connecting WiFi */
void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
}
 
/* check command received from Android Device */
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}
 
/* send command echo back to android device */
void sendBackEcho(String echo)
{
  client.println("HTTP/1.1 200 OK ");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println(echo);
  client.println("</html>");
  client.stop();
  delay(1);
}