#include "ESP8266WiFi.h"
#include "NTPClient.h"
#include "WiFiUdp.h"
#include "ESP8266HTTPClient.h"

#define LED_POWER_IS_ACTIVE D4
#define LED_DETECT_WATER_FLOW D3
#define SENSOR D2

const char* ssid = "Kimi No Na wa";
const char* password = "Makoto Shinkai";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

unsigned long Epoch_Time;

unsigned long previousMillis = 0;
const unsigned long interval = 1000;

float calibrationFactor = 4.5;
volatile byte pulseCount;
volatile byte detectorPulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned long flowMilliLitres;
unsigned int totalMilliLitres;
float flowLitres;

int totalSeconds = 0;
float totalLitres1Minute;

// Host Rest API
String HOST = "http://<BACKEND_HOST>/api/waterUsage";

// Dummy Pelanggan ID
String userId = "c47158fd-069d-40ae-bb10-9bdfe1b6fd56";

unsigned long getEpochTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
  detectorPulseCount++;
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_POWER_IS_ACTIVE, OUTPUT);
  pinMode(LED_DETECT_WATER_FLOW, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.print("Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print("*");
  }
  digitalWrite(LED_POWER_IS_ACTIVE, HIGH);
  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.print(WiFi.localIP());
  
  pinMode(SENSOR, INPUT);
  timeClient.begin();
  
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
 
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, RISING);
}

void sendPost(String time, String usage) {
  Serial.print("Usage : ");
  Serial.println(usage);
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, HOST);
    http.addHeader("Content-Type", "application/json");
    String jsonPayload = "{\"user_id\": \""+ userId + "\", \"usage\": "+ String(usage) +", \"unit\": \"liter\", \"usage_at\": "+String(time)+" }";
    Serial.print("Payload: ");
    Serial.println(jsonPayload);
    int httpResponseCode = http.POST(jsonPayload);
    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);
    http.end();
  }
}

void loop() {
  unsigned long currentMillis = millis();
  Epoch_Time = getEpochTime();
  if (detectorPulseCount > 0) {
    digitalWrite(LED_DETECT_WATER_FLOW, HIGH);
  } else {
    digitalWrite(LED_DETECT_WATER_FLOW, LOW);
  }
  if (currentMillis - previousMillis >= interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;

    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    flowMilliLitres = (flowRate / 60) * 1000;
    flowLitres = (flowRate / 60);

    totalSeconds += 1;
    totalLitres1Minute += flowLitres;

    // When 1 minute
    if (totalSeconds >= 60) {
      if (totalLitres1Minute > 0) {
        sendPost(String(Epoch_Time) + "000", String(totalLitres1Minute));
      }
      totalSeconds = 0;
      totalLitres1Minute = 0;
    }

    Serial.print("FlowRate: ");
    Serial.println(String(flowRate));
    Serial.print("FlowMillisLitres: ");
    Serial.println(String(flowMilliLitres));
    Serial.print("Flow Litres: ");
    Serial.println(String(flowLitres));

    previousMillis = currentMillis; 
  }
  detectorPulseCount = 0;
}