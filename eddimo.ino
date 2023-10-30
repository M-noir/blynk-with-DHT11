#define BLYNK_TEMPLATE_ID ""//your template id
#define BLYNK_TEMPLATE_NAME ""//your template name

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = ""; //Enter the Auth code which was sent by Blynk

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";  // Enter your WIFI Name
char pass[] = "";  // Enter your WIFI Password

#define DHTPIN D4          // Digital pin 4

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);

int fan = D0;

SimpleTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V0, h);  // V5 is for Humidity
  Blynk.virtualWrite(V1, t);  // V6 is for Temperature

  if (t > 28) {
    digitalWrite(fan, HIGH); // Turn on the fan
    Serial.println("Fan turned on");
  } else {
    digitalWrite(fan, LOW); // Turn off the fan
    Serial.println("Fan turned off");
  }
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(fan, OUTPUT);
  digitalWrite(fan, LOW); // Ensure the fan starts in the OFF state

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
