#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <MQTT.h>
#include <esp8266_peri.h>
#include <SPI.h>


#ifndef STASSID
#define STASSID "ARCHIMED"
#define STAPSK  "katamaran"
#endif

#define mqttbroker "192.168.2.4"
#define mqttname   "EspClient"

const char* ssid = STASSID;
const char* password = STAPSK;

WiFiClient net;
ESP8266WebServer server(80);
MQTTClient mqttclient;

const char* www_username = "admin";
const char* www_password = "esp8266";

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}


void setup() {
  Serial.begin(115200);





  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  ArduinoOTA.begin();

  //mqttclient.connect(mqttname);

  mqttclient.begin(mqttbroker,net);
  mqttclient.onMessage(messageReceived);
  mqttclient.connect(mqttname);


  server.on("/", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
    server.send(200, "text/plain", "Login OK - "+ server.client().remoteIP().toString() );
  });
  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");

}

void loop() {
	long stt, ett;
	int aval;
  ArduinoOTA.handle();
  server.handleClient();
  stt=micros();
  aval=analogRead(0);
  aval=analogRead(0);
  aval=analogRead(0);
  ett=micros();
  Serial.print(aval);
  Serial.println("Adc ret");
  Serial.print((int)(ett-stt));
  Serial.println(" micros");
  delay(500);
  mqttclient.publish("/esp",String(aval) );


}
