#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SFE_BMP180.h>
#include <Wire.h>

#include "DHTesp.h"
String apiKey = "318HXNY64WEEY9FM";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "V2033";     // replace with your wifi ssid and wpa2 key
const char *pass =  "bibha123";
const char* server = "api.thingspeak.com";
 
#define DHTPIN 14          //pin where the dht11 is connected
#define LED 2

SFE_BMP180 pressure;

#define ALTITUDE 1655.0

DHTesp dht;
 
WiFiClient client;

float humidity, temperature;
double pressureInHg,pressureInMb;
int rain;
void handleADC() {
    char status;
    double T,P,p0,a;
    double Tdeg, Tfar, phg, pmb;
    
    status = pressure.startTemperature();
    if (status != 0)
    {
        // Wait for the measurement to complete:
        delay(status);
        status = pressure.getTemperature(T);
        if (status != 0)
        {
        // Print out the measurement:
        Serial.print("temperature: ");
        Serial.print(T,2);
        Tdeg = T;
        Serial.print(" deg C, ");
        Tfar = (9.0/5.0)*T+32.0;
        Serial.print((9.0/5.0)*T+32.0,2);
        Serial.println(" deg F");
        
        status = pressure.startPressure(3);
        if (status != 0)
        {
            // Wait for the measurement to complete:
            delay(status);
            status = pressure.getPressure(P,T);
            if (status != 0)
            {
                // Print out the measurement:
                Serial.print("absolute pressure: ");
                Serial.print(P,2);
                pmb = P;
                Serial.print(" mb, ");
                phg = P*0.0295333727;
                Serial.print(P*0.0295333727,2);
                Serial.println(" inHg");
                
                p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
                Serial.print("relative (sea-level) pressure: ");
                Serial.print(p0,2);
                Serial.print(" mb, ");
                Serial.print(p0*0.0295333727,2);
                Serial.println(" inHg");
                
                a = pressure.altitude(P,p0);
                Serial.print("computed altitude: ");
                Serial.print(a,0);
                Serial.print(" meters, ");
                Serial.print(a*3.28084,0);
                Serial.println(" feet");
            }
            else Serial.println("error retrieving pressure measurement\n");
        }
        else Serial.println("error starting pressure measurement\n");
        }
    else Serial.println("error retrieving temperature measurement\n");
    }
    else Serial.println("error starting temperature measurement\n");
    
    
    //rain = analogRead(A0);
    int sensorValue2 = analogRead(A0);
    sensorValue2 = constrain(sensorValue2, 150, 440); 
    sensorValue2 = map(sensorValue2, 150, 440, 1023, 0);
    rain = sensorValue2;
    
    //Create JSON data
    String data = "{\"Rain\":\""+String(rain)+"\",\"Pressuremb\":\""+String(pmb)+"\",\"Pressurehg\":\""+String(phg)+"\", \"Temperature\":\""+ String(temperature) +"\", \"Humidity\":\""+ String(humidity) +"\"}";
    
    digitalWrite(LED,!digitalRead(LED)); //Toggle LED on data request ajax
    //server.send(200, "text/plane", data); //Send ADC value, temperature and humidity JSON to client ajax request
    
    delay(dht.getMinimumSamplingPeriod());
    
    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
    
    Serial.print("H:");
    Serial.println(humidity);
    Serial.print("T:");
    Serial.println(temperature); //dht.toFahrenheit(temperature));
    Serial.print("R:");
    Serial.println(rain);
    pressureInHg = phg;
    pressureInMb = pmb;
}

void setup() 
{
       Serial.begin(115200);
       delay(10);
       
       //DHT11 Sensor
       dht.setup(DHTPIN, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 17
       pinMode(LED,OUTPUT);

       //BMP180 Sensor
        if (pressure.begin())
            Serial.println("BMP180 init success");
        else
        {
            Serial.println("BMP180 init fail\n\n");
            while(1); // Pause forever.
        }
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
       WiFi.begin(ssid, pass);
       Serial.println("");
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.print("WiFi connected to ");
      Serial.println(ssid);
     
 
}
 
void loop() 
{
      handleADC();
      
              if (isnan(humidity) || isnan(temperature)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {      
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(temperature);
                             postStr +="&field2=";
                             postStr += String(humidity);
                             postStr +="&field3=";
                             postStr += String(pressureInMb);
                             postStr +="&field4=";
                             postStr += String(rain);
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
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}
