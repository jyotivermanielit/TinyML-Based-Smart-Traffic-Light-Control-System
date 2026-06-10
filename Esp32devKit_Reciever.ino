// Receiver ESP32 DevKit - Green Light Only

\#include <WiFi.h>

\#include <WebServer.h>



// =============================

// WiFi AP Settings

// =============================

const char\* ap\_ssid = "Arch";

const char\* ap\_password = "ilvpizza";



// =============================

// Green LED Pin

// =============================

\#define GREEN\_PIN 5



// =============================

// Web Server

// =============================

WebServer server(80);



// =============================

// Variables

// =============================

int greenDuration = 10;

float receivedConfidence = 0.0;

String receivedDensity = "Unknown";

bool newPredictionReceived = false;



// =============================

// Dynamic Green Time (mirrors sender formula)

// Low=1x, Medium=2x, High=3x

// =============================

int computeGreenTime(String density, float confidence) {

&#x20; float weight = 1.0;



&#x20; if (density == "Medium\_vehicleDensity") weight = 2.0;

&#x20; else if (density == "High\_vehicleDensity") weight = 3.0;



&#x20; int t = (int)(weight \* confidence \* 30.0);

&#x20; if (t < 5)  t = 5;

&#x20; if (t > 60) t = 60;

&#x20; return t;

}



// =============================

// Root Status Page

// =============================

void handleRoot() {

&#x20; String html = "<h2>Traffic Light Receiver</h2>";

&#x20; html += "<p><b>Density:</b> "    + receivedDensity + "</p>";

&#x20; html += "<p><b>Confidence:</b> " + String(receivedConfidence, 3) + "</p>";

&#x20; html += "<p><b>Green Time:</b> " + String(greenDuration) + " sec</p>";

&#x20; server.send(200, "text/html", html);

}



// =============================

// Handle /update from Sender

// =============================

void handleUpdate() {

&#x20; if (server.hasArg("density") \&\& server.hasArg("confidence")) {

&#x20;   receivedDensity    = server.arg("density");

&#x20;   receivedConfidence = server.arg("confidence").toFloat();

&#x20;   greenDuration      = computeGreenTime(receivedDensity, receivedConfidence);



&#x20;   Serial.println("=== Prediction Received ===");

&#x20;   Serial.print("Density:    "); Serial.println(receivedDensity);

&#x20;   Serial.print("Confidence: "); Serial.println(receivedConfidence, 3);

&#x20;   Serial.print("Green Time: "); Serial.print(greenDuration); Serial.println(" sec");



&#x20;   newPredictionReceived = true;

&#x20;   server.send(200, "text/plain",

&#x20;     "OK | Density: " + receivedDensity +

&#x20;     " | Green Time: " + String(greenDuration) + "s");

&#x20; }

&#x20; else if (server.hasArg("time")) {

&#x20;   // Fallback: plain time only

&#x20;   greenDuration = server.arg("time").toInt();

&#x20;   if (greenDuration < 5)  greenDuration = 5;

&#x20;   if (greenDuration > 60) greenDuration = 60;



&#x20;   Serial.print("Fallback time received: ");

&#x20;   Serial.print(greenDuration); Serial.println(" sec");



&#x20;   newPredictionReceived = true;

&#x20;   server.send(200, "text/plain", "OK | Green Time: " + String(greenDuration) + "s");

&#x20; }

&#x20; else {

&#x20;   server.send(400, "text/plain", "Missing parameters");

&#x20; }

}



// =============================

// Setup

// =============================

void setup() {

&#x20; Serial.begin(115200);



&#x20; pinMode(GREEN\_PIN, OUTPUT);

&#x20; digitalWrite(GREEN\_PIN, LOW);  // LED off at start



&#x20; WiFi.softAP(ap\_ssid, ap\_password);

&#x20; Serial.println("\\nWiFi AP Started");

&#x20; Serial.print("IP: "); Serial.println(WiFi.softAPIP());



&#x20; server.on("/",       handleRoot);

&#x20; server.on("/update", handleUpdate);

&#x20; server.begin();



&#x20; Serial.println("Receiver Ready");

}



// =============================

// Main Loop

// =============================

void loop() {

&#x20; server.handleClient();



&#x20; if (newPredictionReceived) {

&#x20;   newPredictionReceived = false;



&#x20;   // GREEN ON

&#x20;   Serial.print("GREEN ON for ");

&#x20;   Serial.print(greenDuration);

&#x20;   Serial.println(" sec");

&#x20;   digitalWrite(GREEN\_PIN, HIGH);



&#x20;   // Non-blocking wait — keeps server alive during green phase

&#x20;   unsigned long start = millis();

&#x20;   while (millis() - start < (unsigned long)(greenDuration \* 1000)) {

&#x20;     server.handleClient();

&#x20;     delay(10);

&#x20;   }



&#x20;   // GREEN OFF

&#x20;   digitalWrite(GREEN\_PIN, LOW);

&#x20;   Serial.println("GREEN OFF");

&#x20; }

}

