
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
 #include <ESP8266WebServer.h>
  #include "Adafruit_VEML6070.h"
#define SEALEVELPRESSURE_HPA (1013.25)
BH1750 lightMeter;
Adafruit_BME280 bme;
 Adafruit_VEML6070 uv = Adafruit_VEML6070();
float temperature, humidity, pressure, altitude;
String status = "";
int uvs;

const char* ssid = "TurkTelekom_ZWEUU";
const char* password = "15dFd498e735d";
 
ESP8266WebServer server(80);              
 
void setup() {
  Serial.begin(115200);
  delay(100);
   uv.begin(VEML6070_1_T);  //3< LOW  3 < 6 Moderate  6 < 8 High 8 < 11 Very High 11 < 12 Extreme Hight
  bme.begin(0x76);    
  lightMeter.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);
 
  //Connect to your local wi-fi network
  WiFi.begin(ssid, password);
 
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
 
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
 
  server.begin();
  Serial.println("HTTP server started");
 
}
void loop() {
  server.handleClient();
}
 
void handle_OnConnect() {
  float lux = lightMeter.readLightLevel();
  uvs = (uv.readUV()/230);
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  
    if(uvs>=0 && uvs<3 ) //3< LOW
  {
 status = "LOW";
  
  } else 

   if(uvs=3 && uvs<6) //3 < 6 Moderate
  {

 status = "Moderate";
  
  } else 

   if(uvs>=6 && uvs<8) //6 < 8 High
  {
 
 status = "High";
  } else


  if(uvs>=8 && uvs<11) //8 < 11 Very High
  {
 
  status = "Very High";
  }

  if(uvs>=11 && uvs<12)  //11 < 12 Extreme Hight
  {
 
   status = "Extreme high";
  }
  server.send(200, "text/html", SendHTML(temperature,humidity,pressure,altitude,uvs,status, lux)); 
}
 
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
 
String SendHTML(float temperature,float humidity,float pressure,float altitude,int uvs, String status, float lux){
  
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta http-equiv=\"refresh\" content=\"2\" >\n";
  ptr +="<title>CERES AIR Weather Station</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>CERES AIR  Weather Station</h1>\n";
  ptr +="<p>Temperature: ";
  ptr +=temperature;
  ptr +="&deg;C</p>";
  ptr +="<p>Humidity: ";
  ptr +=humidity;
  ptr +="%</p>";
  ptr +="<p>Pressure: ";
  ptr +=pressure;
  ptr +="hPa</p>";
  ptr +="<p>Altitude: ";
  ptr +=altitude;
  ptr +="m</p>";
  ptr +="<p>Uv Index: ";
  ptr +=uvs;
ptr += " "+status;
ptr +="<p>Light Indensity: ";
  ptr +=lux;
  ptr +="lx</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
