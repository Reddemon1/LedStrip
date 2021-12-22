#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <FastLED.h>

#define NUM_LEDS  6
#define LED_PIN   2
#define BRIGHTNESS 255  /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */

const char* ssid = "PT. Grha Digital Indonesia";
const char* pass = "satuuntuksemua";
HTTPClient http;
String tes = "Hidup";
CRGB leds[NUM_LEDS];

void setup() {
      
    WiFiClient wifiClient;

    http.begin(wifiClient,"http://192.168.88.247:8080/led/");

  WiFi.begin(ssid,pass);

  while(WiFi.status() != WL_CONNECTED){
     delay(1000);
     Serial.println("Connecting");
  }
    Serial.begin(9600);  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
//  FastLED.setBrightness(50);
  FastLED.setCorrection(TypicalPixelString);
}

void loop() {
      char charMessage[500];
      String JSONMessage = http.getString();
      JSONMessage.replace("[","");
      JSONMessage.replace("]","");
      JSONMessage.toCharArray(charMessage,500);
      
      StaticJsonBuffer<300> JSONBuffer;                         //Memory pool
      JsonObject& parsed = JSONBuffer.parseObject(JSONMessage); //Parse message
     
//      if (!parsed.success()) {   //Check for errors in parsing
//     
//        Serial.println("Parsing failed");
//        delay(5000);
//        return;
//     
//      }
     
      const char * Id     = parsed["id"]; //Get sensor type value
      const char * Status    = parsed["status"]; //Get sensor type value
      const char * Numled = parsed["numLed"]; //Get sensor type value

     if(String(Status) == "ON"){
      for (int j = 0; j < 255; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS); /* The higher the value 4 the less fade there is and vice versa */ 
        }
        FastLED.show();
        delay(25); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
      }
     }else{
        for(int i = 0; i < NUM_LEDS; i++){
          leds[i] = 0x000000;
        }
     }
}
