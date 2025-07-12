#include "WifiCam.hpp"
#include <WiFi.h>

static const char* WIFI_SSID = "";
static const char* WIFI_PASS = "";

//-----------------------------------------CONFIGURACION DEL PARPADEO ESP-32-CAM------------------------------------

int led= 4;
int status = 15;
int lectura = 0;

//-----------------------------------------CONFIGURACION DEFAULT DEL ESP32-CAM------------------------------------

esp32cam::Resolution initialResolution;

WebServer server(80);

void
setup()
{
  Serial.begin(115200);
  Serial.println();
  delay(2000);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi failure");
    delay(5000);
    ESP.restart();
  }
  Serial.println("WiFi connected");

  {
    using namespace esp32cam;

    initialResolution = Resolution::find(1024, 768);

    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(initialResolution);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    if (!ok) {
      Serial.println("camera initialize failure");
      delay(5000);
      ESP.restart();
    }
    Serial.println("camera initialize success");
  }

  Serial.println("camera starting");
  Serial.print("http://");
  Serial.println(WiFi.localIP());

  addRequestHandlers();
  server.begin();

  pinMode(led, OUTPUT);
  pinMode(status, INPUT);

  digitalWrite(led, LOW);
}

void
loop()
{
  server.handleClient();

  lectura = digitalRead(status); 

  if(lectura == 1){
    digitalWrite(led, HIGH);
  } else{
    digitalWrite(led, LOW);
  }

}
