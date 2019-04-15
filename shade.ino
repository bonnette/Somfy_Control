/*********
  Origional code by:
  Rui Santos
  at http://randomnerdtutorials.com 
  
Modified 08/2017 by Larry Bonnette for use with Somfy mororized shades
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>

MDNSResponder mdns;
Servo servo;  // create servo object to control a servo 

// Replace with your network credentials
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";

ESP8266WebServer server(80); // We used port 80 but you can choose your own

String webPage = "";

int gpio0_pin = 0;
//int gpio2_pin = 2;

void setup(void){
  servo.attach(2);  // attaches the servo on GIO2 to the servo object
  servo.write(90); // Set servo to center position //Set servo to 90 position (no button pressed)
  
  //The following is the web page
  webPage += "<style> button {background-color: #4CAF50; border: none; color: white; padding: 20px 37px; text-align: center; font-size: 76px; margin: 4px 2px; cursor: pointer;} </style>";
  webPage += "<h1>Shade Control</h1><h1>LED ----- <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></h1>";
  webPage += "</br></br></br></br></br></br></br></br></br></br>";
  webPage += "<h1>Shade ---------- <a href=\"shadeup\"><button>Up</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"shadedown\"><button>Down</button></a></h1>";
  //End web page
  
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, HIGH);

  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  IPAddress ip(192,168,0,133);   
  IPAddress gateway(192,168,0,1);   
  IPAddress subnet(255,255,255,0);   
  WiFi.config(ip, gateway, subnet);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  //This is the led control. 
  server.on("/", [](){
    server.send(200, "text/html", webPage); //Send web page
  });
  server.on("/socket1On", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio0_pin, LOW);
    delay(1000);
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio0_pin, HIGH);
    delay(1000); 
  });
  // End LED control
  //Begin Shade control
  server.on("/shadeup", [](){
    server.send(200, "text/html", webPage);
    servo.write(74); //Sets servo to 74 position (press up button)
    delay(5000); //Hold button pressed for 5 seconds
    servo.write(90); //Return servo to 90 position (no button pressed)
  });
  server.on("/shadedown", [](){
    server.send(200, "text/html", webPage);
    servo.write(105); //Sets servo to 105 position (press down button)
    delay(5000); //Hold button pressed for 5 seconds
    servo.write(90); //Return servo to 90 position (no button pressed)
  });
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 
